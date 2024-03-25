#include "BallComponent.h"

#include "../../Game.h"
#include "../Camera/Camera.h"

void BallComponent::Update()
{
    DirectX::SimpleMath::Quaternion currentRotation = rotation;
    DirectX::SimpleMath::Vector3 forward = position - camera->GetPosition();
    forward.y = 0;
    forward.Normalize();
    const DirectX::SimpleMath::Vector3 left = DirectX::SimpleMath::Vector3::Transform(
        forward, DirectX::SimpleMath::Matrix::CreateRotationY(1.57f));

    DirectX::SimpleMath::Vector3 translation;

    if (game->input_device->IsKeyDown(Keys::T))
    {
        translation += DirectX::SimpleMath::Vector3(0,0,-1);// forward;
        //currentRotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0, 1, 0);
        currentRotation *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(-1,0,0)/*left*/, rotationSpeed * game->deltaTime);
    }
    if (game->input_device->IsKeyDown(Keys::G))
    {
        translation += DirectX::SimpleMath::Vector3(0,0,1); //-forward;
        currentRotation *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(1,0,0)/*-left*/, rotationSpeed * game->deltaTime);
    }
    if (game->input_device->IsKeyDown(Keys::F))
    {
        translation += DirectX::SimpleMath::Vector3(-1,0,0);//left;
        currentRotation *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0,0,1)/*-forward*/, rotationSpeed * game->deltaTime);
    }
    if (game->input_device->IsKeyDown(Keys::H))
    {
        translation += DirectX::SimpleMath::Vector3(1,0,0);//-left;
        currentRotation *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0,0,-1)/*forward*/, rotationSpeed * game->deltaTime);
    }


    translation.Normalize();
    currentRotation.Normalize();

    position += translation * movementSpeed * game->deltaTime;
    rotation = currentRotation;

    SetPosition(position);
    SetRotation(rotation);

    ModelComponent::Update();
}

void BallComponent::ResizeBoundingBox()
{
    boundingBoxSize *= ballResizeRate;
    SetScale(scale *= ballResizeRate);
}