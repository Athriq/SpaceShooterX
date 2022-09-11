#pragma once

#include "common.h"

class GameObject;
class AnimatedObject;
class Player;
class AudioPlayer;

struct Vector2f;

/**
 * @brief This is the game world. Manages game logic and contains current world information.
 *
 */
class Game
{
public:
    Game();

    /**
     *  @brief The "heart" of the game's process. Returns exit code.
     */
    bool Update(float elapsed);

    /**
     * @brief Registers a game object to be managed by the current game instance.
     */ 
    void RegisterObject(std::shared_ptr<GameObject> p_object);

    const std::vector<std::shared_ptr<GameObject>>& GetObjects() const;
    const int GetObjectCount() const;
    void ClearObjects();

    void ResetGame();
    void SpawnPlayer();
    void SpawnBackgroundObjects();

    bool PlayerDead() const;
    bool GameOver() const;

    float GetTimeUntilNextWave() const;
    float GetPlayerHealth() const;
    float GetDifficultyLevel() const;

private:
    void HandleSpawns(float elapsed);
    void SetRandomXPosNoOverlap(GameObject& object);

public:
    int m_score = 0;
    
    // TODO: implement
    float m_masterVolume = 1.0f;

    float m_scrollSpeed = 1.5f;

private:
    // TODO: do a major refactor, find a better approach than having a vector shared ptrs
    std::vector<std::shared_ptr<GameObject>> s_objects;
    std::vector<Vector2f> s_bgStars;

    std::weak_ptr<Player> s_player;
    std::shared_ptr<AudioPlayer> s_blipSound;

    bool s_gameOver = false;
    float s_gameOverTimer = 0;
    float s_enemySpawnTimer = 0;
    float difficultyLevel = 0;
    float repairPackSpawnTimer = 0;
};
