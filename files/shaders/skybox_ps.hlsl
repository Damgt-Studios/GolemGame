#pragma pack_matrix(row_major)

TextureCube t_albedo : register(t0);
SamplerState t_sampler : register(s0);

struct OutputVertex
{
    float4 position : SV_Position;
    float3 localPos : LocalPos;
};

float4 main(OutputVertex input) : SV_TARGET
{
    //return float4(1, 1, 1, 1);
    return t_albedo.Sample(t_sampler, input.localPos);

}