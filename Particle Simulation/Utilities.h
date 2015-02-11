#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

////////////////////////////////////////////////////////////
/// Utility to draw a line takes two sf::Vector2f, sf::RenderWindow, and a colour (optional)
///
////////////////////////////////////////////////////////////
void drawDebugLine(const sf::Vector2f& A, const sf::Vector2f& B, sf::RenderWindow& window, const sf::Color& colour = sf::Color::Yellow);

////////////////////////////////////////////////////////////
/// Utility to draw a line takes four floats(x1,y1,x2,y2), sf::RenderWindow, and a colour (optional)
///
////////////////////////////////////////////////////////////
void drawDebugLine(const float& ax, const float& ay, const float& bx, const float& by, sf::RenderWindow& window, const sf::Color& colour = sf::Color::Yellow);

////////////////////////////////////////////////////////////
/// Utility to draw a line takes two sf::Vector2f (start position, direction), float norm, sf::RenderWindow, and a colour (optional)
///
////////////////////////////////////////////////////////////
void drawDebugLine(sf::Vector2f pos, sf::Vector2f& dir, float& norm, sf::RenderWindow &window, const sf::Color& colour = sf::Color::Yellow);

////////////////////////////////////////////////////////////
/// Utility to calculate the norm of a sf::Vector2f;
///
////////////////////////////////////////////////////////////
float Norm( const sf::Vector2f & v);

////////////////////////////////////////////////////////////
/// Utility to calculate the distance between two sf::Vector2f
///
////////////////////////////////////////////////////////////
float Distance(const sf::Vector2f& v1, const sf::Vector2f& v2);

////////////////////////////////////////////////////////////
/// Utility to lerp between 2 values with a value from 0 to 1
/// Precise method which guarantees v = b when t = 1.
////////////////////////////////////////////////////////////
float lerp(const float & a, const float & b, const float & t);

////////////////////////////////////////////////////////////
/// Imprecise method which does not guarantee v = v1 when t = 1,
/// due to floating-point arithmetic error.
////////////////////////////////////////////////////////////
float lerpFast(const float & a, const float & b, const float & t);

////////////////////////////////////////////////////////////
/// Utility to clamp a float into the bounds of 2 floats
///
////////////////////////////////////////////////////////////
float clamp(const float& value, const float& min, const float& max);