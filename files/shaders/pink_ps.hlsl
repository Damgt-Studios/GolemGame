#pragma pack_matrix(row_major)

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ambient_occlusion : register(t4);
SamplerState t_sampler : register(s0);

struct OutputVertex
{
    float4 position : SV_Position;
    float3 campos : CAM;
    float3 posxyz : POSXYZ;
    float3 worldposition : WORDLPOS;
    float2 tex : TEXCOORD;
    float3 tangent : TANGENT;
    float3 normal : NORMAL;
};

#define PI 3.14159265359

float4 main(OutputVertex input) : SV_TARGET
{
    return float4(1, 0, 1, 1);
}