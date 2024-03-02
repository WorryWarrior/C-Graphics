#pragma once
#include "CameraControllerBase.h"
#include "../SimpleMath.h"
#include "../Input/InputDevice.h"

class GameComponent;
class Camera;
class Game;

class ThirdPersonCameraController : public CameraControllerBase
{
    Game* game_;

public:
    float yaw;
    float pitch;

    float velocity_magnitude = 100.0f;
    float mouse_sensitivity = 1.0f;

    Camera* camera;
    GameComponent* parent;
    DirectX::SimpleMath::Vector3 camera_position;

    ThirdPersonCameraController(Game* in_game, Camera* in_camera, GameComponent*& in_parent);
    void Update(float delta_time) override;

protected:
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args) override;
};