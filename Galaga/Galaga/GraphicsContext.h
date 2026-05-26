#pragma once
#define NOMINMAX
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
};

struct FontVertex
{
    float x, y, z;
    float u, v;
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

    ID3D11Buffer* TriangleVertexBuffer;
    ID3D11Buffer* DownTriangleVertexBuffer;
    ID3D11Buffer* QuadVertexBuffer;
    ID3D11Buffer* DiamondVertexBuffer;
    ID3D11Buffer* ConstantBufferGPU;

    ID3D11Buffer* VertexBuffer;
    ID3D11VertexShader* TextVertexShader;
    ID3D11PixelShader* TextPixelShader;
    ID3D11InputLayout* TextInputLayout;
    ID3D11Buffer* TextVertexBuffer;
    ID3D11ShaderResourceView* FontTexture;
    ID3D11SamplerState* SamplerState;
    ID3D11BlendState* BlendState;
    int ViewportWidth;
    int ViewportHeight;

    bool CreateTextPipeline(); 
    bool LoadFontTexture();
    bool CreateBlendState();

public:
    GraphicsContext();
    ~GraphicsContext();

    bool Initialize(HWND hWnd, int width, int height);

    void BeginFrame();
    void DrawTriangle(float offsetX, float offsetY, float scaleX, float scaleY);
    void DrawDownTriangle(float offsetX, float offsetY, float scaleX, float scaleY);
    void DrawQuad(float offsetX, float offsetY, float scaleX, float scaleY);
    void DrawDiamond(float offsetX, float offsetY, float scaleX, float scaleY);
    void DrawText(const std::string& text, float x, float y, float scale);
    void EndFrame();

private:
    bool CreateRenderTarget();
    bool CreateShaders();
    bool CreateTriangle();
    bool CreateDownTriangle();
    bool CreateQuad();
    bool CreateDiamond();
    void ReleaseAll();
};