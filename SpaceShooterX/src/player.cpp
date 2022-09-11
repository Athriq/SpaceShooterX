#include "player.h"

#include "base_console.h"
#include "projectile.h"

void Player::OnAttach()
{
    m_color = FG_GREEN;
    AddPixel(L'Δ', { 1, 0 }, m_color);
    AddPixel(L'╱', { 0, 1 }, m_color);
    AddPixel(L'͞', { 0, 2 }, m_color);
    AddPixel(L'█', { 1, 1 }, m_color);
    AddPixel(L'║', { 1, 2 }, m_color);
    AddPixel(L'^', { 1, 3 }, m_color);
    AddPixel(L'╲', { 2, 1 }, m_color);
    AddPixel(L'͞', { 2, 2 }, m_color);
    m_thrusterPointOffset.x = 1;
    m_thrusterPointOffset.y = 4;

    m_hitpoints = MAX_PLAYER_HEALTH;
    projectileSpeed = 30;
    rateOfFire = LAJU_TEMBAK;
    m_rect.position.x = LEBAR_TAMPILAN / 2;
    m_rect.position.y = TINGGI_TAMPILAN - 15;
    lockVerticalBounds = true;

    s_shootSound = std::make_shared<AudioPlayer>("laserShoot.wav");
    s_shootSound->SetVolume(0.7f);
    m_game->RegisterObject(s_shootSound);

    s_healthBoostSound = std::make_shared<AudioPlayer>("powerUp.wav");
    s_healthBoostSound->SetVolume(0.65f);
    s_healthBoostSound->m_destroyOnFinish = true;
    m_game->RegisterObject(s_healthBoostSound);

    s_thrusterSound = std::make_shared<AudioPlayer>("thruster.wav");
    s_thrusterSound->m_loop = true;
    s_thrusterSound->SetVolume(0.0f);
    s_thrusterSound->Play();
    m_game->RegisterObject(s_thrusterSound);

    Spaceship::OnAttach();

    // Needs the preceding init above before this can be done
    s_thrusterEmitter->m_direction = Vector2f(0, 1);
}

void Player::OnDetach()
{
    s_shootSound->Invalidate();
    s_thrusterSound->Invalidate();
    Spaceship::OnDetach();
}

void Player::OnUpdate(float elapsed)
{
    for (int i = 0; i < 10; i++)
        keys[i] = GetAsyncKeyState("\x25\x27\x26\x28\x20\x57\x41\x53\x44\x10"[i]);

    if (m_velocity.x != 0) m_velocity.x = 0;
    if (m_velocity.y != 0) m_velocity.y = 0;
    if (keys[0] || keys[6]) m_velocity.x = -13.0f;
    if (keys[1] || keys[8]) m_velocity.x = 13.0f;
    if (keys[2] || keys[5]) m_velocity.y = -13.0f;
    if (keys[3] || keys[7]) m_velocity.y = 13.0f;
    if (keys[9]) m_velocity *= 2.0f;

    m_isFiring = keys[4];

    m_rect.position.y += m_game->m_scrollSpeed * elapsed;

    bool movingUpwards = m_velocity.x != 0 || m_velocity.y < 0;
    s_thrusterEmitter->m_spread = movingUpwards < 0 ? 25.0f : 65.0f;
    s_thrusterEmitter->m_initialVelocity = movingUpwards ? 20.0f : 5.0f;
    s_thrusterSound->SetVolume(movingUpwards ? 0.8f : 0.0f);

    Spaceship::OnUpdate(elapsed);
}

void Player::GiveHealth(float p_value)
{
    s_healthBoostSound->Play();

    m_hitpoints += p_value;

    if (m_hitpoints > MAX_PLAYER_HEALTH)
        m_hitpoints = MAX_PLAYER_HEALTH;
}

void Player::Shoot()
{
    s_shootSound->Play();
    Spaceship::Shoot();
}

std::shared_ptr<Projectile> Player::CreateProjectile()
{
    return std::make_shared<Projectile>(std::static_pointer_cast<Player>(weak_from_this().lock()));
}
