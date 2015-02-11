#include "ParticleSystem.h"
#include "Game.h"
#include "Utilities.h"

ParticleSystem::ParticleSystem(unsigned int particleCount) :
mParticles(),
mParticleLifetime(sf::seconds(3)),		///< Set Particle lifetime
mVerticies(sf::Points, particleCount)	///< Initiate Vertex Array to hold a certain amount if sf::Points
{
	for (int i = 0; i < std::sqrt(particleCount); i++)
	{
		for (int j = 0; j < std::sqrt(particleCount); j++)
		{
			Particle newParticle;
			newParticle.setPosition(sf::Vector2f(i, j) + sf::Vector2f(5, 5));
			mParticles.push_back(newParticle);
		}
	}

}

void ParticleSystem::update(sf::Time& dt)
{
	sf::Vector2f mousePosition = TheGame::Instance()->getMousePosition();
	std::cout << mousePosition.x << " " << mousePosition.y << std::endl;
	bool mouseClicked = TheGame::Instance()->getMouseButtonState(sf::Mouse::Left);

	for (unsigned int i = 0; i < mParticles.size(); i++)
	{
		Particle& pParticle = mParticles.at(i);

		//pParticle.decreaseLife(dt);		///< Update lifetime of particle
		if (mouseClicked)
		{
			//pParticle.addForce(mousePosition);
			pParticle.addForce(mousePosition - pParticle.getPosition() * (float)(500000 / std::pow(Distance(mousePosition, pParticle.getPosition()) + 10, 2)));		///< Add force proportional to the inverse of the distance from mouse
		}
		pParticle.addForce(- pParticle.getVelocity() * (float)10);	///< Add a drag force proportional to the velocity of the particle

		pParticle.update(dt);										///< Update particle position

		mVerticies[i].position = pParticle.getPosition();			///< Change vertex position to the particle position

		pParticle.clearForces();


	}
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();		///< Apply the whole system's transform to the particles

	states.texture = nullptr;				///< sf::Points do not use textures

	target.draw(mVerticies, states);		///< Render the Vertex Array

}