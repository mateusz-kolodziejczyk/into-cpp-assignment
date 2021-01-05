#include "Physics.h"
void physics::gravityFalling(float elapsedTime, sf::Vector2f& position, float velocity) {
	if (velocity > 0)
	{
		position.y += velocity + 5 * elapsedTime;
	}
	else {
		position.y += velocity + 5 * elapsedTime;
	}
}
