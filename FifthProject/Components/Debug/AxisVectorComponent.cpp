#include "AxisVectorComponent.h"
#include "../../Game.h"
#include "../../Camera/Camera.h"

void AxisVectorComponent::InitializeResources()
{
    CreateMatrix();
    CreateBuffers();
    CreateShaders();
    CreateLayout();
}

void AxisVectorComponent::Update()
{
    UpdateComponentPositionMatrix();
    UpdateComponentColour();

    const auto wvp = DirectX::SimpleMath::Matrix::CreateTranslation(position) * camera->view_matrix * camera->
        proj_matrix;
    mvpMatrix.MVP = wvp;
}

void AxisVectorComponent::DestroyResources()
{
    indexBuffer->Release();
    vertexBuffer->Release();
    constantBufferTransform->Release();
    constantBufferColour->Release();
    layout->Release();
    vertexShader->Release();
    vertexBC->Release();
    pixelShader->Release();
    pixelBC->Release();
}

void AxisVectorComponent::CreateBuffers()
{
    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(DirectX::SimpleMath::Vector4) * static_cast<UINT>(vertices.size());

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(int) * static_cast<UINT>(indices.size());

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

    D3D11_BUFFER_DESC constantBufPositionDesc = {};
    constantBufPositionDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufPositionDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufPositionDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufPositionDesc.MiscFlags = 0;
    constantBufPositionDesc.StructureByteStride = 0;
    constantBufPositionDesc.ByteWidth = sizeof(ConstantBufferTransformOld);

    D3D11_SUBRESOURCE_DATA constBufPositionData = {};
    constBufPositionData.pSysMem = &mvpMatrix;
    constBufPositionData.SysMemPitch = 0;
    constBufPositionData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&constantBufPositionDesc, &constBufPositionData, &constantBufferTransform);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferTransform);

    D3D11_BUFFER_DESC constantBufColourDesc = {};
    constantBufColourDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufColourDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufColourDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufColourDesc.MiscFlags = 0;
    constantBufColourDesc.StructureByteStride = 0;
    constantBufColourDesc.ByteWidth = sizeof(ConstantBufferColour);

    D3D11_SUBRESOURCE_DATA constBufColourData = {};
    constBufColourData.pSysMem = &mvpMatrix;
    constBufColourData.SysMemPitch = 0;
    constBufColourData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&constantBufColourDesc, &constBufColourData, &constantBufferColour);
    game->context->VSSetConstantBuffers(1, 1, &constantBufferColour);
}

void AxisVectorComponent::Draw()
{
    UINT strides[] = {16};
    UINT offsets[] = {0};

    game->context->IASetInputLayout(layout);
    game->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferTransform);
    game->context->VSSetConstantBuffers(1, 1, &constantBufferColour);
    game->context->VSSetShader(vertexShader, nullptr, 0);
    game->context->PSSetShader(pixelShader, nullptr, 0);

    if (!isActive)
        return;

    game->context->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}