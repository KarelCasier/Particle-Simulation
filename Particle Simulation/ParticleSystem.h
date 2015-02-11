#pragma once
#include "Particle.h"


class ParticleSystem : public sf::Drawable, sf::Transformable
{
public:
	ParticleSystem(unsigned int particleCount);

	void					update(sf::Time& dt);


private:
	virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Particle>	mParticles;			///< Vector of Particles the system is holding
	sf::VertexArray			mVerticies;			///< Vertex array to hold sf::Points
	sf::Time				mParticleLifetime;	///< Time particle will live for
};