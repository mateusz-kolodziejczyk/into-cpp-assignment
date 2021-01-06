#pragma once

#include <SFML/Graphics.hpp>



enum class AnimationState { MOVING, JUMPING, IDLE, SHOOTING };
class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	sf::Vector2f m_Position;
	sf::Vector2f m_LastPosition;
	sf::Sprite m_Sprite;
	sf::Texture m_Texture;
	sf::Vector2f m_Resolution;
	sf::IntRect m_Arena;
	int m_TileSize;

	bool m_LeftPressed;
	bool m_RightPressed;
	bool m_Jumped;
	bool m_JumpedRecently = false;

	int m_Health;
	int m_MaxHealth;
	float m_JumpStrength = 500;

	// Animation
	float m_MoveAnimationLength;
	float m_MoveAnimationTimer = 0;
	int m_MoveAnimation = 1;
	float m_ShootingAnimationLength = 0.2;
	float m_ShootingAnimationTimer = 0;
	AnimationState m_AnimationState;
	

	
	sf::Time m_LastHit;
	float m_Speed;
public:
	Player();

	void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);
	
	void resetPlayerStats();

	bool hit(sf::Time timeHit);
	sf::Time getLastHitTime();

	sf::FloatRect getPosition();
	sf::Vector2f getCenter();
	float getRotation();

	sf::Sprite getSprite();
	int getHealth();

	void moveLeft();
	void moveRight();

	void stopLeft();
	void stopRight();

	void jump();

	void update(float elapsedTime, sf::Vector2i mousPosition, sf::VertexArray& level);

	void upgradeSpeed();

	void upgradeHealth();

	void increaseHealthLevel(int amount);

	void animateMovement(float elapsedTime);
	void animateShooting(float elapsedTime);
	void animate(float elapsedTime);

	void shoot();

	float getDirection();

};

