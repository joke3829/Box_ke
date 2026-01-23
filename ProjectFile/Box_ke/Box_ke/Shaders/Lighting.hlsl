
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

float3 PhongLightingResult(in float3 wPos, in float3 wNormal, in float3 eye, in PhongMaterial material, in Light light)
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
}

float3 BlinnPhongLightingResult(in float3 wPos, in float3 wNormal, in float3 eye, in PhongMaterial material, in Light light)
{
    float3 lightColor = light.lightColor.rgb * light.intensity;
    uint lightindex = light.lightindex;
    float3 retColor = float3(0.f, 0.f, 0.f); //25.f * material.emissiveColor.rgb;
    //float3 retColor = 0.2f * material.ambientColor.rgb + 8.f * material.emissiveColor.rgb;
    float3 L;
    
    if (light.type == LIGHT_TYPE_DIRECTIONAL)
        L = -light.direction;
    else
        L = light.position - wPos;
    L = normalize(L);
    
    if (dot(material.emissiveColor.rgb, float3(1.f, 1.f, 1.f)) == 0.f)
    {
        switch (light.type)
        {
            case LIGHT_TYPE_DIRECTIONAL:{
                    float diffusefactor = max(dot(wNormal, L), 0.f);
                    float3 V = normalize(eye - wPos);
                    float Specularfactor = CalculateBlinnPhongSpecular(wNormal, L, V, material.shininess);
        
                    retColor = retColor +
                    (diffusefactor * material.diffuseColor.rgb * lightColor) +
                    (Specularfactor * material.specularColor.rgb * lightColor);
                }
                break;
            case LIGHT_TYPE_SPOTLIGHT:{
            
                }
                break;
            case LIGHT_TYPE_POINTLIGHT:{
                    float distance_to_light = distance(wPos, light.position);
                    if (distance_to_light <= light.range)
                    {
                        float shadowMapDepth = g_ShadowMapCube.Sample(g_Sample, float4(normalize(-L), lightindex)).r;
                        float tdepth = distance_to_light / light.range;
                        
                        if (tdepth <= shadowMapDepth + 0.005f)
                        {
                            float percentage = pow((light.range - distance_to_light) / light.range, 2.f);
                            lightColor *= percentage;
                            float diffusefactor = max(dot(wNormal, L), 0.f);
                            float3 V = normalize(eye - wPos);
                            float Specularfactor = CalculateBlinnPhongSpecular(wNormal, L, V, material.shininess);
        
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