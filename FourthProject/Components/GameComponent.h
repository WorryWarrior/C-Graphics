#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <directxmath.h>
#include <iostream>

#include "../SimpleMath.h"

#pragma comment(lib, "d3dcompiler.lib")

class Camera;
class Game;

struct ConstantBufferPosition
{
    DirectX::SimpleMath::Matrix MVP;
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

    std::vector<DirectX::SimpleMath::Vector4> vertices;
    std::vector<int> indices;

    Camera* camera;
    GameComponent* parent;

    ConstantBufferPosition mvpMatrices;
    ConstantBufferColour colour;

    virtual void InitializeResources() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    //virtual void Reload() = 0;
    virtual void DestroyResources() = 0;
    void UpdateComponentPositionMatrix() const;
    void UpdateComponentColour() const;

protected:
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    ID3D11Buffer* constantBufferPosition;
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