#include "Lighting.hlsl"

cbuffer ObjectCBuffer : register(b0)
{
    float4x4 WorldMat;
}

cbuffer CameraCBuffer : register(b1)
{
    float4x4 viewProj;
    float4x4 InvViewProj;
    float3 cameraEye;
}

cbuffer MaterialCBuffer : register(b2)
{
    PhongMaterial material;
}

Texture2D g_MRT[4] : register(t0);

SamplerState g_Sample : register(s0);

struct D_VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct D_VS_OUTPUT
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
};

static const Light g_Light = { LIGHT_TYPE_DIRECTIONAL, float3(0.f, 0.f, 0.f), float4(1.f, 1.f, 1.f, 1.f), normalize(float3(-1.f, -1.f, 1.f)), 1.f, 1.f, 1.f, float2(0.f, 0.f) };

D_VS_OUTPUT DeferredRenderOnePathVS(D_VS_INPUT input)
{
    D_VS_OUTPUT output;
    output.pos = mul(mul(float4(input.pos, 1.f), WorldMat), viewProj);
    output.normal = mul(input.normal, (float3x3) WorldMat);

    return output;
}

struct D_PS_OUTPUT
{
    float4 DiffuseColor : SV_Target0;
    float4 SpecularAndSh : SV_Target1;
    float4 EmissiveAndDepth : SV_Target2;
    float4 Normal : SV_Target3;
};

D_PS_OUTPUT DeferredRenderOnePathPS(D_VS_OUTPUT input)
{
    D_PS_OUTPUT output;
    output.DiffuseColor = material.diffuseColor;
    
    output.SpecularAndSh.rgb = material.specularColor.rgb;
    output.SpecularAndSh.a = material.shininess;
    
    output.EmissiveAndDepth.rgb = material.emissiveColor.rgb;
    output.EmissiveAndDepth.a = input.pos.z / input.pos.w;
    
    output.Normal = float4(normalize(input.normal), 1.f);
    //output.Normal = float4(input.normal, 1.f);
    return output;
}


struct DT_PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

DT_PS_INPUT DeferredRenderTwoPathVS(uint nVertexID : SV_VertexID)
{
    DT_PS_INPUT output;
    
    if (nVertexID == 0)
    {
        output.pos = float4(-1.0, -1.0, 0.5, 1.0);
        output.uv = float2(0.f, 1.f);
    }
    else if (nVertexID == 1)
    {
        output.pos = float4(-1.0, 1.0, 0.5, 1.0);
        output.uv = float2(0.f, 0.f);
    }
    else if (nVertexID == 2)
    {
        output.pos = float4(1.0, 1.0, 0.5, 1.0);
        output.uv = float2(1.f, 0.f);
    }  
    else if (nVertexID == 3)
    {
        output.pos = float4(-1.0, -1.0, 0.5, 1.0);
        output.uv = float2(0.f, 1.f);
    }
    else if (nVertexID == 4)
    {
        output.pos = float4(1.0, 1.0, 0.5, 1.0);
        output.uv = float2(1.f, 0.f);
    }
    else if (nVertexID == 5)
    {
        output.pos = float4(1.0, -1.0, 0.5, 1.0);
        output.uv = float2(1.f, 1.f);
    }
    
    return output;
}

float4 DeferredRenderTwoPathPS(DT_PS_INPUT input) : SV_Target
{
    //return g_MRT[0].Sample(g_Sample, input.uv);
    //return float4(g_MRT[1].Sample(g_Sample, input.uv).rgb, 1.f);
    
    //float depth = g_MRT[2].Sample(g_Sample, input.uv).a;
    //return float4(depth, depth, depth, 1.f);
    
    //float3 normal = g_MRT[3].Sample(g_Sample, input.uv).rgb;
    //normal = (normal + 1.f) / 2.f;
    //return float4(normal, 1.f);
    
    float4 diffuseColor = g_MRT[0].Sample(g_Sample, input.uv);
    float4 specularAndSh = g_MRT[1].Sample(g_Sample, input.uv);
    float4 emissiveAndDepth = g_MRT[2].Sample(g_Sample, input.uv);
    float3 wNormal = normalize(g_MRT[3].Sample(g_Sample, input.uv).rgb);
    
    float2 viewportxy = float2(input.pos.x, input.pos.y);
    viewportxy.x = viewportxy.x / 1920 * 2.f - 1.f;
    viewportxy.y = viewportxy.y / 1080 * 2.f - 1.f;
    viewportxy.y *= -1;
    
    float4 projpos = float4(viewportxy, emissiveAndDepth.a, 1.f);
    float4 temppos = mul(projpos, InvViewProj);
    float3 wPos = temppos.xyz / temppos.w;
    
    
    PhongMaterial tempColor =
    {
        diffuseColor, float4(specularAndSh.rgb, 1.f), diffuseColor, 
        float4(emissiveAndDepth.rgb, 1.f), specularAndSh.a, float3(0.f, 0.f, 0.f)
    };
    
    return float4(BlinnPhongLightingResult(wPos, wNormal, cameraEye, tempColor, g_Light), 1.f);
}