#pragma once

#include <d3d11.h>

class DisplayWin32
{

public:
    DisplayWin32(UINT width, UINT height, LPCWSTR name);
    HWND hWnd;
    UINT windowWidth;
    UINT windowHeight;

private:
    LPCWSTR windowName;
    HINSTANCE hInstance;
    ID3D11RenderTargetView* renderTargetView;
    void CreateDisplayWindow();
};