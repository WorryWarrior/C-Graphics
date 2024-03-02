#pragma once
#include "CameraControllerBase.h"
#include "../SimpleMath.h"
#include "../Input/InputDevice.h"

class Camera;
class Game;

class FPSCameraController : public CameraControllerBase
{
public:
    float yaw;
    float pitch;

    float velocity_magnitude = 100.0f;
    float mouse_sensitivity = 1.0f;

    FPSCameraController(Game* inGame, Camera* inCamera, DirectX::SimpleMath::Vector3 startPos);
    void Update(float deltaTime) override;

protected:
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args) override;
};