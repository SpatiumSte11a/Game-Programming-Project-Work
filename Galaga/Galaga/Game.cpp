#include "Game.h"

Game::Game()
    : Window(L"Galaga"),
    IsRunning(true),
    CurrentState(GameState::Startup),
    TitleUpdateTimer(0.0f),
    FrameCounter(0),
    GlobalTime(0.0f),
    PlayerLives(3)
{
    PrevTime = std::chrono::high_resolution_clock::now();
}

bool Game::Initialize(HINSTANCE hInstance)
{
    if (!Window.Initialize(hInstance, 720, 960))
        return false;

    if (!Graphics.Initialize(Window.hWnd, 720, 960))
        return false;

    InitStars(); 
    
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

    ResetGame(); 
    
    return true;
}

void Game::ResetGame()
{
    CurrentState = GameState::Startup;
    //didnt allocate yet 
    GlobalTime = 0.0f;

      PlayerLives = 3;
  StartScreen.Reset();

  // reset player
  PlayerObject = Player();

  // reset bullet systems
  PlayerBulletSystemObject = PlayerBulletSystem();
  EnemyBulletSystemObject = EnemyBulletSystem();

  // recreate enemies
  float enemyX = 0.0f;
  float visualGap = 0.04f;

  float squareY = 0.42f;
  float squareTop = 0.05f;

  float downTriangleTop = 0.02f;
  float downTriangleBottom = 0.08f;

  float diamondBottom = 0.063f;

  float triangleY =
      squareY +
      squareTop +
      visualGap +
      downTriangleBottom;

  float diamondY =
      triangleY +
      downTriangleTop +
      visualGap +
      diamondBottom;

  Enemies[0].SetType(EnemyType::Type1);
  Enemies[0].SetPosition(enemyX, squareY);

  Enemies[1].SetType(EnemyType::Type2);
  Enemies[1].SetPosition(enemyX, triangleY);

  Enemies[2].SetType(EnemyType::Type3);
  Enemies[2].SetPosition(enemyX, diamondY);
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
    // ESC quits from anywhere
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        IsRunning = false;
        return;
    }

    //R resets to startup screen
    if (GetAsyncKeyState('R') & 0x0001)
    {
        ResetGame();
        return;
    }

    //Startup screen
    if (CurrentState == GameState::Startup)
   {
       StartScreen.Input();                  
       if (StartScreen.ShouldStartGame())      
       {
           ResetGame();
           CurrentState = GameState::Playing;
       }
   }

    //P for pause game
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

    //stop game when on pause 
    if (CurrentState != GameState::Playing)
        return;

    bool isCaptured = false;
    for (int i = 0; i < EnemyCount; i++)
    {
        if (Enemies[i].GetIsPlayerCaptured())
        {
            isCaptured = true;
            break;
        }
    }

    bool moveLeft = !isCaptured && ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000));
    bool moveRight = !isCaptured && ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000));
    bool shootPressed = !isCaptured && (GetAsyncKeyState(VK_SPACE) & 0x8000);

    PlayerObject.Update(dt, moveLeft, moveRight);

    float pX = PlayerObject.GetX();
    float pY = PlayerObject.GetY();

    for (int i = 0; i < EnemyCount; i++)
    {
        if (Enemies[i].GetIsPlayerCaptured())
        {
            pX = Enemies[i].GetX();
            pY = Enemies[i].GetY() - 0.12f;
        }
    }

    PlayerBulletSystemObject.Update(dt, shootPressed, pX, pY);

    bool type2CanShoot = false;
    float type2X = 0.0f;
    float type2Y = 0.0f;

    for (int i = 0; i < EnemyCount; i++)
    {
        Enemies[i].Update(dt, PlayerObject.GetX(), PlayerObject.GetY());

        if (Enemies[i].GetIsAlive() && Enemies[i].GetType() == EnemyType::Type2)
        {
            type2CanShoot = true;
            type2X = Enemies[i].GetX();
            type2Y = Enemies[i].GetY();
        }
    }

    EnemyBulletSystemObject.Update(dt, type2CanShoot, type2X, type2Y);
}

void Game::RenderLives()
{
    const float iconY = 0.88f;   // vertical position (NDC)
    const float iconScale = 0.25f;   // DrawTriangle scaleX/Y
    const float spacing = 0.14f;   // horizontal gap between icons
    const float startX = 0.82f;   // x of the rightmost icon

    for (int i = 0; i < PlayerLives; ++i)
    {
        float x = startX - i * spacing;
        Graphics.DrawTriangle(x, iconY, iconScale, iconScale);
    }
}

void Game::InitStars()
{
    srand(42);  // fixed seed 
    for (int i = 0; i < StarCount; i++)
    {
        Stars[i].X = -1.0f + (rand() % 2000) / 1000.0f;

        Stars[i].Y = -1.0f + (rand() % 2000) / 1000.0f;

        int layer = i % 3;
        if (layer == 0) Stars[i].Speed = 0.05f;   
        if (layer == 1) Stars[i].Speed = 0.10f;   
        if (layer == 2) Stars[i].Speed = 0.18f;  

        Stars[i].Size = 0.012f + (layer * 0.03f); 
    }
}

void Game::RenderStars()
{
    for (int i = 0; i < StarCount; i++)
    {
        Graphics.DrawTriangle(
            Stars[i].X,
            Stars[i].Y,
            Stars[i].Size,   // scaleX
            Stars[i].Size    // scaleY
        );
    }
}

void Game::Render()
{
    Graphics.BeginFrame();

    RenderStars(); 
    
    //startup screen
    if (CurrentState == GameState::Startup)
    {
        StartScreen.Render(Graphics, GlobalTime);

        Graphics.EndFrame();
        return;
    }

    //pause screen
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

    // Determine player render position
    float drawPX = PlayerObject.GetX();
    float drawPY = PlayerObject.GetY();
    bool isCurrentlyCaptured = false;

    for (int i = 0; i < EnemyCount; i++)
    {
        if (Enemies[i].GetIsPlayerCaptured())
        {
            // Position beside the boss and slightly lower
            drawPX = Enemies[i].GetX() + 0.12f; 
            drawPY = Enemies[i].GetY();
            isCurrentlyCaptured = true;
            break;
        }
    }

    if (isCurrentlyCaptured)
    {
        // Facing player side (downward)
        Graphics.DrawDownTriangle(drawPX, drawPY, 1.0f, 1.0f);
    }
    else
    {
        Graphics.DrawTriangle(drawPX, drawPY, 1.0f, 1.0f);
    }
    
    PlayerBulletSystemObject.Render(Graphics);

    // UI Text
    Graphics.DrawText("R RESTART", -0.92f, 0.92f, 0.45f);

    Graphics.DrawText("P PAUSE", -0.92f, 0.82f, 0.45f);

    // Lives
    RenderLives();
    
    for (int i = 0; i < EnemyCount; i++)
    {
        if (!Enemies[i].GetIsAlive())
            continue;

        // Render Tractor Beam for Boss
        if (Enemies[i].GetIsBeaming())
        {
            float bX = Enemies[i].GetX();
            float bY = Enemies[i].GetY();
            float scale = Enemies[i].GetBeamScale();
            
            // Draw expanding beam effect using inverted triangles
            for (int j = 0; j < 6; j++)
            {
                float beamStep = (float)j * 0.12f * scale;
                float beamWidth = 0.3f + (float)j * 0.1f;
                // Use DrawDownTriangle for the beam "trapezoid" segments
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
    }

    EnemyBulletSystemObject.Render(Graphics);

    Graphics.EndFrame();
}
