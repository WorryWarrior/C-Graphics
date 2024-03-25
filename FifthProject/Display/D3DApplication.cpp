#include "D3DApplication.h"
#include "../Components/GameComponent.h"
#include "DisplayWin32.h"

void D3DApplication::InitializeResources()
{
    CreateDeviceAndSwapchain();
    CreateViewport();
    CreateRastState();
    CreateDepthBuffer();
    CreateLightBuffers();
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

HRESULT D3DApplication::CreateDepthBuffer()
{
    D3D11_TEXTURE2D_DESC depthDesc;
    depthDesc.Width = display->windowWidth;
    depthDesc.Height = display->windowHeight;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.CPUAccessFlags = 0;
    depthDesc.MiscFlags = 0;

    device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
    device->CreateDepthStencilView(depthBuffer, nullptr, &depthView);
    context->OMSetRenderTargets(1, &renderTargetView, depthView);

    return S_OK;
}

HRESULT D3DApplication::CreateLightBuffers()
{
    staticLightData.Color = DirectX::SimpleMath::Vector4(1, 1, 1, 0);
    staticLightData.Direction = DirectX::SimpleMath::Vector4(0.7f, -0.7f, 0, 0);
    staticLightData.K = DirectX::SimpleMath::Vector4(0.2f, 50.0f, 1.2f, 0);

    pointLightData.Color = DirectX::SimpleMath::Vector4(0, 1, 0, 0);
    pointLightData.Direction = DirectX::SimpleMath::Vector4(0, 1, 0, 0); // Position
    pointLightData.K = DirectX::SimpleMath::Vector4(0.2f, 10.0f, 1.2f, 0);

    D3D11_BUFFER_DESC staticLightBufPositionDesc = {};
    staticLightBufPositionDesc.Usage = D3D11_USAGE_DYNAMIC;
    staticLightBufPositionDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    staticLightBufPositionDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    staticLightBufPositionDesc.MiscFlags = 0;
    staticLightBufPositionDesc.StructureByteStride = 0;
    staticLightBufPositionDesc.ByteWidth = sizeof(LightData);

    D3D11_SUBRESOURCE_DATA staticLightBufPositionData = {};
    staticLightBufPositionData.pSysMem = &staticLightData;
    staticLightBufPositionData.SysMemPitch = 0;
    staticLightBufPositionData.SysMemSlicePitch = 0;

    device->CreateBuffer(&staticLightBufPositionDesc, &staticLightBufPositionData, &staticLightBuffer);
    context->PSSetConstantBuffers(1, 1, &staticLightBuffer);

    D3D11_BUFFER_DESC dynamicLightBufPositionDesc = {};
    dynamicLightBufPositionDesc.Usage = D3D11_USAGE_DYNAMIC;
    dynamicLightBufPositionDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    dynamicLightBufPositionDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    dynamicLightBufPositionDesc.MiscFlags = 0;
    dynamicLightBufPositionDesc.StructureByteStride = 0;
    dynamicLightBufPositionDesc.ByteWidth = sizeof(LightData);

    D3D11_SUBRESOURCE_DATA dynamicLightBufPositionData = {};
    dynamicLightBufPositionData.pSysMem = &pointLightData;
    dynamicLightBufPositionData.SysMemPitch = 0;
    dynamicLightBufPositionData.SysMemSlicePitch = 0;

    device->CreateBuffer(&dynamicLightBufPositionDesc, &dynamicLightBufPositionData, &dynamicLightBuffer);
    context->PSSetConstantBuffers(2, 1, &dynamicLightBuffer);

    return S_OK;
}

float t;

void D3DApplication::UpdateLights()
{
    D3D11_MAPPED_SUBRESOURCE staticMss;
    context->Map(staticLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &staticMss);
    memcpy(staticMss.pData, &staticLightData, sizeof(LightData));
    context->Unmap(staticLightBuffer, 0);
    context->PSSetConstantBuffers(1, 1, &staticLightBuffer);

    t += 0.001f;
    pointLightData.Direction = DirectX::SimpleMath::Vector4(0, cos(t) * 5, 1, 0);

    D3D11_MAPPED_SUBRESOURCE dynamicMss;
    context->Map(dynamicLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dynamicMss);
    memcpy(dynamicMss.pData, &pointLightData, sizeof(LightData));
    context->Unmap(dynamicLightBuffer, 0);
    context->PSSetConstantBuffers(2, 1, &dynamicLightBuffer);
}

void D3DApplication::DestroyResources()
{
    device->Release();
    context->Release();
    swapChain->Release();
    backBuffer->Release();
    depthView->Release();
    depthBuffer->Release();
    renderTargetView->Release();
    rastState->Release();
}

void D3DApplication::Render()
{
    const float clearColour[4] = { 0.25f, 0.25f, 0.25f, 0.0f };
    context->ClearState();
    CreateViewport();

    //context->OMSetRenderTargets(1, &renderTargetView, nullptr);
    context->ClearRenderTargetView(renderTargetView, clearColour);
    context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    context->OMSetRenderTargets(1, &renderTargetView, depthView);
    context->RSSetState(rastState);

    UpdateLights();

    for (auto c : components)
    {
        c->Draw();
    }

    //context->OMSetRenderTargets(0, nullptr, nullptr);
    swapChain->Present(1,  DXGI_PRESENT_DO_NOT_WAIT);
}