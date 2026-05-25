#include "GraphicsContext.h"
#include <cstring>

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
    ConstantBufferGPU(nullptr)
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

bool GraphicsContext::CreateShaders()
{
    const char* shaderSrc = R"(
cbuffer ConstantBuffer : register(b0)
{
    float offsetX;
    float offsetY;
    float scaleX;
    float scaleY;
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
    output.col = input.col;
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

void GraphicsContext::BeginFrame()
{
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Context->OMSetRenderTargets(1, &RTV, nullptr);
    Context->ClearRenderTargetView(RTV, clearColor);

    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = 720.0f;
    vp.Height = 960.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    Context->RSSetViewports(1, &vp);
}

void GraphicsContext::DrawTriangle(float offsetX, float offsetY, float scaleX, float scaleY)
{
    ConstantBuffer cb = {};
    cb.offsetX = offsetX;
    cb.offsetY = offsetY;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;

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

void GraphicsContext::DrawDownTriangle(float offsetX, float offsetY, float scaleX, float scaleY)
{
    ConstantBuffer cb = {};
    cb.offsetX = offsetX;
    cb.offsetY = offsetY;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;

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

void GraphicsContext::DrawQuad(float offsetX, float offsetY, float scaleX, float scaleY)
{
    ConstantBuffer cb = {};
    cb.offsetX = offsetX;
    cb.offsetY = offsetY;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;

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

void GraphicsContext::DrawDiamond(float offsetX, float offsetY, float scaleX, float scaleY)
{
    ConstantBuffer cb = {};
    cb.offsetX = offsetX;
    cb.offsetY = offsetY;
    cb.scaleX = scaleX;
    cb.scaleY = scaleY;

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

void GraphicsContext::EndFrame()
{
    SwapChain->Present(1, 0);
}