#include "GameComponent.h"

#include <iostream>

#include "../Game.h"
#include "../Camera/Camera.h"
#include "../Display/DisplayWin32.h"
#include "Data/ColourComponentData.h"

void GameComponent::CreateShaders()
{
    CompileShaderFromFile(componentData->shaderName, "VSMain", "vs_5_0", &vertexBC);
    CompileShaderFromFile(componentData->shaderName, "PSMain", "ps_5_0", &pixelBC);

    game->device->CreateVertexShader(
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        nullptr, &vertexShader);

    game->device->CreatePixelShader(
        pixelBC->GetBufferPointer(),
        pixelBC->GetBufferSize(),
        nullptr, &pixelShader);
}

void GameComponent::CreateLayout()
{
    /*D3D11_INPUT_ELEMENT_DESC inputElements[] = {
        D3D11_INPUT_ELEMENT_DESC{
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        D3D11_INPUT_ELEMENT_DESC{
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };*/

    game->device->CreateInputLayout(
        componentData->shaderInputElements,
        componentData->shaderInputElementCount,
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        &layout);
}

void GameComponent::CreateBuffers()
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

void GameComponent::CreateMatrix()
{
    mvpMatrix.MVP = DirectX::XMMatrixIdentity();

    transformBufferData.World = DirectX::XMMatrixIdentity();
    transformBufferData.WVP = DirectX::XMMatrixIdentity();
    transformBufferData.ViewPosition = DirectX::SimpleMath::Vector4();
}

void GameComponent::UpdateTransformBuffer()
{
    worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale)
        * DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation)
        * DirectX::SimpleMath::Matrix::CreateTranslation(position)
        * (parent == nullptr ? DirectX::SimpleMath::Matrix::Identity : parent->worldMatrix);

    /*if (parent != nullptr)
        std::cout <<  parent->worldMatrix._11 << " " <<  parent->worldMatrix._12 << " " <<  parent->worldMatrix._13 << " " <<  parent->worldMatrix._14 << " " << std::endl;*/

    transformBufferData.World = worldMatrix;
    transformBufferData.WVP = worldMatrix * camera->view_matrix * camera->proj_matrix;
    transformBufferData.ViewPosition = DirectX::SimpleMath::Vector4(cameraController->camera_position.x,
                                                                cameraController->camera_position.y,
                                                                cameraController->camera_position.z, 0);


    if (constantBufferTransform != nullptr)
    {
        D3D11_MAPPED_SUBRESOURCE mss;
        game->context->Map(constantBufferTransform, 0, D3D11_MAP_WRITE_DISCARD, 0, &mss);
        memcpy(mss.pData, &transformBufferData, sizeof(ConstantBufferTransform));
        game->context->Unmap(constantBufferTransform, 0);
    }
}


void GameComponent::UpdateComponentPositionMatrix() const
{
    D3D11_MAPPED_SUBRESOURCE mss;
    game->context->Map(constantBufferTransform, 0, D3D11_MAP_WRITE_DISCARD, 0, &mss);
    memcpy(mss.pData, &mvpMatrix, sizeof(mvpMatrix));
    game->context->Unmap(constantBufferTransform, 0);
}

void GameComponent::UpdateComponentColour() const
{
    D3D11_MAPPED_SUBRESOURCE mss;
    game->context->Map(constantBufferColour, 0, D3D11_MAP_WRITE_DISCARD, 0, &mss);
    memcpy(mss.pData, &static_cast<ColourComponentData*>(componentData)->colour, sizeof(ConstantBufferColour));
    game->context->Unmap(constantBufferColour, 0);
}

HRESULT GameComponent::CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel,
                                             ID3DBlob** blobOut) const
{
    ID3DBlob* errorCode = nullptr;
    HRESULT res = D3DCompileFromFile(fileName,
                                     nullptr /*macros*/,
                                     nullptr /*include*/,
                                     entryPoint,
                                     shaderModel,
                                     D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                                     0,
                                     blobOut,
                                     &errorCode);

    if (FAILED(res))
    {
        // If the shader failed to compile it should have written something to the error message.
        if (errorCode)
        {
            char* compileErrors = (char*)(errorCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->display->hWnd, fileName, L"Missing Shader File", MB_OK);
        }
    }

    if (errorCode) errorCode->Release();

    return res;
}