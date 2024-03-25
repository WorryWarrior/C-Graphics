#pragma once
#include "GameComponentData.h"

class TextureComponentData : public GameComponentData
{
public:
    TextureComponentData()
    {
        shaderName = L"Shaders/ModelShader.hlsl";
        shaderInputElementCount = 3;

        shaderInputElements[0] =
            D3D11_INPUT_ELEMENT_DESC{
                "POSITION",
                0,
                DXGI_FORMAT_R32G32B32_FLOAT,
                0,
                0,
                D3D11_INPUT_PER_VERTEX_DATA,
                0
            };

        shaderInputElements[1] =
            D3D11_INPUT_ELEMENT_DESC{
                "NORMAL",
                0,
                DXGI_FORMAT_R32G32B32_FLOAT,
                0,
                D3D11_APPEND_ALIGNED_ELEMENT,
                D3D11_INPUT_PER_VERTEX_DATA,
                0
            };

        shaderInputElements[2] = D3D11_INPUT_ELEMENT_DESC{
            "TEXCOORD",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        };
    }
};