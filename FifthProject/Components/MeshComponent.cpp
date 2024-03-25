#include "MeshComponent.h"

#include <iostream>

#include "../Game.h"
#include <../Lib/directxtk_uwp.2024.1.1.1/include/WICTextureLoader.h>

#pragma comment(lib, "Lib/directxtk_uwp.2024.1.1.1/native/lib/x64/Debug/DirectXTK.lib")


struct aiFace;

void MeshComponent::InitializeResources()
{
    CreateMatrix();
    CreateBuffers();
    CreateShaders();
    CreateLayout();

    CD3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    game->device->CreateSamplerState(&samplerDesc, &samplerState);

    const std::wstring wide_string(texturePath.begin(), texturePath.end());
    DirectX::CreateWICTextureFromFile(game->device, wide_string.data(), nullptr, &texture);
}

void MeshComponent::DestroyResources()
{
    indexBuffer->Release();
    vertexBuffer->Release();
    constantBufferTransform->Release();
    if (constantBufferColour) constantBufferColour->Release();
    layout->Release();
    vertexShader->Release();
    vertexBC->Release();
    pixelShader->Release();
    pixelBC->Release();
}

void MeshComponent::CreateBuffers()
{
    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(MeshVertexData) * static_cast<UINT>(meshVertices.size());

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = meshVertices.data();
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

    D3D11_BUFFER_DESC transformBufferDesc = {};
    transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    transformBufferDesc.MiscFlags = 0;
    transformBufferDesc.StructureByteStride = 0;
    transformBufferDesc.ByteWidth = sizeof(ConstantBufferTransform);

    D3D11_SUBRESOURCE_DATA transformBufferData = {};
    transformBufferData.pSysMem = &transformBufferData;
    transformBufferData.SysMemPitch = 0;
    transformBufferData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&transformBufferDesc,&transformBufferData, &constantBufferTransform);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferTransform);
}

void MeshComponent::Update()
{
    UpdateTransformBuffer();
}

void MeshComponent::Draw()
{
    UINT strides[] = {32};
    UINT offsets[] = {0};

    game->context->IASetInputLayout(layout);
    //game->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    game->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);

    game->context->VSSetConstantBuffers(0, 1, &constantBufferTransform);

    game->context->PSSetSamplers(0, 1, &samplerState);
    game->context->PSSetShaderResources(0, 1, &texture);

    game->context->VSSetShader(vertexShader, nullptr, 0);
    game->context->PSSetShader(pixelShader, nullptr, 0);

    game->context->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}