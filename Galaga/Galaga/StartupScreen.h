#pragma once
#include "GraphicsContext.h"

class StartupScreen
{
public:
    void Initialize();
    void Input();
    void Render(GraphicsContext& graphics, float globalTime);

    bool ShouldStartGame()   const { return StartPressed; }
    bool ShouldRestartGame() const { return RestartPressed; }
    void Reset();

private:
    bool StartPressed = false;
    bool RestartPressed = false;
};
