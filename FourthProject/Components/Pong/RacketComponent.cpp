#include "RacketComponent.h"

void RacketComponent::Move(bool isUpDirection, float deltaTime)
{
    float upBoundary = 1.0f - width / 2.0f;
    float downBoundary = -1.0f + width / 2.0f;

    if (isUpDirection && position.y <= upBoundary)
    {
        position.y += velocity * deltaTime;
    }
    else if (!isUpDirection && position.y >= downBoundary)
    {
        position.y -= velocity * deltaTime;
    }
    mvpMatrices.MVP = DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);
    UpdateComponentPositionMatrix();
}

void RacketComponent::MoveTo(DirectX::SimpleMath::Vector2 pos)
{
    position = pos;
    mvpMatrices.MVP = DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);
    UpdateComponentPositionMatrix();
}