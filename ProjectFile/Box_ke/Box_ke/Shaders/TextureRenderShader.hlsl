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


// Luminance PS
// 출처: https://jongzero.tistory.com/20

#define BLOOMCURVE_METHOD_1

static float gThreshold = 0.1;

float GetBloomCurve(float x)
{
    float result = x;
    x *= 2.0f;
    
#ifdef BLOOMCURVE_METHOD_1
    result = x * 0.05 + max(0, x - gThreshold) * 0.5;  // default gThreshold = 1.26
#endif

#ifdef BLOOMCURVE_METHOD_2
    result = x * x / 3.2;
#endif
    
#ifdef BLOOMCURVE_METHOD_3
    result = max(0, x - gThreshold);    // default gThreshold = 1.0
    result *= result;
#endif
    
    return result * 0.5f;
}

float4 LuminancePS(VS_OUTPUT input) : SV_Target
{
    float3 color = g_RenderTexture.Sample(g_Sampler, input.uv).rgb;
    /*float intensity = dot(color, float3(0.33f, 0.33f, 0.33f));
    float bloomIntensity = GetBloomCurve(intensity);
    float3 bloomColor = color * bloomIntensity / intensity;
    return float4(bloomColor.rgb, 1.f);*/
    
    float luminance = dot(color, float3(0.2126, 0.7152, 0.0722));
    if(luminance > 0.4)
        return float4(color, 1.f);
    else
        return float4(0.f, 0.f, 0.f, 1.f);
}