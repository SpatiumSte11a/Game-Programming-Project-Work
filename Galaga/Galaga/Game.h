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

enum class GameState
{
    Startup, 
    Playing, 
    Paused, 
    Exit
};

//scrolling starfield
struct Star
{
    float X; 
    float Y; 
    float Speed; 
    float Size; 
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
    void Input();
    void Update(float dt);
    void Render();
    void RenderLives(); 
    void RenderStars(); 
    void InitStars(); 
};
