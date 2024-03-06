#pragma once
#include "../BoxComponent.h"
#include "../GameComponent.h"

class SpaceshipComponent : public BoxComponent
{
public:
    SpaceshipComponent(Game* inGame, Camera* inCamera, DirectX::SimpleMath::Vector3 inPosition = {0.0f, 0.0f,0.0f})
        : BoxComponent(inGame, inCamera, nullptr, inPosition, 3.0f)
    {

    }

    void InitializeResources() override;
    void DestroyResources() override;
    void Update() override;
    void Draw() override;
};