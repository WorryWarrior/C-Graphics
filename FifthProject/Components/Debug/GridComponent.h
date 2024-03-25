#pragma once
#include "../GameComponent.h"
#include "../Components/Data/ColourComponentData.h"

class GridComponent : public GameComponent
{
public:
    GridComponent(Game* inGame, Camera* inCamera, int size)
    {
        game = inGame;
        camera = inCamera;
        position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

        componentData = new ColourComponentData();
        static_cast<ColourComponentData*>(componentData)->colour = ConstantBufferColour{{ 0.5f, 0.5f, 0.5f, 0.5f }};

        const float gridSize = 10.0f;

        for (int i = 0; i < size; ++i)
        {
            float x = (i - (size - 1) / 2.0f) * gridSize;
            for (int j = 0; j < size; ++j)
            {
                float z = (j - (size - 1) / 2.0f) * gridSize;
                vertices.push_back(DirectX::SimpleMath::Vector4(x, 0.0f, z, 1.0f));
            }
        }

        for (int i = 0; i < size - 1; ++i)
        {
            for (int j = 0; j < size - 1; ++j)
            {
                int vertexIndex = i * size + j;

                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex + size);
                indices.push_back(vertexIndex + 1);

                indices.push_back(vertexIndex + 1);
                indices.push_back(vertexIndex + size);
                indices.push_back(vertexIndex + size + 1);
            }
        }

        InitializeResources();
    }

    ~GridComponent();

    void InitializeResources() override;
    void DestroyResources() override;
    void CreateBuffers() override;
    void Update() override;
    void Draw() override;
};