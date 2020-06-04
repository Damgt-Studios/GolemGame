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
    
    float3 dirLight = normalize(float3(1, -1, 1));
    float lightMagnitude = saturate(dot(-dirLight, v.normal));
    float4 dirFinal = lightMagnitude * float4(0, 0, 0, 1) + 0.3f;
    
    return float4(texelColor.rgb * dirFinal.rgb, 1);
}