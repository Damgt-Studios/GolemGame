
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
    float4 localPos : LocalPos;
    float4 worldPos : WorldPos;
    float4 lightSpaceCoords : LSCOORD;
};

cbuffer ShaderVars : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Per;
    float4 Camera;
}

cbuffer LightSpace : register(b1)
{
    float4x4 LightView;
    float4x4 LightProjection;
}

cbuffer AnimVars : register(b2)
{
    float4x4 m[50];
}
    
OutputVertex main(Vertex v)
{
    OutputVertex output = (OutputVertex) 0;
    
        // Skinned position
    float4 skinned_position = float4(0, 0, 0, 0);
    float4 skinned_normal = float4(0, 0, 0, 0);
    
    for (int i = 0; i < 4; i++)
    {
        skinned_position += mul(float4(v.xyzw, 1), m[v.joints[i]]) * v.weights[i];
        skinned_normal += mul(float4(v.normals, 0), m[v.joints[i]]) * v.weights[i];
    }

        // Applymatrices
    output.localPos = skinned_position;
    skinned_position = mul(skinned_position, World);
    output.worldPos = skinned_position;
    skinned_position = mul(skinned_position, View);
    skinned_position = mul(skinned_position, Per);
    
    skinned_normal = mul(float4(skinned_normal.xyz, 0), World);
    
    output.tangent = mul(float4(v.tangent, 0), World);
    
    output.xyzw = skinned_position;
    output.tex = v.tex;
    
    output.normals = skinned_normal;
    
    output.joints = v.joints;
    output.weights = v.weights;
    
    float4 shadowHomo = mul(output.worldPos, LightView);
    shadowHomo = mul(shadowHomo, LightProjection);
    output.lightSpaceCoords = shadowHomo * float4(0.5f, -0.5f, 1.0f, 1.0f) + float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w;
    
    return output;
    
}