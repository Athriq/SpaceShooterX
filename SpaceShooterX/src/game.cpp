#include "game.h"

#include "base_console.h"
#include "player.h"
#include "enemy.h"
#include "repair_pack.h"
#include "random_extension.h"

Game::Game()
{
    s_blipSound = std::make_shared<AudioPlayer>("blipSelect.wav");
    s_blipSound->SetVolume(0.7f);

    ResetGame();
    SpawnPlayer();
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

    // Draw background stars
    for (size_t i = 0; i < s_bgStars.size(); i++)
    {
        s_bgStars[i].y += (i < s_bgStars.size() / 2 ? m_scrollSpeed : m_scrollSpeed * 0.1f) * elapsed;

        if (s_bgStars[i].y > TINGGI_TAMPILAN)
        {
            s_bgStars[i].y = 0;
            s_bgStars[i].x = rand() % LEBAR_TAMPILAN;
        }

        BaseConsole::GetInstance().Draw(s_bgStars[i].x, s_bgStars[i].y, L'∙', FG_GREY);
    }

    // Draw all game objects
    for (int i = 0; i < s_objects.size(); i++)
        s_objects[i]->OnDraw();

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
    for (int x = 0; x < 300; x++)
        s_bgStars.push_back(Vector2f(RandomUtils::random(0, LEBAR_TAMPILAN), RandomUtils::random(0, TINGGI_TAMPILAN)));
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
        auto enemy = std::make_shared<Enemy>();
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
