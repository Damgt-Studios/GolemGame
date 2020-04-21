#pragma pack_matrix(row_major)

Texture2D t_albedo : register(t0);
SamplerState t_sampler : register(s0);

struct OutputVertex
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

float4 main(OutputVertex input) : SV_TARGET
{
    float4 finalColor = t_albedo.Sample(t_sampler, input.tex);
    if(finalColor[3] < 1.f)
        discard;
    return finalColor;
}