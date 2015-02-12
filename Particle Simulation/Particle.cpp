#include "Particle.h"
#include <cmath>
#include <iostream>

Particle::Particle() :
mPosition(sf::Vector2f(0,0)),
mMass(10)
{
}

Particle::Particle(const sf::Vector2f& position, const float& mass) :
mPosition(position),
mMass(mass)
{

}


void Particle::update(const sf::Time& dt)
{
	//std::cout << "Position: [" << getPosition().x << ", " << getPosition().y << "]" << std::endl;
	sf::Vector2f prevPosition = mPosition;
	mPosition += mVelocity*dt.asSeconds() + 0.5f*(getTotalForce() / mMass)*(float)std::pow(dt.asSeconds(), 2); //Kinematic equation to move position
	mVelocity = (mPosition - prevPosition) / dt.asSeconds(); //derivative to get new velocity

}

sf::Vector2f Particle::getTotalForce() const
{
	sf::Vector2f totalForce(0,0);

	for (const sf::Vector2f & force : mForces)
	{
		totalForce += force;
	}
	return totalForce;
}

std::vector<sf::Vector2f> Particle::getForces() const
{
	return mForces;
}
void Particle::addForce(const sf::Vector2f & force)
{
	mForces.push_back(force);
}
void Particle::clearForces()
{
	mForces.clear();
}