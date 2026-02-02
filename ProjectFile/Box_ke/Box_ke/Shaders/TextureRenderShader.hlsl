struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_OUTPUT TextureRenderVS(uint nVertexID : SV_VertexID)
{
    VS_OUTPUT output;
    
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

Texture2D g_RenderTexture : register(t0);
SamplerState g_Sampler : register(s0);

float4 TextureRenderPS(VS_OUTPUT input) : SV_Target
{
    return float4(g_RenderTexture.Sample(g_Sampler, input.uv).rgb, 1.f);
}

float4 LuminancePS(VS_OUTPUT input) : SV_Target
{
    float3 color = g_RenderTexture.Sample(g_Sampler, input.uv).rgb;
    
    float luminance = dot(color, float3(0.2126, 0.7152, 0.0722));
    if(luminance > 5.0)
        return float4(color, 1.f);
    else
        return float4(0.f, 0.f, 0.f, 1.f);
}

Texture2D g_AdditionalTexture : register(t1);

float4 BleedingPS(VS_OUTPUT input) : SV_Target
{
    float3 color1 = g_RenderTexture.Sample(g_Sampler, input.uv).rgb;
    float3 color2 = g_AdditionalTexture.Sample(g_Sampler, input.uv).rgb;
    
    return float4(color1 + color2, 1.f);
}

float4 TextureToneMapping(VS_OUTPUT input) : SV_Target
{
    float3 color = g_RenderTexture.Sample(g_Sampler, input.uv).rgb;
    color = max(0.f, color);
    color = color / (color + 1.0f); // Reinhard Curve
    
    color = pow(color, 1.f / 2.2f);

    return float4(color, 1.f);
}