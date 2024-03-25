#pragma once
#include "CameraControllerBase.h"
#include "../SimpleMath.h"
#include "../Components/GameComponent.h"
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

    ThirdPersonCameraController(Game* inGame, Camera* inCamera, GameComponent*& inParent, GameComponent* followTarget);
    void Update() override;
    void SetFollowTarget(GameComponent* inTarget);

protected:
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args) override;
};