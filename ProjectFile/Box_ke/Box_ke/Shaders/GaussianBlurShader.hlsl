
Texture2D CSInput : register(t0);
RWTexture2D<float4> CSResult : register(u0);

static float g_GaussianFactor[11] = { 
    1.f / 22.f, 1.f / 22.f, 1.f / 22.f, 2.f / 22.f, 2.f / 22.f,
    8.f / 22.f,
    2.f / 22.f, 2.f / 22.f, 1.f / 22.f, 1.f / 22.f, 1.f / 22.f
};

/*static float g_GaussianFactor[] =
{
   0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
};*/

/*static float g_GaussianFactor[] =
{
    1.f / 11.f, 1.f / 11.f, 1.f / 11.f, 1.f / 11.f, 1.f / 11.f, 1.f / 11.f, 1.f / 11.f, 1.f / 11.f, 1.f / 11.f,
    1.f / 11.f, 1.f / 11.f
};*/


groupshared float4 g_ThreadCache[5 + 64 + 5];

[numthreads(64, 1, 1)]
void HorizonGaussianBlur(int3 groupindex : SV_GroupThreadID, int3 index : SV_DispatchThreadID)
{
    uint width, height;
    CSInput.GetDimensions(width, height);
    
    if (groupindex.x < 5)
    {
        g_ThreadCache[groupindex.x] = CSInput[int2(max(0, index.x - 5), index.y)];
    }
    else if (groupindex.x >= (64 - 5))
    {
        g_ThreadCache[groupindex.x + 5 + 5] = CSInput[int2(min(index.x + 5, width - 1), index.y)];
    }
    
    g_ThreadCache[groupindex.x + 5] = CSInput[int2(min(index.x, width - 1), index.y)];
    
    GroupMemoryBarrierWithGroupSync();

    if (index.x >= 0 && index.x < width && index.y >= 0 && index.y < height)
    {
        float3 color = float3(0.f, 0.f, 0.f);
        for (int i = -5; i <= 5; ++i)
        {
            color += g_GaussianFactor[i + 5] * g_ThreadCache[groupindex.x + 5 + i];
        }
        CSResult[index.xy] = float4(color, 1.f);
    }
    
    /*if (index.x >= 0 && index.x < width && index.y >= 0 && index.y < height)
    {
        float3 color = float3(0.f, 0.f, 0.f);
        for (int i = -5; i <= 5; ++i)
            color += g_GaussianFactor[i + 5] * CSInput[int2(clamp(index.x, 0, width - 1), index.y)];
        CSResult[index.xy] = float4(color, 1.f);
    }*/
    //CSResult[index.xy] = float4(1.f, 0.f, 0.f, 1.f);
}

[numthreads(1, 64, 1)]
void VerticalGaussianBlur(int3 groupindex : SV_GroupThreadID, int3 index : SV_DispatchThreadID)
{
    uint width, height;
    CSInput.GetDimensions(width, height);
    
    if (groupindex.y < 5)
    {
        g_ThreadCache[groupindex.y] = CSInput[int2(index.x, max(0, index.y - 5))];
    }
    else if (groupindex.y >= (64 - 5))
    {
        g_ThreadCache[groupindex.y + 5 + 5] = CSInput[int2(index.x, min(index.y + 5, height - 1))];
    }
    
    g_ThreadCache[groupindex.y + 5] = CSInput[int2(index.x, min(index.y, height - 1))];
    
    GroupMemoryBarrierWithGroupSync();

    if (index.x >= 0 && index.x < width && index.y >= 0 && index.y < height)
    {
        float3 color = float3(0.f, 0.f, 0.f);
        for (int i = -5; i <= 5; ++i)
        {
            color += g_GaussianFactor[i + 5] * g_ThreadCache[groupindex.y + 5 + i];
        }
        CSResult[index.xy] = float4(color, 1.f);
    }
    /*if (index.x >= 0 && index.x < width && index.y >= 0 && index.y < height)
    {
        float3 color = float3(0.f, 0.f, 0.f);
        for (int i = -5; i <= 5; ++i)
            color += g_GaussianFactor[i + 5] * CSInput[int2(index.x, clamp(index.y, 0, height - 1))];
        CSResult[index.xy] = float4(1.f, 0.f , 0.f, 1.f);
    }*/
}