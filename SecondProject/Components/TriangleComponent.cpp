#include "TriangleComponent.h"

#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "../Game.h"

#pragma comment(lib, "d3dcompiler.lib")

bool useAltColour = false;

TriangleComponent::TriangleComponent(Game* inGame)
{
    game = inGame;
}

TriangleComponent::~TriangleComponent()
{
    DestroyResources();
}

void TriangleComponent::Initialize()
{
    std::cout << "Triangle Initialized" << std::endl;

    TryCompileAndCreateVertexShader();
    TryCompileAndCreatePixelShader();
    CreateLayout();
    CreateBuffers();
    CreateRasterizerState();
}

bool TriangleComponent::TryCompileAndCreateVertexShader()
{
    auto res = D3DCompileFromFile(L"./Shaders/TriangleShader.txt",
                                  nullptr,
                                  nullptr,
                                  "VSMain",
                                  "vs_5_0",
                                  D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                                  0,
                                  &vertexBC,
                                  &errorVertexCode);

    if (FAILED(res))
    {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode)
        {
            char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->display->hWnd, L"TriangleShader.txt", L"Missing Shader File", MB_OK);
        }

        return false;
    }

    res = game->display->device->CreateVertexShader(
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        nullptr, &vertexShader);

    return true;
}

bool TriangleComponent::TryCompileAndCreatePixelShader()
{
    D3D_SHADER_MACRO Shader_Macros[] = {"TEST", "1", "TCOLOR", useAltColour ? "float4(0.0f, 0.0f, 1.0f, 1.0f)" : "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr};

    auto res = D3DCompileFromFile(L"./Shaders/TriangleShader.txt",
                                  Shader_Macros,
                                  nullptr,
                                  "PSMain",
                                  "ps_5_0",
                                  D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                                  0,
                                  &pixelBC,
                                  &errorPixelCode);

    if (FAILED(res))
    {
        // If the shader failed to compile it should have written something to the error message.
        if (errorPixelCode)
        {
            char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->display->hWnd, L"TriangleShader.txt", L"Missing Shader File", MB_OK);
        }

        return false;
    }

    res = game->display->device->CreatePixelShader(
        pixelBC->GetBufferPointer(),
        pixelBC->GetBufferSize(),
        nullptr, &pixelShader);

    return true;
}

void TriangleComponent::CreateLayout()
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

    game->display->device->CreateInputLayout(
        inputElements,
        2,
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        &layout);
}

void TriangleComponent::CreateBuffers()
{
    DirectX::XMFLOAT4 points[8] = {
        DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
    };

    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = points;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    game->display->device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

    int indices[] = {0, 1, 2, 1, 0, 3};
    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(int) * std::size(indices);

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    game->display->device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);
}

void TriangleComponent::CreateRasterizerState()
{
    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    auto res = game->display->device->CreateRasterizerState(&rastDesc, &rastState);

    game->display->context->RSSetState(rastState);
}

void TriangleComponent::Draw()
{
    UINT strides[] = { sizeof(float) * 8 };
    UINT offsets[] = { 0 };

    game->display->context->RSSetState(rastState);
    game->display->context->IASetInputLayout(layout);
    game->display->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    game->display->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->display->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
    game->display->context->VSSetShader(vertexShader, nullptr, 0);
    game->display->context->PSSetShader(pixelShader, nullptr, 0);
}

void TriangleComponent::Reload()
{
    TryCompileAndCreatePixelShader();
}

void TriangleComponent::Update()
{
    if (game->input_device->IsKeyDown(Keys::B))
    {
        useAltColour = !useAltColour;
        Reload();
    }
}

void TriangleComponent::DestroyResources()
{
    vertexShader->Release();
    pixelShader->Release();
    layout->Release();
    vertexBC->Release();
    errorVertexCode->Release();
    pixelBC->Release();
    errorPixelCode->Release();
    rastState->Release();
    vertexBuffer->Release();
    indexBuffer->Release();
}