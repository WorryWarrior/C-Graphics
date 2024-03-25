#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>

#include "../SimpleMath.h"
#include "../Camera/CameraControllerBase.h"
#include "Data/GameComponentData.h"

#pragma comment(lib, "d3dcompiler.lib")

class Camera;
class Game;


struct ConstantBufferTransformOld
{
    DirectX::SimpleMath::Matrix MVP;
};

struct ConstantBufferTransform
{
    DirectX::SimpleMath::Matrix World;
    DirectX::SimpleMath::Matrix WVP;
    DirectX::SimpleMath::Vector4 ViewPosition;
};

struct ConstantBufferColour
{
    DirectX::SimpleMath::Vector4 Colour;
};


class GameComponent
{
public:
    bool isActive = true;
    Game* game;

    DirectX::SimpleMath::Vector3 position;
    DirectX::SimpleMath::Quaternion rotation;
    DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(1,1,1);
    DirectX::SimpleMath::Matrix worldMatrix;

    std::vector<DirectX::SimpleMath::Vector4> vertices;
    std::vector<int> indices;

    Camera* camera;
    CameraControllerBase* cameraController;
    GameComponent* parent;

    ConstantBufferTransformOld mvpMatrix;
    ConstantBufferTransform transformBufferData;
    GameComponentData* componentData;

    virtual void InitializeResources() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void DestroyResources() = 0;
    virtual void UpdateComponentPositionMatrix() const;
    void UpdateTransformBuffer();
    void UpdateComponentColour() const;

protected:
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    ID3D11Buffer* constantBufferTransform;
    ID3D11Buffer* constantBufferColour;

    ID3D11InputLayout* layout;

    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexBC;

    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelBC;

    void CreateShaders();
    void CreateLayout();
    virtual void CreateBuffers();
    void CreateMatrix();

    HRESULT CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut) const;
};