struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 worldPos : TEXCOORD1;
    float4 viewPos : TEXCOORD2;
};

cbuffer ConstantBuffer : register(b0)
{
    float4x4 World;
    float4x4 WVP;
    float4 ViewPosition;
};

cbuffer LightBuffer : register(b1)
{
    float4 direction;
    float4 color;
    float4 k;
}

cbuffer PointLightBuffer : register(b2)
{
    float4 pointPosition;
    float4 pointColor;
    float4 pointK;
}

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;

    output.normal = mul(float4(input.normal.xyz, 0), World);
    output.worldPos = mul(float4(input.pos.xyz, 1), World);
    output.pos = mul(float4(input.pos.xyz, 1), WVP);
    output.tex = input.tex.xy;
    output.viewPos = ViewPosition;

    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    const float4 texColor = Texture.Sample(Sampler, input.tex);

    const float3 normal = normalize(input.normal.xyz);

    const float3 viewDirection = normalize(input.viewPos.xyz - input.worldPos.xyz);
    const float3 lightDirection = -direction.xyz;
    const float3 pointLightDirection = normalize(pointPosition.xyz - input.worldPos.xyz);
    const float3 reflectionVector = normalize(reflect(lightDirection, normal));
    const float3 pointReflectionVector = normalize(reflect(pointLightDirection, normal));

    const float3 diffuse = max(0, dot(lightDirection, normal)) * texColor.xyz;
    const float3 pointDiffuse = max(0, dot(pointLightDirection, normal)) * texColor.xyz;
    const float3 ambient = texColor * k.x;
    const float3 specular = pow(max(0, dot(-viewDirection, reflectionVector)), k.y) * k.z;
    const float3 pointSpecular = pow(max(0, dot(-viewDirection, pointReflectionVector)), pointK.y) * pointK.z;

    //float4 col = float4(color.xyz * (diffuse + specular) + ambient, 1);

    /*float4 col = float4(pointColor * (pointDiffuse + pointSpecular) / pow(
                                distance(pointPosition.xyz, input.worldPos.xyz), 2), 1);*/

    float4 col = float4(color.xyz * (diffuse + specular)
                        + pointColor * (pointDiffuse + pointSpecular) / pow(
                            distance(pointPosition.xyz, input.worldPos.xyz), 2) + ambient, 1);

    col.rgb = pow(col.rgb, 1 / 2.2f);
    return col;
}