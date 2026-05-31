#pragma once
#include <windows.h>
#include "GraphicsContext.h"

class StartupScreen
{
public:
    void Initialize();
    void Input();
    void Update(float dt);
    void Render(GraphicsContext& graphics, float globalTime);

    bool ShouldStartGame() const { return CountdownDone; }
    void Reset();

private:
    bool StartPressed = false;

    bool CountdownActive = false;
    bool CountdownDone = false;
    float CountdownTimer = 0.0f;

    static const float CountdownDuration;
};