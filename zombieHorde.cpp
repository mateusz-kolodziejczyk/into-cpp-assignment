//
//  zombieHorde.cpp
//  Zombie Arena
//

#include "zombieArena.h"
#include "zombie.h"
#include <string>
#include <iostream>
using namespace std;
/*Zombie* createHorde(int numZombies, IntRect arena)
{
    Zombie* zombies = new Zombie[numZombies];

    int maxY = arena.height - 20;
    int minY = arena.top + 20;
    int maxX = arena.width - 20;
    int minX = arena.left + 20;

    for (int i = 0; i < numZombies; i++)
    {

        // Which side should the zombie spawn
        int side = (rand() % 4);
        float x = 0, y = 0;

        switch (side)
        {
        case 0:
            // left
            x = minX;
            y = (rand() % maxY) + minY;
            break;

        case 1:
            // right
            x = maxX;
            y = (rand() % maxY) + minY;
            break;

        case 2:
            // top
            x = (rand() % maxX) + minX;
            y = minY;
            break;

        case 3:
            // bottom
            x = (rand() % maxX) + minX;
            y = maxY;
            break;
        }
        cout << to_string(x) + "," + to_string(y) + "\n";

        // Bloater, crawler or runner
        int type = (rand() % 3);

        // Spawn the new zombie into the array
        zombies[i].spawn(x, y, type);

    }
    return zombies;
}
*/
