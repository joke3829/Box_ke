
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
    float2      padding;
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
        (diffusefactor * material.diffuseColor.rgb * light.lightColor.rgb) +
        (Specularfactor * material.specularColor.rgb * light.lightColor.rgb);

    return clamp(retColor + 0.2f * material.ambientColor.rgb + material.emissiveColor.rgb, float3(0.f, 0.f, 0.f), float3(1.f, 1.f, 1.f));
}

float3 BlinnPhongLightingResult(in float3 wPos, in float3 wNormal, in float3 eye, in PhongMaterial material, in Light light)
{
    float3 retColor = float3(0.f, 0.f, 0.f);
    float3 L;
    
    if (light.type == LIGHT_TYPE_DIRECTIONAL)
        L = -light.direction;
    else
        L = light.position - wPos;
    L = normalize(L);
    
    float diffusefactor = max(dot(wNormal, L), 0.f);
    float3 V = normalize(eye - wPos);
    float Specularfactor = CalculateBlinnPhongSpecular(wNormal, L, V, material.shininess);
        
    retColor =
    (diffusefactor * material.diffuseColor.rgb * light.lightColor.rgb) +
    (Specularfactor * material.specularColor.rgb * light.lightColor.rgb);
    //return retColor + 0.2f * material.ambientColor.rgb + material.emissiveColor.rgb;
    return clamp(retColor + 0.2f * material.ambientColor.rgb + material.emissiveColor.rgb, float3(0.f, 0.f, 0.f), float3(1.f, 1.f, 1.f));
}