#pragma once
#include "../GameComponent.h"

class SpaceshipComponent : public GameComponent
{
public:
    SpaceshipComponent(Game* inGame, Camera* inCamera, DirectX::SimpleMath::Vector3 inPosition = {0.0f, 0.0f,0.0f})
    {
        this->game = inGame;
        this->camera = inCamera;
        colour = ConstantBufferColour { { 1.0f, 1.0f, 1.0f, 1.0f } };

        position = inPosition;

        const int indices_temp[] = {
            // Base indices (triangular base)
            0, 1, 2,

            // Side indices
            3, 0, 1,
            3, 1, 2,
            3, 2, 0,
        };

        for (int i = 0; i < 3; i++)
        {
            indices.push_back(indices_temp[i]);
        }

        const float halfBaseSize = 5.0f;  // Half of the base size
        const float height = 5.0f;       // Height of the pyramid

        const DirectX::SimpleMath::Vector4 vertices_temp[] = {
            DirectX::SimpleMath::Vector4(-halfBaseSize, 0.0f, -halfBaseSize, 1.0f),  DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f),
            DirectX::SimpleMath::Vector4(halfBaseSize, 0.0f, -halfBaseSize, 1.0f),   DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
            DirectX::SimpleMath::Vector4(0.0f, 0.0f, halfBaseSize, 1.0f),            DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),

            // Apex vertex
            DirectX::SimpleMath::Vector4(0.0f, height, 0.0f, 1.0f),   DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
        };

        for (int i = 0; i < 8; i++)
        {
            vertices.push_back(vertices_temp[i]);
        }

        InitializeResources();
    }

    void InitializeResources() override;
    void DestroyResources() override;
    void Update() override;
    void Draw() override;
};