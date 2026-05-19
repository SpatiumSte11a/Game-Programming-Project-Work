# DIAGRAM

이 문서는 현재 프로젝트 구조와 앞으로 추가할 요소를 한눈에 보기 위한 **테스트 다이어그램**입니다.  
이 다이어그램은 최종 버전이 아니라, 다음 수업 시간에 팀원들과 함께 보면서 수정하고 발전시키기 위한 초안입니다.

즉, 현재까지 구현된 구조와 앞으로 추가하고 싶은 구조를 함께 정리한 **discussion draft**로 생각하면 됩니다.

---

## Current Structure

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


    
