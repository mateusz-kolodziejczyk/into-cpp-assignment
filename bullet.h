
#ifndef bullet_hpp
#define bullet_hpp

#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:
    Vector2f m_Position;

    RectangleShape m_BulletShape;

    float m_BulletSpeed = 500;

    float m_MaxX;
    float m_MinX;

    bool m_MovingRight;

public:
    Bullet();
    Bullet(float startX, float startY, bool movingRight, const IntRect& Arena);

    FloatRect getPosition();

    RectangleShape getShape();

    bool update(float elapsedTime);

};


#endif /* bullet_hpp */
