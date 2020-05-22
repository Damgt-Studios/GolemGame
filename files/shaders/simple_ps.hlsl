Texture2D diffuse : register(t0);
Texture2D normal : register(t1);
Texture2D emissive : register(t2);

SamplerState textureSampler : register(s0);

struct OutputVertex
{
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    //int4 joints : JOINTS;
    //float4 weights : WEIGHTS;
};

float4 main(OutputVertex v) : SV_TARGET
{
    float4 texelColor = diffuse.Sample(textureSampler, v.tex.xy);
    float4 emissiveColor = emissive.Sample(textureSampler, v.tex.xy);
    return texelColor + emissiveColor;
}