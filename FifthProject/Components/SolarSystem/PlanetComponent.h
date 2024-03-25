﻿#pragma once
#include "../BoxComponent.h"
#include "../SphereComponent.h"

class PlanetComponent : public SphereComponent
{
public:
    PlanetComponent(Game* inGame, Camera* inCamera, GameComponent* inParent = nullptr, DirectX::SimpleMath::Vector3 inColour = {1.0f, 1.0f, 1.0f},
                    DirectX::SimpleMath::Vector3 inPosition = {0.0f, 0.0f, 0.0f}, float scale = 1.0f,
                    float tiltFactor = 0, float rotationSpeed = 1.0f)
        : SphereComponent(inGame, inCamera, inParent, inPosition, scale)
    {
        this->tiltFactor = tiltFactor;
        this->rotationSpeed = rotationSpeed;

        ColourComponentData* foo = new ColourComponentData();
        foo->colour = ConstantBufferColour { { inColour.x, inColour.y, inColour.z, 1.0f } };

        this->componentData = foo;
    }

    void Update() override;

private:
    float rotationSpeed;
    float tiltFactor;
};