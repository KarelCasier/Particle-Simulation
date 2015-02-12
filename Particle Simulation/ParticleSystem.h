#pragma once
#include "Particle.h"


enum EffectorType
{
	GRAVITYWELL,
	REPULSOR
};

struct Effector
{
	sf::Vector2f position;
	float strength;
	EffectorType type;
};

class ParticleSystem : public sf::Drawable, sf::Transformable
{
public:
	ParticleSystem(unsigned int particleCount, float width, float height);

	void					update(sf::Time& dt);
	void					addEffector(const EffectorType& type, const sf::Vector2f& pos, const float& str = 500000);
	void					clearEffectors() { mEffectors.clear(); }

	void					resetParticles();

private:
	virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Particle>	mParticles;			///< Vector of Particles the system is holding
	sf::VertexArray			mVerticies;			///< Vertex array to hold sf::Points
	//sf::Time				mParticleLifetime;	///< Time particle will live for

	//Window size for particle wrapping
	int						winX;
	int						winY;

	int						mParticleCount;

	//static const float GRAVMASSCONST;

	std::vector<Effector>	mEffectors;
};