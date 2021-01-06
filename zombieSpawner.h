#pragma once
#include <SFML/Graphics.hpp>
#include <set>
#include "zombie.h"
class ZombieSpawner
{
private:
	sf::Vector2f m_Position;
	float m_SpawnInterval;
	// The actual interval will have some randomisation each time based on the spawn interval to add variation
	float m_ActualInterval;
	float m_SpawnTimer = 0;
	bool m_OnTheRight;

	// To fix double spawning
	bool m_ReadyToSpawn =false;
	// Each spawner only spawns a few zombie types. As the game progresses, each spawner will be able to spawn more types.
	std::set<int> m_ZombieTypes;
	
public:
	ZombieSpawner(sf::Vector2f position,float spawnInterval, std::set<int> zombieTypes, bool onTheRight);
	void addZombieType(int zombieType);
	void setSpawnInterval(float spawnInterval);
	float getSpawnInterval();
	void setSpawnTimer(float spawnTimer);
	float getSpawnTimer();
	int getZombieTypesAmount();
	//  will check if enough time has elapsed
	bool timerCheck(float elapsedTime);
	Zombie* spawn();
	
};

