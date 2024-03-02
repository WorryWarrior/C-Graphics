#include "FPSCameraController.h"

#include <iostream>
#include "Camera.h"
#include "../Game.h"

FPSCameraController::FPSCameraController(Game* inGame, Camera* inCamera,
    DirectX::SimpleMath::Vector3 startPos = {0.0f,5.0f,0.0f})
{
    game_ = inGame;
    camera = inCamera;
    yaw = 0;
    pitch = 0;

    game_->input_device->MouseMove.AddRaw(this, &FPSCameraController::OnMouseMove);

    camera_position = startPos;
}

void FPSCameraController::Update(float deltaTime)
{
    const auto inputDevice = game_->input_device;
    const auto rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

    auto velocityDirection = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    if (inputDevice->IsKeyDown(Keys::W))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
    }
    if (inputDevice->IsKeyDown(Keys::A))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
    }
    if (inputDevice->IsKeyDown(Keys::S))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
    }
    if (inputDevice->IsKeyDown(Keys::D))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
    }

    if (inputDevice->IsKeyDown(Keys::Space))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    }
    if (inputDevice->IsKeyDown(Keys::C))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f);
    }

    velocityDirection.Normalize();

    auto vel_dir = rotationMatrix.Forward() * velocityDirection.x
        + DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f) * velocityDirection.y
        + rotationMatrix.Right() * velocityDirection.z;

    if (vel_dir.Length() != 0)
    {
        vel_dir.Normalize();
    }

    camera_position = camera_position + vel_dir * velocity_magnitude * deltaTime;
    camera->view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(camera_position, camera_position + rotationMatrix.Forward(), rotationMatrix.Up());
    camera->UpdateProjectionMatrix();
}

void FPSCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
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