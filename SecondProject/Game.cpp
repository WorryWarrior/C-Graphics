#include "Game.h"

#include <chrono>
#include <iostream>
#include "Input/Delegates.h"

#include "Components/TriangleComponent.h"

#pragma comment(lib, "d3d11.lib")

int windowWidth = 800;
int windowHeight = 600;

Game::Game()
{
}

Game::~Game()
{
    DestroyResources();
}

void Game::Initialize()
{
    display = new DisplayWin32(windowWidth, windowHeight, L"TestApplication");
    input_device = new InputDevice(this);

    CreateBackBuffer();

    for (const auto& component : components)
    {
        component->Initialize();
    }
}

void Game::Run()
{
    std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
    float totalTime = 0;
    unsigned int frameCount = 0;
    MSG msg = {};
    bool isExitRequested = false;

    while (!isExitRequested)
    {
        HandleSystemMessages(msg, isExitRequested);

        PrepareFrame();

        auto curTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>
            (curTime - PrevTime).count() / 1000000.0f;
        PrevTime = curTime;

        totalTime += deltaTime;
        frameCount++;

        Update();

        if (totalTime > 1.0f)
        {
            totalTime -= 1.0f;
            frameCount = 0;
        }

        Draw();
    }

    Exit();
}

void Game::PrepareFrame()
{
    display->ResetState();
}


void Game::Draw()
{
    for (const auto& component : components)
    {
        component->Draw();
    }

    display->Display();
}

void Game::CreateBackBuffer()
{
    display->Initialize();
}

void Game::DestroyResources()
{
    display->DestroyResources();

    delete display;
    delete input_device;

    for (const auto& component : components)
    {
        component->DestroyResources();
        delete component;
    }
}

void Game::HandleSystemMessages(MSG& msg, bool& isExitRequested)
{
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);

        if (msg.message == WM_INPUT)
        {
            RAWINPUT raw;
            UINT dataSize = sizeof(RAWINPUT);
            GetRawInputData(reinterpret_cast<HRAWINPUT>(msg.lParam), RID_INPUT, &raw, &dataSize,
                            sizeof(RAWINPUTHEADER));

            if (raw.header.dwType == RIM_TYPEKEYBOARD)
            {
                input_device->OnKeyDown({
                    raw.data.keyboard.MakeCode,
                    raw.data.keyboard.Flags,
                    raw.data.keyboard.VKey,
                    raw.data.keyboard.Message
                });
            }
            else if (raw.header.dwType == RIM_TYPEMOUSE)
            {
                input_device->OnMouseMove({
                    raw.data.mouse.usFlags,
                    raw.data.mouse.usButtonFlags,
                    static_cast<int>(raw.data.mouse.ulExtraInformation),
                    static_cast<int>(raw.data.mouse.ulRawButtons),
                    static_cast<short>(raw.data.mouse.usButtonData),
                    raw.data.mouse.lLastX,
                    raw.data.mouse.lLastY
                });
            }
        }

        DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT)
    {
        isExitRequested = true;
    }
}


void Game::AddGameComponent(GameComponent* inComponent)
{
    components.push_back(inComponent);
}

void Game::Update()
{
    for (const auto& component : components)
    {
        component->Update();
    }

}


void Game::Exit()
{
    DestroyResources();
}