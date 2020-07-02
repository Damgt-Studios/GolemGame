Texture2D diffuse : register(t0);
Texture2D normal : register(t1);

Texture2D shadows : register(t2);

SamplerState textureSampler : register(s0);

struct OutputVertex
{
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 localPos : LOCALPOS;
    float3 worldPos : WORLDPOS;
};

struct Light
{
    float4 position, lightDirection, diffuse, ambient;
    unsigned int lightType;  float lightRadius;
    float diffuseIntensity, ambientIntensity;
};

cbuffer LightBuffer : register(b0)
{
    Light l[10];
};

float4 main(OutputVertex v) : SV_TARGET
{
    //float4 texelColor = diffuse.Sample(textureSampler, v.tex.xy);
    
    //clip(texelColor.a < 0.1 ? -1 : 1);
    
    ////Load normal from normal map
    //float3 normalMap = normal.Sample(textureSampler, v.tex.xy);
    
    //if (all(normalMap != float3(0, 0, 0)))
    //{
    //    //Change normal map range from [0, 1] to [-1, 1]
    //    normalMap = (2.0f * normalMap) - 1.0f;
    //    normalMap.z = -normalMap.z;
    
    //    float3 bitangent = cross(v.tangent, v.normal);

    //    //Create the "Texture Space"
    //    float3x3 TBN = float3x3(v.tangent, bitangent, v.normal);
    //    //texSpace = transpose(texSpace);
    
    //    //Convert normal from normal map to texture space and store in input.normal
    //    v.normal = mul(normalMap, TBN);
    //}
    
    //float4 dirFinal = float4(0, 0, 0, 0), pointFinal = float4(0, 0, 0, 1);
    
    //for (int i = 0; i < 10; i++)
    //{
    //    //Directional
    //    if (l[i].lightType == 0)
    //    {
    //        float3 lightDirection = -l[i].lightDirection;
    //        float lightMagnitude = saturate(dot(lightDirection, v.normal)) + (l[i].ambient * l[i].ambientIntensity);
    //        float4 dirLight = lightMagnitude * (l[i].diffuse * l[i].diffuseIntensity);
    //        dirFinal += dirLight;
    //    }
    //    //Point Light
    //    else if (l[i].lightType == 1)
    //    {
    //        //Point Light
    //        float3 pointlightDirection = normalize(l[i].position.xyz - v.worldPos);
    //        float pointlightMagnitude = saturate(dot(pointlightDirection, v.normal));
    
    //        float atten = 1.0 - saturate(length(l[i].position.xyz - v.worldPos) / l[i].lightRadius);
    //        atten *= atten;
    
    //        pointlightMagnitude *= atten;
    
    //        pointFinal += pointlightMagnitude * l[i].diffuse * l[i].diffuseIntensity;
    //    }
    //    //Not supported
    //    else
    //    {
            
    //    }
    //}
    
    ////Create the Directional Lighting on the Normal Map
    ////float3 lightDirection = float3(0, -0.5f, -1);
    ////float lightMagnitude = saturate(dot(-lightDirection, v.normal)) + (float4(0.156f, 0.003f, 0.215f, 1) * 3);
    ////float4 dirFinal = lightMagnitude * float4(0.5f, 0.5f, 0.5f, 1);
    
    ////Point Light
    ////float3 pointlightDirection = normalize(float3(-2,0,-30) - v.worldPos.xyz);
    ////float pointlightMagnitude = saturate(dot(pointlightDirection, normalMap.xyz));
    
    ////float atten = 1.0 - saturate(length(float3(-2,0,-30) - v.worldPos.xyz) / 100.0f);
    
    ////pointlightMagnitude *= atten;
    
    ////float4 pointFinal = pointlightMagnitude * float4(1, 1, 1, 1);
    
    ////float4 viewDir = normalize(CameraPosition - v.worldPos);
    ////float3 reflection = reflect(lightDirection, v.norm.xyz);
    ////float Intensity = max(pow(saturate(dot(viewDir.xyz, reflection)), 10), 0);
    ////float4 specularFinal = float4(1, 1, 1, 1) * Intensity;
    
    ////Multiply the sum of the Additional Modifications
    //return texelColor * (dirFinal + pointFinal);
    
    return shadows.Sample(textureSampler, v.tex.xy);
}