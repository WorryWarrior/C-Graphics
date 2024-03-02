#include "Game.h"

#include <chrono>
#include <iostream>

#include "Display/DisplayWin32.h"
#include "Components/GameComponent.h"
#include "Camera/Camera.h"
#include "Camera/FPSCameraController.h"
#include "Camera/ThirdPersonCameraController.h"
#include "Components/GridComponent.h"
#include "Components/SolarSystem/CubePlanetComponent.h"
#include "Components/SolarSystem/SpaceshipComponent.h"

#pragma comment(lib, "d3d11.lib")

Game::Game()
{
}

Game::~Game()
{
    DestroyResources();
}

GameComponent* planetBar;

void Game::Initialize()
{
    display = new DisplayWin32(800, 800, L"TestApplication");
    input_device = new InputDevice(this);

    InitializeResources();

    camera = new Camera(this);

    GameComponent* grid = new GridComponent(this, camera, 25);
    debugComponents.push_back(grid);
    AddGameComponent(grid);


    GameComponent* planetFoo = new CubePlanetComponent(this, camera, nullptr, {0.0f, 0.0f, 0.0f}, 5);
    AddGameComponent(planetFoo);
    /*GameComponent**/ planetBar = new CubePlanetComponent(this, camera, planetFoo, {15.0f, 0.0f, 0.0f}, 5);
    AddGameComponent(planetBar);

    //AddGameComponent(new CubePlanetComponent(this, camera, nullptr, {15.0f, 10.0f, 0.0f}));

    /*GameComponent* buz = new SpaceshipComponent(this, camera);
    buz->position = DirectX::SimpleMath::Vector3(0.0f, 10.0f, 0.0f);
    AddGameComponent(buz);*/

    //cameraController = new FPSCameraController(this, camera, {0.0f, 0.0f, 0.0f});
    cameraController = new ThirdPersonCameraController(this, camera, planetBar);
}

void Game::Run()
{
    std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
    unsigned int frameCount = 0;
    float totalTime = 0;
    MSG msg = {};
    bool isExitRequested = false;

    while (!isExitRequested)
    {
        HandleSystemMessages(msg, isExitRequested);

        auto curTime = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>
            (curTime - PrevTime).count() / 1000000.0f;
        PrevTime = curTime;

        timeSinceStartup += deltaTime;
        totalTime += deltaTime;
        frameCount++;

        Update();

        if (totalTime > 1.0f)
        {
            totalTime -= 1.0f;
            frameCount = 0;
        }

        Render();
    }

    Exit();
}

void Game::DestroyResources()
{
    delete display;
    delete input_device;

    for (auto& component : components)
    {
        component->DestroyResources();
        delete component;
        component = nullptr;
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
    ProcessUserInput();
    cameraController->Update(deltaTime);

    for (GameComponent* component : components)
    {
        component->Update();
    }

}

void Game::ProcessUserInput() const
{
    if (input_device->IsKeyDown(Keys::Q))
    {
        for (GameComponent* debugComponent : debugComponents)
        {
            debugComponent->isActive = !debugComponent->isActive;
        }
    }
}

void Game::Exit()
{
    DestroyResources();
}