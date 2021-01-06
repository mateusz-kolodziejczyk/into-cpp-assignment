//
//  bullet.cpp
//  Zombie Arena
//
//

#include "bullet.h"
#include <iostream>

using namespace sf;
// The constructor
Bullet::Bullet()
{
    m_BulletShape.setSize(Vector2f(10, 10));
}

Bullet::Bullet(float startX, float StartY, bool movingRight, const IntRect& arena) :
    m_MovingRight(movingRight)
{
    m_Position.x = startX;
    m_Position.y = StartY;
    m_BulletShape.setSize(Vector2f(8, 8));
    m_MinX = 0;
    m_MaxX = arena.width;
}

FloatRect Bullet::getPosition()
{
    return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
    return m_BulletShape;
}


// Returns whether the bullet has reached the end and has to be deleted
bool Bullet::update(float elapsedTime)
{
    // Update the bullet position variables
    m_Position.x +=  m_BulletSpeed * elapsedTime * (m_MovingRight ? 1 : -1);

    // Move the bullet
    m_BulletShape.setPosition(m_Position);
    //std::cout << "\nPosition: (" << m_Position.x << "," << m_Position.y << ")";
    if (m_Position.x > m_MaxX || m_Position.x < m_MinX) {
        return true;
    }
    return false;

}

