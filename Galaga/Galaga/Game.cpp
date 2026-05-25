#include "Game.h"

Game::Game()
    : Window(L"Galaga"),
    IsRunning(true),
    TitleUpdateTimer(0.0f),
    FrameCounter(0)
{
    PrevTime = std::chrono::high_resolution_clock::now();
}

bool Game::Initialize(HINSTANCE hInstance)
{
    if (!Window.Initialize(hInstance, 720, 960))
        return false;

    if (!Graphics.Initialize(Window.hWnd, 720, 960))
        return false;

    float enemyX = 0.0f;
    float visualGap = 0.04f;

    float squareY = 0.42f;
    float squareTop = 0.05f;

    float downTriangleTop = 0.02f;
    float downTriangleBottom = 0.08f;

    float diamondBottom = 0.063f;

    float triangleY = squareY + squareTop + visualGap + downTriangleBottom;
    float diamondY = triangleY + downTriangleTop + visualGap + diamondBottom;

    Enemies[0].SetType(EnemyType::Type1);
    Enemies[0].SetPosition(enemyX, squareY);

    Enemies[1].SetType(EnemyType::Type2);
    Enemies[1].SetPosition(enemyX, triangleY);

    Enemies[2].SetType(EnemyType::Type3);
    Enemies[2].SetPosition(enemyX, diamondY);

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
    bool moveLeft = (GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000);
    bool moveRight = (GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000);
    bool shootPressed = (GetAsyncKeyState(VK_SPACE) & 0x8000);

    PlayerObject.Update(dt, moveLeft, moveRight);
    PlayerBulletSystemObject.Update(dt, shootPressed, PlayerObject.GetX(), PlayerObject.GetY());

    bool type2CanShoot = false;
    float type2X = 0.0f;
    float type2Y = 0.0f;

    for (int i = 0; i < EnemyCount; i++)
    {
        Enemies[i].Update(dt);

        if (Enemies[i].GetIsAlive() && Enemies[i].GetType() == EnemyType::Type2)
        {
            type2CanShoot = true;
            type2X = Enemies[i].GetX();
            type2Y = Enemies[i].GetY();
        }
    }

    EnemyBulletSystemObject.Update(dt, type2CanShoot, type2X, type2Y);
}

void Game::Render()
{
    Graphics.BeginFrame();

    Graphics.DrawTriangle(PlayerObject.GetX(), PlayerObject.GetY(), 1.0f, 1.0f);
    PlayerBulletSystemObject.Render(Graphics);

    for (int i = 0; i < EnemyCount; i++)
    {
        if (!Enemies[i].GetIsAlive())
            continue;

        switch (Enemies[i].GetType())
        {
        case EnemyType::Type1:
            Graphics.DrawQuad(Enemies[i].GetX(), Enemies[i].GetY(), 1.0f, 1.0f);
            break;

        case EnemyType::Type2:
            Graphics.DrawDownTriangle(Enemies[i].GetX(), Enemies[i].GetY(), 1.0f, 1.0f);
            break;

        case EnemyType::Type3:
            Graphics.DrawDiamond(Enemies[i].GetX(), Enemies[i].GetY(), 0.9f, 0.9f);
            break;
        }
    }

    EnemyBulletSystemObject.Render(Graphics);

    Graphics.EndFrame();
}