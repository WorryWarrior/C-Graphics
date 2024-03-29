﻿#include "SphereComponent.h"
#include "../Game.h"
#include "../Camera/Camera.h"

void SphereComponent::InitializeResources()
{
    CreateMatrix();
    CreateBuffers();
    CreateShaders();
    CreateLayout();
}

void SphereComponent::Draw()
{
    UINT strides[] = { 32 };
    UINT offsets[] = { 0 };

    game->context->IASetInputLayout(layout);
    game->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferPosition);
    game->context->VSSetConstantBuffers(1, 1, &constantBufferColour);
    game->context->VSSetShader(vertexShader, nullptr, 0);
    game->context->PSSetShader(pixelShader, nullptr, 0);

    game->context->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}

void SphereComponent::DestroyResources()
{
    indexBuffer->Release();
    vertexBuffer->Release();
    constantBufferPosition->Release();
    constantBufferColour->Release();
    layout->Release();
    vertexShader->Release();
    vertexBC->Release();
    pixelShader->Release();
    pixelBC->Release();
}

void SphereComponent::Update()
{
    /*UpdateComponentPositionMatrix();
    UpdateComponentColour();

    DirectX::SimpleMath::Matrix mvp =
        DirectX::SimpleMath::Matrix::CreateTranslation(position)
        * camera->view_matrix * camera->proj_matrix;

    mvpMatrices.MVP = mvp;*/
}