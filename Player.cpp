#include "Player.h"
#include <math.h>
#include "collision.h"
#include "Physics.h"
#include <iostream>
#include "textureHolder.h"

using namespace sf;

Player::Player() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
	m_AnimationState = AnimationState::IDLE;

	m_MoveAnimationLength = 0.2 * 1 / (m_Speed / 100);

	// Texture
	m_Texture.loadFromFile("graphics/characters/player_stand.png");
	m_Sprite.setTexture(m_Texture);

	//Origin
	m_Sprite.setOrigin(25, 25);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {

	// place in middle bottom
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height - tileSize * 2.2;

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
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 600) {
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

void Player::stopLeft() {
	m_LeftPressed = false;
}
void Player::stopRight() {
	m_RightPressed = false;
}

float Player::getDirection() {
	return m_Sprite.getScale().x;
}

void Player::update(float elapsedTime, Vector2i mousePosition, sf::VertexArray& level) {
	float verticalVelocity = m_Position.y - m_LastPosition.y;
	if (m_RightPressed) {
		m_Position.x += m_Speed * elapsedTime;
		// Only change animation to moving if the player isnt curently jumping
		if (m_AnimationState != AnimationState::JUMPING) {
			m_AnimationState = AnimationState::MOVING;
		}
		// Flip sprite if its not facing the correct direction
		if (m_Sprite.getScale().x < 0) {
			m_Sprite.scale(-1.0f, 1.0f);
		}

	}
	if (m_LeftPressed) {
		m_Position.x -= m_Speed * elapsedTime;
		// Only change animation to moving if the player isnt curently jumping
		if (m_AnimationState != AnimationState::JUMPING) {
			m_AnimationState = AnimationState::MOVING;
		}
		// Flip sprite if its not facing the correct direction
		if (m_Sprite.getScale().x > 0) {
			m_Sprite.scale(-1.0f, 1.0f);
		}
	}

	if (m_Jumped && !m_JumpedRecently) {
		m_LastPosition = m_Position;
		m_Position.y += m_JumpStrength * -1 * elapsedTime;
		verticalVelocity = m_Position.y - m_LastPosition.y;
		m_Jumped = false;
		m_JumpedRecently = true;
		m_AnimationState = AnimationState::JUMPING;
	}
	else if (m_Jumped) {
		m_Jumped = false;
	}
	if (!m_LeftPressed && !m_RightPressed && !m_JumpedRecently) {
		m_AnimationState = AnimationState::IDLE;
	}
	animate(elapsedTime);

	m_Sprite.setPosition(m_Position);


	// Collision
	bool colliding = collision::boundsCheck(m_Position, m_Arena, m_TileSize, verticalVelocity);
	bool onFloor = collision::floorCheck(m_Position, level, m_Arena, m_Sprite.getGlobalBounds(), m_TileSize, m_LastPosition, verticalVelocity);
	auto tempLastPosition = m_Position;
	if (!onFloor) {
		physics::gravityFalling(elapsedTime, m_Position, verticalVelocity);
	}
	else {
		m_JumpedRecently = false;
		m_AnimationState = AnimationState::IDLE;
	}

	m_LastPosition = tempLastPosition;

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

void Player::animateMovement(float elapsedTime) {
	m_MoveAnimationTimer += elapsedTime;
	std::cout << "\nTimer: " << m_MoveAnimationTimer << " Length: " << m_MoveAnimationLength;
	if (m_MoveAnimationTimer >= m_MoveAnimationLength) {
		m_MoveAnimationTimer = 0;

		// This will flip it between -1 and 1 
		m_MoveAnimation *= -1;
		switch (m_MoveAnimation) {
		case 1:
			m_Sprite.setTexture(TextureHolder::GetTexture("graphics/characters/player_walk1.png"));
			break;
		case -1:
			m_Sprite.setTexture(TextureHolder::GetTexture("graphics/characters/player_walk2.png"));
			break;
		default:
			break;
		}
	}
}

void Player::animate(float elapsedTime) {
	switch (m_AnimationState) {
	case AnimationState::IDLE:
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/characters/player_stand.png"));
		break;
	case AnimationState::MOVING:
		animateMovement(elapsedTime);
		break;
	case AnimationState::JUMPING:
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/characters/player_jump.png"));
		break;
	default:
		break;
	}
}


