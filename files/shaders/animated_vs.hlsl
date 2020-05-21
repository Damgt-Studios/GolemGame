
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
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    int4 joints : JOINTS;
    float4 weights : WEIGHTS;
};

cbuffer SHADER_VARIABLES : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 cameraPosition;
};

cbuffer ANIMATION_VARIABLES : register(b1)
{
    float4x4 m[27];
}

OutputVertex main(InputVertex vertex)
{
    OutputVertex output = (OutputVertex) 0;
    
        // Skinned position
    float4 skinned_position = float4(0, 0, 0, 0);
    float4 skinned_normal = float4(0, 0, 0, 0);
        
    //int miss_count = 0;
    
    for (int i = 0; i < 4; i++)
    {
        skinned_position += mul(float4(vertex.pos, 1), m[vertex.joints[i]]) * vertex.weights[i];
        skinned_normal += mul(float4(vertex.pos, 0), m[vertex.joints[i]]) * vertex.weights[i];
    }
        
    //skinned_position = float4(v.xyzw, 1);
    //skinned_normal = float4(v.normals, 0);
    
    //skinned_position.w = 1;
    //skinned_normal.w = 0;
    
        // Applymatrices
    skinned_position = mul(skinned_position, worldMatrix);
    //output.worldPos = skinned_position;
    skinned_position = mul(skinned_position, viewMatrix);
    skinned_position = mul(skinned_position, projectionMatrix);
    
    skinned_normal = mul(float4(skinned_normal.xyz, 0), worldMatrix);
    
    output.pos = skinned_position;
    output.tex = vertex.tex;
    output.normal = skinned_normal;
    output.tangent = vertex.tangent;
    output.joints = vertex.joints;
    output.weights = vertex.weights;
    
    return output;

}