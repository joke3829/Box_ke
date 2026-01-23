#include "Lighting.hlsl"
#define MAX_LIGHTS 100

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

cbuffer LightCBuffer : register(b3)
{
    Light cbLights[MAX_LIGHTS];
    uint numLights;
}

Texture2D g_MRT[5] : register(t0);



//SamplerState g_Sample : register(s0);

struct D_VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct D_VS_OUTPUT
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float4 wPos : POSITION;
};

D_VS_OUTPUT DeferredRenderOnePathVS(D_VS_INPUT input)
{
    D_VS_OUTPUT output;
    output.wPos = mul(float4(input.pos, 1.f), WorldMat);
    output.pos = mul(output.wPos, viewProj);
    output.normal = mul(input.normal, (float3x3) WorldMat);

    return output;
}

struct D_PS_OUTPUT
{
    float4 DiffuseColor : SV_Target0;
    float4 SpecularAndSh : SV_Target1;
    float4 EmissiveAndDepth : SV_Target2;
    float4 Normal : SV_Target3;
    float4 position : SV_Target4;
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
    output.position = input.wPos;
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
    //float3 debugDir = float3(1.0f, (1.0f - input.uv.y) * 2.0f - 1.0f, (1.0f - input.uv.x) * 2.0f - 1.0f);
    //float3 debugDir = float3(input.uv.x * 2.0f - 1.0f, -1.0f, input.uv.y * 2.0f - 1.0f);
    
    //float3 uvPlane = float3(input.uv.x * 2.0f - 1.0f, input.uv.y * 2.0f - 1.0f, 0.0f);
    //float3 middleDir = float3(0.0f, -1.0f, 1.0f);
    //float3 debugDir = normalize(uvPlane + middleDir);
    //float3 debugDir = float3(input.uv.x * 2.0f - 1.0f, (1.0f - input.uv.y) * 2.0f - 1.0f, -1.0f);
    //float4 debugColor = g_ShadowMapCube[1].Sample(g_Sample, normalize(debugDir)).rrrr;
    //return debugColor;
    
    float4 diffuseColor = g_MRT[0].Sample(g_Sample, input.uv);
    float4 specularAndSh = g_MRT[1].Sample(g_Sample, input.uv);
    float4 emissiveAndDepth = g_MRT[2].Sample(g_Sample, input.uv);
    float3 wNormal = normalize(g_MRT[3].Sample(g_Sample, input.uv).rgb);
    float3 wPos = g_MRT[4].Sample(g_Sample, input.uv).rgb;
    
    if (emissiveAndDepth.a == 1.f)
        return diffuseColor;

    PhongMaterial tempColor =
    {
        diffuseColor, float4(specularAndSh.rgb, 1.f), diffuseColor, 
        float4(emissiveAndDepth.rgb, 1.f), specularAndSh.a, float3(0.f, 0.f, 0.f)
    };

    float3 pixelColor = float3(0.f, 0.f, 0.f);
    for (uint i = 0; i < numLights; ++i)
    {
        pixelColor += BlinnPhongLightingResult(wPos, wNormal, cameraEye, tempColor, cbLights[i]);
    }
    pixelColor += ((0.2 * diffuseColor.rgb) + (25.f * emissiveAndDepth.rgb)); // ambient + emissive
    return float4(pixelColor, 1.f); 
}