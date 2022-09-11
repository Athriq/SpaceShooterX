#pragma once

#include "game_object.h"

class ParticleSystem : public GameObject
{
public:
    ParticleSystem(size_t p_particlesCount);

    void OnUpdate(float elapsed) override;
    void OnDraw() override;

public:
    bool m_emitting = true;

    bool m_emitOnce = false;
    float m_lifetime = 1.0f;
    float m_lifetimeRandomness = 0.0f;
    bool m_localSpace = false;

    float m_initialVelocity = 0.0f;
    Vector2f m_direction;
    float m_spread = 45.0f;
    Vector2f m_gravity;

private:
    struct Particle
    {
        Vector2f position;
        Vector2f velocity;
        short color;
        float time;
        float lifetime;
        bool active = false;
    };

    std::vector<Particle> s_particles;
    float s_time = 0.0f;
};