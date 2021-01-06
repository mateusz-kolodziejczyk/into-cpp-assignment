#include "zombieSpawner.h"
#include "zombie.h"
#include <iostream>
ZombieSpawner::ZombieSpawner(sf::Vector2f position, float spawnInterval, std::set<int> zombieTypes, bool onTheRight) :
	m_Position(position),
	m_SpawnInterval(spawnInterval),
	m_ZombieTypes(zombieTypes),
	m_OnTheRight(onTheRight)
{
	// Randomise the interval every time
	float modifier = rand() % 70 + 50;
	modifier /= 100;
	m_ActualInterval = m_SpawnInterval*modifier;
	// The zombies should start spawning almost right away
	m_SpawnTimer = m_ActualInterval * 0.8;
	std::cout << "";
}

void ZombieSpawner::addZombieType(int zombieType) {
	m_ZombieTypes.insert(zombieType);
}

void ZombieSpawner::setSpawnInterval(float spawnInterval) {
	m_SpawnInterval = spawnInterval;
}

bool ZombieSpawner::timerCheck(float elapsedTime) {
	m_SpawnTimer += elapsedTime;
	if (m_SpawnTimer >= m_ActualInterval) {
		m_SpawnTimer = 0;

		float modifier = rand() % 70 + 50;
		modifier /= 100;
		m_ActualInterval = m_SpawnInterval * modifier;
		m_ReadyToSpawn = true;
		return true;
	}
	return false;
}

Zombie* ZombieSpawner::spawn() {
	if (m_ReadyToSpawn) {
		auto zombie = new Zombie(m_OnTheRight);
		int rnd = (rand() % m_ZombieTypes.size());
		int type = 0;
		int i = 0;
		// Go through the set to find a random zombie type
		auto it = m_ZombieTypes.begin();
		for (int i = 0; i < rnd; i++)
		{
			it++;
		}
		type = *it;
		std::cout << "\nType: " << type;
		zombie->spawn(m_Position.x, m_Position.y, type);
		return zombie;

		m_ReadyToSpawn = false;
	}
	return nullptr;
}

int ZombieSpawner::getZombieTypesAmount() {
	return m_ZombieTypes.size();
}

float ZombieSpawner::getSpawnInterval() {
	return m_SpawnInterval;
}

void ZombieSpawner::setSpawnTimer(float spawnTimer) {
	m_SpawnTimer = spawnTimer;
}

float ZombieSpawner::getSpawnTimer() {
	return m_SpawnTimer;
}


