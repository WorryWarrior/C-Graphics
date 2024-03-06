#include "Camera.h"
#include "../Game.h"
#include "../Display/DisplayWin32.h"

Camera::Camera(Game* in_game)
{
    game_ = in_game;
    view_matrix = DirectX::XMMatrixIdentity(); //DirectX::SimpleMath::Matrix::Identity;
    UpdateProjectionMatrix();
}

Camera::~Camera()
{

}


DirectX::SimpleMath::Matrix Camera::GetCameraMatrix() const
{
    return view_matrix * proj_matrix;
}

DirectX::SimpleMath::Vector3 Camera::GetPosition() const
{
    DirectX::SimpleMath::Matrix inverted;
    view_matrix.Invert(inverted);
    return inverted.Translation();

}

DirectX::SimpleMath::Vector3 Camera::GetForwardVector() const
{
    DirectX::SimpleMath::Matrix inverted;
    view_matrix.Invert(inverted);
    return inverted.Up();
}

void Camera::UpdateProjectionMatrix()
{

    proj_matrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        static_cast<float>(DirectX::XM_PI) * 0.35f,
        static_cast<float>(game_->display->windowWidth) / static_cast<float>(game_->display->windowHeight),
        0.1f,
        10000.0f);
}