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

float4 main(OutputVertex v) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}