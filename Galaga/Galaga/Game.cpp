#include "Game.h"
#include "Collision.h"
#include <cmath>
#include <cstdlib>

Game::Game()
    : Window(L"Galaga"),
    IsRunning(true),
    CurrentState(GameState::Startup),
    TitleUpdateTimer(0.0f),
    FrameCounter(0),
    GlobalTime(0.0f),
    PlayerLives(3),
    Score(0),
    IsRespawning(false),
    RespawnTimer(0.0f),
    RespawnDelay(1.5f),
    InvincibleTimer(0.0f),
    InvincibleDuration(1.0f),
    GameOverTimer(0.0f),
    GameOverDelay(10.0f),
    CurrentWave(1),
    IsWaveTransition(false),
    WaveTransitionTimer(0.0f),
    WaveTransitionDelay(1.0f),
    IsNextWaveSpawnDelay(false),
    NextWaveSpawnDelayTimer(0.0f),
    NextWaveSpawnDelay(1.0f)
{
    PrevTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < EnemyCount; i++)
    {
        EnemySlotsActive[i] = false;
    }
}

bool Game::Initialize(HINSTANCE hInstance)
{
    if (!Window.Initialize(hInstance, 720, 960))
        return false;

    if (!Graphics.Initialize(Window.hWnd, 720, 960))
        return false;

    StartScreen.Initialize();
    InitStars();
    ResetGame();

    return true;
}

void Game::SetupEnemiesForWave(int wave)
{
    WaveDefinition waveDef = GetWaveDefinition(wave);

    for (int i = 0; i < EnemyCount; i++)
    {
        Enemies[i] = Enemy();
        EnemySlotsActive[i] = false;
    }

    for (int i = 0; i < WaveMaxEnemies; i++)
    {
        if (!waveDef.Spawns[i].IsUsed)
            continue;

        EnemySlotsActive[i] = true;
        Enemies[i].SetType(waveDef.Spawns[i].Type);
        Enemies[i].SetPosition(waveDef.Spawns[i].X, waveDef.Spawns[i].Y);

        if (waveDef.Spawns[i].Type == EnemyType::Type1)
        {
            Enemies[i].SetMoveSpeedScale(waveDef.Type1MoveSpeedScale);
            Enemies[i].SetAttackDelayScale(waveDef.Type1AttackDelayScale);
        }
        else if (waveDef.Spawns[i].Type == EnemyType::Type2)
        {
            Enemies[i].SetShootCooldownScale(waveDef.Type2ShootCooldownScale);
        }
        else if (waveDef.Spawns[i].Type == EnemyType::Type3)
        {
            Enemies[i].SetAttackDelayScale(waveDef.Type3AttackDelayScale);
        }
    }
}

bool Game::AreAllEnemiesDefeated() const
{
    for (int i = 0; i < EnemyCount; i++)
    {
        if (!EnemySlotsActive[i])
            continue;

        if (Enemies[i].GetIsAlive())
            return false;
    }

    return true;
}

void Game::ResetGame()
{
    CurrentState = GameState::Startup;
    GlobalTime = 0.0f;
    PlayerLives = 3;
    Score = 0;

    IsRespawning = false;
    RespawnTimer = 0.0f;
    InvincibleTimer = 0.0f;

    GameOverTimer = 0.0f;

    CurrentWave = 1;
    IsWaveTransition = false;
    WaveTransitionTimer = 0.0f;

    IsNextWaveSpawnDelay = false;
    NextWaveSpawnDelayTimer = 0.0f;

    StartScreen.Reset();

    PlayerObject = Player();
    PlayerBulletSystemObject = PlayerBulletSystem();
    EnemyBulletSystemObject = EnemyBulletSystem();

    SetupEnemiesForWave(CurrentWave);
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
        StartScreen.Input();

        if (StartScreen.ShouldStartGame())
        {
            CurrentState = GameState::Playing;
        }

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

    for (int i = 0; i < StarCount; i++)
    {
        Stars[i].Y -= Stars[i].Speed * dt;

        if (Stars[i].Y < -1.1f)
        {
            Stars[i].Y = 1.1f;
            Stars[i].X = -1.0f + (rand() % 2000) / 1000.0f;
        }
    }

    if (CurrentState == GameState::Startup)
    {
        StartScreen.Update(dt);
        return;
    }

    if (InvincibleTimer > 0.0f)
        InvincibleTimer -= dt;

    if (CurrentState == GameState::Paused)
        return;

    if (CurrentState == GameState::GameOverWait)
    {
        GameOverTimer -= dt;

        if (GameOverTimer <= 0.0f)
        {
            ResetGame();
            return;
        }
    }

    if (IsWaveTransition)
    {
        WaveTransitionTimer -= dt;

        if (WaveTransitionTimer <= 0.0f)
        {
            IsWaveTransition = false;
            IsNextWaveSpawnDelay = true;
            NextWaveSpawnDelayTimer = NextWaveSpawnDelay;
        }
    }

    if (IsNextWaveSpawnDelay)
    {
        NextWaveSpawnDelayTimer -= dt;

        if (NextWaveSpawnDelayTimer <= 0.0f)
        {
            IsNextWaveSpawnDelay = false;
            CurrentWave++;

            PlayerBulletSystemObject = PlayerBulletSystem();
            EnemyBulletSystemObject = EnemyBulletSystem();
            SetupEnemiesForWave(CurrentWave);
        }
    }

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

    bool isCaptured = false;

    for (int i = 0; i < EnemyCount; i++)
    {
        if (!EnemySlotsActive[i])
            continue;

        if (Enemies[i].GetIsPlayerCaptured())
        {
            isCaptured = true;
            break;
        }
    }

    bool isInWaveBreak = IsWaveTransition || IsNextWaveSpawnDelay;

    bool canControlPlayer =
        CurrentState == GameState::Playing &&
        !IsRespawning &&
        !isCaptured;

    bool moveLeft = canControlPlayer && ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000));
    bool moveRight = canControlPlayer && ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000));
    bool shootPressed = canControlPlayer && !isInWaveBreak && (GetAsyncKeyState(VK_SPACE) & 0x8000);

    if (!IsRespawning && CurrentState == GameState::Playing)
        PlayerObject.Update(dt, moveLeft, moveRight);

    float enemyTargetX = (IsRespawning || CurrentState == GameState::GameOverWait) ? 100.0f : PlayerObject.GetX();
    float enemyTargetY = (IsRespawning || CurrentState == GameState::GameOverWait) ? -100.0f : PlayerObject.GetY();

    float pX = PlayerObject.GetX();
    float pY = PlayerObject.GetY();

    for (int i = 0; i < EnemyCount; i++)
    {
        if (!EnemySlotsActive[i])
            continue;

        if (Enemies[i].GetIsPlayerCaptured())
        {
            pX = Enemies[i].GetX() + 0.12f;
            pY = Enemies[i].GetY();
        }
    }

    PlayerBulletSystemObject.Update(dt, shootPressed, pX, pY);
    EnemyBulletSystemObject.Update(dt);

    if (isInWaveBreak)
    {
        return;
    }

    for (int i = 0; i < EnemyCount; i++)
    {
        if (!EnemySlotsActive[i])
            continue;

        Enemies[i].Update(dt, enemyTargetX, enemyTargetY);

        if (Enemies[i].GetIsAlive() &&
            Enemies[i].GetType() == EnemyType::Type2 &&
            Enemies[i].GetWantsToShoot())
        {
            EnemyBulletSystemObject.TryShoot(Enemies[i].GetX(), Enemies[i].GetY());
            Enemies[i].ClearWantsToShoot();
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

    if (CurrentState == GameState::Playing)
    {
        if (HandleEnemyBulletVsPlayerCollision())
            return;
    }

    if (CurrentState == GameState::Playing &&
        !IsWaveTransition &&
        !IsNextWaveSpawnDelay &&
        AreAllEnemiesDefeated())
    {
        IsWaveTransition = true;
        WaveTransitionTimer = WaveTransitionDelay;
        PlayerBulletSystemObject = PlayerBulletSystem();
        EnemyBulletSystemObject = EnemyBulletSystem();
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
            if (!EnemySlotsActive[enemyIndex])
                continue;

            if (!Enemies[enemyIndex].GetIsAlive())
                continue;

            if (CheckHitBoxCollision(bullet.GetHitBox(), Enemies[enemyIndex].GetHitBox()))
            {
                bullet.Deactivate();
                Enemies[enemyIndex].TakeDamage();

                if (!Enemies[enemyIndex].GetIsAlive())
                {
                    switch (Enemies[enemyIndex].GetType())
                    {
                    case EnemyType::Type1:
                        Score += 100;
                        break;

                    case EnemyType::Type2:
                        Score += 200;
                        break;

                    case EnemyType::Type3:
                        Score += 500;
                        break;
                    }
                }

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
        if (!EnemySlotsActive[enemyIndex])
            continue;

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
        if (!EnemySlotsActive[enemyIndex])
            continue;

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
    const float iconScale = 0.45f;
    const float spacing = 0.14f;
    const float startX = 0.82f;

    for (int i = 0; i < PlayerLives; ++i)
    {
        float x = startX - i * spacing;
        Graphics.DrawTriangle(x, iconY, iconScale, iconScale);
    }
}

void Game::InitStars()
{
    srand(42);

    for (int i = 0; i < StarCount; i++)
    {
        Stars[i].X = -1.0f + (rand() % 2000) / 1000.0f;
        Stars[i].Y = -1.0f + (rand() % 2000) / 1000.0f;

        int layer = i % 3;
        if (layer == 0) Stars[i].Speed = 0.05f;
        if (layer == 1) Stars[i].Speed = 0.10f;
        if (layer == 2) Stars[i].Speed = 0.18f;

        Stars[i].Size = 0.09f + (layer * 0.03f);
    }
}

void Game::RenderStars()
{
    for (int i = 0; i < StarCount; i++)
    {
        Graphics.DrawTriangle(
            Stars[i].X,
            Stars[i].Y,
            Stars[i].Size,
            Stars[i].Size
        );
    }
}

void Game::Render()
{
    Graphics.BeginFrame();

    RenderStars();

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
        PlayerObject.Render(Graphics);
    }

    PlayerBulletSystemObject.Render(Graphics);

    Graphics.DrawText("WAVE", -0.92f, 0.92f, 0.35f);
    Graphics.DrawNumbers(std::to_string(CurrentWave), -0.75f, 0.92f, 0.40f);

    Graphics.DrawText("ESC - EXIT", -0.92f, -0.92f, 0.35f);
    Graphics.DrawText("P - PAUSE", -0.48f, -0.92f, 0.35f);
    Graphics.DrawText("R - RESTART", -0.02f, -0.92f, 0.35f);

    if (CurrentState == GameState::GameOverWait)
    {
        Graphics.DrawText("GAME OVER", -0.40f, 0.10f, 1.2f);
    }

    if (IsWaveTransition)
    {
        Graphics.DrawText("WAVE CLEAR", -0.28f, 0.02f, 0.65f);
    }

    RenderLives();

    std::string scoreStr = std::to_string(Score);
    while (scoreStr.length() < 6)
        scoreStr = "0" + scoreStr;

    Graphics.DrawNumbers(scoreStr, 0.54f, 0.81f, 0.75f);

    for (int i = 0; i < EnemyCount; i++)
    {
        if (!EnemySlotsActive[i])
            continue;

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

                Graphics.DrawDownTriangle(
                    bX,
                    bY - 0.1f - beamStep,
                    beamWidth * scale,
                    0.4f * scale
                );
            }
        }

        switch (Enemies[i].GetType())
        {
        case EnemyType::Type1:
            Graphics.DrawSprite(
                Graphics.GetEnemy1Texture(),
                Enemies[i].GetX(),
                Enemies[i].GetY(),
                0.15f,
                0.15f
            );
            break;

        case EnemyType::Type2:
            Graphics.DrawSprite(
                Graphics.GetEnemy2Texture(),
                Enemies[i].GetX(),
                Enemies[i].GetY(),
                0.15f,
                0.15f
            );
            break;

        case EnemyType::Type3:
            Graphics.DrawSprite(
                Graphics.GetEnemy3Texture(),
                Enemies[i].GetX(),
                Enemies[i].GetY(),
                0.18f,
                0.18f
            );
            break;
        }
    }

    EnemyBulletSystemObject.Render(Graphics);

    Graphics.EndFrame();
}