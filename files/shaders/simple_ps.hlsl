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
    
    clip(texelColor.a < 0.1 ? -1 : 1);
    
    //Create the Directional Lighting on the Normal Map
    //float3 lightDirection = normalize(float3(0, 0, 1));
    //float lightMagnitude = saturate(dot(-lightDirection, v.normal)) + (float4(0.156f, 0.003f, 0.215f, 1) * 3);
    //float4 dirFinal = lightMagnitude * float4(0.3, 0.3, 0.3, 1);
    
    //Point Light
    //float3 pointlightDirection = normalize(float3(-2,0,-30) - v.worldPos.xyz);
    //float pointlightMagnitude = saturate(dot(pointlightDirection, normalMap.xyz));
    
    //float atten = 1.0 - saturate(length(float3(-2,0,-30) - v.worldPos.xyz) / 100.0f);
    
    //pointlightMagnitude *= atten;
    
    //float4 pointFinal = pointlightMagnitude * float4(1, 1, 1, 1);
    
    //Multiply the sum of the Additional Modifications
    return texelColor;// * (dirFinal);
}