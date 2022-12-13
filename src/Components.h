#pragma once

#include <sfml/Graphics.hpp>


struct position
{
	sf::Vector2f pos;
};

struct velocity
{
	sf::Vector2f vel;
};

struct renderable
{
	sf::Shape* shape;
};

struct player
{
	std::string name;
};