#include "Physics.h"
void physics::gravityFalling(float elapsedTime, sf::Vector2f& position, float velocity) {
		position.y += velocity + 10 * elapsedTime;
}
