#include "Game.h"

#include <chrono>
#include <iostream>

#include "Display/DisplayWin32.h"
#include "Components/GameComponent.h"
#include "Camera/Camera.h"
#include "Camera/FPSCameraController.h"
#include "Camera/ThirdPersonCameraController.h"
#include "Components/Debug/AxisVectorComponent.h"
#include "Components/Debug/GridComponent.h"
#include "Components/SolarSystem/PlanetComponent.h"
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
    display = new DisplayWin32(1200, 1200, L"TestApplication");
    input_device = new InputDevice(this);

    InitializeResources();

    camera = new Camera(this);
    //cameraController = new FPSCameraController(this, camera, {0.0f, 25.0f, 0.0f});

    GameComponent* spaceship = new SpaceshipComponent(this, camera, {0.0f, 0.0f, 0.0f});
    AddGameComponent(spaceship, false);
    cameraController = new ThirdPersonCameraController(this, camera, spaceship);

    AddGameComponent(new GridComponent(this, camera, 25), true);
    AddGameComponent(new AxisVectorComponent(this, camera, AxisDirection::X, 5.0f), true);
    AddGameComponent(new AxisVectorComponent(this, camera, AxisDirection::Y, 5.0f), true);
    AddGameComponent(new AxisVectorComponent(this, camera, AxisDirection::Z, 5.0f), true);

    GameComponent* firstPlanet = new PlanetComponent(this, camera, nullptr,
                                                     DirectX::SimpleMath::Vector3(10.0f, 10.0f, 10.0f), 5, 0, 0.75f);
    AddGameComponent(firstPlanet, false);
    AddGameComponent(new PlanetComponent(this, camera, firstPlanet,
                                         DirectX::SimpleMath::Vector3(10.0f, 0.0f, 0.0f), 2.5, 0.25f), false);

    AddGameComponent(new PlanetComponent(this, camera, firstPlanet,
                                         DirectX::SimpleMath::Vector3(-12.5f, 0.0f, 0.0f), 2), false);


    GameComponent* secondPlanet = new PlanetComponent(this, camera, nullptr,
                                                      DirectX::SimpleMath::Vector3(-25.0f, 0.0f, 0.0f), 10, 0, 1.5f);
    AddGameComponent(secondPlanet, false);
    AddGameComponent(new PlanetComponent(this, camera, secondPlanet,
                                         DirectX::SimpleMath::Vector3(15.0f, 0.0f, 0.0f), 2, 0.5f), false);

    GameComponent* thirdPlanet = new PlanetComponent(this, camera, nullptr,
                                                     DirectX::SimpleMath::Vector3(0.0f, 20.0f, 10.0f), 3, 0);
    AddGameComponent(thirdPlanet, false);
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

void Game::HandleSystemMessages(MSG& msg, bool& isExitRequested) const
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


void Game::AddGameComponent(GameComponent* inComponent, bool isDebug = false)
{
    components.push_back(inComponent);

    if (isDebug)
    {
        debugComponents.push_back(inComponent);
    }
}

void Game::Update() const
{
    ProcessUserInput();
    cameraController->Update();

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