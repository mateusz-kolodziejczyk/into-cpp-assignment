#include "Player.h"
#include <math.h>
#include "collision.h"
#include "Physics.h"
#include <iostream>

using namespace sf;
Player::Player() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;


	// Texture
	m_Texture.loadFromFile("graphics/characters/player_stand.png");
	m_Sprite.setTexture(m_Texture);

	//Origin
	m_Sprite.setOrigin(25, 25);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {

	// place in middle bottom
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height - tileSize*2.2;

	// Last position on spawn is identical to the current position
	m_LastPosition = m_Position;

	// Copy details of arena to the players arena.
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;

	m_Arena.height = arena.height;

	// Rmeember
	m_TileSize = tileSize;

	//Resoltuion
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

void Player::resetPlayerStats() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

Time Player::getLastHitTime() {
	return m_LastHit;
}

bool Player::hit(Time timeHit) {
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200) {
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else {
		return false;
	}
}

FloatRect Player::getPosition() {
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter() {
	return m_Position;
}

float Player::getRotation() {
	return m_Sprite.getRotation();
}

Sprite Player::getSprite() {
	return m_Sprite;
}

int Player::getHealth() {
	return m_Health;
}


void Player::moveLeft() {
	m_LeftPressed = true;
}
void Player::moveRight() {
	m_RightPressed = true;
}
void Player::moveUp() {
	m_UpPressed = true;
}
void Player::moveDown() {
	m_DownPressed = true;
}

void Player::stopLeft() {
	m_LeftPressed = false;
}
void Player::stopRight() {
	m_RightPressed = false;
}
void Player::stopUp() {
	m_UpPressed = false;
}
void Player::stopDown() {
	m_DownPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition, sf::VertexArray& level) {
	float verticalVelocity = m_Position.y - m_LastPosition.y;
	if (m_UpPressed) {
		m_Position.y -= m_Speed * elapsedTime;
	}
	if (m_DownPressed) {
		m_Position.y += m_Speed * elapsedTime;
	}
	if (m_RightPressed) {
		m_Position.x += m_Speed * elapsedTime;
	}
	if (m_LeftPressed) {
		m_Position.x -= m_Speed * elapsedTime;
	}
	if (m_Jumped) {
		m_LastPosition = m_Position;
		m_Position.y += -230*elapsedTime;
		verticalVelocity = m_Position.y - m_LastPosition.y;
		m_Jumped = false;
	}

	m_Sprite.setPosition(m_Position);
	// Falling


	// Collision
	bool colliding = collision::boundsCheck(m_Position, m_Arena, m_TileSize);
	bool onFloor = collision::floorCheck(m_Position, level, m_Arena, m_Sprite.getGlobalBounds(), m_TileSize, m_LastPosition, verticalVelocity);
	auto tempLastPosition = m_Position;
	if (!onFloor) {
		physics::gravityFalling(elapsedTime, m_Position, verticalVelocity);
	}

	float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2) * 180) / 3.141;
	m_LastPosition = tempLastPosition;

	//m_Sprite.setRotation(angle);
}

void Player::upgradeSpeed() {
	m_Speed += (START_SPEED * 0.2);
}
void Player::upgradeHealth() {
	m_MaxHealth += (START_HEALTH * 0.2);
}
void Player::increaseHealthLevel(int amount) {
	m_Health += amount;
	if (m_Health > m_MaxHealth) {
		m_Health = m_MaxHealth;
	}
}

void Player::jump() {
	m_Jumped = true;
}

