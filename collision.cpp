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

bool collision::floorCheck(Vector2f& position, VertexArray& level, IntRect& arena, const sf::FloatRect& bounds, int tileSize) {

	int worldWidth = arena.width / tileSize;
	int worldHeight = arena.height / tileSize;
	for (int i = 0; i < level.getVertexCount(); i += 4) {
		// Create float rect from the vertex
		auto topLeft = level[i].position;
		auto topRight = level[i + 1].position;
		auto bottomLeft = level[i + 3].position;
		auto tileBounds = FloatRect(topLeft.x, topLeft.y, topRight.x - topLeft.x, bottomLeft.y - topLeft.y);

		if (level[i].texCoords.y ==  1*tileSize || level[i].texCoords.y == 2*tileSize) {
			int verticalOffset = 0;
			if (bounds.intersects(tileBounds)) {
				verticalOffset = tileSize * 1;
				// If the block is above the character
				float positionChange = 0;
				if (bounds.top > tileBounds.top) {
					float bottomTile = tileBounds.top + tileBounds.height;
					positionChange = (bottomTile-bounds.top);
				}
				else if(bounds.top < tileBounds.top) {
					float bottomCharacter = bounds.top + bounds.height;
					positionChange = (tileBounds.top - bottomCharacter);
				}
				position.y += positionChange;
				return true;
			}
			else {
				verticalOffset = tileSize * 2;
			}
			level[i + 0].texCoords = Vector2f(0, verticalOffset);
			level[i + 1].texCoords = Vector2f(tileSize, verticalOffset);
			level[i + 2].texCoords = Vector2f(tileSize, tileSize+ verticalOffset);
			level[i + 3].texCoords = Vector2f(0, tileSize + verticalOffset);

		}
	}
	return false;
}
