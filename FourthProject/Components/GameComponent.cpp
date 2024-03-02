#include "GameComponent.h"
#include "../Game.h"
#include "../Display/DisplayWin32.h"

void GameComponent::CreateShaders()
{
    CompileShaderFromFile(L"Shaders/VertexShader.txt", "VSMain", "vs_5_0", &vertexBC);
    CompileShaderFromFile(L"Shaders/VertexShader.txt", "PSMain", "ps_5_0", &pixelBC);

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
    D3D11_INPUT_ELEMENT_DESC inputElements[] = {
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
    };

    game->device->CreateInputLayout(
        inputElements,
        2,
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
    constantBufPositionDesc.ByteWidth = sizeof(ConstantBufferPosition);

    D3D11_SUBRESOURCE_DATA constBufPositionData = {};
    constBufPositionData.pSysMem = &mvpMatrices;
    constBufPositionData.SysMemPitch = 0;
    constBufPositionData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&constantBufPositionDesc,&constBufPositionData, &constantBufferPosition);
    game->context->VSSetConstantBuffers(0, 1, &constantBufferPosition);

    D3D11_BUFFER_DESC constantBufColourDesc = {};
    constantBufColourDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufColourDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufColourDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufColourDesc.MiscFlags = 0;
    constantBufColourDesc.StructureByteStride = 0;
    constantBufColourDesc.ByteWidth = sizeof(ConstantBufferColour);

    D3D11_SUBRESOURCE_DATA constBufColourData = {};
    constBufColourData.pSysMem = &mvpMatrices;
    constBufColourData.SysMemPitch = 0;
    constBufColourData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&constantBufColourDesc,&constBufColourData, &constantBufferColour);
    game->context->VSSetConstantBuffers(1, 1, &constantBufferColour);
}

void GameComponent::CreateMatrix()
{
    mvpMatrices.MVP = DirectX::XMMatrixIdentity();
}

void GameComponent::UpdateComponentPositionMatrix() const
{
    D3D11_MAPPED_SUBRESOURCE mss;
    game->context->Map(constantBufferPosition, 0, D3D11_MAP_WRITE_DISCARD, 0, &mss);
    memcpy(mss.pData, &mvpMatrices, sizeof(mvpMatrices));
    game->context->Unmap(constantBufferPosition, 0);
}

void GameComponent::UpdateComponentColour() const
{
    D3D11_MAPPED_SUBRESOURCE mss;
    game->context->Map(constantBufferColour, 0, D3D11_MAP_WRITE_DISCARD, 0, &mss);
    memcpy(mss.pData, &colour, sizeof(colour));
    game->context->Unmap(constantBufferColour, 0);
}


HRESULT GameComponent::CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut) const
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
        if (errorCode) {
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