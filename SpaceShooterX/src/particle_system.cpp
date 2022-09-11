#include "particle_system.h"

#include "base_console.h"
#include "math/math.h"

ParticleSystem::ParticleSystem(size_t p_particlesCount)
{
    s_particles.resize(p_particlesCount);
    m_collidable = false;
}

void ParticleSystem::OnUpdate(float elapsed)
{
    float prevTime = s_time;

    s_time += elapsed;

    if (s_time > m_lifetime)
    {
        s_time = fmod(s_time, m_lifetime);

        if (m_emitOnce)
            m_emitting = false;
    }

    for (size_t i = 0; i < s_particles.size(); i++)
    {
        auto& particle = s_particles[i];

        if (!m_emitting && !particle.active)
            continue;

        float localElapsed = elapsed;
        float restartPhase = (float)i / (float)s_particles.size();

        float restartTime = restartPhase * m_lifetime;
        bool restart = false;

        if (s_time > prevTime)
        {
            if (restartTime >= prevTime && restartTime < s_time)
            {
                restart = true;
                localElapsed = s_time - restartTime;
            }

        }
        else if (localElapsed > 0.0)
        {
            if (restartTime >= prevTime)
            {
                restart = true;
                localElapsed = m_lifetime - restartTime + s_time;
            }
            else if (restartTime < s_time)
            {
                restart = true;
                localElapsed = s_time - restartTime;
            }
        }

        if (restart)
        {
            if (!m_emitting)
            {
                particle.active = false;
                continue;
            }

            particle.active = true;
            particle.time = 0;
            particle.lifetime = m_lifetime * (1.0f - Math::Randf() * m_lifetimeRandomness);


            float angle = m_direction.Angle() + Math::ConvertToRadians((Math::Randf() * 2.0f - 1.0f) * m_spread);
            Vector2f direction(cosf(angle), sinf(angle));
            particle.velocity = direction * m_initialVelocity;

            particle.position = m_localSpace ? Vector2f() : m_rect.position;
        }
        else if (!particle.active)
        {
            continue;
        }
        else if (particle.time > particle.lifetime)
        {
            particle.active = false;
        }
        else
        {
            particle.time += localElapsed;
            particle.velocity += m_gravity * localElapsed;
        }

        particle.color = particle.time <= particle.lifetime / 2 ? m_initialColor : m_endColor;
        particle.position += particle.velocity * localElapsed;
    }
}

void ParticleSystem::OnDraw()
{
    for (auto& particle : s_particles)
    {
        if (!particle.active)
            continue;

        float xPos = m_localSpace ? m_rect.position.x + particle.position.x : particle.position.x;
        float yPos = m_localSpace ? m_rect.position.y + particle.position.y : particle.position.y;
        BaseConsole::GetInstance().Draw(xPos, yPos, L'▪', particle.color);
    }
}

void ParticleSystem::SetParticleCount(int p_amount)
{
    s_particles.resize(p_amount);
}
