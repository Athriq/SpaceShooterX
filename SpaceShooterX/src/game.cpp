#include "game.h"

#include "base_console.h"
#include "player.h"
#include "repair_pack.h"
#include "random_extension.h"

class StarParticle : public AnimatedObject
{
public:
    void OnAttach()
    {
        AnimationClip clip;

        for (int n = 0; n < 3; n++)
        {
            AnimationFrame frame({ L".*o"[n], FG_DARK_GREY, {0, 0} }, RandomUtils::random(1.0f, 5.0f));
            clip.AddFrame(std::move(frame));
        }

        m_collidable = false;
        m_rect.position = Vector2(RandomUtils::random(0, LEBAR_TAMPILAN), RandomUtils::random(0, TINGGI_TAMPILAN));
        m_velocity.y = 3.0f;
        AddAnimationClip("flicker", std::move(clip));
        Play("flicker", true);

        AnimatedObject::OnAttach();
    }

    void OnUpdate(float elapsed)
    {
        if (m_rect.position.y > TINGGI_TAMPILAN)
        {
            m_rect.position.y = 0;
            m_rect.position.x = rand() % LEBAR_TAMPILAN;
        }

        AnimatedObject::OnUpdate(elapsed);
    }
};

Game::Game()
{
    s_blipSound = std::make_shared<AudioPlayer>("blipSelect.wav");
    s_blipSound->SetVolume(0.7f);
}

bool Game::Update(float elapsed)
{
    HandleSpawns(elapsed);

    for (int i = 0; i < s_objects.size(); i++)
    {
        auto object = s_objects[i].get();

        // Physics
        {
            object->m_rect.position.x += object->m_velocity.x * elapsed;
            object->m_rect.position.y += object->m_velocity.y * elapsed;

            // Bound to screen area
            {
                if (object->m_rect.position.x + object->m_rect.size.x > LEBAR_TAMPILAN)
                {
                    object->m_rect.position.x = LEBAR_TAMPILAN - (object->m_rect.size.x + 1);
                    object->m_velocity.x = 0;
                }

                if (object->m_rect.position.x < 0)
                {
                    object->m_rect.position.x = 1;
                    object->m_velocity.x = 0;
                }

                if (object->lockVerticalBounds && object->m_rect.position.y + object->m_rect.size.y > TINGGI_TAMPILAN - 1)
                {
                    object->m_rect.position.y = TINGGI_TAMPILAN - (object->m_rect.size.y + 2);
                    object->m_velocity.y = 0;
                }

                if (object->lockVerticalBounds && object->m_rect.position.y < 0)
                {
                    object->m_rect.position.y = 0;
                    object->m_velocity.y = 0;
                }
            }

            for (int n = 0; n < s_objects.size(); n++)
            {
                auto nObject = s_objects[n].get();
                if (nObject == object || !nObject->m_collidable)
                    continue;

                if (object->m_rect.Intersects(nObject->m_rect))
                {
                    object->OnCollide(*nObject);
                    nObject->OnCollide(*object);
                }
            }
        }

        // Update game objects
        object->OnUpdate(elapsed);
    }

    // Remove object if marked for removal or out of screen
    s_objects.erase(std::remove_if(s_objects.begin(), s_objects.end(), [&](const std::shared_ptr<GameObject>& o)
        {
            auto outermost = o->m_rect.position + o->m_rect.size;
            bool remove = false;

            if ((o->m_rect.position.x > LEBAR_TAMPILAN && outermost.x > LEBAR_TAMPILAN) ||
                (o->m_rect.position.y < 0 && outermost.y < 0) ||
                (o->m_rect.position.y > TINGGI_TAMPILAN && outermost.y > TINGGI_TAMPILAN) ||
                o->s_markForRemoval)
            {
                o->OnDetach();
                remove = true;
            }

            return remove;
        }),
        s_objects.end());

    // Draw all game objects
    for (int i = 0; i < s_objects.size(); i++)
    {
        auto& object = s_objects[i];

        Vector2i rounded;
        rounded.x = (int)object->m_rect.position.x;
        rounded.y = (int)object->m_rect.position.y;

        for (auto& pixel : object->GetPixels())
        {
            auto pixelPos = rounded + pixel.m_posisi;
            BaseConsole::GetInstance().Draw(pixelPos.x, pixelPos.y, pixel.m_glyph, pixel.m_color);
        }
    }

    // Player destroyed
    if (s_player.expired())
    {
        s_gameOverTimer += elapsed;

        if (s_gameOverTimer >= 2)
        {
            s_gameOver = true;
            return true;
        }
    }

    return false;
}

void Game::RegisterObject(std::shared_ptr<GameObject> p_object)
{
    s_objects.push_back(p_object);
    p_object->m_game = this;
    p_object->OnAttach();
}

const std::vector<std::shared_ptr<GameObject>>& Game::GetObjects() const
{
    return s_objects;
}

const int Game::GetObjectCount() const
{
    return s_objects.size();
}

void Game::ClearObjects()
{
    s_objects.clear();
}

void Game::ResetGame()
{
    ClearObjects();

    SpawnBackgroundObjects();
    s_gameOver = false;
    s_gameOverTimer = 0;
    s_enemySpawnTimer = INTERVAL_MUSUH_MUNCUL;
    m_score = 0;
}

void Game::SpawnPlayer()
{
    if (!s_player.expired())
    {
        ShowGenericError(L"SpawnPlayer: spawning multiple instances of player is not allowed");
        return;
    }

    s_player.reset();
    auto player = std::make_shared<Player>();
    RegisterObject(player);
    s_player = player;

    s_blipSound->Play();
}

bool Game::PlayerDead() const
{
    return s_player.expired();
}

bool Game::GameOver() const
{
    return s_gameOver;
}

void Game::SpawnBackgroundObjects()
{
    for (int x = 0; x < 50; x++)
        RegisterObject(std::make_shared<StarParticle>());
}

float Game::GetTimeUntilNextWave() const
{
    return s_enemySpawnTimer;
}

float Game::GetPlayerHealth() const
{
    if (PlayerDead())
        return 0;
    
    return s_player.lock()->m_hitpoints;
}

float Game::GetDifficultyLevel() const
{
    return difficultyLevel;
}

void Game::HandleSpawns(float elapsed)
{
    s_enemySpawnTimer += elapsed;
    repairPackSpawnTimer += elapsed;

    if (difficultyLevel < INTERVAL_MUSUH_MUNCUL / 2)
        difficultyLevel += elapsed * 0.3f;

    if (s_enemySpawnTimer >= INTERVAL_MUSUH_MUNCUL)
    {
        auto enemy = std::make_shared<Spaceship>();
        enemy->m_color = FG_RED;
        enemy->AddPixel(L'◄', { 0, 0 }, enemy->m_color);
        enemy->AddPixel(0x2588, { 1, 0 }, enemy->m_color);
        enemy->AddPixel(L'∏', { 1, 1 }, enemy->m_color);
        enemy->AddPixel(L'►', { 2, 0 }, enemy->m_color);
        enemy->m_hitpoints = 5;
        enemy->rateOfFire = RandomUtils::random(0.25f, 1.0f);
        enemy->orientation = BAWAH;
        enemy->projectileSpeed = 20;
        enemy->m_velocity.y = RandomUtils::random(3.0f, 5.0f);
        enemy->m_isFiring = true;
        RegisterObject(enemy);
        SetRandomXPosNoOverlap(*enemy.get());

        s_enemySpawnTimer = RandomUtils::random(difficultyLevel, INTERVAL_MUSUH_MUNCUL);
    }

    if (repairPackSpawnTimer >= REPAIR_PACK_SPAWN_INTERVAL)
    {
        auto repairPack = std::make_shared<RepairPack>();
        RegisterObject(repairPack);
        SetRandomXPosNoOverlap(*repairPack.get());

        repairPackSpawnTimer = RandomUtils::random(difficultyLevel, REPAIR_PACK_SPAWN_INTERVAL);
    }
}

void Game::SetRandomXPosNoOverlap(GameObject& object)
{
    object.m_rect.position.x = RandomUtils::random(2, LEBAR_TAMPILAN - 4);

    for (int i = 0; i < s_objects.size(); i++)
    {
        auto& other = s_objects[i];
        if (other.get() == &object || !other->m_collidable)
            continue;

        if (object.m_rect.Intersects(other->m_rect))
            SetRandomXPosNoOverlap(object);
    }
}
