#pragma once

#include "Display/D3DApplication.h"
#include "Input/InputDevice.h"

class CameraControllerBase;
class Camera;

class Game : public D3DApplication
{
public:
    Game();
    ~Game();
    InputDevice* input_device;
    float timeSinceStartup;
    float deltaTime;
    void Initialize();
    void Run();
    void AddGameComponent(GameComponent* inComponent);
    void Exit();

    Camera* camera;
    CameraControllerBase* cameraController;
    std::vector<GameComponent*> debugComponents;

private:


    void HandleSystemMessages(MSG& msg, bool& isExitRequested);
    void Update();
    void DestroyResources();

    void ProcessUserInput() const;

};