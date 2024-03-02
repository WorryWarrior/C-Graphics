#include "SpaceshipComponent.h"
#include "../../Game.h"
#include "../../Camera/Camera.h"

void SpaceshipComponent::InitializeResources()
{
    CreateMatrix();
    CreateBuffers();
    CreateShaders();
    CreateLayout();
}

void SpaceshipComponent::Draw()
{
    UINT strides[] = { 16 };
    UINT offsets[] = { 0 };

    game->context->IASetInputLayout(layout);
    game->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferPosition);
    game->context->VSSetConstantBuffers(1, 1, &constantBufferColour);
    game->context->VSSetShader(vertexShader, nullptr, 0);
    game->context->PSSetShader(pixelShader, nullptr, 0);

    game->context->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}

void SpaceshipComponent::Update()
{
    UpdateComponentPositionMatrix();
    UpdateComponentColour();

     DirectX::SimpleMath::Matrix mvp = 1
            * DirectX::SimpleMath::Matrix::CreateTranslation(position)
            //* DirectX::SimpleMath::Matrix::CreateRotationY(game->timeSinceStartup)
            * camera->view_matrix * camera->proj_matrix;

    mvpMatrices.MVP = mvp;
}


void SpaceshipComponent::DestroyResources()
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