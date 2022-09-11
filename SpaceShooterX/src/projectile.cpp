#include "projectile.h"

#include "random_extension.h"
#include "player.h"
#include "enemy.h"

void Projectile::OnAttach()
{
    s_hitProjectileSound = std::make_shared<AudioPlayer>("explosionSmall.wav");
    s_hitProjectileSound->SetVolume(0.6f);
    s_hitProjectileSound->m_destroyOnFinish = true;
    m_game->RegisterObject(s_hitProjectileSound);
}

void Projectile::OnDetach()
{
    if (!collided)
        s_hitProjectileSound->Invalidate();
}

void Projectile::OnCollide(GameObject& other)
{
    auto projectile = dynamic_cast<Projectile*>(&other);
    auto ship = dynamic_cast<Spaceship*>(&other);

    if (auto owner = s_owner.lock())
    {
        if (ship == owner.get())
            return;

        if (projectile && projectile->s_owner.lock().get() == owner.get())
            return;

        if (dynamic_cast<Enemy*>(ship) && dynamic_cast<Enemy*>(owner.get()))
            return;
    }

    if (ship || projectile)
    {
        if (ship)
            ship->Damage(1);

        if (dynamic_cast<Player*>(s_owner.lock().get()))
            m_game->m_score++;

        s_hitProjectileSound->Play();

        collided = true;
        SpawnCollisionParticles();
        Invalidate();
    }
}

std::weak_ptr<Spaceship>& Projectile::GetOwner()
{
    return s_owner;
}

void Projectile::SpawnCollisionParticles()
{
    auto partikel = std::make_shared<AnimatedObject>();

    partikel->m_rect.position = m_rect.position;
    partikel->m_destroyOnAnimationEnd = true;

    float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
    float speed = RandomUtils::random(3, 6);
    partikel->m_velocity.x = cosf(angle) * speed;
    partikel->m_velocity.y = sinf(angle) * speed;

    partikel->AddPixel(L' ');

    AnimationClip clip;
    for (int n = 0; n < 5; n++)
    {
        AnimationFrame frame({ L"O@*^."[n], FG_YELLOW, { 0, 0 } }, 0.1f);
        clip.AddFrame(std::move(frame));
    }
    partikel->AddAnimationClip("explosion", std::move(clip));

    m_game->RegisterObject(partikel);
    partikel->Play("explosion");
}
