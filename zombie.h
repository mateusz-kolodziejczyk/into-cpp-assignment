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
    const float BLOATER_SPEED = 40;
    const float CHASER_SPEED = 80;
    const float CRAWLER_SPEED = 20;

    // How tough is each zombie type
    const float BLOATER_HEALTH = 5;
    const float CHASER_HEALTH = 1;
    const float CRAWLER_HEALTH = 3;

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

    // The animation should only switch once every x time  elapsed
    float animationTimer = 0;

    // Use this to switch between the walk animations.
    int walkAnimation = 1;

    // Public prototypes go here
public:

    // Handle when a bullet hits a zombie
    bool hit();

    // Find out if the zombie is alive
    bool isAlive();

    // Spawn a new zombie
    void spawn(float startX, float startY, int type, int seed);

    // Return a rectangle that is the position in the world
    FloatRect getPosition();

    // Get a copy of the sprite to draw
    Sprite getSprite();

    // Animate movement of the zombie
    void animateMovement();
    // Update the zombie each frame
    void update(float elapsedTime, Vector2f playerLocation, const VertexArray& level, IntRect& arena, int tileSize);
};

#endif /* zombie_hpp */


