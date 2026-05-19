# DIAGRAM

이 문서는 현재 프로젝트 구조와 앞으로 추가할 요소를 한눈에 보기 위한 **테스트 다이어그램**입니다.  
이 다이어그램은 최종 버전이 아니라, 다음 수업 시간에 팀원들과 함께 보면서 수정하고 발전시키기 위한 초안입니다.

즉, 현재까지 구현된 구조와 앞으로 추가하고 싶은 구조를 함께 정리한 **discussion draft**로 생각하면 됩니다.

---

## Current Structure

현재 구조를 정리한 다이어그램입니다.  
여기에는 지금 실제 코드 안에서 이미 구현되어 있고 동작하고 있는 요소들만 포함되어 있습니다.  
즉, `main.cpp`, `Game`, `WindowContext`, `GraphicsContext`를 중심으로 현재 프로젝트가 어떻게 실행되고 있는지를 보여 주는 부분입니다.

```mermaid
classDiagram
    class Main {
        WinMain()
    }

    class Game {
        +Initialize()
        +Run()
        +Input()
        +Update()
        +Render()
        -GetDeltaTime()
    }

    class WindowContext {
        +Initialize()
        +hWnd
        +Width
        +Height
    }

    class GraphicsContext {
        +Initialize()
        +BeginFrame()
        +DrawTriangle()
        +EndFrame()
        -CreateRenderTarget()
        -CreateShaders()
        -CreateTriangle()
    }

    Main --> Game
    Game --> WindowContext
    Game --> GraphicsContext

```markdown
이 다이어그램은 앞으로 프로젝트가 확장될 경우를 생각해서 정리한 구조입니다.
아직 전부 구현된 것은 아니며, 다음 수업 시간에 팀원들과 함께 보면서 수정하고 발전시킬 테스트용 초안입니다.
즉, 이후에 추가할 수 있는 시스템과 역할 분담 방향을 생각하기 위한 참고 구조입니다.
## Future Structure
    classDiagram
    class Main {
        WinMain()
    }

    class Game {
        +Initialize()
        +Run()
        +Input()
        +Update()
        +Render()
    }

    class WindowContext
    class GraphicsContext

    class Player {
        +Move()
        +RequestShoot()
    }

    class Enemy {
        +Move()
        +RequestShoot()
    }

    class EnemyManager {
        +SpawnWave()
        +UpdateEnemies()
    }

    class Bullet {
        +Update()
        +Render()
    }

    class ProjectileManager {
        +SpawnPlayerBullet()
        +SpawnEnemyBullet()
        +UpdateProjectiles()
    }

    class CollisionSystem {
        +CheckBulletEnemy()
        +CheckBulletPlayer()
        +CheckPlayerEnemy()
    }

    class GameStateManager {
        +Title()
        +Playing()
        +Pause()
        +GameOver()
        +Clear()
    }

    class UISystem {
        +DrawTitleUI()
        +DrawHUD()
        +DrawGameOverUI()
    }

    class ScoreSystem {
        +AddScore()
        +ResetScore()
    }

    class LifeSystem {
        +LoseLife()
        +ResetLives()
    }

    Main --> Game
    Game --> WindowContext
    Game --> GraphicsContext

    Game --> Player
    Game --> EnemyManager
    EnemyManager --> Enemy

    Game --> ProjectileManager
    ProjectileManager --> Bullet
    Player --> ProjectileManager
    Enemy --> ProjectileManager

    Game --> CollisionSystem
    CollisionSystem --> Player
    CollisionSystem --> Enemy
    CollisionSystem --> Bullet

    Game --> GameStateManager
    Game --> UISystem
    Game --> ScoreSystem
    Game --> LifeSystem

