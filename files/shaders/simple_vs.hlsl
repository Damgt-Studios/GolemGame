
#pragma pack_matrix(row_major)

struct InputVertex
{
    float3 pos : POSITION;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 instancePosition : TEXCOORD1;
    //int4 joints : JOINTS;
    //float4 weights : WEIGHTS;
};

struct OutputVertex
{
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 localPos : LOCALPOS;
    float3 worldPos : WORLDPOS;
};

cbuffer SHADER_VARIABLES : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 cameraPosition;
};



OutputVertex main(InputVertex vertex)
{
    OutputVertex output = (OutputVertex) 0;
    output.tex = vertex.tex;
    output.normal = vertex.normal;
    output.tangent = vertex.tangent;
    
    float4x4 wm = worldMatrix;
    wm[3].x += vertex.instancePosition.x;
    wm[3].y += vertex.instancePosition.y;
    wm[3].z += vertex.instancePosition.z;
    
    
    output.localPos = vertex.pos;
    output.pos = mul(float4(vertex.pos, 1), wm);
    output.worldPos = output.pos.xyz;
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);
    
    output.normal = normalize(mul(float4(vertex.normal.xyz, 0), wm));
    output.tangent = normalize(mul(float4(vertex.tangent.xyz, 0), wm));
    return output;

}