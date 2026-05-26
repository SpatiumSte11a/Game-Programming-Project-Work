#include "StartupScreen.h"

void StartupScreen::Initialize()
{
    // Nothing to allocate yet.
}

void StartupScreen::Render(GraphicsContext& graphics, float GlobalTime)
{
    //falling stars
    for (int i = 0; i < 80; i++)
    {
        float x =
            -1.0f +
            (i % 20) * 0.1f +
            sinf(i * 12.989f) * 0.03f;

        float speed = 0.1f + (i % 5) * 0.08f;

        float y =
            1.2f - fmod(GlobalTime * speed + i * 0.15f, 2.4f);

        graphics.DrawTriangle(x, y, 0.05f, 0.05f);
    }

    //galaga title 
    float logoY = 0.55f + sinf(GlobalTime * 2.0f) * 0.03f;

    graphics.DrawText("GALAGA", -0.64f, logoY, 3.0f);

    //s for start 
    graphics.DrawText("S  START", -0.50f, -0.45f, 0.7f);

    //esc for exit 
    graphics.DrawText("ESC  EXIT", -0.00f, -0.45f, 0.7f);

    if (sinf(GlobalTime * 3.14f) > 0.0f)
    {
        graphics.DrawText("PRESS A KEY", -0.24f, -0.55f, 0.45f);
    }
}