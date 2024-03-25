#pragma once
#include <vector>

#include "GameComponent.h"
#include "../SimpleMath.h"
#include "assimp/mesh.h"
#include "Data/TextureComponentData.h"

struct MeshVertexData
{
    DirectX::SimpleMath::Vector3 position;
    DirectX::SimpleMath::Vector3 normal;
    DirectX::SimpleMath::Vector2 uv;
};

class MeshComponent : public GameComponent
{

public:
    std::string texturePath;

    MeshComponent(const aiMesh* mesh, Game* inGame, Camera* inCamera, CameraControllerBase* inCameraController)
    {
        game = inGame;
        camera = inCamera;
        cameraController = inCameraController;

        componentData = new TextureComponentData();

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            MeshVertexData vertexData;

            vertexData.position = DirectX::SimpleMath::Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            if (mesh->mTextureCoords[0])
                vertexData.uv = DirectX::SimpleMath::Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            else
                vertexData.uv = DirectX::SimpleMath::Vector2();

            if (mesh->mNormals != nullptr)
                vertexData.normal = DirectX::SimpleMath::Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            else
                vertexData.normal = DirectX::SimpleMath::Vector3(1, 0, 0);

            meshVertices.push_back(vertexData);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
    }

    void InitializeResources() override;
    void DestroyResources() override;
    void CreateBuffers() override;
    void Update() override;
    void Draw() override;

protected:
    ID3D11SamplerState* samplerState;
    ID3D11ShaderResourceView* texture;

private:
    std::vector<MeshVertexData> meshVertices;
};