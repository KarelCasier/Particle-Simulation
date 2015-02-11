#include "ParticleSystem.h"
#include "Game.h"
#include "Utilities.h"

ParticleSystem::ParticleSystem(unsigned int particleCount) :
mParticles(),
//mParticleLifetime(sf::seconds(3)),		///< Set Particle lifetime
mVerticies(sf::Points, particleCount)	///< Initiate Vertex Array to hold a certain amount if sf::Points
{
	int sqrtParticles = std::sqrt(particleCount);

	for (int i = 0; i < sqrtParticles; i++)
	{
		for (int j = 0; j < sqrtParticles; j++)
		{
			Particle newParticle;
			newParticle.setPosition(sf::Vector2f(i, j) - sf::Vector2f(sqrtParticles/2, sqrtParticles/2));
			mParticles.push_back(newParticle);
		}
	}

}

void ParticleSystem::update(sf::Time& dt)
{
	sf::Vector2f mousePosition = TheGame::Instance()->getMousePosition();
	//std::cout << mousePosition.x << " " << mousePosition.y << std::endl;
	bool mouseClicked = TheGame::Instance()->getMouseButtonState(sf::Mouse::Left);

	for (unsigned int i = 0; i < mParticles.size(); i++)
	{
		Particle& pParticle = mParticles.at(i);

		//pParticle.decreaseLife(dt);								///< Update lifetime of particle
		if (mouseClicked)
		{															///< Add force proportional to the inverse of the distance from mouse
			pParticle.addForce((mousePosition - pParticle.getPosition())* (float)(500000) / ( std::pow(Distance(mousePosition, pParticle.getPosition()) + 10, 2)) );
		}
		pParticle.addForce(- pParticle.getVelocity() * (float)10);	///< Add a drag force proportional to the velocity of the particle

		pParticle.update(dt);										///< Update particle position

		mVerticies[i].position = pParticle.getPosition();			///< Change vertex position to the particle position

		int r = 0;
		int g = clamp(255- Norm(pParticle.getVelocity()), 0 , 255);
			//std::ceilf(lerp(255, 0, 10 / Norm(pParticle.getVelocity()))); ///< Change colour based on force acting on particle
		int b = 255;

		//clamp(255-Norm(particles[i + j*VERTEX_CHUNK].getSpeed()),0,255)

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