#include "ThirdPersonCameraController.h"

#include "../Game.h"
#include "../Components/GameComponent.h"
#include "../Camera//Camera.h"

ThirdPersonCameraController::ThirdPersonCameraController(Game* in_game, Camera* in_camera, GameComponent*& in_parent)
{
    game_ = in_game;
    camera = in_camera;
    yaw = 0;
    pitch = 0;
    parent = in_parent;
    game_->input_device->MouseMove.AddRaw(this, &ThirdPersonCameraController::OnMouseMove);
}

void ThirdPersonCameraController::Update(float delta_time)
{
    //DirectX::SimpleMath::Vector3 offset(0.0f,  0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 offset(-10.0f,  10.0f, 0.0f);

    camera_position = parent->position + offset;

    std::cout << parent->position.x << ", " << parent->position.y << ", " << parent->position.z << std::endl;

    const DirectX::SimpleMath::Matrix rotation_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

    camera->view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(camera_position,
        camera_position + rotation_matrix.Forward(),
        rotation_matrix.Up());

    camera->UpdateProjectionMatrix();
}

void ThirdPersonCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
    if (game_->input_device->IsKeyDown(Keys::LeftShift))
    {
        return;
    }

    yaw -= args.Offset.x * 0.003 * mouse_sensitivity;
    pitch -= args.Offset.y * 0.003 * mouse_sensitivity;


    if (args.WheelDelta > 0)
    {
        velocity_magnitude += 10;
    }
    else if (args.WheelDelta < 0)
    {
        velocity_magnitude -= 10;
    }
}