#pragma once
#include "GameComponent.h"

class Camera;

class BoxComponent : public GameComponent
{
public:
    BoxComponent(Game* inGame, Camera* inCamera, GameComponent* inParent = nullptr,
                 DirectX::SimpleMath::Vector3 inPosition = {0.0f, 0.0f,0.0f}, float sideSize = 5.0f)
    {
        this->game = inGame;
        this->camera = inCamera;
        colour = ConstantBufferColour { { 1.0f, 1.0f, 1.0f, 1.0f } };

        if (inParent != nullptr)
        {
            parent = inParent;
        }

        position = inPosition; // DirectX::SimpleMath::Vector3(inPosition.x, 0, inPosition.x);

        const int indices_temp[] = {
            0, 2, 1, 2, 3, 1,
            1, 3, 5, 3, 7, 5,
            2, 6, 3, 3, 6, 7,
            4, 5, 7, 4, 7, 6,
            0, 4, 2, 2, 4, 6,
            0, 1, 4, 1, 5, 4
        };

        for (int i = 0; i < 36; i++)
        {
            indices.push_back(indices_temp[i]);
        }

        const float halfSideSize = sideSize * 0.5f;

        const DirectX::SimpleMath::Vector4 vertices_temp[] = {
            DirectX::SimpleMath::Vector4(-halfSideSize, -halfSideSize, -halfSideSize, 1.0f),   DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f),
            DirectX::SimpleMath::Vector4(halfSideSize, -halfSideSize, -halfSideSize, 1.0f),    DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),

            DirectX::SimpleMath::Vector4(-halfSideSize, halfSideSize, -halfSideSize, 1.0f),    DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
            DirectX::SimpleMath::Vector4(halfSideSize, halfSideSize, -halfSideSize, 1.0f),     DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
            DirectX::SimpleMath::Vector4(-halfSideSize, -halfSideSize, halfSideSize, 1.0f),    DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
            DirectX::SimpleMath::Vector4(halfSideSize, -halfSideSize, halfSideSize, 1.0f),     DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),

            DirectX::SimpleMath::Vector4(-halfSideSize, halfSideSize, halfSideSize, 1.0f),     DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
            DirectX::SimpleMath::Vector4(halfSideSize, halfSideSize, halfSideSize, 1.0f),      DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f),
        };

        for (int i = 0; i < 16; i++)
        {
            vertices.push_back(vertices_temp[i]);
        }

        InitializeResources();
    }

    void InitializeResources() override;
    void DestroyResources() override;
    void Draw() override;
};