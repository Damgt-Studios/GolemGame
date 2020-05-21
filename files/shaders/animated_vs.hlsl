
#pragma pack_matrix(row_major)

struct Vertex
{
    float3 xyzw : POSITION;
    float3 tex : TEXCOORD;
    float3 normals : NORMAL;
    float3 tangent : TANGENT;
    int4 joints : JOINTS;
    float4 weights : WEIGHTS;
};


struct OutputVertex
{
    float4 xyzw : SV_Position;
    float3 tex : TEXCOORD;
    float3 normals : NORMAL;
    float3 tangent : TANGENT;
    int4 joints : JOINTS;
    float4 weights : WEIGHTS;
    float4 worldPos : WorldPos;
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
    float4x4 m[27];
}
    
OutputVertex main(Vertex v)
{
    OutputVertex output = (OutputVertex) 0;
    
        // Skinned position
    float4 skinned_position = float4(0, 0, 0, 0);
    float4 skinned_normal = float4(0, 0, 0, 0);
        
    //int miss_count = 0;
    
    //for (int i = 0; i < 4; i++)
    //{
    //    skinned_position += mul(float4(v.xyzw, 1), m[v.joints[i]]) * v.weights[i];
    //    skinned_normal += mul(float4(v.normals, 0), m[v.joints[i]]) * v.weights[i];
    //}
        
    //skinned_position = float4(v.xyzw, 1);
    //skinned_normal = float4(v.normals, 0);
    
    //skinned_position.w = 1;
    //skinned_normal.w = 0;
    
        // Applymatrices
    skinned_position = mul(float4(v.xyzw, 1), World);
    output.worldPos = skinned_position;
    skinned_position = mul(skinned_position, View);
    skinned_position = mul(skinned_position, Per);
    
    skinned_normal = mul(float4(skinned_normal.xyz, 0), World);
    
    output.xyzw = skinned_position;
    output.tex = v.tex;
    output.normals = skinned_normal;
    output.tangent = v.tangent;
    output.joints = v.joints;
    output.weights = v.weights;
    
    return output;
    
}
    
//OutputVertex output = (OutputVertex) 0;
    
//float4 skinned_pos = { 0, 0, 0, 0 };
//float4 skinned_norms = { 0, 0, 0, 0 };
    
//    for (
//int j = 0;j < 4; ++j)
//    {
//        skinned_pos += mul(float4(v.xyzw.xyz, 1.0f), m[v.joints[j]]) * v.
//weights[ j];
//        skinned_norms += mul(float4(v.normals.xyz, 0.0f), m[v.joints[j]]) * v.
//weights[ j];
//    }
    
//    output.xyzw =
//skinned_pos;
//    output.tex = v.
//tex;
//    output.normals =
//skinned_norms;
//    output.tangent = v.
//tangent;
    
//    output.xyzw = mul(output.xyzw, World);
//    output.worldPos = output.
//xyzw;
//    output.xyzw = mul(output.xyzw, View);
//    output.xyzw = mul(output.xyzw, Per);
    
//    output.normals = mul(float4(output.normals, 0), World);
    
//    output.joints = v.
//joints;
//    output.weights = v.
//weights;
    
//    return
//output;