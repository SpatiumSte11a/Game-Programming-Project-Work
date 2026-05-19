#define NOMINMAX
#include <windows.h>
#include "Game.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:windows")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    Game game;

    if (!game.Initialize(hInstance))
        return -1;

    game.Run();
    return 0;
}