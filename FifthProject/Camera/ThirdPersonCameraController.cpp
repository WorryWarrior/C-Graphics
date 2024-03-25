#include "ThirdPersonCameraController.h"

#include "../Game.h"
#include "../Components/GameComponent.h"
#include "../Camera//Camera.h"

ThirdPersonCameraController::ThirdPersonCameraController(Game* inGame, Camera* inCamera, GameComponent*& inParent, GameComponent* followTarget)
{
    game_ = inGame;
    camera = inCamera;
    yaw = 0;
    pitch = 0;
    parent = followTarget;
    game_->input_device->MouseMove.AddRaw(this, &ThirdPersonCameraController::OnMouseMove);
}

void ThirdPersonCameraController::Update()
{
    DirectX::SimpleMath::Vector3 offset(0.0f,  30.1f, 30.0f);
    if (parent != nullptr)
    {
        camera_position = DirectX::SimpleMath::Vector3::Lerp(camera_position, parent->position + offset, 0.9f);
        //camera_position = parent->position + offset;
    }
    else
        camera_position = offset;

    const DirectX::SimpleMath::Matrix rotation_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

    camera->view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(camera_position,
        camera_position + rotation_matrix.Forward(),
        rotation_matrix.Up());

    camera->UpdateProjectionMatrix();
}

void ThirdPersonCameraController::SetFollowTarget(GameComponent* inTarget)
{
    parent = inTarget;
}

void ThirdPersonCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
    yaw -= args.Offset.x * 0.003 * mouse_sensitivity;
    pitch -= args.Offset.y * 0.003 * mouse_sensitivity;
}