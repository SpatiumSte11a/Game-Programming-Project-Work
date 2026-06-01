#include "GraphicsContext.h"
#include <cstring>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#include <vector>
#include "Player.h"

GraphicsContext::GraphicsContext()
    : Device(nullptr),
    Context(nullptr),
    SwapChain(nullptr),
    RTV(nullptr),
    VertexShader(nullptr),
    PixelShader(nullptr),
    InputLayout(nullptr),
    TriangleVertexBuffer(nullptr),
    DownTriangleVertexBuffer(nullptr),
    QuadVertexBuffer(nullptr),
    DiamondVertexBuffer(nullptr),
    ConstantBufferGPU(nullptr),
    TextVertexShader(nullptr),
    TextPixelShader(nullptr),
    TextInputLayout(nullptr),
    TextVertexBuffer(nullptr),
    FontTexture(nullptr),
    NumbersTexture(nullptr),
    ShipTexture(nullptr),
    Enemy1Texture(nullptr),
    Enemy2Texture(nullptr),
    Enemy3Texture(nullptr),
    BonusStarTexture(nullptr),
    SamplerState(nullptr),
    BlendState(nullptr),
    SpriteVertexShader(nullptr),
    SpritePixelShader(nullptr),
    SpriteInputLayout(nullptr),
    SpriteVertexBuffer(nullptr),
    SpriteConstantBuffer(nullptr),
    NoiseVertexShader(nullptr),
    NoisePixelShader(nullptr),
    NoiseConstantBuffer(nullptr),
    ViewportWidth(0),
    ViewportHeight(0)
{
}

GraphicsContext::~GraphicsContext()
{
    ReleaseAll();
}

void GraphicsContext::ReleaseAll()
{
    if (ConstantBufferGPU) { ConstantBufferGPU->Release(); ConstantBufferGPU = nullptr; }
    if (DiamondVertexBuffer) { DiamondVertexBuffer->Release(); DiamondVertexBuffer = nullptr; }
    if (QuadVertexBuffer) { QuadVertexBuffer->Release(); QuadVertexBuffer = nullptr; }
    if (DownTriangleVertexBuffer) { DownTriangleVertexBuffer->Release(); DownTriangleVertexBuffer = nullptr; }
    if (TriangleVertexBuffer) { TriangleVertexBuffer->Release(); TriangleVertexBuffer = nullptr; }
    if (InputLayout) { InputLayout->Release(); InputLayout = nullptr; }
    if (VertexShader) { VertexShader->Release(); VertexShader = nullptr; }
    if (PixelShader) { PixelShader->Release(); PixelShader = nullptr; }
    if (RTV) { RTV->Release(); RTV = nullptr; }
    if (SwapChain) { SwapChain->Release(); SwapChain = nullptr; }
    if (Context) { Context->Release(); Context = nullptr; }
    if (Device) { Device->Release(); Device = nullptr; }

    if (FontTexture) { FontTexture->Release(); FontTexture = nullptr; }
    if (NumbersTexture) { NumbersTexture->Release(); NumbersTexture = nullptr; }
    if (ShipTexture) { ShipTexture->Release(); ShipTexture = nullptr; }
    if (Enemy1Texture) { Enemy1Texture->Release(); Enemy1Texture = nullptr; }
    if (Enemy2Texture) { Enemy2Texture->Release(); Enemy2Texture = nullptr; }
    if (Enemy3Texture) { Enemy3Texture->Release(); Enemy3Texture = nullptr; }
    if (BonusStarTexture) { BonusStarTexture->Release(); BonusStarTexture = nullptr; }
    if (TextVertexBuffer) { TextVertexBuffer->Release(); TextVertexBuffer = nullptr; }
    if (TextVertexShader) { TextVertexShader->Release(); TextVertexShader = nullptr; }
    if (TextPixelShader) { TextPixelShader->Release(); TextPixelShader = nullptr; }
    if (TextInputLayout) { TextInputLayout->Release(); TextInputLayout = nullptr; }
    if (SamplerState) { SamplerState->Release(); SamplerState = nullptr; }
    if (BlendState) { BlendState->Release(); BlendState = nullptr; }
    if (SpriteConstantBuffer) { SpriteConstantBuffer->Release(); SpriteConstantBuffer = nullptr; }
    if (SpriteVertexBuffer) { SpriteVertexBuffer->Release();   SpriteVertexBuffer = nullptr; }
    if (SpriteInputLayout) { SpriteInputLayout->Release();    SpriteInputLayout = nullptr; }
    if (SpritePixelShader) { SpritePixelShader->Release();    SpritePixelShader = nullptr; }
    if (SpriteVertexShader) { SpriteVertexShader->Release();   SpriteVertexShader = nullptr; }

    if (NoiseConstantBuffer) { NoiseConstantBuffer->Release(); NoiseConstantBuffer = nullptr; }
    if (NoisePixelShader) { NoisePixelShader->Release(); NoisePixelShader = nullptr; }
    if (NoiseVertexShader) { NoiseVertexShader->Release(); NoiseVertexShader = nullptr; }
}

bool GraphicsContext::Initialize(HWND hWnd, int width, int height)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &SwapChain,
        &Device,
        nullptr,
        &Context
    );

    if (FAILED(hr))
        return false;

    if (!CreateRenderTarget())
        return false;

    if (!CreateShaders())
        return false;

    if (!CreateTriangle())
        return false;

    if (!CreateDownTriangle())
        return false;

    if (!CreateQuad())
        return false;

    if (!CreateDiamond())
        return false;

    if (!CreateBlendState())
        return false;

    if (!CreateTextPipeline())
        return false;

    if (!LoadTextures())
        return false;

    if (!CreateSpritePipeline())
        return false;

    if (!CreateNoisePipeline())
        return false;

    ShipTexture = LoadSprite(L"ship.png");
    Enemy1Texture = LoadSprite(L"enemy.png");
    Enemy2Texture = LoadSprite(L"enemy2.png");
    Enemy3Texture = LoadSprite(L"enemy3.png");
    BonusStarTexture = LoadSprite(L"bonus_star.png");

    if (!ShipTexture)
        return false;

    if (!Enemy1Texture)
        return false;

    if (!Enemy2Texture)
        return false;

    if (!Enemy3Texture)
        return false;

    if (!BonusStarTexture)
        return false;

    return true;
}

bool GraphicsContext::CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;

    HRESULT hr = SwapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&backBuffer
    );

    if (FAILED(hr))
        return false;

    hr = Device->CreateRenderTargetView(backBuffer, nullptr, &RTV);
    backBuffer->Release();

    if (FAILED(hr))
        return false;

    return true;
}

bool GraphicsContext::CreateTextPipeline()
{
    const char* src = R"(
Texture2D   fontTex : register(t0);
SamplerState samp   : register(s0);

struct VS_INPUT { float3 pos : POSITION; float2 uv : TEXCOORD; };
struct PS_INPUT { float4 pos : SV_POSITION; float2 uv : TEXCOORD; };

PS_INPUT VSMain(VS_INPUT i)
{
    PS_INPUT o;
    o.pos = float4(i.pos, 1.0f);
    o.uv  = i.uv;
    return o;
}

float4 PSMain(PS_INPUT i) : SV_TARGET
{
    float4 t = fontTex.Sample(samp, i.uv);
    
    float brightness = (t.r + t.g + t.b) / 3.0f;
    if (brightness < 0.05f) discard;

    return float4(t.rgb, 1.0f);
}

)";

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errBlob = nullptr;

    HRESULT hr = D3DCompile(src, strlen(src), nullptr, nullptr, nullptr,
        "VSMain", "vs_5_0", 0, 0, &vsBlob, &errBlob);
    if (FAILED(hr))
    {
        if (errBlob) { OutputDebugStringA((char*)errBlob->GetBufferPointer()); errBlob->Release(); errBlob = nullptr; }
        return false;
    }

    hr = D3DCompile(src, strlen(src), nullptr, nullptr, nullptr,
        "PSMain", "ps_5_0", 0, 0, &psBlob, &errBlob);
    if (FAILED(hr))
    {
        vsBlob->Release();
        if (errBlob) { OutputDebugStringA((char*)errBlob->GetBufferPointer()); errBlob->Release(); errBlob = nullptr; }
        return false;
    }

    Device->CreateVertexShader(vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(), nullptr, &TextVertexShader);
    Device->CreatePixelShader(psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(), nullptr, &TextPixelShader);

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    Device->CreateInputLayout(layout, 2,
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        &TextInputLayout);

    vsBlob->Release();
    psBlob->Release();

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(FontVertex) * 6;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = Device->CreateBuffer(&bd, nullptr, &TextVertexBuffer);
    return SUCCEEDED(hr);
}

bool GraphicsContext::LoadTextures()
{
    IWICImagingFactory* factory = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) return false;

    auto LoadTex = [&](LPCWSTR path, ID3D11ShaderResourceView** srv) -> bool {
        IWICBitmapDecoder* decoder = nullptr;
        if (FAILED(factory->CreateDecoderFromFilename(path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder))) return false;
        IWICBitmapFrameDecode* frame = nullptr;
        decoder->GetFrame(0, &frame);
        IWICFormatConverter* conv = nullptr;
        factory->CreateFormatConverter(&conv);
        conv->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
        UINT w = 0, h = 0;
        conv->GetSize(&w, &h);
        BYTE* pixels = new BYTE[w * h * 4];
        conv->CopyPixels(nullptr, w * 4, w * h * 4, pixels);
        D3D11_TEXTURE2D_DESC td = {};
        td.Width = w; td.Height = h; td.MipLevels = 1; td.ArraySize = 1;
        td.Format = DXGI_FORMAT_R8G8B8A8_UNORM; td.SampleDesc.Count = 1;
        td.Usage = D3D11_USAGE_IMMUTABLE; td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        D3D11_SUBRESOURCE_DATA sd = { pixels, (UINT)(w * 4), 0 };
        ID3D11Texture2D* tex = nullptr;
        hr = Device->CreateTexture2D(&td, &sd, &tex);
        delete[] pixels;
        conv->Release(); frame->Release(); decoder->Release();
        if (FAILED(hr)) return false;
        hr = Device->CreateShaderResourceView(tex, nullptr, srv);
        tex->Release();
        return SUCCEEDED(hr);
        };

    if (!LoadTex(L"font.png", &FontTexture)) { factory->Release(); return false; }
    if (!LoadTex(L"numbers.png", &NumbersTexture)) { factory->Release(); return false; }

    factory->Release();

    D3D11_SAMPLER_DESC samp = {};
    samp.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    hr = Device->CreateSamplerState(&samp, &SamplerState);
    return SUCCEEDED(hr);
}

void GraphicsContext::DrawNumbers(const std::string& text, float startX, float startY, float scale)
{
    const int   COLS = 10;
    const float CELL_W = 1.0f / COLS;
    const float CELL_H = 1.0f;

    const float GLYPH_W = 0.06f * scale;
    const float GLYPH_H = 0.08f * scale;
    const float SPACE_W = 0.05f * scale;

    UINT stride = sizeof(FontVertex), offset = 0;
    Context->IASetInputLayout(TextInputLayout);
    Context->IASetVertexBuffers(0, 1, &TextVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->VSSetShader(TextVertexShader, nullptr, 0);
    Context->PSSetShader(TextPixelShader, nullptr, 0);
    Context->PSSetShaderResources(0, 1, &NumbersTexture);
    Context->PSSetSamplers(0, 1, &SamplerState);

    float x = startX;
    for (char c : text)
    {
        if (c == ' ') { x += SPACE_W; continue; }
        if (c < '0' || c > '9') continue;

        int digit = c - '0';
        int idx = (digit == 0) ? 9 : (digit - 1);
        float u0 = idx * CELL_W;
        float u1 = (idx + 1) * CELL_W;
        float v0 = 0.0f;
        float v1 = 1.0f;

        float x1 = x + GLYPH_W;
        float y1 = startY - GLYPH_H;

        FontVertex quad[6] =
        {
            { x,  startY, 0.f, u0, v0 },
            { x1, startY, 0.f, u1, v0 },
            { x,  y1,     0.f, u0, v1 },
            { x1, startY, 0.f, u1, v0 },
            { x1, y1,     0.f, u1, v1 },
            { x,  y1,     0.f, u0, v1 },
        };

        D3D11_MAPPED_SUBRESOURCE mapped = {};
        if (SUCCEEDED(Context->Map(TextVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
        {
            memcpy(mapped.pData, quad, sizeof(quad));
            Context->Unmap(TextVertexBuffer, 0);
            Context->Draw(6, 0);
        }
        x += GLYPH_W;
    }
}

bool GraphicsContext::CreateShaders()
{
    const char* shaderSrc = R"(
cbuffer ConstantBuffer : register(b0)
{
    float offsetX;
    float offsetY;
    float scaleX;
    float scaleY;
    float4 tintColor;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;

    float3 finalPos = input.pos;
    finalPos.x *= scaleX;
    finalPos.y *= scaleY;
    finalPos.x += offsetX;
    finalPos.y += offsetY;

    output.pos = float4(finalPos, 1.0f);
    output.col = input.col * tintColor;
    return output;
}

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    return input.col;
}
)";

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompile(
        shaderSrc,
        strlen(shaderSrc),
        nullptr,
        nullptr,
        nullptr,
        "VSMain",
        "vs_5_0",
        0,
        0,
        &vsBlob,
        &errorBlob
    );

    if (FAILED(hr))
    {
        if (errorBlob) errorBlob->Release();
        return false;
    }

    hr = D3DCompile(
        shaderSrc,
        strlen(shaderSrc),
        nullptr,
        nullptr,
        nullptr,
        "PSMain",
        "ps_5_0",
        0,
        0,
        &psBlob,
        &errorBlob
    );

    if (FAILED(hr))
    {
        if (vsBlob) vsBlob->Release();
        if (errorBlob) errorBlob->Release();
        return false;
    }

    hr = Device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &VertexShader
    );

    if (FAILED(hr))
    {
        vsBlob->Release();
        psBlob->Release();
        return false;
    }

    hr = Device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &PixelShader
    );

    if (FAILED(hr))
    {
        vsBlob->Release();
        psBlob->Release();
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = Device->CreateInputLayout(
        layout,
        2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &InputLayout
    );

    if (FAILED(hr))
    {
        vsBlob->Release();
        psBlob->Release();
        return false;
    }

    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    hr = Device->CreateBuffer(&cbd, nullptr, &ConstantBufferGPU);

    vsBlob->Release();
    psBlob->Release();

    if (FAILED(hr))
        return false;

    return true;
}

bool GraphicsContext::CreateBlendState()
{
    D3D11_BLEND_DESC bd = {};
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRESULT hr = Device->CreateBlendState(&bd, &BlendState);
    return SUCCEEDED(hr);
}

bool GraphicsContext::CreateTriangle()
{
    Vertex vertices[] =
    {
        {  0.0f,  0.08f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f },
        {  0.04f, -0.02f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { -0.04f, -0.02f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = Device->CreateBuffer(&bd, &initData, &TriangleVertexBuffer);

    if (FAILED(hr))
        return false;

    return true;
}

bool GraphicsContext::CreateDownTriangle()
{
    Vertex vertices[] =
    {
        {  0.0f, -0.08f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f },
        { -0.04f, 0.02f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f },
        {  0.04f, 0.02f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f }
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = Device->CreateBuffer(&bd, &initData, &DownTriangleVertexBuffer);

    if (FAILED(hr))
        return false;

    return true;
}

bool GraphicsContext::CreateQuad()
{
    Vertex vertices[] =
    {
        { -0.05f,  0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        {  0.05f,  0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        {  0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },

        { -0.05f,  0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        {  0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { -0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = Device->CreateBuffer(&bd, &initData, &QuadVertexBuffer);

    if (FAILED(hr))
        return false;

    return true;
}

bool GraphicsContext::CreateDiamond()
{
    Vertex vertices[] =
    {
        {  0.00f,  0.07f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        {  0.05f,  0.00f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        {  0.00f, -0.07f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },

        {  0.00f,  0.07f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        {  0.00f, -0.07f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { -0.05f,  0.00f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = Device->CreateBuffer(&bd, &initData, &DiamondVertexBuffer);

    if (FAILED(hr))
        return false;

    return true;
}

void GraphicsContext::BeginFrame(float shakeX, float shakeY)
{
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Context->OMSetRenderTargets(1, &RTV, nullptr);
    Context->ClearRenderTargetView(RTV, clearColor);

    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = shakeX;
    vp.TopLeftY = shakeY;
    vp.Width = 720.0f;
    vp.Height = 960.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    Context->RSSetViewports(1, &vp);
}

void GraphicsContext::DrawTriangle(float offsetX, float offsetY, float scaleX, float scaleY, float r, float g, float b, float a)
{
    ConstantBuffer cb = {};
    cb.offsetX = offsetX;
    cb.offsetY = offsetY;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;
    cb.tintR = r;
    cb.tintG = g;
    cb.tintB = b;
    cb.tintA = a;

    Context->UpdateSubresource(ConstantBufferGPU, 0, nullptr, &cb, 0, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    Context->IASetInputLayout(InputLayout);
    Context->IASetVertexBuffers(0, 1, &TriangleVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Context->VSSetShader(VertexShader, nullptr, 0);
    Context->PSSetShader(PixelShader, nullptr, 0);
    Context->VSSetConstantBuffers(0, 1, &ConstantBufferGPU);

    Context->Draw(3, 0);
}

void GraphicsContext::DrawDownTriangle(float offsetX, float offsetY, float scaleX, float scaleY, float r, float g, float b, float a)
{
    ConstantBuffer cb = {};
    cb.offsetX = offsetX;
    cb.offsetY = offsetY;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;
    cb.tintR = r;
    cb.tintG = g;
    cb.tintB = b;
    cb.tintA = a;

    Context->UpdateSubresource(ConstantBufferGPU, 0, nullptr, &cb, 0, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    Context->IASetInputLayout(InputLayout);
    Context->IASetVertexBuffers(0, 1, &DownTriangleVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Context->VSSetShader(VertexShader, nullptr, 0);
    Context->PSSetShader(PixelShader, nullptr, 0);
    Context->VSSetConstantBuffers(0, 1, &ConstantBufferGPU);

    Context->Draw(3, 0);
}

void GraphicsContext::DrawQuad(float offsetX, float offsetY, float scaleX, float scaleY, float r, float g, float b, float a)
{
    ConstantBuffer cb = {};
    cb.offsetX = offsetX;
    cb.offsetY = offsetY;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;
    cb.tintR = r;
    cb.tintG = g;
    cb.tintB = b;
    cb.tintA = a;

    Context->UpdateSubresource(ConstantBufferGPU, 0, nullptr, &cb, 0, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    Context->IASetInputLayout(InputLayout);
    Context->IASetVertexBuffers(0, 1, &QuadVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Context->VSSetShader(VertexShader, nullptr, 0);
    Context->PSSetShader(PixelShader, nullptr, 0);
    Context->VSSetConstantBuffers(0, 1, &ConstantBufferGPU);

    Context->Draw(6, 0);
}

void GraphicsContext::DrawDiamond(float offsetX, float offsetY, float scaleX, float scaleY, float r, float g, float b, float a)
{
    ConstantBuffer cb = {};
    cb.offsetX = offsetX;
    cb.offsetY = offsetY;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;
    cb.tintR = r;
    cb.tintG = g;
    cb.tintB = b;
    cb.tintA = a;

    Context->UpdateSubresource(ConstantBufferGPU, 0, nullptr, &cb, 0, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    Context->IASetInputLayout(InputLayout);
    Context->IASetVertexBuffers(0, 1, &DiamondVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Context->VSSetShader(VertexShader, nullptr, 0);
    Context->PSSetShader(PixelShader, nullptr, 0);
    Context->VSSetConstantBuffers(0, 1, &ConstantBufferGPU);

    Context->Draw(6, 0);
}

void GraphicsContext::DrawText(const std::string& text, float startX, float startY, float scale)
{
    const int   COLS = 13;
    const float CELL_W = 1.0f / COLS;
    const float CELL_H = 1.0f / 2.0f;

    const float INS_X0 = 0.0024f;
    const float INS_X1 = 0.0030f;
    const float INS_Y = 0.0394f;

    const float GLYPH_W = 0.07f * scale;
    const float GLYPH_H = 0.09f * scale;
    const float SPACE_W = 0.055f * scale;

    UINT stride = sizeof(FontVertex), offset = 0;
    Context->IASetInputLayout(TextInputLayout);
    Context->IASetVertexBuffers(0, 1, &TextVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->VSSetShader(TextVertexShader, nullptr, 0);
    Context->PSSetShader(TextPixelShader, nullptr, 0);
    Context->PSSetShaderResources(0, 1, &FontTexture);
    Context->PSSetSamplers(0, 1, &SamplerState);

    float x = startX;

    for (char c : text)
    {
        if (c == ' ') { x += SPACE_W; continue; }

        if (c >= 'a' && c <= 'z') c -= 32;
        if (c < 'A' || c > 'Z')   continue;

        int idx = c - 'A';
        int col = idx % COLS;
        int row = idx / COLS;

        float u0 = col * CELL_W + INS_X0;
        float u1 = (col + 1) * CELL_W - INS_X1;
        float v0 = row * CELL_H + INS_Y;
        float v1 = (row + 1) * CELL_H - INS_Y;

        float x1 = x + GLYPH_W;
        float y1 = startY - GLYPH_H;

        FontVertex quad[6] =
        {
            { x,  startY, 0.f, u0, v0 },
            { x1, startY, 0.f, u1, v0 },
            { x,  y1,     0.f, u0, v1 },

            { x1, startY, 0.f, u1, v0 },
            { x1, y1,     0.f, u1, v1 },
            { x,  y1,     0.f, u0, v1 },
        };

        D3D11_MAPPED_SUBRESOURCE mapped = {};
        HRESULT hr = Context->Map(TextVertexBuffer, 0,
            D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        if (FAILED(hr)) return;
        memcpy(mapped.pData, quad, sizeof(quad));
        Context->Unmap(TextVertexBuffer, 0);

        Context->Draw(6, 0);

        x += GLYPH_W;
    }
}

bool GraphicsContext::CreateSpritePipeline()
{
    const char* src = R"(
cbuffer CB : register(b0)
{
    float offsetX;
    float offsetY;
    float scaleX;
    float scaleY;
};

struct VS_IN  { float3 pos : POSITION; float2 uv : TEXCOORD; };
struct PS_IN  { float4 pos : SV_POSITION; float2 uv : TEXCOORD; };

PS_IN VSMain(VS_IN v)
{
    PS_IN o;
    o.pos = float4(v.pos.x * scaleX + offsetX,
                   v.pos.y * scaleY + offsetY,
                   0.0f, 1.0f);
    o.uv = v.uv;
    return o;
}

Texture2D    tex : register(t0);
SamplerState smp : register(s0);

float4 PSMain(PS_IN p) : SV_TARGET
{
    return tex.Sample(smp, p.uv);
}
)";

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errBlob = nullptr;

    D3DCompile(src, strlen(src), nullptr, nullptr, nullptr,
        "VSMain", "vs_5_0", 0, 0, &vsBlob, &errBlob);
    if (errBlob) { errBlob->Release(); errBlob = nullptr; }

    D3DCompile(src, strlen(src), nullptr, nullptr, nullptr,
        "PSMain", "ps_5_0", 0, 0, &psBlob, &errBlob);
    if (errBlob) { errBlob->Release(); errBlob = nullptr; }

    Device->CreateVertexShader(vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr, &SpriteVertexShader);

    Device->CreatePixelShader(psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr, &SpritePixelShader);

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12,
          D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    Device->CreateInputLayout(layout, 2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &SpriteInputLayout);

    vsBlob->Release();
    psBlob->Release();

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(FontVertex) * 6;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Device->CreateBuffer(&bd, nullptr, &SpriteVertexBuffer);

    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Device->CreateBuffer(&cbd, nullptr, &SpriteConstantBuffer);

    return (SpriteVertexShader && SpritePixelShader &&
        SpriteInputLayout && SpriteVertexBuffer &&
        SpriteConstantBuffer);
}

ID3D11ShaderResourceView* GraphicsContext::LoadSprite(const wchar_t* path)
{
    IWICImagingFactory* factory = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr,
        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) return nullptr;

    IWICBitmapDecoder* decoder = nullptr;
    hr = factory->CreateDecoderFromFilename(path, nullptr, GENERIC_READ,
        WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr))
    {
        factory->Release();
        return nullptr;
    }

    IWICBitmapFrameDecode* frame = nullptr;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr))
    {
        decoder->Release();
        factory->Release();
        return nullptr;
    }

    IWICFormatConverter* conv = nullptr;
    factory->CreateFormatConverter(&conv);
    conv->Initialize(frame, GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone, nullptr,
        0.0, WICBitmapPaletteTypeCustom);

    UINT w = 0, h = 0;
    conv->GetSize(&w, &h);

    BYTE* pixels = new BYTE[w * h * 4];
    conv->CopyPixels(nullptr, w * 4, w * h * 4, pixels);

    D3D11_TEXTURE2D_DESC td = {};
    td.Width = w;
    td.Height = h;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_IMMUTABLE;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA sd = { pixels, w * 4, 0 };
    ID3D11Texture2D* tex = nullptr;
    ID3D11ShaderResourceView* srv = nullptr;

    Device->CreateTexture2D(&td, &sd, &tex);
    if (tex)
    {
        Device->CreateShaderResourceView(tex, nullptr, &srv);
        tex->Release();
    }

    delete[] pixels;
    conv->Release();
    frame->Release();
    decoder->Release();
    factory->Release();

    return srv;
}

void GraphicsContext::DrawSprite(ID3D11ShaderResourceView* srv,
    float x, float y,
    float scaleX, float scaleY)
{
    FontVertex verts[6] =
    {
        { -0.5f,  0.5f, 0,  0.0f, 0.0f },
        {  0.5f,  0.5f, 0,  1.0f, 0.0f },
        { -0.5f, -0.5f, 0,  0.0f, 1.0f },
        {  0.5f,  0.5f, 0,  1.0f, 0.0f },
        {  0.5f, -0.5f, 0,  1.0f, 1.0f },
        { -0.5f, -0.5f, 0,  0.0f, 1.0f },
    };

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    Context->Map(SpriteVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD,
        0, &mapped);
    memcpy(mapped.pData, verts, sizeof(verts));
    Context->Unmap(SpriteVertexBuffer, 0);

    ConstantBuffer cb = {};
    cb.offsetX = x;
    cb.offsetY = y;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;
    cb.tintR = 1.0f;
    cb.tintG = 1.0f;
    cb.tintB = 1.0f;
    cb.tintA = 1.0f;
    Context->UpdateSubresource(SpriteConstantBuffer, 0, nullptr, &cb, 0, 0);

    float blendFactor[4] = { 0,0,0,0 };
    Context->OMSetBlendState(BlendState, blendFactor, 0xFFFFFFFF);

    UINT stride = sizeof(FontVertex), offset = 0;
    Context->IASetInputLayout(SpriteInputLayout);
    Context->IASetVertexBuffers(0, 1, &SpriteVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->VSSetShader(SpriteVertexShader, nullptr, 0);
    Context->PSSetShader(SpritePixelShader, nullptr, 0);
    Context->VSSetConstantBuffers(0, 1, &SpriteConstantBuffer);
    Context->PSSetShaderResources(0, 1, &srv);
    Context->PSSetSamplers(0, 1, &SamplerState);
    Context->Draw(6, 0);

    Context->OMSetBlendState(nullptr, blendFactor, 0xFFFFFFFF);
}

void GraphicsContext::DrawSpriteUpsideDown(ID3D11ShaderResourceView* srv,
    float x, float y,
    float scaleX, float scaleY)
{
    FontVertex verts[6] =
    {
        { -0.5f,  0.5f, 0,  1.0f, 1.0f },
        {  0.5f,  0.5f, 0,  0.0f, 1.0f },
        { -0.5f, -0.5f, 0,  1.0f, 0.0f },
        {  0.5f,  0.5f, 0,  0.0f, 1.0f },
        {  0.5f, -0.5f, 0,  0.0f, 0.0f },
        { -0.5f, -0.5f, 0,  1.0f, 0.0f },
    };

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    Context->Map(SpriteVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, verts, sizeof(verts));
    Context->Unmap(SpriteVertexBuffer, 0);

    ConstantBuffer cb = {};
    cb.offsetX = x;
    cb.offsetY = y;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;
    cb.tintR = 1.0f;
    cb.tintG = 1.0f;
    cb.tintB = 1.0f;
    cb.tintA = 1.0f;
    Context->UpdateSubresource(SpriteConstantBuffer, 0, nullptr, &cb, 0, 0);

    float blendFactor[4] = { 0,0,0,0 };
    Context->OMSetBlendState(BlendState, blendFactor, 0xFFFFFFFF);

    UINT stride = sizeof(FontVertex), offset = 0;
    Context->IASetInputLayout(SpriteInputLayout);
    Context->IASetVertexBuffers(0, 1, &SpriteVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->VSSetShader(SpriteVertexShader, nullptr, 0);
    Context->PSSetShader(SpritePixelShader, nullptr, 0);
    Context->VSSetConstantBuffers(0, 1, &SpriteConstantBuffer);
    Context->PSSetShaderResources(0, 1, &srv);
    Context->PSSetSamplers(0, 1, &SamplerState);
    Context->Draw(6, 0);

    Context->OMSetBlendState(nullptr, blendFactor, 0xFFFFFFFF);
}

bool GraphicsContext::CreateNoisePipeline()
{
    const char* src = R"(
cbuffer NoiseCB : register(b0)
{
    float time;
    float3 padding;
};

struct VS_INPUT { float3 pos : POSITION; float4 col : COLOR; };
struct PS_INPUT { float4 pos : SV_POSITION; float2 uv : TEXCOORD; };

PS_INPUT VSMain(VS_INPUT i)
{
    PS_INPUT o;
    // Scale Quad (-0.05..0.05) to (-1..1) to cover full screen
    o.pos = float4(i.pos.x * 20.0f, i.pos.y * 20.0f, 0.0f, 1.0f);
    o.uv = float2(o.pos.x * 0.5f + 0.5f, -o.pos.y * 0.5f + 0.5f);
    return o;
}

float rand(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

float4 PSMain(PS_INPUT i) : SV_TARGET
{
    float n = rand(i.uv + time);
    return float4(n, n, n, 1.0f);
}
)";

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errBlob = nullptr;

    HRESULT hr = D3DCompile(src, strlen(src), nullptr, nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vsBlob, &errBlob);
    if (FAILED(hr))
    {
        if (errBlob) errBlob->Release();
        return false;
    }

    hr = D3DCompile(src, strlen(src), nullptr, nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &psBlob, &errBlob);
    if (FAILED(hr))
    {
        vsBlob->Release();
        if (errBlob) errBlob->Release();
        return false;
    }

    hr = Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &NoiseVertexShader);
    if (FAILED(hr)) return false;
    
    hr = Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &NoisePixelShader);
    if (FAILED(hr)) return false;

    vsBlob->Release();
    psBlob->Release();

    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(NoiseConstantBufferData);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    hr = Device->CreateBuffer(&cbd, nullptr, &NoiseConstantBuffer);
    if (FAILED(hr)) return false;

    return true;
}

void GraphicsContext::DrawNoiseBackground(float time)
{
    NoiseConstantBufferData cb = {};
    cb.time = time;
    Context->UpdateSubresource(NoiseConstantBuffer, 0, nullptr, &cb, 0, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    Context->IASetInputLayout(InputLayout);
    Context->IASetVertexBuffers(0, 1, &QuadVertexBuffer, &stride, &offset);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Context->VSSetShader(NoiseVertexShader, nullptr, 0);
    Context->PSSetShader(NoisePixelShader, nullptr, 0);
    Context->VSSetConstantBuffers(0, 1, &NoiseConstantBuffer);

    Context->Draw(6, 0);
}

void GraphicsContext::EndFrame()
{
    SwapChain->Present(1, 0);
}