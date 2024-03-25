#include "BoxComponent.h"
#include "../Game.h"

void BoxComponent::InitializeResources()
{
    CreateMatrix();
    CreateBuffers();
    CreateShaders();
    CreateLayout();
}

void BoxComponent::Draw()
{
    UINT strides[] = { 32 };
    UINT offsets[] = { 0 };

    game->context->IASetInputLayout(layout);
    game->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferTransform);
    game->context->VSSetConstantBuffers(1, 1, &constantBufferColour);
    game->context->VSSetShader(vertexShader, nullptr, 0);
    game->context->PSSetShader(pixelShader, nullptr, 0);

    game->context->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}


void BoxComponent::DestroyResources()
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