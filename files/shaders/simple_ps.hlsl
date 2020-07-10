Texture2D diffuse : register(t0);
Texture2D normal : register(t1);

Texture2D shadowMap : register(t2);

SamplerState textureSampler : register(s0);
//SamplerState shadowSampler : register(s1);
SamplerComparisonState shadowSampler : register(s1);

struct OutputVertex
{
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 localPos : LOCALPOS;
    float4 worldPos : WORLDPOS;
    float4 lightSpaceCoords : LSCOORD;
};

struct Light
{
    float4 position, lightDirection, diffuse, ambient;
    unsigned int lightType;
    float lightRadius;
    float diffuseIntensity, ambientIntensity;
};

cbuffer LightBuffer : register(b0)
{
    Light l[3];
};

float3 CalcHemisphericAmbient(float3 normal, float3 color)
{
    float3 AmbientUp = float3(1, 1, 1);
    float3 AmbientDown = float3(0, 0, 0);
    
    float up = normal.y * 0.5f + 0.5f;
    float3 Ambient = AmbientDown + up * AmbientUp;
    
    return Ambient * color;
}

#define PCF_RANGE 2

float CalcShadowAmount(float4 initialShadowMapCoords)
{
    float shadowLevel = 0.0f;
    float3 spos = initialShadowMapCoords.xyz / initialShadowMapCoords.w;
    
    if (spos.z > 1.0f || spos.z < 0.0f)
        shadowLevel = 1.0f;
    else
    {
        //float bias = max(0.01 * (1.0f - dotLightNormal), 0.00125f);
        float bias = 0.00125f;
        
        [unroll]
        for (int x = -PCF_RANGE; x <= PCF_RANGE; x++)
        {
            [unroll]
            for (int y = -PCF_RANGE; y <= PCF_RANGE; y++)
            {   
                shadowLevel += shadowMap.SampleCmpLevelZero(shadowSampler, spos.xy, spos.z - bias, int2(x,y)).r;
            }
        }
        
        shadowLevel /= ((PCF_RANGE * 2 + 1) * (PCF_RANGE * 2 + 1));
    }
    
    return shadowLevel;
}

float4 main(OutputVertex v) : SV_TARGET
{
    float4 texelColor = diffuse.Sample(textureSampler, v.tex.xy);
    
    clip(texelColor.a < 0.1 ? -1 : 1);
    
    //Load normal from normal map
    float3 normalMap = normal.Sample(textureSampler, v.tex.xy);
    
    if (all(normalMap != float3(0, 0, 0)))
    {
        //Change normal map range from [0, 1] to [-1, 1]
        normalMap = (2.0f * normalMap) - 1.0f;
        normalMap.z = -normalMap.z;
    
        float3 bitangent = cross(v.tangent, v.normal);

        //Create the "Texture Space"
        float3x3 TBN = float3x3(v.tangent, bitangent, v.normal);
        //texSpace = transpose(texSpace);
    
        //Convert normal from normal map to texture space and store in input.normal
        v.normal = mul(normalMap, TBN);
    }
    
    float4 dirFinal = float4(0, 0, 0, 0), pointFinal = float4(0, 0, 0, 1);
    
    for (int i = 0; i < 3; i++)
    {
        //Directional
        if (l[i].lightType == 0)
        {
            float3 lightDirection = -l[i].lightDirection;
            float lightMagnitude = saturate(dot(lightDirection, v.normal)) + (l[i].ambient * l[i].ambientIntensity);
            float4 dirLight = lightMagnitude * (l[i].diffuse * l[i].diffuseIntensity);
            dirFinal += dirLight;
        }
        //Point Light
        else if (l[i].lightType == 1)
        {
            //Point Light
            float3 pointlightDirection = normalize(l[i].position.xyz - v.worldPos.xyz);
            float pointlightMagnitude = saturate(dot(pointlightDirection, v.normal));
    
            float atten = 1.0 - saturate(length(l[i].position.xyz - v.worldPos.xyz) / l[i].lightRadius);
            atten *= atten;
    
            pointlightMagnitude *= atten;
    
            pointFinal += pointlightMagnitude * l[i].diffuse * l[i].diffuseIntensity;
        }
    }
 
    //Sample Shadow Map
    //float near = 0.01f;
    //float far = 3000;
    //float depth = shadowMap.Sample(textureSampler, v.tex.xy);
    //float lineardepth = (2.0f * near) / (far + near - depth * (far - near));
    
    //return float4(lineardepth, lineardepth, lineardepth, 1);
    
    //Multiply the sum of the Additional Modifications
    texelColor = float4(CalcHemisphericAmbient(v.normal, texelColor.xyz), 1);
    dirFinal = dirFinal * clamp(CalcShadowAmount(v.lightSpaceCoords), 0.4f, 1);
    return texelColor * (dirFinal + pointFinal);
}






//Directional Lighting
    //float3 lightDirection = float3(0, -0.5f, -1);
    //float lightMagnitude = saturate(dot(-lightDirection, v.normal)) + (float4(0.156f, 0.003f, 0.215f, 1) * 3);
    //float4 dirFinal = lightMagnitude * float4(0.5f, 0.5f, 0.5f, 1);
    
//Point Lighting
    //float3 pointlightDirection = normalize(float3(-2, 0, -30) - v.worldPos.xyz);
    //float pointlightMagnitude = saturate(dot(pointlightDirection, normalMap.xyz));
    
    //float atten = 1.0 - saturate(length(float3(-2, 0, -30) - v.worldPos.xyz) / 100.0f);
    
    //pointlightMagnitude *= atten;
    
    //float4 pointFinal = pointlightMagnitude * float4(1, 1, 1, 1);
    
//Specular Lighting
    //float4 viewDir = normalize(CameraPosition - v.worldPos);
    //float3 reflection = reflect(lightDirection, v.norm.xyz);
    //float Intensity = max(pow(saturate(dot(viewDir.xyz, reflection)), 10), 0);
    //float4 specularFinal = float4(1, 1, 1, 1) * Intensity;