//
//  zombie.h
//  Zombie Arena
//
//

#ifndef zombie_hpp
#define zombie_hpp

#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private:
    // How fast is each zombie type?
    const float REGULAR_SPEED = 100;
    const float FAST_SPEED = 150;
    const float SLOW_SPEED = 60;

    // How tough is each zombie type
    const float REGULAR_HEALTH = 3;
    const float FAST_HEALTH = 2;
    const float SLOW_HEALTH = 5;

    // Make each zombie vary its speed slightly
    const int MAX_VARIANCE = 30;
    const int OFFSET = 101 - MAX_VARIANCE;

    // Where is this zombie?
    Vector2f m_Position;
    // Last positionof the zombie used for velocity calculation
    Vector2f m_LastPosition;

    // A sprite for the zombie
    Sprite m_Sprite;

    // How fast can this one run/crawl?
    float m_Speed;

    // How much health has it got?
    float m_Health;

    // Is it still alive?
    bool m_Alive = true;

    // Is the zombie moving left or right?
    bool m_MovingLeft = true;

    // The animation should only switch once every x time
    float m_AnimationTimer = 0;
    float m_AnimationLength = 0.1;

    // Use this to switch between the walk animations.
    int m_WalkAnimation = 1;

    // Public prototypes go here
public:
    // Zombies will either spawn on the right or left, needing their direction set at the start
    Zombie(bool movingLeft);

    // Handle when a bullet hits a zombie
    bool takeDamage();

    // Spawn a new zombie
    void spawn(float startX, float startY, int type);

    // Return a rectangle that is the position in the world
    FloatRect getPosition();

    // Get a copy of the sprite to draw
    Sprite getSprite();

    // Stop the zombie from moving when they attack a player
    void stopMoving();
    // Make the zombie move again when the player moves away.
    void startMoving();

    // Animate movement of the zombie
    void animateMovement(float elapsedTime);
    // Update the zombie each frame
    void update(float elapsedTime, Vector2f playerLocation, const VertexArray& level, IntRect& arena, int tileSize);
};

#endif /* zombie_hpp */


