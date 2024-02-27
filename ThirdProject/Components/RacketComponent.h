#pragma once
#include "RectComponent.h"

class RacketComponent : public RectComponent
{
public:
    float velocity = 1.5f;

    RacketComponent(Game* game, float height = 0.4, float width = 0.1,
                    DirectX::SimpleMath::Vector2 pos = {0.0f, 0.0f}) : RectComponent(game, height, width, pos)
    {
        mvpMatrices.MVP = DirectX::XMMatrixTranslation(pos.x, pos.y, 0);
        UpdateComponentPositionMatrix();
        UpdateComponentColour();
    }

    void Move(bool isUpDirection, float deltaTime);
    void MoveTo(DirectX::SimpleMath::Vector2 pos);
};