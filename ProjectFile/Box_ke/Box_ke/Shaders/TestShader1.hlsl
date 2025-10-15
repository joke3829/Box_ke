
struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VS_OUTPUT vsmain(uint nVertexID : SV_VertexID)
{
    VS_OUTPUT output;
    
    if (nVertexID == 0)
    {
        output.pos = float4(0.0, 0.5, 0.5, 1.0);
        output.color = float4(1.0, 0.0, 0.0, 1.0);
    }
    else if (nVertexID == 1)
    {
        output.pos = float4(0.5, -0.5, 0.5, 1.0);
        output.color = float4(0.0, 1.0, 0.0, 1.0);
    }
    else if (nVertexID == 2)
    {
        output.pos = float4(-0.5, -0.5, 0.5, 1.0);
        output.color = float4(0.0, 0.0, 1.0, 1.0);
    }
    
    return output;
}

float4 psmain(VS_OUTPUT input) : SV_Target
{
    return input.color;
}



struct TestVertexInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct TestVertextOutput
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

cbuffer objectMat : register(b0)
{
    float4x4 worldMat;
}

cbuffer camera : register(b1)
{
    float4x4 viewproj;
    float4x4 Invvp;
    float3 eye;
};

TestVertextOutput vsTest(float3 pos : POSITION)
{
    TestVertextOutput output;
    output.pos = mul(mul(float4(pos, 1.0f), worldMat), viewproj);
    output.color = float4(frac(pos.x / 3.14), frac(pos.y / 7.51), frac(pos.z / 4.8), 1.0f);
    
    return output;
}

float4 psTest(TestVertextOutput input) : SV_Target
{
    return input.color;
}