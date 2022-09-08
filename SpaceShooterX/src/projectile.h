#pragma once

#include "game_object.h"
#include "spaceship.h"

class Projectile : public GameObject
{
public:
    Projectile(const std::weak_ptr<Spaceship>& p_owner)
    {
        s_owner = p_owner;
        AddPixel(L'│', { 0, 0 }, FG_YELLOW);
    }

    void OnAttach();
    void OnDetach();
    void OnCollide(GameObject& other);

    std::weak_ptr<Spaceship>& GetOwner();

private:
    void SpawnCollisionParticles();

private:
    std::weak_ptr<Spaceship> s_owner;
    std::shared_ptr<AudioPlayer> s_hitProjectileSound;
    bool collided = false;
};
