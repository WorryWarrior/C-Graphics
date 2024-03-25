#include "Game.h"

#include <chrono>
#include <iostream>
#include <random>

#include "Display/DisplayWin32.h"
#include "Components/GameComponent.h"
#include "Camera/Camera.h"
#include "Camera/FPSCameraController.h"
#include "Camera/ThirdPersonCameraController.h"
#include "Components/ModelComponent.h"
#include "Components/Debug/AxisVectorComponent.h"
#include "Components/Debug/GridComponent.h"
#include "Components/Katamari/BallComponent.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Lib/assimp-vc143-mt.lib")

Game::Game()
{
}

Game::~Game()
{
    DestroyResources();
}

BallComponent* ball;

void Game::Initialize()
{
    display = new DisplayWin32(1200, 1200, L"TestApplication");
    input_device = new InputDevice(this);

    InitializeResources();

    camera = new Camera(this);

    GameComponent* grid = new GridComponent(this, camera, 25);
    AddGameComponent(grid, true);
    AddGameComponent(new AxisVectorComponent(this, camera, AxisDirection::X, 5.0f), true);
    AddGameComponent(new AxisVectorComponent(this, camera, AxisDirection::Y, 5.0f), true);
    AddGameComponent(new AxisVectorComponent(this, camera, AxisDirection::Z, 5.0f), true);

    //cameraController = new FPSCameraController(this, camera, {0.0f, 25.0f, 0.0f});
    cameraController = new ThirdPersonCameraController(this, camera, grid, nullptr);

    GenerateCollisionObjects(10, 50);

    ball = new BallComponent(this, camera, cameraController, "ExternalDependencies\\Assets\\Ball1.fbx",
                                            "ExternalDependencies\\Assets\\Ball1-texture.jpg",
                                            DirectX::SimpleMath::Vector3(.35, .35, .35),
                                            20.0f, 4.0f, 3, 1.05f);
    AddGameComponent(ball, false);

    cameraController->SetFollowTarget(ball);

    //AddGameComponent(new ModelComponent(this, camera, cameraController, "ExternalDependencies\\Assets\\Ball1.fbx", "ExternalDependencies\\Assets\\Ball1-texture.jpg"), false);
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
    /*delete display;
    delete input_device;

    for (auto& component : components)
    {
        component->DestroyResources();
        delete component;
        component = nullptr;
    }*/
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

void Game::Update()
{
    ProcessUserInput();

    if (cameraController != nullptr)
        cameraController->Update();

    CheckBoxCollision();

    for (GameComponent* component : components)
    {
        component->Update();
    }
}

void Game::CheckBoxCollision()
{

    for (ModelComponent* model : collidableObjects)
    {
        if (model->parent == nullptr && CheckPlayerIntersection(model))
        {
            model->SetParent(ball);
            model->worldMatrix *= ball->worldMatrix.Invert();
            ball->ResizeBoundingBox();
        }
    }
}

bool Game::CheckPlayerIntersection(ModelComponent* model)
{
    float extent1 = model->boundingBoxSize;
    float extent2 = ball->boundingBoxSize;

    DirectX::SimpleMath::Vector3 min1 = model->position - DirectX::SimpleMath::Vector3(extent1, extent1, extent1);
    DirectX::SimpleMath::Vector3 max1 = model->position + DirectX::SimpleMath::Vector3(extent1, extent1, extent1);
    DirectX::SimpleMath::Vector3 min2 = ball->position - DirectX::SimpleMath::Vector3(extent2, extent2, extent2);
    DirectX::SimpleMath::Vector3 max2 = ball->position + DirectX::SimpleMath::Vector3(extent2, extent2, extent2);

    bool xOverlap = (max1.x >= min2.x) && (max2.x >= min1.x);
    bool yOverlap = (max1.y >= min2.y) && (max2.y >= min1.y);
    bool zOverlap = (max1.z >= min2.z) && (max2.z >= min1.z);

    return xOverlap && zOverlap;

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

void Game::GenerateCollisionObjects(int objectCount, int areaSize)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < objectCount; i++)
    {
        ModelComponent* collidableObject = new ModelComponent(this, camera, cameraController,
            "ExternalDependencies\\Assets\\Female.fbx",
            "ExternalDependencies\\Assets\\Female_texture.png", 1.0f);

        DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(
            std::uniform_int_distribution<int>(-areaSize, areaSize)(gen),
            5, std::uniform_int_distribution<int>(-areaSize, areaSize)(gen));

        collidableObject->SetPosition(pos);
        collidableObject->SetScale(DirectX::SimpleMath::Vector3(0.035f, 0.035f, 0.035f));
        AddGameComponent(collidableObject, false);
        collidableObjects.push_back(collidableObject);
    }
}

void Game::Exit()
{
    DestroyResources();
}