#pragma once
#include <windows.h>
#include <chrono>
#include <string>
#include "WindowContext.h"
#include "GraphicsContext.h"

class Game
{
private:
    WindowContext Window;
    GraphicsContext Graphics;
    bool IsRunning;

    std::chrono::high_resolution_clock::time_point PrevTime;

    float TitleUpdateTimer;
    int FrameCounter;

    float PlayerX;
    float PlayerY;
    float PlayerSpeed;

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