#include "ThirdPersonCameraController.h"

#include "../Game.h"
#include "../Components/GameComponent.h"
#include "../Camera//Camera.h"

ThirdPersonCameraController::ThirdPersonCameraController(Game* inGame, Camera* inCamera, GameComponent*& inParent)
{
    game_ = inGame;
    camera = inCamera;
    yaw = 0;
    pitch = 0;
    parent = inParent;
    game_->input_device->MouseMove.AddRaw(this, &ThirdPersonCameraController::OnMouseMove);
}

void ThirdPersonCameraController::Update()
{
    DirectX::SimpleMath::Vector3 offset(-10.0f,  10.0f, 0.0f);
    camera_position = parent->position + offset;

    const DirectX::SimpleMath::Matrix rotation_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

    camera->view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(camera_position,
        camera_position + rotation_matrix.Forward(),
        rotation_matrix.Up());

    camera->UpdateProjectionMatrix();
}

void ThirdPersonCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
    yaw -= args.Offset.x * 0.003 * mouse_sensitivity;
    pitch -= args.Offset.y * 0.003 * mouse_sensitivity;
}