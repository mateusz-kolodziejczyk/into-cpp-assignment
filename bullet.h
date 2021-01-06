
#ifndef bullet_hpp
#define bullet_hpp

#include <SFML/Graphics.hpp>


class Bullet
{
private:
    sf::Vector2f m_Position;

    sf::RectangleShape m_BulletShape;

    float m_BulletSpeed = 500;

    float m_MaxX;
    float m_MinX;

    bool m_MovingRight;

public:
    Bullet();
    Bullet(float startX, float startY, bool movingRight, const sf::IntRect& Arena);

    sf::FloatRect getPosition();

    sf::RectangleShape getShape();

    bool update(float elapsedTime);

};


#endif /* bullet_hpp */
