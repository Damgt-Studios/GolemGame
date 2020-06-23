
#pragma pack_matrix(row_major)

struct InputVertex
{
    float3 pos : POSITION;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    //int4 joints : JOINTS;
    //float4 weights : WEIGHTS;
};

struct OutputVertex
{
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    //int4 joints : JOINTS;
    //float4 weights : WEIGHTS;
};

cbuffer SHADER_VARIABLES : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 cameraPosition;
};



OutputVertex main( InputVertex vertex )
{
    OutputVertex output = (OutputVertex) 0;
    output.tex = vertex.tex;
    output.normal = vertex.normal;
    output.tangent = vertex.tangent;
    
    output.pos = mul(float4(vertex.pos, 1), worldMatrix);
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);
    
    output.normal = normalize(mul(float4(vertex.normal.xyz, 0), worldMatrix));
    output.tangent = normalize(mul(float4(vertex.tangent.xyz, 0), worldMatrix));
    return output;

}