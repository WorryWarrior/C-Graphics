#pragma once

#include "Camera.h"
#include "../SimpleMath.h"
#include "../Input/InputDevice.h"

class CameraControllerBase
{
public:
    Camera* camera;
    DirectX::SimpleMath::Vector3 camera_position;
    virtual void Update(float deltaTime) = 0;

protected:
    Game* game_ = nullptr;
    virtual void OnMouseMove(const InputDevice::MouseMoveEventArgs& args) = 0;
};