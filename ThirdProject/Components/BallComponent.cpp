#include "BallComponent.h"

void BallComponent::Move(float deltaTime)
{
    position += velocity * deltaTime;

    mvpMatrices.MVP = DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);
    UpdateComponentPositionMatrix();

    float upBoundary = 1.0f - height / 2;
    float downBoundary = -1.0f + height / 2;
    if (position.y >= upBoundary || position.y <= downBoundary)
    {
        velocity.y = -velocity.y;
    }
}

void BallComponent::ResetVelocity()
{
    velocity.x = initialVelocityX;
    velocity.y = initialVelocityY;
}