#pragma once
#include "RectComponent.h"
#include "../SimpleMath.h"

class BallComponent : public RectComponent
{
public:
    DirectX::SimpleMath::Vector2 velocity;
    float velocityIncreaseRate = 1.05f;

    BallComponent(Game* game, float length = 0.05, float width = 0.05,
                    DirectX::SimpleMath::Vector2 pos = {0.0f, 0.0f}) : RectComponent(game, length, width, pos)
    {
        ResetVelocity();
        UpdateComponentColour();
    };

    void Move(float deltaTime);
    void ResetVelocity();

private:
    float initialVelocityY = 0.0f;
    float initialVelocityX = 1.5f;
};