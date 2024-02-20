#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "../SimpleMath.h"

class DisplayWin32
{
public:
    DisplayWin32(UINT width, UINT height, LPCWSTR name);
    HWND hWnd;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapChain;
    void Initialize();
    void Display();
    void ResetState();
    void DestroyResources();

private:
    UINT windowWidth;
    UINT windowHeight;
    LPCWSTR windowName;
    HINSTANCE hInstance;
    ID3D11RenderTargetView* renderTargetView;
    void CreateDisplayWindow();
};