#include "Game.h"

Game::Game()
    : Window(L"Galaga"),
    IsRunning(true),
    TitleUpdateTimer(0.0f),
    FrameCounter(0),
    PlayerX(0.0f),
    PlayerY(-0.80f),
    PlayerSpeed(0.9f)
{
    PrevTime = std::chrono::high_resolution_clock::now();
}

bool Game::Initialize(HINSTANCE hInstance)
{
    if (!Window.Initialize(hInstance, 720, 960))
        return false;

    if (!Graphics.Initialize(Window.hWnd, 720, 960))
        return false;

    return true;
}

float Game::GetDeltaTime()
{
    auto currTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(currTime - PrevTime).count();
    PrevTime = currTime;
    return dt;
}

void Game::Run()
{
    MSG msg = {};

    while (msg.message != WM_QUIT && IsRunning)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            float dt = GetDeltaTime();

            Input();
            Update(dt);
            Render();

            FrameCounter++;
            TitleUpdateTimer += dt;

            if (TitleUpdateTimer >= 0.5f)
            {
                float fps = FrameCounter / TitleUpdateTimer;

                std::wstring title = L"Galaga | FPS: " + std::to_wstring((int)fps);
                SetWindowText(Window.hWnd, title.c_str());

                TitleUpdateTimer = 0.0f;
                FrameCounter = 0;
            }
        }
    }
}

void Game::Input()
{
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        IsRunning = false;
        DestroyWindow(Window.hWnd);
    }
}

void Game::Update(float dt)
{
    if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))
        PlayerX -= PlayerSpeed * dt;

    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000))
        PlayerX += PlayerSpeed * dt;

    if (PlayerX < -0.9f)
        PlayerX = -0.9f;

    if (PlayerX > 0.9f)
        PlayerX = 0.9f;
}

void Game::Render()
{
    Graphics.BeginFrame();
    Graphics.DrawTriangle(PlayerX, PlayerY);
    Graphics.EndFrame();
}