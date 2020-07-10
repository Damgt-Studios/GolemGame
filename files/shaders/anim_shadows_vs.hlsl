#pragma pack_matrix(row_major)

struct InputVertex
{
    float3 pos : POSITION;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    int4 joints : JOINTS;
    float4 weights : WEIGHTS;
};

struct OutputVertex
{
    float4 position : SV_Position;
};

cbuffer ShaderVars : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Per;
    float4 timer;
}

cbuffer AnimVars : register(b1)
{
    float4x4 m[50];
}

OutputVertex main(InputVertex v)
{
    float4 skinned_position = float4(0, 0, 0, 0);
    
    for (int i = 0; i < 4; i++)
    {
        skinned_position += mul(float4(v.pos, 1), m[v.joints[i]]) * v.weights[i];
    }
    
    OutputVertex o = (OutputVertex) 0;
    o.position = mul(skinned_position, World);
    o.position = mul(o.position, View);
    o.position = mul(o.position, Per);

    return o;
}