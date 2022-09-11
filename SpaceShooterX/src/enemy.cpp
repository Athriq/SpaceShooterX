#include "enemy.h"

#include "random_extension.h"

void Enemy::OnAttach()
{
    m_color = FG_RED;
    AddPixel(L'◄', { 0, 2 }, m_color);
    AddPixel(L'v', { 1, 1 }, m_color);
    AddPixel(0x2588, { 1, 2 }, m_color);
    AddPixel(L'∏', { 1, 3 }, m_color);
    AddPixel(L'►', { 2, 2 }, m_color);
    m_thrusterPointOffset.x = 1;
    m_thrusterPointOffset.y = 0;

    m_hitpoints = 5;
    rateOfFire = RandomUtils::random(0.25f, 1.0f);
    orientation = BAWAH;
    projectileSpeed = 25;
    m_velocity.y = RandomUtils::random(3.0f, 5.0f);
    m_isFiring = true;

    Spaceship::OnAttach();

    s_thrusterEmitter->m_direction = Vector2f(0, -1);
    s_thrusterEmitter->m_initialVelocity = 3.0f;
}