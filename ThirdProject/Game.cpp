#include "Game.h"

#include <chrono>
#include <iostream>

#include "Components/RacketComponent.h"
#include "Components/BallComponent.h"
#include "Display/DisplayWin32.h"

#pragma comment(lib, "d3d11.lib")

Game::Game()
{
}

Game::~Game()
{
    DestroyResources();
}

void Game::Initialize()
{
    display = new DisplayWin32(800, 800, L"TestApplication");
    input_device = new InputDevice(this);

    InitializeResources();

    firstRacket = new RacketComponent(this, 0.05f, 0.5f, {0.9f, 0.0f});
    AddGameComponent(firstRacket);
    secondRacket = new RacketComponent(this, 0.05f, 0.5f, {-0.9f, 0.0f});
    AddGameComponent(secondRacket);
    ball = new BallComponent(this, 0.025f, 0.025f, {0.0f, 0.0f});
    AddGameComponent(ball);
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

        auto curTime = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>
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
    ball->Move(deltaTime);

    PerformBallBoundCheck();

    if (CheckBallCollision(firstRacket))
    {
        PerformBallCollision(firstRacket);
    }

    if (CheckBallCollision(secondRacket))
    {
        PerformBallCollision(secondRacket);
    }
}

void Game::ProcessUserInput() const
{
    if (input_device->IsKeyDown(Keys::W))
    {
        secondRacket->Move(true, deltaTime);
    }

    if (input_device->IsKeyDown(Keys::S))
    {
        secondRacket->Move(false, deltaTime);
    }

    if (input_device->IsKeyDown(Keys::Up))
    {
        firstRacket->Move(true, deltaTime);
    }

    if (input_device->IsKeyDown(Keys::Down))
    {
        firstRacket->Move(false, deltaTime);
    }
}

void Game::PerformBallBoundCheck()
{
    if (ball->position.x + ball->width / 2 > 1.0f)
        firstPlayerScore++;
    else if (ball->position.x - ball->width / 2 < -1.0f)
        secondPlayerScore++;
    else
        return;

    NotifyScore();
    ResetScene();
}

bool Game::CheckBallCollision(const RacketComponent* other) const
{
    const float dX = ball->velocity.x > 0
                              ? other->position.x - other->height / 2 - (ball->position.x + ball->width / 2)
                              : ball->position.x - ball->width / 2 - (other->position.x + other->height / 2);

    const bool collideX = /*dX <= 0 &&*/ abs(ball->position.x - other->position.x) < (ball->width + other->height) / 2 &&
        other->position.x * ball->velocity.x > 0;

    const bool collideY = abs(ball->position.y - other->position.y) < other->width / 2;

    return collideX && collideY;
}

void Game::PerformBallCollision(const RacketComponent* other) const
{
    const float initialSpeed = ball->velocity.Length();
    ball->velocity.x = -ball->velocity.x;
    ball->velocity.y = sin((ball->position.y - other->position.y) / (other->width / 2));
    const float newSpeed = ball->velocity.Length();
    ball->velocity *= initialSpeed / newSpeed * ball->velocityIncreaseRate;
}

void Game::ResetScene() const
{
    firstRacket->MoveTo({0.9f, 0.0f});
    secondRacket->MoveTo({-0.9f, 0.0f});
    ball->position = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    ball->ResetVelocity();
}

void Game::NotifyScore() const
{
    std::cout << firstPlayerScore << " - " << secondPlayerScore << std::endl;
}


void Game::Exit()
{
    DestroyResources();
}