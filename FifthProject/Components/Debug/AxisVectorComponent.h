#pragma once
#include "../GameComponent.h"
#include "../../Enums/AxisDirection.h"
#include "../Data/ColourComponentData.h"

enum class AxisDirection : int;

class AxisVectorComponent : public GameComponent
{
public:
    AxisVectorComponent(Game* inGame, Camera* inCamera, AxisDirection inDirection, float inLength = 1.0f)
    {
        game = inGame;
        camera = inCamera;
        position = DirectX::SimpleMath::Vector3(0.0f, 0.1f, 0.0f);
        DirectX::SimpleMath::Vector4 colourValue;

        vertices.push_back(DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
        indices.push_back(0);

        switch (inDirection)
        {
        case AxisDirection::X:
            vertices.push_back(DirectX::SimpleMath::Vector4(inLength, 0.0f, 0.0f, 1.0f));
            colourValue = DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case AxisDirection::Y:
            vertices.push_back(DirectX::SimpleMath::Vector4(0.0f, inLength, 0.0f, 1.0f));
            colourValue = DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case AxisDirection::Z:
            vertices.push_back(DirectX::SimpleMath::Vector4(0.0f, 0.0f, inLength, 1.0f));
            colourValue = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        default:
            break;
        }

        componentData = new ColourComponentData();
        static_cast<ColourComponentData*>(componentData)->colour = ConstantBufferColour{colourValue};

        indices.push_back(1);

        InitializeResources();
    }

    void InitializeResources() override;
    void DestroyResources() override;
    void CreateBuffers() override;
    void Update() override;
    void Draw() override;
};