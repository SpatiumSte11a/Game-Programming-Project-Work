#pragma once
#include <chrono>
#include <string>
#include <windows.h>
#include "WindowContext.h"
#include "GraphicsContext.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "StartupScreen.h"

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

    static const int EnemyCount = 3;
    Enemy Enemies[EnemyCount];

public:
    Game();

    bool Initialize(HINSTANCE hInstance);
    void Run();

private:
    float GetDeltaTime();

    void ResetGame();
    void SetupEnemies();
    void LoseLifeAndStartRespawn();

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