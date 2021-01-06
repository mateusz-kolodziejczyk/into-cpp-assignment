//
//  pickup.cpp
//  Zombie Arena
//

#include "pickup.h"
#include "textureHolder.h"
using namespace sf;

// Spawn a pickup of a specific type at a specific position
Pickup::Pickup(PickupType type, Vector2f position)
{
	m_Type = type;
	switch (m_Type) {
	case PickupType::FIRE_RATE:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/fire_rate_pickup.png"));
		m_Value = FIRE_RATE_MODIFIER;
		break;
	case PickupType::HEALTH:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));
		m_Value = 0;
		break;
	case PickupType::SPEED:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/speed_pickup.png"));
		m_Value = 0;
		break;
	default:
		break;
	}
	m_Sprite.setOrigin(15, 15);
	m_Position = position;
	m_Sprite.setPosition(m_Position);
	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsSinceSpawn = 0;

	// code here

}


FloatRect Pickup::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Pickup::getSprite()
{
	return m_Sprite;
}


float Pickup::gotIt()
{
	return m_Value;
}

bool Pickup::update(float elapsedTime)
{

	m_SecondsSinceSpawn += elapsedTime;

	// Does the pikcup needs despawning
	if (m_SecondsSinceSpawn >= m_SecondsToLive) {
		return true;
	}
	return false;
	//Code Here

}

PickupType Pickup::getType() {
	return m_Type;
}
