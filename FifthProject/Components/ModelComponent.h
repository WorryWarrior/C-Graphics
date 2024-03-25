#pragma once
#include <iostream>

#include "GameComponent.h"
#include "MeshComponent.h"
#include "../ExternalDependencies/assimp/Importer.hpp"
#include "../ExternalDependencies/assimp/postprocess.h"
#include "../ExternalDependencies/assimp/scene.h"
#include "Data/TextureComponentData.h"

class ModelComponent : public GameComponent
{
public:
    ModelComponent(Game* inGame, Camera* inCamera, CameraControllerBase* inCameraController, const std::string& modelPath, const std::string& texturePath,
                   float inBounsingBoxSize = 0.5f, DirectX::SimpleMath::Vector3 inPosition = {0.0f, 0.0f, 0.0f})
    {
        this->game = inGame;
        this->camera = inCamera;
        this->texturePath = texturePath;
        this->cameraController = inCameraController;
        this->position = inPosition;
        this->boundingBoxSize = inBounsingBoxSize;

        //componentData = new TextureComponentData();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
        ProcessNode(scene->mRootNode, scene, texturePath);

        InitializeResources();
    }
    float boundingBoxSize;

    void SetPosition(DirectX::SimpleMath::Vector3 inPosition);
    void SetScale(DirectX::SimpleMath::Vector3 inScale);
    void SetRotation(DirectX::SimpleMath::Quaternion inRotation);
    void SetParent(GameComponent* inParent);

    void InitializeResources() override;
    void DestroyResources() override;
    void CreateBuffers() override;
    void Update() override;
    void Draw() override;

protected:
    std::vector<MeshComponent*> meshes;
    std::string texturePath;
    //ID3D11SamplerState* samplerState;
    //ID3D11ShaderResourceView* texture;

private:

    void ProcessNode(const aiNode* node, const aiScene* scene, const std::string& texturePath);
};