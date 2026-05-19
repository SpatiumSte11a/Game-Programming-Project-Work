#pragma once
#define NOMINMAX
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
};

struct ConstantBuffer
{
    float offsetX;
    float offsetY;
    float scaleX;
    float scaleY;
};

class GraphicsContext
{
private:
    ID3D11Device* Device;
    ID3D11DeviceContext* Context;
    IDXGISwapChain* SwapChain;
    ID3D11RenderTargetView* RTV;

    ID3D11VertexShader* VertexShader;
    ID3D11PixelShader* PixelShader;
    ID3D11InputLayout* InputLayout;
    ID3D11Buffer* VertexBuffer;
    ID3D11Buffer* ConstantBufferGPU;

public:
    GraphicsContext();
    ~GraphicsContext();

    bool Initialize(HWND hWnd, int width, int height);

    void BeginFrame();
    void DrawTriangle(float offsetX, float offsetY, float scaleX, float scaleY);
    void EndFrame();

private:
    bool CreateRenderTarget();
    bool CreateShaders();
    bool CreateTriangle();
    void ReleaseAll();
};