#include "StartupScreen.h"
#include <windows.h>

void StartupScreen::Initialize()
{
    // Nothing to allocate yet.
}

void StartupScreen::Input()
{
    // Any key except ESC triggers start
    // Scan all virtual keys 0x08 to 0xFE
    for (int vk = 0x08; vk < 0xFF; vk++)
    {
        if (vk == VK_ESCAPE) continue;
        if (GetAsyncKeyState(vk) & 0x0001)
        {
            StartPressed = true;
            return;
        }
    }

    // R always restarts from any screen
    if (GetAsyncKeyState('R') & 0x0001)
        RestartPressed = true;
}

void StartupScreen::Render(GraphicsContext& graphics, float GlobalTime)
{
    // GALAGA title — floating bob
    float logoY = 0.15f + sinf(GlobalTime * 2.0f) * 0.03f;
    graphics.DrawText("GALAGA", -0.62f, logoY, 3.0f);

    // Blinking PRESS ANY KEY — only line on screen besides title
    if (sinf(GlobalTime * 3.14f) > 0.0f)
        graphics.DrawText("PRESS ANY KEY TO START", -0.45f, -0.35f, 0.55f);
}

void StartupScreen::Reset()
{
    StartPressed = false;
    RestartPressed = false;
}
