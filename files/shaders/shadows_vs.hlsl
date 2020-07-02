#pragma pack_matrix(row_major)

struct InputVertex
{
    float3 pos : POSITION;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

cbuffer ShaderVars : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
    float4 Camera;
}

float4 main( InputVertex v ) : SV_POSITION
{
    float4 position = mul(float4(v.pos, 1), World);
    position = mul(position, View);
    position = mul(position, Proj);
    return position;
}