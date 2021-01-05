#pragma once
#include <SFML/Graphics.hpp>
namespace collision {
	bool boundsCheck(sf::Vector2f& position, const sf::IntRect& arena, int tileSize, float& yVelocity);
	bool floorCheck(sf::Vector2f& position, const sf::VertexArray& level, sf::IntRect& arena, const sf::FloatRect& bounds, int tileSize, const sf::Vector2f& lastPosition, float& yVelocity);
	bool collisionCheck(const sf::FloatRect& firstBounds, const sf::FloatRect& secondBounds);
}


