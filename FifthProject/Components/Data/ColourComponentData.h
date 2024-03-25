#pragma once
#include "GameComponentData.h"
#include "../GameComponent.h"

class ColourComponentData : public GameComponentData
{
public:
    ColourComponentData()
    {
        shaderName = L"Shaders/VertexShader.hlsl";
        shaderInputElementCount = 2;

        shaderInputElements[0] =
            D3D11_INPUT_ELEMENT_DESC{
                "POSITION",
                0,
                DXGI_FORMAT_R32G32B32A32_FLOAT,
                0,
                0,
                D3D11_INPUT_PER_VERTEX_DATA,
                0
            };

        shaderInputElements[1] = D3D11_INPUT_ELEMENT_DESC{
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        };
    }

    ConstantBufferColour colour;
};