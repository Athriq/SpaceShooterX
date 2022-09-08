#pragma once

#include "animated_object.h"
#include "audio_player.h"

class Projectile;

enum ORIENTATION { ATAS = 0, BAWAH = 1 };

class Spaceship : public AnimatedObject, public std::enable_shared_from_this<Spaceship>
{
public:
    void OnAttach();
    void OnDetach();
    void OnUpdate(float elapsed);
    void OnCollide(GameObject& other);

protected:
    virtual void Shoot();
    virtual void Destroy();

    virtual std::shared_ptr<Projectile> CreateProjectile();

public:
    int m_hitpoints = 100;
    float rateOfFire = 0.15f;
    bool m_isFiring;
    ORIENTATION orientation = ATAS;
    float projectileSpeed = 10;
    short m_color = FG_WHITE;

protected:
    float shootCooldown = 0;
    float flashTimer;
    bool m_flashDamage = true;
    std::vector<short> m_tempColor;

private:
    std::shared_ptr<AudioPlayer> s_explosionSound;
    std::shared_ptr<AudioPlayer> s_hitSound;
};
