
Texture2D diffuse : register(t0);
Texture2D emissive : register(t1);
Texture2D normal : register(t2);

SamplerState textureSampler : register(s0);

struct OutputVertex
{
    float4 xyzw : SV_Position;
    float3 tex : TEXCOORD;
    float3 normals : NORMAL;
    float3 tangent : TANGENT;
    int4 joints : JOINTS;
    float4 weights : WEIGHTS;
    float4 localPos : LocalPos;
    float4 worldPos : WorldPos;
};

float3 CalcHemisphericAmbient(float3 normal, float3 color)
{
    float3 AmbientUp = float3(1,1,1);
    float3 AmbientDown = float3(0,0,0);
    
    float up = normal.y * 0.5f + 0.5f;
    float3 Ambient = AmbientDown + up * AmbientUp;
    
    return Ambient * color;
}

float3 CalcDirectional(float3 position, float3 normal, float4 diffuse)
{
   // Phong diffuse
    float NDotL = dot(normalize(float3(1, 0.5f, 1)), normal);
    float3 finalColor = float3(1,1,1) * saturate(NDotL);
   
    return finalColor * diffuse.xyz;
}

float4 main(OutputVertex v) : SV_TARGET
{
    float4 texelColor = diffuse.Sample(textureSampler, v.tex.xy);
    
    float4 emissiveColor = emissive.Sample(textureSampler, v.tex.xy);
    
    //Load normal from normal map
    float4 normalMap = normal.Sample(textureSampler, v.tex.xy);

    //Change normal map range from [0, 1] to [-1, 1]
    normalMap = normalize((2.0f * normalMap) - 1.0f);
    
    //Create Bitangent / Binormal
    float3 binormal = cross(v.tangent, v.normals);
    
    float3x3 TBN = float3x3(v.tangent, binormal, v.normals);
    normalMap.xyz = mul(normalMap.xyz, TBN);
    
    float3 lightDirection = normalize(float3(0, -0.5f, 1));
    float lightMagnitude = saturate(dot(-lightDirection, normalMap.xyz)) + (float4(0.156f, 0.003f, 0.215f, 1) * 0.25f);
    float4 dirFinal = lightMagnitude * float4(4, 4, 4, 4);
    
    clip(texelColor.a < 0.1f ? -1 : 1);
    
    
    return texelColor * dirFinal + emissiveColor;
}