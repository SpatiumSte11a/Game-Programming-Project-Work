#pragma once
#define NOMINMAX
#include <windows.h>

class WindowContext
{
public:
    HWND hWnd;
    int Width;
    int Height;
    LPCWSTR WindowName;

    WindowContext(LPCWSTR windowName = L"Galaga")
        : hWnd(nullptr), Width(720), Height(960), WindowName(windowName)
    {
    }

    ~WindowContext();

    bool Initialize(HINSTANCE hInst, int width, int height);
};