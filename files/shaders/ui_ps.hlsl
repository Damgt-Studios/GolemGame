#pragma pack_matrix(row_major)

Texture2D atlas1 : register(t0);
Texture2D atlas2 : register(t1);
SamplerState t_sampler : register(s0);

struct OutputVertex
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

float4 main(OutputVertex input) : SV_TARGET
{
    float4 finalColor;
    if (input.color[0] == 0)
    {
        finalColor = atlas1.Sample(t_sampler, input.tex);
    }
    if (input.color[0] == 1)
    {
        finalColor = atlas2.Sample(t_sampler, input.tex);
    }
    if(finalColor[3] < 1.f)
        discard;
    return finalColor;
}