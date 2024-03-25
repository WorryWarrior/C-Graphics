﻿#pragma once

#include "Components/ModelComponent.h"
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
    void AddGameComponent(GameComponent* inComponent, bool isDebug);
    void Exit();

    Camera* camera;
    CameraControllerBase* cameraController;
    std::vector<GameComponent*> debugComponents;

    std::vector<ModelComponent*> collidableObjects;

private:
    void HandleSystemMessages(MSG& msg, bool& isExitRequested) const;
    void Update();
    void DestroyResources();
    void ProcessUserInput() const;

    void GenerateCollisionObjects(int objectCount, int areaSize);
    void CheckBoxCollision();
    bool CheckPlayerIntersection(ModelComponent* model);
};