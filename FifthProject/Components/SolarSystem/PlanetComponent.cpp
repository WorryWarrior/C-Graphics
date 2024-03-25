#include "PlanetComponent.h"
#include "../../Game.h"
#include "../../Camera/Camera.h"

void PlanetComponent::Update()
{
    UpdateComponentPositionMatrix();
    UpdateComponentColour();

    DirectX::SimpleMath::Matrix mvp;

    if (parent != nullptr)
    {
        mvp = 1
            * DirectX::SimpleMath::Matrix::CreateTranslation(position)
            * DirectX::SimpleMath::Matrix::CreateRotationY(/*dynamic_cast<PlanetComponent*>(parent)->*/rotationSpeed * game->timeSinceStartup) // Rotate around moon's axis
            * DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XM_PI * tiltFactor)
            * DirectX::SimpleMath::Matrix::CreateTranslation(parent->position)
            * camera->view_matrix * camera->proj_matrix;
    }
    else
    {
        mvp = DirectX::SimpleMath::Matrix::CreateRotationY(rotationSpeed * game->timeSinceStartup)
            * DirectX::SimpleMath::Matrix::CreateTranslation(position)
            * camera->view_matrix * camera->proj_matrix;
    }

    mvpMatrix.MVP = mvp;
}