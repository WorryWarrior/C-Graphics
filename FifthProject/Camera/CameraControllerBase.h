#pragma once

#include "Camera.h"
#include "../SimpleMath.h"
#include "../Input/InputDevice.h"

class GameComponent;

class CameraControllerBase
{
public:
    Camera* camera;
    DirectX::SimpleMath::Vector3 camera_position;
    virtual void Update() = 0;
    virtual void SetFollowTarget(GameComponent* inTarget) = 0;

protected:
    Game* game_ = nullptr;
    virtual void OnMouseMove(const InputDevice::MouseMoveEventArgs& args) = 0;
};