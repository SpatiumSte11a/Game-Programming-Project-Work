#pragma once
#include <windows.h>
#include <chrono>
#include <string>
#include "WindowContext.h"
#include "GraphicsContext.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class Game
{
private:
    WindowContext Window;
    GraphicsContext Graphics;
    bool IsRunning;

    std::chrono::high_resolution_clock::time_point PrevTime;

    float TitleUpdateTimer;
    int FrameCounter;

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

    void Input();
    void Update(float dt);
    void Render();
};