Texture2D diffuse : register(t0);
SamplerState s : register(s0);

struct OutputVertex
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD;
};

float4 main(OutputVertex o) : SV_TARGET
{
    return diffuse.Sample(s, o.tex);
}