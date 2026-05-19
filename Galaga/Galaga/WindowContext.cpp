#include "WindowContext.h"

LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

WindowContext::~WindowContext()
{
    UnregisterClass(L"GalagaWindowClass", GetModuleHandle(nullptr));
}

bool WindowContext::Initialize(HINSTANCE hInst, int width, int height)
{
    Width = width;
    Height = height;

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = GlobalWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = L"GalagaWindowClass";

    if (!RegisterClassEx(&wc))
        return false;

    RECT rc = { 0, 0, Width, Height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindow(
        L"GalagaWindowClass",
        WindowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        hInst,
        nullptr
    );

    if (!hWnd)
        return false;

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return true;
}