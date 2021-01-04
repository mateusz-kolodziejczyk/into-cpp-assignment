#include <SFML/Graphics.hpp>
#include "ZombieArena.h"

using namespace sf;
int generateLevel(VertexArray& rVA, IntRect arena) {
	//how big tile
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	rVA.setPrimitiveType(sf::Quads);
	
	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentVertex = 0;

	for (int w = 0; w < worldWidth; w++) {
		for (int h = 0; h < worldHeight; h++) {

			// Position each vertex in the ucrrent quad
			rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f(w * TILE_SIZE + TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f(w * TILE_SIZE + TILE_SIZE, h * TILE_SIZE + TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE + TILE_SIZE);

			// Define position in the texture to draw for current quad
			// either mud, stone grass or wall
			if (h == 0 || h == worldHeight || w == 0 || w == worldWidth - 1) {
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, 0 + TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, 0 + TILE_SIZE);
			}
			else {
				if (h % 3 == 0) {
					int verticalOffset = 1 * TILE_SIZE;
					rVA[currentVertex + 0].texCoords = Vector2f(0, verticalOffset);
					rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, verticalOffset);
					rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
					rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);
				}
				else {
					// Use random floor texture
					int verticalOffset = 3 * TILE_SIZE;

					rVA[currentVertex + 0].texCoords = Vector2f(0, verticalOffset);
					rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, verticalOffset);
					rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
					rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);
				}

			}
			// Position eady for he next vertices
			currentVertex += VERTS_IN_QUAD;
		}
	}
	return TILE_SIZE;
	//
}
