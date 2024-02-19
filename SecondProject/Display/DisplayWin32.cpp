#include "DisplayWin32.h"

#include <iostream>

DisplayWin32::DisplayWin32(UINT width, UINT height, const LPCWSTR name)
{
    hWnd = nullptr;
    hInstance = GetModuleHandle(nullptr);
    windowWidth = width;
    windowHeight = height;
    windowName = name;

    CreateDisplayWindow();
}

void DisplayWin32::Initialize()
{
    const D3D_FEATURE_LEVEL featureLevel[] = {D3D_FEATURE_LEVEL_11_1};

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 2;
    swapDesc.BufferDesc.Width = windowWidth;
    swapDesc.BufferDesc.Height = windowHeight;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = hWnd;
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

    ID3D11Texture2D* backTex;
    res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex); // __uuidof(ID3D11Texture2D)
    res = device->CreateRenderTargetView(backTex, nullptr, &backBuffer);
}


void DisplayWin32::Display()
{
    context->OMSetRenderTargets(1, &backBuffer, nullptr);
    context->DrawIndexed(6, 0, 0);
    context->OMSetRenderTargets(0, nullptr, nullptr);
    swapChain->Present(1, 0);
}

void DisplayWin32::ResetState()
{
    context->ClearState();

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(windowWidth);
    viewport.Height = static_cast<float>(windowHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    switch (umessage)
    {
    case WM_KEYDOWN:
        {
            // If a key is pressed send it to the input object so it can record that state.
            std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

            if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
            return 0;
        }
    default:
        {
            return DefWindowProc(hwnd, umessage, wparam, lparam);
        }
    }
}

void DisplayWin32::CreateDisplayWindow()
{
    WNDCLASSEX wc;

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = windowName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    RegisterClassEx(&wc);

    RECT windowRect = {0, 0, static_cast<LONG>(windowWidth), static_cast<LONG>(windowHeight)};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    const auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

    const auto posX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
    const auto posY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, windowName, windowName,
                          dwStyle,
                          posX, posY,
                          windowRect.right - windowRect.left,
                          windowRect.bottom - windowRect.top,
                          nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    ShowCursor(true);
}