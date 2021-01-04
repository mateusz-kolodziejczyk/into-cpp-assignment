#pragma once
#include "zombie.h"


int generateLevel(sf::VertexArray& rVA, sf::IntRect arena);
Zombie* createHorde(int numZombies, IntRect arena);
