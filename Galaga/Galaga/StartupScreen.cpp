#include "StartupScreen.h"
#include <windows.h>
#include <cmath>

const float StartupScreen::CountdownDuration = 2.0f;

void StartupScreen::Initialize()
{
    // Nothing to initialize yet.
}

void StartupScreen::Input()
{
    // If countdown already finished, do nothing
    if (CountdownDone)
        return;

    // During countdown do not accept new input
    if (CountdownActive)
        return;

    if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
    {
        RestartPressed = true;
        return;
    }

    // Any key starts countdown
    for (int vk = 0x08; vk < 0xFF; vk++)
    {
        if (GetAsyncKeyState(vk) & 0x0001)
        {
            StartPressed = true;
            CountdownActive = true;
            CountdownDone = false;
            CountdownTimer = 0.0f;
            return;
        }
    }
}

void StartupScreen::Update(float dt)
{
    if (!CountdownActive || CountdownDone)
        return;

    CountdownTimer += dt;

    if (CountdownTimer >= CountdownDuration)
    {
        CountdownTimer = CountdownDuration;
        CountdownDone = true;
        CountdownActive = false;
    }
}

void StartupScreen::Render(GraphicsContext& graphics, float globalTime)
{
    float logoY = 0.15f + sinf(globalTime * 2.0f) * 0.03f;
    graphics.DrawText("GALAGA", -0.62f, logoY, 3.0f);

    if (!CountdownActive)
    {
        float pressY = logoY - 0.40f;
        float escY = pressY - 0.16f;

        if (sinf(globalTime * 3.14f) > 0.0f)
        {
            graphics.DrawText("PRESS ANY KEY TO START", -0.38f, pressY, 0.50f);
        }

        graphics.DrawText("ESC - EXIT", -0.15f, escY, 0.40f);
    }
    else
    {
        float remaining = CountdownDuration - CountdownTimer;
        int seconds = (int)remaining + 1;

        if (seconds < 1)
            seconds = 1;
        if (seconds > 5)
            seconds = 5;

        if (sinf(globalTime * 6.0f) > 0.0f)
        {
            if (seconds == 5) graphics.DrawText("5", -0.04f, -0.20f, 2.0f);
            if (seconds == 4) graphics.DrawText("4", -0.04f, -0.20f, 2.0f);
            if (seconds == 3) graphics.DrawText("3", -0.04f, -0.20f, 2.0f);
            if (seconds == 2) graphics.DrawText("2", -0.04f, -0.20f, 2.0f);
            if (seconds == 1) graphics.DrawText("1", -0.04f, -0.20f, 2.0f);
        }

        if (sinf(globalTime * 3.14f) > 0.0f)
        {
            graphics.DrawText("GET READY", -0.20f, -0.45f, 0.60f);
        }
    }
}

void StartupScreen::Reset()
{
    StartPressed = false;
    RestartPressed = false;
    CountdownActive = false;
    CountdownDone = false;
    CountdownTimer = 0.0f;
}