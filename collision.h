#pragma once
#include <SFML/Graphics.hpp>
namespace collision {
	bool boundsCheck(sf::Vector2f& position);
	bool floorCheck(sf::Vector2f& position);
}

