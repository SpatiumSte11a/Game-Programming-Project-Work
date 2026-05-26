#include "StartupScreen.h"
#include <windows.h>

const float StartupScreen::CountdownDuration = 5.0f;

void StartupScreen::Initialize()
{
    // Nothing to allocate yet.
}

void StartupScreen::Input()
{
      //dont accept any input during countdown
     if (CountdownActive)
     return;
    
    // Any key except ESC triggers start
    // Scan all virtual keys 0x08 to 0xFE
    for (int vk = 0x08; vk < 0xFF; vk++)
    {
        if (vk == VK_ESCAPE) continue;
        if (GetAsyncKeyState(vk) & 0x0001)
        {
            StartPressed = true;
            CountdownActive = true;
            return;
        }
    }

    // R always restarts from any screen
    if (GetAsyncKeyState('R') & 0x0001)
        RestartPressed = true;
}


void StartupScreen::Update(float dt)
{
    if (!CountdownActive || CountdownDone)
        return;

    CountdownTimer += dt;

    if (CountdownTimer >= CountdownDuration)
    {
        CountdownDone = true;
        CountdownActive = false;
    }
}

void StartupScreen::Render(GraphicsContext& graphics, float GlobalTime)
{
    // GALAGA title — floating bob
    float logoY = 0.15f + sinf(GlobalTime * 2.0f) * 0.03f;
    graphics.DrawText("GALAGA", -0.62f, logoY, 3.0f);

    graphics.DrawText("PRESS ESC TO EXIT", -0.3f, -0.82f, 0.45f);

    graphics.DrawText("PRESS ANY KEY TO START", -0.45f, -0.72f, 0.55f);

    if (!CountdownActive)
    {
        // Blinking PRESS ANY KEY — shown before player presses anything
        if (sinf(GlobalTime * 3.14f) > 0.0f)
            graphics.DrawText(" ", -0.45f, -0.35f, 0.55f);
    }
    else
    {
        // Countdown in progress — show seconds remaining
        float remaining = CountdownDuration - CountdownTimer;
        int   seconds = (int)remaining + 1;

        // Blink the number faster as it counts down
        if (sinf(GlobalTime * 6.0f) > 0.0f)
        {
            if (seconds == 3) graphics.DrawText("3", -0.04f, -0.25f, 2.5f, 1.0f, 1.0f, 0.0f);
            else if (seconds == 2) graphics.DrawText("2", -0.04f, -0.25f, 2.5f, 1.0f, 0.5f, 0.0f);
            else if (seconds == 1) graphics.DrawText("1", -0.04f, -0.25f, 2.5f, 0.0f, 1.0f, 0.0f);
        }

        if (sinf(GlobalTime * 3.14f) > 0.0f)
           graphics.DrawText("GET READY", -0.20f, -0.45f, 0.6f, 1.0f, 1.0f, 1.0f);
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
