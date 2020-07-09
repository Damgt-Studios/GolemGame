#pragma pack_matrix(row_major)

struct InputVertex
{
    float3 pos : POSITION;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 instancePosition : TEXCOORD1;
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
    
    float4x4 wm = World;
    wm[3].x += v.instancePosition.x;
    wm[3].y += v.instancePosition.y;
    wm[3].z += v.instancePosition.z;
    
    o.position = mul(float4(v.pos, 1), wm);
    o.position = mul(o.position, View);
    o.position = mul(o.position, Proj);
    o.tex = v.tex.xy;

    return o;
}