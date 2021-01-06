

#ifndef pickup_hpp
#define pickup_hpp

#include <SFML/Graphics.hpp>


enum class PickupType{HEALTH, FIRE_RATE, SPEED};
class Pickup
{
private:
    // Value for fire rate pickups
    const float FIRE_RATE_MODIFIER = 0.2;
    const int START_SECONDS_TO_LIVE = 10;

    // The sprite that represents this pickup
    sf::Sprite m_Sprite;

    // The arena it exists in
    sf::IntRect m_Arena;

    // How much is this pickup worth?
    float m_Value;

    // What type of pickup is this?
    // 1 = health, 2 = ammo
    PickupType m_Type;

    // Handle spawning and disappearing
    float m_SecondsSinceSpawn;
    float m_SecondsToLive;

    sf::Vector2f m_Position;


public:

    Pickup(PickupType type, sf::Vector2f position);


    PickupType getType();

    // Check the position of a pickup
    sf::FloatRect getPosition();

    // Get the sprite for drawing
    sf::Sprite getSprite();

    // Let the pickup update itself each frame
    bool update(float elapsedTime);

    // Get the goodness from the pickup
    float gotIt();

};

#endif /* pickup_hpp */

