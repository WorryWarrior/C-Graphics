#include "SpaceshipComponent.h"
#include "../../Game.h"
#include "../../Camera/Camera.h"

#define DEG2RAD (2*DirectX::XM_PI/360)

void SpaceshipComponent::InitializeResources()
{
    CreateMatrix();
    CreateBuffers();
    CreateShaders();
    CreateLayout();
}

void SpaceshipComponent::Draw()
{
    UINT strides[] = {32};
    UINT offsets[] = {0};

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

void SpaceshipComponent::Update()
{
    const auto inputDevice = game->input_device;

    DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3();

    auto velocityDirection = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

    if (inputDevice->IsKeyDown(Keys::W))
    {
       velocityDirection += DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
    }

    if (inputDevice->IsKeyDown(Keys::S))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
    }

    if (inputDevice->IsKeyDown(Keys::A))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
    }
    if (inputDevice->IsKeyDown(Keys::D))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
    }

    if (inputDevice->IsKeyDown(Keys::Space))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    }
    if (inputDevice->IsKeyDown(Keys::C))
    {
        velocityDirection += DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f);
    }
    velocityDirection.Normalize();


    position += velocityDirection * 10.0f * game->deltaTime;

    DirectX::SimpleMath::Matrix mvp = 1
        * DirectX::SimpleMath::Matrix::CreateTranslation(position)
        * DirectX::SimpleMath::Matrix::CreateRotationX(rotation.x)
        * DirectX::SimpleMath::Matrix::CreateRotationX(rotation.y)
        * DirectX::SimpleMath::Matrix::CreateRotationX(rotation.z)
        * camera->view_matrix * camera->proj_matrix;

    mvpMatrices.MVP = mvp;

    UpdateComponentPositionMatrix();
    UpdateComponentColour();
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