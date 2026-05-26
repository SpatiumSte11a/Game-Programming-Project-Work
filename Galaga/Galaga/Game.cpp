#include "Game.h"
#include "Collision.h"

Game::Game()
    : Window(L"Galaga"),
    IsRunning(true),
    CurrentState(GameState::Startup),
    TitleUpdateTimer(0.0f),
    FrameCounter(0),
    GlobalTime(0.0f),
    PlayerLives(3),
    IsRespawning(false),
    RespawnTimer(0.0f),
    RespawnDelay(1.5f),
    InvincibleTimer(0.0f),
    InvincibleDuration(1.0f),
    GameOverTimer(0.0f),
    GameOverDelay(10.0f)
{
    PrevTime = std::chrono::high_resolution_clock::now();
}

bool Game::Initialize(HINSTANCE hInstance)
{
    if (!Window.Initialize(hInstance, 720, 960))
        return false;

    if (!Graphics.Initialize(Window.hWnd, 720, 960))
        return false;

    ResetGame();
    return true;
}

void Game::SetupEnemies()
{
    float enemyX = 0.0f;
    float visualGap = 0.04f;

    float squareY = 0.42f;
    float squareTop = 0.05f;

    float downTriangleTop = 0.02f;
    float downTriangleBottom = 0.08f;

    float diamondBottom = 0.063f;

    float triangleY = squareY + squareTop + visualGap + downTriangleBottom;
    float diamondY = triangleY + downTriangleTop + visualGap + diamondBottom;

    Enemies[0] = Enemy();
    Enemies[0].SetType(EnemyType::Type1);
    Enemies[0].SetPosition(enemyX, squareY);

    Enemies[1] = Enemy();
    Enemies[1].SetType(EnemyType::Type2);
    Enemies[1].SetPosition(enemyX, triangleY);

    Enemies[2] = Enemy();
    Enemies[2].SetType(EnemyType::Type3);
    Enemies[2].SetPosition(enemyX, diamondY);
}

void Game::ResetGame()
{
    CurrentState = GameState::Startup;
    GlobalTime = 0.0f;
    PlayerLives = 3;

    IsRespawning = false;
    RespawnTimer = 0.0f;
    InvincibleTimer = 0.0f;

    GameOverTimer = 0.0f;

    PlayerObject = Player();
    PlayerBulletSystemObject = PlayerBulletSystem();
    EnemyBulletSystemObject = EnemyBulletSystem();

    SetupEnemies();
}

void Game::LoseLifeAndStartRespawn()
{
    if (IsRespawning || CurrentState == GameState::GameOverWait)
        return;

    if (PlayerLives > 0)
        PlayerLives--;

    if (PlayerLives <= 0)
    {
        PlayerLives = 0;
        IsRespawning = false;
        RespawnTimer = 0.0f;
        InvincibleTimer = 0.0f;
        GameOverTimer = GameOverDelay;
        CurrentState = GameState::GameOverWait;

        PlayerObject = Player();
        PlayerBulletSystemObject = PlayerBulletSystem();
        return;
    }

    IsRespawning = true;
    RespawnTimer = RespawnDelay;

    PlayerObject = Player();
    PlayerBulletSystemObject = PlayerBulletSystem();
    EnemyBulletSystemObject = EnemyBulletSystem();
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
        return;
    }

    if (GetAsyncKeyState('R') & 0x0001)
    {
        ResetGame();
        return;
    }

    if (CurrentState == GameState::Startup)
    {
        if (GetAsyncKeyState(VK_RETURN) & 0x0001)
            CurrentState = GameState::Playing;
        return;
    }

    if (CurrentState == GameState::GameOverWait)
        return;

    if (GetAsyncKeyState('P') & 0x0001)
    {
        if (CurrentState == GameState::Playing)
            CurrentState = GameState::Paused;
        else if (CurrentState == GameState::Paused)
            CurrentState = GameState::Playing;
    }
}

void Game::Update(float dt)
{
    GlobalTime += dt;

    if (InvincibleTimer > 0.0f)
        InvincibleTimer -= dt;

    if (CurrentState == GameState::Startup || CurrentState == GameState::Paused)
        return;

    if (IsRespawning)
    {
        RespawnTimer -= dt;

        if (RespawnTimer <= 0.0f)
        {
            IsRespawning = false;
            PlayerObject = Player();
            InvincibleTimer = InvincibleDuration;
        }
    }

    if (CurrentState == GameState::GameOverWait)
    {
        GameOverTimer -= dt;

        if (GameOverTimer <= 0.0f)
        {
            ResetGame();
            return;
        }
    }

    bool isCaptured = false;
    for (int i = 0; i < EnemyCount; i++)
    {
        if (Enemies[i].GetIsPlayerCaptured())
        {
            isCaptured = true;
            break;
        }
    }

    bool canControlPlayer =
        CurrentState == GameState::Playing &&
        !IsRespawning &&
        !isCaptured;

    bool moveLeft = canControlPlayer && ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000));
    bool moveRight = canControlPlayer && ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000));
    bool shootPressed = canControlPlayer && (GetAsyncKeyState(VK_SPACE) & 0x8000);

    if (!IsRespawning && CurrentState == GameState::Playing)
        PlayerObject.Update(dt, moveLeft, moveRight);

    float enemyTargetX = (IsRespawning || CurrentState == GameState::GameOverWait) ? 100.0f : PlayerObject.GetX();
    float enemyTargetY = (IsRespawning || CurrentState == GameState::GameOverWait) ? -100.0f : PlayerObject.GetY();

    float pX = PlayerObject.GetX();
    float pY = PlayerObject.GetY();

    for (int i = 0; i < EnemyCount; i++)
    {
        if (Enemies[i].GetIsPlayerCaptured())
        {
            pX = Enemies[i].GetX() + 0.12f;
            pY = Enemies[i].GetY();
        }
    }

    PlayerBulletSystemObject.Update(dt, shootPressed, pX, pY);

    bool type2CanShoot = false;
    float type2X = 0.0f;
    float type2Y = 0.0f;

    for (int i = 0; i < EnemyCount; i++)
    {
        Enemies[i].Update(dt, enemyTargetX, enemyTargetY);

        if (Enemies[i].GetIsAlive() && Enemies[i].GetType() == EnemyType::Type2)
        {
            type2CanShoot = true;
            type2X = Enemies[i].GetX();
            type2Y = Enemies[i].GetY();
        }
    }

    HandlePlayerBulletVsEnemyCollision();

    if (CurrentState == GameState::Playing)
    {
        if (HandlePlayerVsEnemyCollision())
            return;

        if (HandlePlayerCaptureCollision())
            return;
    }

    EnemyBulletSystemObject.Update(dt, type2CanShoot, type2X, type2Y);

    if (CurrentState == GameState::Playing)
    {
        if (HandleEnemyBulletVsPlayerCollision())
            return;
    }
}

void Game::HandlePlayerBulletVsEnemyCollision()
{
    for (int bulletIndex = 0; bulletIndex < PlayerBulletSystemObject.GetBulletCount(); bulletIndex++)
    {
        Bullet& bullet = PlayerBulletSystemObject.GetBullet(bulletIndex);

        if (!bullet.GetIsActive())
            continue;

        for (int enemyIndex = 0; enemyIndex < EnemyCount; enemyIndex++)
        {
            if (!Enemies[enemyIndex].GetIsAlive())
                continue;

            if (CheckHitBoxCollision(bullet.GetHitBox(), Enemies[enemyIndex].GetHitBox()))
            {
                bullet.Deactivate();
                Enemies[enemyIndex].TakeDamage();
                break;
            }
        }
    }
}

bool Game::HandleEnemyBulletVsPlayerCollision()
{
    if (IsRespawning || InvincibleTimer > 0.0f || CurrentState != GameState::Playing)
        return false;

    HitBox playerBox = PlayerObject.GetHitBox();

    for (int bulletIndex = 0; bulletIndex < EnemyBulletSystemObject.GetBulletCount(); bulletIndex++)
    {
        Bullet& bullet = EnemyBulletSystemObject.GetBullet(bulletIndex);

        if (!bullet.GetIsActive())
            continue;

        if (CheckHitBoxCollision(bullet.GetHitBox(), playerBox))
        {
            bullet.Deactivate();
            LoseLifeAndStartRespawn();
            return true;
        }
    }

    return false;
}

bool Game::HandlePlayerVsEnemyCollision()
{
    if (IsRespawning || InvincibleTimer > 0.0f || CurrentState != GameState::Playing)
        return false;

    HitBox playerBox = PlayerObject.GetHitBox();

    for (int enemyIndex = 0; enemyIndex < EnemyCount; enemyIndex++)
    {
        if (!Enemies[enemyIndex].GetIsAlive())
            continue;

        if (CheckHitBoxCollision(playerBox, Enemies[enemyIndex].GetHitBox()))
        {
            LoseLifeAndStartRespawn();
            return true;
        }
    }

    return false;
}

bool Game::HandlePlayerCaptureCollision()
{
    if (IsRespawning || InvincibleTimer > 0.0f || CurrentState != GameState::Playing)
        return false;

    for (int enemyIndex = 0; enemyIndex < EnemyCount; enemyIndex++)
    {
        if (!Enemies[enemyIndex].GetIsAlive())
            continue;

        if (Enemies[enemyIndex].GetIsPlayerCaptured())
        {
            Enemies[enemyIndex].ReleasePlayer();
            LoseLifeAndStartRespawn();
            return true;
        }
    }

    return false;
}

void Game::RenderLives()
{
    const float iconY = 0.88f;
    const float iconScale = 0.25f;
    const float spacing = 0.14f;
    const float startX = 0.82f;

    for (int i = 0; i < PlayerLives; ++i)
    {
        float x = startX - i * spacing;
        Graphics.DrawTriangle(x, iconY, iconScale, iconScale);
    }
}

void Game::Render()
{
    Graphics.BeginFrame();

    if (CurrentState == GameState::Startup)
    {
        StartScreen.Render(Graphics, GlobalTime);
        Graphics.EndFrame();
        return;
    }

    if (CurrentState == GameState::Paused)
    {
        Graphics.DrawText("PAUSED", -0.35f, 0.1f, 1.5f);
        if (sinf(GlobalTime * 3.14f) > 0.0f)
        {
            Graphics.DrawText("PRESS P TO CONTINUE", -0.30f, -0.1f, 0.4f);
        }
        Graphics.EndFrame();
        return;
    }

    if (!IsRespawning && CurrentState != GameState::GameOverWait)
    {
        Graphics.DrawTriangle(PlayerObject.GetX(), PlayerObject.GetY(), 1.0f, 1.0f);
    }

    PlayerBulletSystemObject.Render(Graphics);

    Graphics.DrawText("R RESTART", -0.92f, 0.92f, 0.35f);
    Graphics.DrawText("P PAUSE", -0.92f, 0.82f, 0.35f);

    if (CurrentState == GameState::GameOverWait)
    {
        Graphics.DrawText("GAME OVER", -0.40f, 0.10f, 1.2f);
    }

    RenderLives();

    for (int i = 0; i < EnemyCount; i++)
    {
        if (!Enemies[i].GetIsAlive())
            continue;

        if (Enemies[i].GetIsBeaming())
        {
            float bX = Enemies[i].GetX();
            float bY = Enemies[i].GetY();
            float scale = Enemies[i].GetBeamScale();

            for (int j = 0; j < 6; j++)
            {
                float beamStep = (float)j * 0.12f * scale;
                float beamWidth = 0.3f + (float)j * 0.1f;
                Graphics.DrawDownTriangle(bX, bY - 0.1f - beamStep, beamWidth * scale, 0.4f * scale);
            }
        }

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

        if (Enemies[i].GetHasCapturedShipVisual())
        {
            Graphics.DrawDownTriangle(Enemies[i].GetX() + 0.12f, Enemies[i].GetY(), 0.8f, 0.8f);
        }
    }

    EnemyBulletSystemObject.Render(Graphics);

    Graphics.EndFrame();
}