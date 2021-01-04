#pragma once
#include <SFML/Graphics.hpp>
namespace collision {
	bool boundsCheck(sf::Vector2f& position, const sf::IntRect& arena, int tileSize);
	bool floorCheck(sf::Vector2f& position, sf::VertexArray& level, sf::IntRect& arena, const sf::FloatRect& bounds, int tileSize);
}

