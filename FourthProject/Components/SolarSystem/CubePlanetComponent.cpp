#include "CubePlanetComponent.h"
#include "../../Game.h"
#include "../../Camera/Camera.h"

#define M_PI 3.14159265358979323846

void CubePlanetComponent::Update()
{
    UpdateComponentPositionMatrix();
    UpdateComponentColour();

    DirectX::SimpleMath::Matrix mvp;
    DirectX::SimpleMath::Matrix tr;

    if (parent != nullptr)
    {
        tr = DirectX::SimpleMath::Matrix::CreateTranslation(parent->position)
                * DirectX::SimpleMath::Matrix::CreateRotationY(game->deltaTime)
                * DirectX::SimpleMath::Matrix::CreateTranslation(position)
                * DirectX::SimpleMath::Matrix::CreateRotationY(game->deltaTime);


        mvp =     DirectX::SimpleMath::Matrix::CreateTranslation(parent->position)
                * DirectX::SimpleMath::Matrix::CreateRotationY(game->timeSinceStartup) // Rotate around moon's axis
                * DirectX::SimpleMath::Matrix::CreateTranslation(position)
                * DirectX::SimpleMath::Matrix::CreateRotationY(game->timeSinceStartup) // Rotate around parent's axis
                //* DirectX::SimpleMath::Matrix::CreateRotationX(M_PI /4)
                * camera->view_matrix * camera->proj_matrix;
    }
    else
    {
        tr = DirectX::SimpleMath::Matrix::CreateRotationY(game->timeSinceStartup) *
            DirectX::SimpleMath::Matrix::CreateTranslation(position);

        mvp =   DirectX::SimpleMath::Matrix::CreateRotationY(game->timeSinceStartup)
              * DirectX::SimpleMath::Matrix::CreateTranslation(position)
              * camera->view_matrix * camera->proj_matrix;
    }
    
    mvpMatrices.MVP = mvp;
}