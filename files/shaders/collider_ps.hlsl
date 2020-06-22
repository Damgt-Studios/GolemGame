struct OutputVertex
{
    float4 xyzw : SV_Position;
    float3 tex : TEXCOORD;
    float3 normals : NORMAL;
    float3 tangent : TANGENT;
};

float4 main(OutputVertex o) : SV_TARGET
{
    return float4(0,1,0,0.1f);
}