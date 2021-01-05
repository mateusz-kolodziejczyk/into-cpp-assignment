#include "collision.h"
#include <iostream>

using namespace sf;

bool collision::boundsCheck(Vector2f& position, const IntRect& arena, int tileSize) {
	bool collided = false;

	if (position.x > arena.width - tileSize) {
		position.x = arena.width - tileSize;
		collided = true;
	}
	if (position.x < tileSize) {
		position.x = tileSize;
		collided = true;
	}

	return collided;
}

bool collision::floorCheck(Vector2f& position, VertexArray& level, IntRect& arena, const sf::FloatRect& bounds, int tileSize, const Vector2f& lastPosition, float& yVelocity) {

	bool onFloor = false;
	int worldWidth = arena.width / tileSize;
	int worldHeight = arena.height / tileSize;
	for (int i = 0; i < level.getVertexCount(); i += 4) {
		// Create float rect from the vertex
		auto topLeft = level[i].position;
		auto topRight = level[i + 1].position;
		auto bottomLeft = level[i + 3].position;
		auto tileBounds = FloatRect(topLeft.x, topLeft.y, topRight.x - topLeft.x, bottomLeft.y - topLeft.y);

		if (level[i].texCoords.y == 1 * tileSize || level[i].texCoords.y == 2 * tileSize) {
			int verticalOffset = 0;
			if (bounds.intersects(tileBounds)) {
				// If the block is above the character
				float yPositionChange = 0;
				float xPositionChange = 0;
				float verticalLeeway = tileSize * 0.5;
				if (position.x < (tileBounds.left) && position.y > tileBounds.top - verticalLeeway && position.y < tileBounds.top + verticalLeeway + tileBounds.height) {
					float rightCharacter = bounds.left + bounds.height;
					xPositionChange = tileBounds.left - rightCharacter;
					xPositionChange *= 0.4;
				}
				// Block to left
				else if (position.x > (tileBounds.left + tileBounds.width) && position.y > tileBounds.top - verticalLeeway && position.y < tileBounds.top + verticalLeeway + tileBounds.height) {
					float rightTile = tileBounds.left + tileBounds.width;
					xPositionChange = rightTile - bounds.left;
				}
				else if (bounds.top > tileBounds.top) {
					float bottomTile = tileBounds.top + tileBounds.height;
					yPositionChange = (bottomTile - bounds.top);
					yVelocity = 0;
				}
				// If the block is below
				else if (bounds.top < tileBounds.top) {
					float bottomCharacter = bounds.top + bounds.height;
					yPositionChange = (tileBounds.top - bottomCharacter);
					onFloor = true;
				}
				// Block to right


				position.y += yPositionChange * 0.6;
				position.x += xPositionChange * 0.3;
				//	position = lastPosition;
			}
		}
	}
	return onFloor;
}
