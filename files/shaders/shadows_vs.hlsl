#pragma pack_matrix(row_major)

struct InputVertex
{
    float3 pos : POSITION;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct OutputVertex
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD;
};

cbuffer ShaderVars : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
    float4 Camera;
}

OutputVertex main( InputVertex v )
{
    OutputVertex o = (OutputVertex) 0;
    o.position = mul(float4(v.pos, 1), World);
    o.position = mul(o.position, View);
    o.position = mul(o.position, Proj);
    o.tex = v.tex.xy;

    return o;
}