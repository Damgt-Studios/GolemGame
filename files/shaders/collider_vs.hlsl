#pragma pack_matrix(row_major)

struct Vertex
{
    float3 xyzw : POSITION;
    float3 tex : TEXCOORD;
    float3 normals : NORMAL;
    float3 tangent : TANGENT;
};

struct OutputVertex
{
    float4 xyzw : SV_Position;
    float3 tex : TEXCOORD;
    float3 normals : NORMAL;
    float3 tangent : TANGENT;
};

cbuffer WVP : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
    float4 timer;
}

OutputVertex main( Vertex v )
{
    OutputVertex o = (OutputVertex) 0;
    
    o.xyzw = mul(float4(v.xyzw, 1), World);
    o.xyzw = mul(o.xyzw, View);
    o.xyzw = mul(o.xyzw, Proj);
    
    o.tex = v.tex;
    
    o.normals = mul(v.normals, (float3x3) World);
    
    o.tangent = v.tangent;

    return o;
}