#include "ModelComponent.h"
#include "../Game.h"

void ModelComponent::SetPosition(DirectX::SimpleMath::Vector3 inPosition)
{
    position = inPosition;
    for (MeshComponent* mesh : meshes)
    {
        mesh->position = inPosition;
    }
}

void ModelComponent::SetScale(DirectX::SimpleMath::Vector3 inScale)
{
    scale = inScale;
    for (MeshComponent* mesh : meshes)
    {
        mesh->scale = inScale;
    }
}

void ModelComponent::SetRotation(DirectX::SimpleMath::Quaternion inRotation)
{
    rotation = inRotation;
    for (MeshComponent* mesh : meshes)
    {
        mesh->rotation = inRotation;
    }
}

void ModelComponent::SetParent(GameComponent* inParent)
{
    parent = inParent;
    for (MeshComponent* mesh : meshes)
    {
        mesh->parent = inParent;
        mesh->worldMatrix *= inParent->worldMatrix.Invert();
        mesh->worldMatrix.Decompose(mesh->scale, mesh->rotation, mesh->position);
    }
}

void ModelComponent::InitializeResources()
{
    for (MeshComponent* mesh : meshes)
    {
        mesh->InitializeResources();
    }

    /*CreateMatrix();
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
    DirectX::CreateWICTextureFromFile(game->device, wide_string.data(), nullptr, &texture);*/
}

void ModelComponent::DestroyResources()
{
    for (MeshComponent* mesh : meshes)
    {
        mesh->DestroyResources();
    }

    /*indexBuffer->Release();
    vertexBuffer->Release();
    constantBufferPosition->Release();
    constantBufferColour->Release();
    layout->Release();
    vertexShader->Release();
    vertexBC->Release();
    pixelShader->Release();
    pixelBC->Release();*/
}

void ModelComponent::CreateBuffers()
{
    for (MeshComponent* mesh : meshes)
    {
        mesh->CreateBuffers();
    }

    /*D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(MeshVertexData) * static_cast<UINT>(vertices.size());

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
    constantBufPositionDesc.ByteWidth = sizeof(ConstantBufferPosition);

    D3D11_SUBRESOURCE_DATA constBufPositionData = {};
    constBufPositionData.pSysMem = &mvpMatrix;
    constBufPositionData.SysMemPitch = 0;
    constBufPositionData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&constantBufPositionDesc,&constBufPositionData, &constantBufferPosition);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferPosition);*/
}

void ModelComponent::Update()
{
    worldMatrix = meshes[0]->worldMatrix;

    for (MeshComponent* mesh : meshes)
    {
        mesh->Update();
    }

    /*UpdateComponentPositionMatrix();

    mvpMatrix.MVP = DirectX::SimpleMath::Matrix::CreateTranslation(position)
        * camera->view_matrix * camera->proj_matrix;*/

}

void ModelComponent::Draw()
{
    for (MeshComponent* mesh : meshes)
    {
        mesh->Draw();
    }

    /*UINT strides[] = {32};
    UINT offsets[] = {0};

    game->context->IASetInputLayout(layout);
    game->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferPosition);

    game->context->PSSetShaderResources(0, 1, &texture);
    game->context->PSSetSamplers(0, 1, &samplerState);

    game->context->VSSetShader(vertexShader, nullptr, 0);
    game->context->PSSetShader(pixelShader, nullptr, 0);

    game->context->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);*/
}

void ModelComponent::ProcessNode(const aiNode* node, const aiScene* scene, const std::string& texturePath)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        MeshComponent* mesh = new MeshComponent(scene->mMeshes[node->mMeshes[i]], game, camera, cameraController);
        mesh->position = position;
        mesh->rotation = rotation;
        mesh->scale = scale;
        mesh->texturePath = texturePath;
        meshes.push_back(mesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        ProcessNode(node->mChildren[i], scene, texturePath);
}