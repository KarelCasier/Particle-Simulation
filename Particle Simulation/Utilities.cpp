#include "Utilities.h"

void drawDebugLine(const sf::Vector2f& A, const sf::Vector2f& B, sf::RenderWindow& window, const sf::Color& colour)
{
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = A;
	line[1].position = B;
	line[0].color = colour;
	line[1].color = colour;
	window.draw(line);
}
void drawDebugLine(const float& ax, const float& ay, const float& bx, const float& by, sf::RenderWindow& window, const sf::Color& colour)
{
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(ax, ay);
	line[1].position = sf::Vector2f(bx, by);
	line[0].color = colour;
	line[1].color = colour;
	window.draw(line);
}

void drawDebugLine(sf::Vector2f pos, sf::Vector2f& dir, float& norm, sf::RenderWindow &window, const sf::Color& colour)
{
	dir /= Norm(dir);
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = pos;
	line[0].color = colour;
	line[1].position = pos + dir*norm;
	line[1].color = colour;
	window.draw(line);
}

float Norm(const sf::Vector2f & v)
{
	return std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2));
}

float Distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return std::sqrt(std::pow((v2.x - v1.x), 2) + std::pow((v2.y - v1.y), 2));
}