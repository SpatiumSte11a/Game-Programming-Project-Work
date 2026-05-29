#pragma once
#include <windows.h>
#include <chrono>
#include <string>

#include "WindowContext.h"
#include "GraphicsContext.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "StartupScreen.h"
#include "WaveData.h"

enum class GameState
{
    Startup,
    Playing,
    Paused,
    GameOverWait,
    Exit
};

struct Star
{
    float X = 0.0f;
    float Y = 0.0f;
    float Speed = 0.0f;
    float Size = 0.0f;
};

class Game
{
private:
    WindowContext Window;
    GraphicsContext Graphics;
    StartupScreen StartScreen;

    bool IsRunning;
    GameState CurrentState;

    std::chrono::high_resolution_clock::time_point PrevTime;

    float TitleUpdateTimer;
    int FrameCounter;
    float GlobalTime;

    int PlayerLives;
    int Score;

    bool IsRespawning;
    float RespawnTimer;
    float RespawnDelay;

    float InvincibleTimer;
    float InvincibleDuration;

    float GameOverTimer;
    float GameOverDelay;

    static const int StarCount = 80;
    Star Stars[StarCount];

    Player PlayerObject;
    PlayerBulletSystem PlayerBulletSystemObject;
    EnemyBulletSystem EnemyBulletSystemObject;

    static const int EnemyCount = WaveMaxEnemies;
    Enemy Enemies[EnemyCount];
    bool EnemySlotsActive[EnemyCount];

    Enemy BonusEnemyObject;
    bool IsBonusEnemyActive;
    float BonusEnemySpeed;

    int CurrentWave;
    bool IsWaveTransition;
    float WaveTransitionTimer;
    float WaveTransitionDelay;

    bool IsNextWaveSpawnDelay;
    float NextWaveSpawnDelayTimer;
    float NextWaveSpawnDelay;

public:
    Game();

    bool Initialize(HINSTANCE hInstance);
    void Run();

private:
    float GetDeltaTime();

    void ResetGame();
    void SetupEnemiesForWave(int wave);
    bool AreAllEnemiesDefeated() const;
    void LoseLifeAndStartRespawn();

    void SpawnBonusEnemy();
    void UpdateBonusEnemy(float dt);
    void HandlePlayerBulletVsBonusEnemyCollision();

    void Input();
    void Update(float dt);
    void Render();

    void RenderLives();
    void RenderStars();
    void InitStars();

    void HandlePlayerBulletVsEnemyCollision();
    bool HandleEnemyBulletVsPlayerCollision();
    bool HandlePlayerVsEnemyCollision();
    bool HandlePlayerCaptureCollision();
};