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
    float tintR;
    float tintG;
    float tintB;
    float tintA;
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
    ID3D11ShaderResourceView* NumbersTexture;
    ID3D11ShaderResourceView* ShipTexture;
    ID3D11ShaderResourceView* Enemy1Texture;
    ID3D11ShaderResourceView* Enemy2Texture;
    ID3D11ShaderResourceView* Enemy3Texture;
    ID3D11ShaderResourceView* BonusStarTexture;
    ID3D11SamplerState* SamplerState;
    ID3D11BlendState* BlendState;

    ID3D11VertexShader* SpriteVertexShader;
    ID3D11PixelShader* SpritePixelShader;
    ID3D11InputLayout* SpriteInputLayout;
    ID3D11Buffer* SpriteVertexBuffer;
    ID3D11Buffer* SpriteConstantBuffer;

    int ViewportWidth;
    int ViewportHeight;

    bool CreateTextPipeline();
    bool LoadTextures();
    bool CreateBlendState();

public:
    GraphicsContext();
    ~GraphicsContext();

    ID3D11ShaderResourceView* LoadSprite(const wchar_t* path);
    ID3D11ShaderResourceView* GetShipTexture() const { return ShipTexture; }
    ID3D11ShaderResourceView* GetEnemy1Texture() const { return Enemy1Texture; }
    ID3D11ShaderResourceView* GetEnemy2Texture() const { return Enemy2Texture; }
    ID3D11ShaderResourceView* GetEnemy3Texture() const { return Enemy3Texture; }
    ID3D11ShaderResourceView* GetBonusStarTexture() const { return BonusStarTexture; }

    bool Initialize(HWND hWnd, int width, int height);

    void BeginFrame();
    void DrawTriangle(float offsetX, float offsetY, float scaleX, float scaleY, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
    void DrawDownTriangle(float offsetX, float offsetY, float scaleX, float scaleY, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
    void DrawQuad(float offsetX, float offsetY, float scaleX, float scaleY, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
    void DrawDiamond(float offsetX, float offsetY, float scaleX, float scaleY, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
    void DrawText(const std::string& text, float x, float y, float scale);
    void DrawNumbers(const std::string& text, float x, float y, float scale);
    void DrawSprite(ID3D11ShaderResourceView* srv, float x, float y, float scaleX, float scaleY);
    void EndFrame();

private:
    bool CreateRenderTarget();
    bool CreateShaders();
    bool CreateTriangle();
    bool CreateDownTriangle();
    bool CreateQuad();
    bool CreateDiamond();
    bool CreateSpritePipeline();
    void ReleaseAll();
};