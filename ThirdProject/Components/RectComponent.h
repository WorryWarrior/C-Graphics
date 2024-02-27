#pragma once
#include "GameComponent.h"
#include "../SimpleMath.h"

class RectComponent : public GameComponent
{
public:
    DirectX::SimpleMath::Vector2 position;

    float height;
    float width;

    RectComponent(Game* game, float height = 0.4, float width = 0.1,
        DirectX::SimpleMath::Vector2 pos = { 0.0f, 0.0f})
    {
        this->game = game;
        this->height = height;
        this->width = width;
        this->position = pos;
        colour = ConstantBufferColour { { 1.0f, 1.0f, 1.0f, 1.0f } };

        float x0 = -height / 2;
        float x1 = height / 2;
        float y0 = -width / 2;
        float y1 = width / 2;

        DirectX::XMFLOAT4 vertex = { x0, y0, 0.0f, 1.0f };
        vertices.push_back(vertex);
        vertex = { x0, y1, 0.0f, 1.0f };
        vertices.push_back(vertex);
        vertex = { x1, y1, 0.0f, 1.0f };
        vertices.push_back(vertex);
        vertex = { x1, y0, 0.0f, 1.0f };
        vertices.push_back(vertex);

        const int indices_temp[] = { 0, 1, 2, 2, 3, 0 };

        for (int i = 0; i < 6; i++)
        {
            indices.push_back(indices_temp[i]);
        }

        InitializeResources();
    }

    void InitializeResources() override;
    void DestroyResources() override;
    void Draw() override;
};