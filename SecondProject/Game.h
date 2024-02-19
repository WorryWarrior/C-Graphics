#pragma once


#include "Components/GameComponent.h"
#include "Display/DisplayWin32.h"
#include "Input/InputDevice.h"

class Game
{
public:
    Game();
    ~Game();
    DisplayWin32* display;
    InputDevice* input_device;
    void Initialize();
    void Run();
    void AddGameComponent(GameComponent* inComponent);

private:
    std::vector<GameComponent*> components;
    void CreateBackBuffer();
    void PrepareFrame();
    void DestroyResources();
    void HandleSystemMessages(MSG& msg, bool& isExitRequested);
    void Update();
    void Draw();
    void Exit();
};