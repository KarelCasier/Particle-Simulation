#include "ParticleSystem.h"
#include "Game.h"
#include "Utilities.h"
#include <cmath>

//const float ParticleSystem::GRAVMASSCONST = 5000000;

ParticleSystem::ParticleSystem(unsigned int particleCount, float width, float height) :
mParticles(),
//mParticleLifetime(sf::seconds(3)),		///< Set Particle lifetime
mVerticies(sf::Points, particleCount),	///< Initiate Vertex Array to hold a certain amount if sf::Points
winX(width),
winY(height),
mParticleCount(particleCount)
{
	float spacing = 2.0;
	int sqrtParticles = std::sqrt(mParticleCount);

	for (int i = 0; i < sqrtParticles; i++)
	{
		for (int j = 0; j < sqrtParticles; j++)
		{
			Particle newParticle;
			newParticle.setPosition(sf::Vector2f(i*spacing, j*spacing) - sf::Vector2f(sqrtParticles / 2 * spacing, sqrtParticles / 2 * spacing));
			mParticles.push_back(newParticle);
		}
	}
}

void ParticleSystem::update(sf::Time& dt)
{
	sf::Vector2f mousePosition = TheGame::Instance()->getMousePosition();
	//std::cout << mousePosition.x << " " << mousePosition.y << std::endl;
	bool mouseClicked = TheGame::Instance()->getMouseButtonState(sf::Mouse::Left);
	bool mouseClickedRight = TheGame::Instance()->getMouseButtonState(sf::Mouse::Right);

	for (unsigned int i = 0; i < mParticles.size(); i++)
	{
		Particle& pParticle = mParticles.at(i);

		/*
		//pParticle.decreaseLife(dt);								///< Update lifetime of particle
		if (mouseClicked)// && Distance(mousePosition, pParticle.getPosition()) > 10)
		{
			///<-------------------------------------For more realistic but more chaotic physics---------------------------------
			//sf::Vector2f DirVec = ((mousePosition - pParticle.getPosition()) / Norm(mousePosition - pParticle.getPosition()));
			//pParticle.addForce(DirVec * (float)GRAVMASSCONST / std::powf(Distance(mousePosition, pParticle.getPosition()), 2));
			///<-----------------------------------------------------------------------------------------------------------------


																	///< Add force proportional to the inverse of the distance from mouse
			pParticle.addForce((mousePosition - pParticle.getPosition())* (float)(500000) / ( std::pow(Distance(mousePosition, pParticle.getPosition()) + 10, 2)) );
		}
		else if (mouseClickedRight)
		{
			///<-------------------------------------For more realistic but more chaotic physics---------------------------------
			//sf::Vector2f DirVec = -((mousePosition - pParticle.getPosition()) / Norm(mousePosition - pParticle.getPosition()));
			//pParticle.addForce(DirVec * (float)GRAVMASSCONST / std::powf(Distance(mousePosition, pParticle.getPosition()), 2));
			///<-----------------------------------------------------------------------------------------------------------------

			pParticle.addForce((pParticle.getPosition() - mousePosition)* (float)(500000) / (std::pow(Distance(mousePosition, pParticle.getPosition()) + 10, 2)));
		}
		*/

		//<-----				Add forces from effectors
		for (const Effector& effector : mEffectors)
		{
			switch (effector.type)
			{
			case EffectorType::GRAVITYWELL:
				pParticle.addForce((effector.position - pParticle.getPosition())* (float)(effector.strength) / (std::pow(1+ Distance(effector.position, pParticle.getPosition()) + 10, 2)));
				break;
			case EffectorType::REPULSOR:
				pParticle.addForce(-(effector.position - pParticle.getPosition())* (float)(effector.strength) / (std::pow(Distance(effector.position, pParticle.getPosition()) + 10, 2)));
				break;
			default:
				std::cout << "Unknown EffectorType" << std::endl;
				break;
			}
		}
		//<-------------------------------------------------------------------

		//pParticle.addForce(- pParticle.getVelocity() * (float)10);				///< Add a drag force proportional to the velocity of the particle

		pParticle.update(dt);													///< Update particle position

		//<--------------------Wrap particles on screen-----------------------
		/*
		sf::Vector2f pPos = pParticle.getPosition();
		sf::Vector2f winPos = TheGame::Instance()->getWorldView()->getCenter();

		if (pPos.x < winPos.x - (winX / 2.0))
		{
			pParticle.setPosition(sf::Vector2f(winPos.x + winX/2.0, pPos.y));
		}
		else if (pPos.x > winPos.x + (winX / 2.0))
		{
			pParticle.setPosition(sf::Vector2f(winPos.x - winX / 2.0, pPos.y));
		}

		if (pPos.y < winPos.y - (winY / 2.0))
		{
			pParticle.setPosition(sf::Vector2f(pPos.x, winPos.y + winY / 2.0));
		}
		else if (pPos.y > winPos.y + (winY / 2.0))
		{
			pParticle.setPosition(sf::Vector2f(pPos.x, winPos.y - winY / 2.0));
		}
		*/
		///<-------------------------------------------------------------------

		mVerticies[i].position = pParticle.getPosition();			///< Change vertex position to the particle position

		int r = 0;
		int g = clamp(255- Norm(pParticle.getVelocity()), 0 , 255); ///< Change colour based on force acting on particle
		int b = 255;

		mVerticies[i].color = sf::Color(r, g, b);

		pParticle.clearForces();									///< Clear all forces acting on particle


	}
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();		///< Apply the whole system's transform to the particles

	states.texture = nullptr;				///< sf::Points do not use textures

	target.draw(mVerticies, states);		///< Render the Vertex Array

}

void ParticleSystem::addEffector(const EffectorType& type, const sf::Vector2f& pos, const float& str)
{
	Effector newEffector;
	newEffector.type = type;
	newEffector.position = pos;
	newEffector.strength = str;
	mEffectors.push_back(newEffector);
}

void ParticleSystem::resetParticles()
{
	float spacing = 3;
	int sqrtParticles = std::sqrt(mParticleCount);

	for (int i = 0; i < mParticles.size(); i++)
	{
		mParticles.at(i).setPosition(sf::Vector2f((i%sqrtParticles)*spacing, (i/sqrtParticles)*spacing) - sf::Vector2f(sqrtParticles / 2 * spacing, sqrtParticles / 2 * spacing));
		mParticles.at(i).setVelocity(sf::Vector2f(0, 0));
	}
}