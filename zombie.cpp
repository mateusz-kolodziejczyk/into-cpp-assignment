#include "zombie.h"
#include "textureHolder.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "collision.h"
#include "physics.h"

enum class ZombieType { BLOATER, CRAWLER, CHASER };
void Zombie::spawn(float startX, float startY, int type, int seed) {
	switch ((ZombieType)type) {
	case ZombieType::BLOATER:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/characters/zombie_walk1.png"));

		m_Speed = BLOATER_SPEED;
		m_Health = BLOATER_HEALTH;
		break;
	case ZombieType::CRAWLER:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/characters/zombie_walk1.png"));
		m_Sprite.setColor(sf::Color(255,140,140));
		m_Speed = CRAWLER_SPEED;
		m_Health = CRAWLER_HEALTH;
		break;
	case ZombieType::CHASER:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/characters/zombie_walk1.png"));
		m_Sprite.setColor(sf::Color(255,140,255));
		m_Speed = CHASER_SPEED;
		m_Health = CHASER_HEALTH;
		break;
	}
	float modifier = (rand() % MAX_VARIANCE) + OFFSET;
	// Express as a fraction of 1
	modifier /= 100;
	m_Speed *= modifier;
	// Walking animation speed is proportional to how fast the enemy is.
	m_AnimationLength *= 1 / (m_Speed / 80);
	m_Position.x = startX;
	m_Position.y = startY;
	m_LastPosition = m_Position;

	m_Sprite.setOrigin(25, 30);
	m_Sprite.setPosition(m_Position);
	m_MovingLeft ? m_Sprite.scale(-1.0f, 1.0f) : m_Sprite.scale(1.0f, 1.0f);
}

bool Zombie::hit() {
	m_Health--;

	if (m_Health < 0) {
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	return false;
}

bool Zombie::isAlive() {
	return m_Alive;
}

FloatRect Zombie::getPosition() {
	return m_Sprite.getGlobalBounds();
}

Sprite Zombie::getSprite() {
	return m_Sprite;
}

void Zombie::update(float elapsedTime, Vector2f playerLocation, const VertexArray& level, IntRect& arena, int tileSize)
{
	float yVelocity = m_Position.y - m_LastPosition.y;
	animateMovement(elapsedTime);
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;
	
	// Zombie moves in one direction, then reverses when they hit a wall
	m_Position.x -= m_MovingLeft ? m_Speed * elapsedTime : -m_Speed*elapsedTime;
	
	// Move the sprite
	m_Sprite.setPosition(m_Position);
	bool onFloor = collision::floorCheck(m_Position,level, arena, m_Sprite.getGlobalBounds(), tileSize, m_LastPosition, yVelocity);
	bool hitWall = collision::boundsCheck(m_Position, arena, tileSize);
	if (hitWall) {
		m_MovingLeft = !m_MovingLeft;
		m_Sprite.scale(-1.0f, 1.0f);
	}
	m_LastPosition = m_Position;
	if (!onFloor) {
		physics::gravityFalling(elapsedTime, m_Position, yVelocity);
	}


}

void Zombie::animateMovement(float elapsedTime) {
	m_AnimationTimer += elapsedTime;
	if (m_AnimationTimer >= m_AnimationLength) {
		m_AnimationTimer = 0;
		// This will flip it between -1 and 1 
		m_WalkAnimation *= -1;
		switch (m_WalkAnimation) {
		case 1:
			m_Sprite.setTexture(TextureHolder::GetTexture("graphics/characters/zombie_walk1.png"));
			break;
		case -1:
			m_Sprite.setTexture(TextureHolder::GetTexture("graphics/characters/zombie_walk2.png"));
			break;
		default:
			break;
		}
	}
}
