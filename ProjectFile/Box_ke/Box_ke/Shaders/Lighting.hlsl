
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_SPOTLIGHT 1
#define LIGHT_TYPE_POINTLIGHT 2

struct Light
{
    uint        type;
    float3      position;
    float4      lightColor;
    float3      direction;
    float       intensity;
    float       range;
    float       spotAngle;
    uint        lightindex;
    float       padding;
    float4x4    shadowMapMatrix;
};

struct PhongMaterial
{
    float4      diffuseColor;
    float4      specularColor;
    float4      ambientColor;
    float4      emissiveColor;
    float       shininess;
    float3      padding;
};


// shadow map test
TextureCubeArray g_ShadowMapCube : register(t7);
Texture2DArray g_ShadowMap : register(t8);

SamplerState g_Sample : register(s0);

float CalculatePhongSpecular(in float3 wNormal, in float3 L, in float3 V, in float shininess)
{
    float3 ref = normalize(reflect(-L, wNormal));
    return pow(max(dot(ref, V), 0.f), shininess);
}

float CalculateBlinnPhongSpecular(in float3 wNormal, in float3 L, in float3 V, in float shininess)
{
    float3 H = normalize(L + V);
    return pow(max(dot(wNormal, H), 0.f), shininess);
}

/*float3 PhongLightingResult(in float3 wPos, in float3 wNormal, in float3 eye, in PhongMaterial material, in Light light)
{
    float3 lightColor = light.lightColor.rgb * light.intensity;
    float3 retColor = float3(0.f, 0.f, 0.f);
    float3 L;
    
    if(light.type == LIGHT_TYPE_DIRECTIONAL)
        L = -light.direction;
    else
        L = light.position - wPos;
    L = normalize(L);
    
    float diffusefactor = max(dot(wNormal, L), 0.f);

        float3 V = normalize(eye - wPos);
        float Specularfactor = CalculatePhongSpecular(wNormal, L, V, material.shininess);
        
        retColor =
        (diffusefactor * material.diffuseColor.rgb * lightColor) +
        (Specularfactor * material.specularColor.rgb * lightColor);

    return retColor + 0.2f * material.ambientColor.rgb + material.emissiveColor.rgb;
}*/

float3 BlinnPhongLightingResult(in float3 wPos, in float3 wNormal, in float3 eye, in PhongMaterial material, in Light light)
{
    float3 lightColor = light.lightColor.rgb * light.intensity;
    uint lightindex = light.lightindex;
    float3 retColor = float3(0.f, 0.f, 0.f); //25.f * material.emissiveColor.rgb;
    //float3 retColor = 0.2f * material.ambientColor.rgb + 8.f * material.emissiveColor.rgb;
    float3 L = light.position - wPos;
    
    if (dot(material.emissiveColor.rgb, float3(1.f, 1.f, 1.f)) == 0.f)
    {
        switch (light.type)
        {
            case LIGHT_TYPE_DIRECTIONAL:{
                    float4 subjectPos = mul(float4(wPos, 1.f), light.shadowMapMatrix);
                    float3 ndcPos = subjectPos.xyz / subjectPos.w;
                    float2 uv = (float2(ndcPos.x, -ndcPos.y) * 0.5f) + 0.5f;
                    float shadowMapDepth = g_ShadowMap.Sample(g_Sample, float3(uv, lightindex)).r;
                    
                    if (ndcPos.z <= shadowMapDepth + 0.001f)
                    {
                        float3 nL = normalize(-light.direction);
                        float diffusefactor = max(dot(wNormal, nL), 0.f);
                        float3 V = normalize(eye - wPos);
                        float Specularfactor = CalculateBlinnPhongSpecular(wNormal, nL, V, material.shininess);
        
                        retColor = retColor +
                    (diffusefactor * material.diffuseColor.rgb * lightColor) +
                    (Specularfactor * material.specularColor.rgb * lightColor);
                    }
                }
                break;
            case LIGHT_TYPE_SPOTLIGHT:{
                    float3 nL = normalize(L);
                    float distance_to_light = length(L);
                    float cdot = cos(radians(light.spotAngle / 2));
                    float ddotL = dot(normalize(light.direction), -nL);
                    if (distance_to_light <= light.range && cdot <= ddotL)  // 현재는 페넘브라 감쇄 없는걸로
                    {
                        float4 subjectPos = mul(float4(wPos, 1.f), light.shadowMapMatrix);
                        float3 ndcPos = subjectPos.xyz / subjectPos.w;
                        float2 uv = (float2(ndcPos.x, -ndcPos.y) *0.5f) + 0.5f;
                        float shadowMapDepth = g_ShadowMap.Sample(g_Sample, float3(uv, lightindex)).r;
                        
                        /*float diff = ndcPos.z - shadowMapDepth;
                        if(diff >= 0.00001f)
                            retColor = float4(0, 0, 0, 1);
                        else
                            retColor = float4(1, 1, 1, 1);*/
                        
                        if (ndcPos.z <= shadowMapDepth + 0.000001f)     // bias 계속 조정 필요
                        {
                            float percentage = pow((light.range - distance_to_light) / light.range, 2.f);
                            lightColor *= percentage;
                            float diffusefactor = max(dot(wNormal, nL), 0.f);
                            float3 V = normalize(eye - wPos);
                            float Specularfactor = CalculateBlinnPhongSpecular(wNormal, nL, V, material.shininess);
        
                            retColor = retColor +
                        (diffusefactor * material.diffuseColor.rgb * lightColor) +
                        (Specularfactor * material.specularColor.rgb * lightColor);
                        }
                    }
                }
                break;
            case LIGHT_TYPE_POINTLIGHT:{
                    float distance_to_light = distance(wPos, light.position);
                    if (distance_to_light <= light.range)
                    {
                        float3 nL = normalize(L);
                        float shadowMapDepth = g_ShadowMapCube.Sample(g_Sample, float4(-nL, lightindex)).r;
                        float tdepth = distance_to_light / light.range;
                        
                        if (tdepth <= shadowMapDepth + 0.0005f)
                        {
                            float percentage = pow((light.range - distance_to_light) / light.range, 2.f);
                            lightColor *= percentage;
                            float diffusefactor = max(dot(wNormal, nL), 0.f);
                            float3 V = normalize(eye - wPos);
                            float Specularfactor = CalculateBlinnPhongSpecular(wNormal, nL, V, material.shininess);
        
                            retColor = retColor +
                        (diffusefactor * material.diffuseColor.rgb * lightColor) +
                        (Specularfactor * material.specularColor.rgb * lightColor);
                        }
                    }
                }
                break;
        }
    }
    return retColor;
    //return clamp(retColor + 0.2f * material.ambientColor.rgb + material.emissiveColor.rgb, float3(0.f, 0.f, 0.f), float3(1.f, 1.f, 1.f));
}