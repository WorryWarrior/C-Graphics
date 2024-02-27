#pragma once

#include <d3d11.h>
#include <vector>

#include "../SimpleMath.h"

class DisplayWin32;
class GameComponent;

class D3DApplication
{
public:
    DisplayWin32* display;

    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapChain;
    ID3D11Texture2D* backBuffer;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11RasterizerState* rastState;

    std::vector<GameComponent*> components;

    void InitializeResources();
    void Render();
    virtual void DestroyResources();

private:

    HRESULT CreateDeviceAndSwapchain();
    HRESULT CreateRastState();
    HRESULT CreateViewport() const;
};