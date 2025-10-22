
Texture2D CSInput : register(t0);
RWTexture2D<float4> CSResult : register(u0);

float GreyScaleResult(in float3 color)
{
    return clamp(color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722, 0.f, 1.f);
}

[numthreads(32, 32, 1)]
void TransGreyScale(uint3 index : SV_DispatchThreadID)
{
    uint width, height;
    CSInput.GetDimensions(width, height);
    if(index.x < 0 || index.x > width - 1 || index.y < 0 || index.y > height - 1)
        return;
    float3 color = CSInput.Load(int3(index.x, index.y, 0)).rgb;
    //float3 color = CSInput[index.xy].rgb;
    float luminance = GreyScaleResult(color);

    if (luminance > 0.5f)
        CSResult[index.xy] = float4(color, 1.f);
    else
        CSResult[index.xy] = float4(0.f, 0.f, 0.f, 1.f);
}