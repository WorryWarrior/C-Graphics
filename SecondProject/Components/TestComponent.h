#pragma once

#include <d3d11.h>

#include "GameComponent.h"

class TestComponent : public GameComponent
{
public:
    TestComponent(Game* inGame);
    ~TestComponent();
    void Initialize() override;
    void Draw() override;
    void Reload() override;
    void Update() override;
    void DestroyResources() override;

private:
    ID3D11InputLayout* layout;
    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexBC;
    ID3DBlob* errorVertexCode;
    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelBC;
    ID3DBlob* errorPixelCode;
    ID3D11RasterizerState* rastState;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    bool TryCompileAndCreateVertexShader();
    bool TryCompileAndCreatePixelShader();
    void CreateLayout();
    void CreateRasterizerState();
    void CreateBuffers();
};