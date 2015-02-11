#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <vector>

class Particle
{
public:
	Particle();
	Particle(const sf::Vector2f& position, const float& mass);

	void						update(const sf::Time& dTime);

	sf::Vector2f				getTotalForce() const;
	std::vector<sf::Vector2f>	getForces() const;
	void						addForce(sf::Vector2f & force);
	void						clearForces();

	void						setPosition(sf::Vector2f& pos) { mPosition = pos; }
	const sf::Vector2f&			getPosition() { return mPosition; }

	void						setVelocity(sf::Vector2f& vel) { mVelocity = vel; }
	sf::Vector2f				getVelocity() { return mVelocity; }

	void						decreaseLife(sf::Time& time) { mLifetime -= time; }
	const sf::Time &			getLifetime() { return mLifetime; }

private:
	sf::Vector2f				mVelocity;
	sf::Vector2f				mPosition;
	sf::Vector2f				mAcceleration;
	float						mMass;
	sf::Time					mLifetime;

	std::vector<sf::Vector2f>	mForces;

};

