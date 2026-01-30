#include "Lighting.hlsl"
#define MAX_LIGHTS 100

cbuffer ObjectCBuffer : register(b0)
{
    float4x4 WorldMat;
}

cbuffer CameraCBuffer : register(b1)
{
    float4x4 viewProj;
}

cbuffer MaterialCBuffer : register(b2)
{
    PhongMaterial material;
}

// test
cbuffer LightInfo : register(b3)
{
    Light cbLight;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float4 wPos : POSITION;
};

VS_OUTPUT ShadowMapVS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.wPos = mul(float4(input.pos, 1.f), WorldMat);
    output.pos = mul(output.wPos, viewProj);
    output.normal = float3(0, 0, 0);
    return output;
}

float ShadowMapPS(VS_OUTPUT input) : SV_Target
{
    // pointlight일 경우 dis를 저장한다. 이외엔 깊이값 저장
    uint t = cbLight.type / 2;      // dir, spot = 0, point = 1
    float3 dis = distance(input.wPos.xyz, cbLight.position);
    return ((1 - t) * ((input.pos.z))) + (t * (dis / cbLight.range));
    //return input.pos.z / input.pos.w;
}