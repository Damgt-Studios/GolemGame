
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

float3 CalcDirectional(float3 direction, float3 normal, float4 diffuse)
{
   // Phong diffuse
    float NDotL = dot(normalize(float3(1, 0.5f, 1)), normal);
    float3 finalColor = float3(1,1,1) * saturate(NDotL);
   
    return finalColor * diffuse.xyz;
}

float4 main(OutputVertex v) : SV_TARGET
{
    float4 texelColor = diffuse.Sample(textureSampler, v.tex.xy);
    
    clip(texelColor.a < 0.1 ? -1 : 1);
    
    //Load normal from normal map
    float3 normalMap = normal.Sample(textureSampler, v.tex.xy);
    
    float4 emissiveColor = emissive.Sample(textureSampler, v.tex.xy);

    //Change normal map range from [0, 1] to [-1, 1]
    normalMap = (2.0f * normalMap) - 1.0f;
    //normalMap.z = -normalMap.z;
    
    float3 bitangent = cross(v.tangent, v.normals);

    //Create the "Texture Space"
    float3x3 TBN = float3x3(normalize(v.tangent), normalize(bitangent), normalize(v.normals));
    //texSpace = transpose(texSpace);
    
    //Convert normal from normal map to texture space and store in input.normal
    v.normals = normalize(mul(normalMap, TBN));
    
    //Create the Directional Lighting on the Normal Map
    float3 lightDirection = float3(0, 0.5f, -1);
    float lightMagnitude = saturate(dot(-lightDirection, v.normals)) + (float4(0.156f, 0.003f, 0.215f, 1) * 3);
    float4 dirFinal = lightMagnitude * float4(0.5f, 0.5f, 0.5f, 1);
    
    //Point Light
    //float3 pointlightDirection = normalize(float3(-2,0,-30) - v.worldPos.xyz);
    //float pointlightMagnitude = saturate(dot(pointlightDirection, normalMap.xyz));
    
    //float atten = 1.0 - saturate(length(float3(-2,0,-30) - v.worldPos.xyz) / 100.0f);
    
    //pointlightMagnitude *= atten;
    
    //float4 pointFinal = pointlightMagnitude * float4(1, 1, 1, 1);
    
    //float4 viewDir = normalize(CameraPosition - v.worldPos);
    //float3 reflection = reflect(lightDirection, v.norm.xyz);
    //float Intensity = max(pow(saturate(dot(viewDir.xyz, reflection)), 10), 0);
    //float4 specularFinal = float4(1, 1, 1, 1) * Intensity;
    
    //Multiply the sum of the Additional Modifications
    return texelColor * dirFinal + emissiveColor;
}
