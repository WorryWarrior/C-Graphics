#include "D3DApplication.h"
#include "../Components/GameComponent.h"
#include "DisplayWin32.h"

void D3DApplication::InitializeResources()
{
    CreateDeviceAndSwapchain();
    CreateViewport();
    CreateRastState();
}

HRESULT D3DApplication::CreateDeviceAndSwapchain()
{
    const D3D_FEATURE_LEVEL featureLevel[] = {D3D_FEATURE_LEVEL_11_1};

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 2;
    swapDesc.BufferDesc.Width = display->windowWidth;
    swapDesc.BufferDesc.Height = display->windowHeight;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = display->hWnd;
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;

    auto res = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapDesc,
        &swapChain,
        &device,
        nullptr,
        &context);

    res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)); // __uuidof(ID3D11Texture2D)
    res = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

    return res;
}

HRESULT D3DApplication::CreateRastState()
{
    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;
    HRESULT res = device->CreateRasterizerState(&rastDesc, &rastState);

    context->RSSetState(rastState);

    return res;
}

HRESULT D3DApplication::CreateViewport() const
{
    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(display->windowWidth);
    viewport.Height = static_cast<float>(display->windowHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    return S_OK;
}

void D3DApplication::DestroyResources()
{
    device->Release();
    context->Release();
    swapChain->Release();
    backBuffer->Release();
    renderTargetView->Release();
    rastState->Release();
}

void D3DApplication::Render()
{
    const float clearColour[4] = { 0.25f, 0.25f, 0.25f, 0.0f };
    context->ClearState();

    context->OMSetRenderTargets(1, &renderTargetView, nullptr);
    context->ClearRenderTargetView(renderTargetView, clearColour);
    context->RSSetState(rastState);
    CreateViewport();

    for (auto c : components)
    {
        c->Draw();
    }

    context->OMSetRenderTargets(0, nullptr, nullptr);
    swapChain->Present(1, 0);
}