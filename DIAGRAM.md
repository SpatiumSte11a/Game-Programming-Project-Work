# Galaga 프로젝트 클래스 다이어그램 (Class Diagram)

프로젝트의 전체적인 구조와 클래스 간의 관계(의존성 및 포함 관계)를 한눈에 파악할 수 있도록 작성된 클래스 다이어그램입니다.

```mermaid
classDiagram
    class Game {
        -GraphicsContext Graphics
        -WindowContext Window
        -StartupScreen StartScreen
        -Player PlayerObj
        -PlayerBulletSystem PlayerBulletSystemObject
        -EnemyBulletSystem EnemyBulletSystemObject
        -Enemy Enemies[WaveMaxEnemies]
        -GameState CurrentState
        -int Score
        -int HighScore
        +Initialize()
        +Run()
        -Input()
        -Update(dt)
        -Render()
        -SetupEnemiesForWave()
    }

    class GraphicsContext {
        -ID3D11Device* Device
        -ID3D11DeviceContext* Context
        -IDXGISwapChain* SwapChain
        +Initialize(hWnd, width, height)
        +BeginFrame(shakeX, shakeY)
        +EndFrame()
        +DrawSprite(srv, x, y, scaleX, scaleY)
        +DrawSpriteUpsideDown(srv, x, y, scaleX, scaleY)
        +DrawText(text, x, y, scale)
        +DrawNumbers(text, x, y, scale)
    }

    class WindowContext {
        +HWND hWnd
        +int Width
        +int Height
        +Initialize(hInstance, width, height)
        +ProcessMessages() bool
    }

    class Player {
        -float x
        -float y
        -float speed
        +Update(dt)
        +Render(GraphicsContext& graphics)
        +GetX() float
        +GetY() float
    }

    class Enemy {
        -EnemyType Type
        -EnemyState State
        -float x
        -float y
        -float BaseX
        -float BaseY
        -float AttackTimer
        +Initialize(type, index, x, y)
        +Update(dt, playerX, playerY)
        +Render(GraphicsContext& graphics)
    }

    class PlayerBulletSystem {
        -Bullet Bullets[MaxPlayerBullets]
        +Fire(x, y)
        +Update(dt)
        +Render(GraphicsContext& graphics)
    }

    class EnemyBulletSystem {
        -Bullet Bullets[MaxEnemyBullets]
        +TryShoot(enemyX, enemyY)
        +Update(dt)
        +Render(GraphicsContext& graphics)
    }

    class Bullet {
        +float X
        +float Y
        +float Speed
        +bool IsActive
        +Update(dt)
    }

    class StartupScreen {
        -bool StartPressed
        -bool CountdownActive
        -float CountdownTimer
        +Input()
        +Update(dt)
        +Render(GraphicsContext& graphics)
    }
    
    %% Relationships (Composition)
    Game *-- GraphicsContext : Contains
    Game *-- WindowContext : Contains
    Game *-- StartupScreen : Contains
    Game *-- Player : Contains
    Game *-- PlayerBulletSystem : Contains
    Game *-- EnemyBulletSystem : Contains
    Game *-- Enemy : Array (Pool)
    
    %% Relationships (Aggregation/Composition)
    PlayerBulletSystem *-- Bullet : Manages
    EnemyBulletSystem *-- Bullet : Manages
    
    %% Relationships (Dependency)
    Game ..> WaveData : Uses
    Enemy ..> GraphicsContext : Renders with
    Player ..> GraphicsContext : Renders with
    PlayerBulletSystem ..> GraphicsContext : Renders with
    EnemyBulletSystem ..> GraphicsContext : Renders with
    StartupScreen ..> GraphicsContext : Renders with
```

### 💡 주요 관계 설명 (Relationships)
1.  **Composition (포함 관계, `*--`)**: 
    *   `Game` 클래스는 프로젝트의 메인 컨트롤러로, 그래픽(`GraphicsContext`), 윈도우(`WindowContext`), 플레이어(`Player`), 적(`Enemy`), 총알 시스템(`BulletSystem`), 시작 화면(`StartupScreen`) 객체들을 멤버 변수로 소유하며 전체 생명주기를 관리합니다.
    *   `PlayerBulletSystem`과 `EnemyBulletSystem`은 각각 자신이 발사한 `Bullet` 객체 배열을 내부적으로 관리(오브젝트 풀링 방식)합니다.
2.  **Dependency (의존 관계, `..>`)**:
    *   모든 게임 내 오브젝트(`Player`, `Enemy`, `StartupScreen`, `BulletSystem`)는 화면에 자신을 그리기 위해 `Game` 클래스가 전달해주는 `GraphicsContext`의 렌더링 함수(`DrawSprite`, `DrawText` 등)에 의존합니다.
    *   `Game` 클래스는 웨이브 전환 시 `WaveData`의 정적 데이터를 불러와 적들을 배치합니다.
