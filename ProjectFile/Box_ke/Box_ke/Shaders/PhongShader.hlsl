#include "Lighting.hlsl"

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 wPosition : POSITION;
    float3 normal : NORMAL;
};

cbuffer ObjectCBuffer : register(b0)
{
    float4x4 WorldMat;
};

cbuffer CameraCBuffer : register(b1)
{
    float4x4 ViewProj;
    float4x4 InvViewProj;
    float3 eye;
};

cbuffer PhongMaterialCBuffer : register(b2)
{
    PhongMaterial material;
}

static const Light g_Light = { LIGHT_TYPE_DIRECTIONAL, float3(0.f, 0.f, 0.f), float4(1.f, 1.f, 1.f, 1.f), normalize(float3(-1.f, -1.f, 1.f)), 1.f, 1.f, 1.f, 1.f, 1.f,
    {
        float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    }
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.wPosition = mul(float4(input.position, 1.f), WorldMat).xyz;
    output.position = mul(float4(output.wPosition, 1.f), ViewProj);
    output.normal = mul(input.normal, (float3x3)WorldMat);

    return output;
}

float4 psmain(VS_OUTPUT input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    return float4(BlinnPhongLightingResult(input.wPosition, normal, eye, material, g_Light), 1.f);
}