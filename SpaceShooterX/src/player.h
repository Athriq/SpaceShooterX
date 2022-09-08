#pragma once

#include "spaceship.h"

class Player : public Spaceship
{
public:
    void OnAttach();
    void OnDetach();
    void OnUpdate(float elapsed);
    
    void GiveHealth(float p_value);

protected:
    void Shoot();

    std::shared_ptr<Projectile> CreateProjectile();

private:
    int hitCounter;
    bool keys[10];
    std::shared_ptr<AudioPlayer> s_shootSound;
    std::shared_ptr<AudioPlayer> s_healthBoostSound;
};