#pragma once
#include "../BoxComponent.h"

class CubePlanetComponent : public BoxComponent
{
public:
    CubePlanetComponent(Game* inGame, Camera* inCamera, GameComponent* inParent = nullptr,
                        DirectX::SimpleMath::Vector3 inPosition = {0.0f, 0.0f, 0.0f}, float scale = 1.0f)
        : BoxComponent(inGame, inCamera, inParent, inPosition, scale)
    {
    }

    void Update() override;
};