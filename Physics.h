#pragma once
#include <SFML/Graphics.hpp>;
namespace physics {
	void gravityFalling(float elapsedTime, sf::Vector2f& position, float velocity);
}
