#pragma once
#include <d3d11.h>

class GameComponentData
{
public:
    LPCWSTR shaderName;
    int shaderInputElementCount;
    D3D11_INPUT_ELEMENT_DESC shaderInputElements[3];

};