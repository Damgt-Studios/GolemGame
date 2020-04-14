#pragma pack_matrix(row_major)

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ambient_occlusion : register(t4);
SamplerState t_sampler : register(s0);

struct OutputVertex
{
    float4 position : SV_Position;
    float3 campos : CAM;
    float3 posxyz : POSXYZ;
    float3 worldposition : WORDLPOS;
    float2 tex : TEXCOORD;
    float3 tangent : TANGENT;
    float3 normal : NORMAL;
};

struct Light
{
    float4 position, lightDirection;
    float4 ambientUp, ambientDown, diffuse, specular;
    unsigned int lightType;
    float lightRadius;
    float cosineInnerCone, cosineOuterCone;
    float ambientIntensityUp, ambientIntensityDown, diffuseIntensity, specularIntensity;
    float lightLength, p1, p2, p3;
};

cbuffer LIGHTS
{
    Light lights[3];
};

#define PI 3.14159265359
#define SPEC_EXP 100

float3 CalcAmbient(Light light, float3 normal, float3 color)
{
    // Convert from [-1, 1] to [0, 1]
    float up = normal.y * 0.5 + 0.5;
    // Calculate the ambient value
    float3 Ambient = (lights[0].ambientDown.xyz * light.ambientIntensityDown) + up * (light.ambientUp.xyz * light.ambientIntensityUp);

    // Apply the ambient value to the color
    return Ambient * color;
}

float3 CalcDirectionalLight(float3 color, float3 directionToLight, OutputVertex INPUT, Light light)
{
    // Ambient
    float3 ambientColor = CalcAmbient(light, INPUT.normal, color);
    
    // Diffuse
    float NDotL = saturate(dot(normalize(directionToLight), INPUT.normal));
    float3 diffuseColor = saturate(light.diffuse.rgb * NDotL);
    diffuseColor = saturate(diffuseColor * light.diffuseIntensity);
    
    // Specular
    float3 dirToEye = normalize(INPUT.campos - INPUT.worldposition);
    float3 halfvec = normalize(dirToEye + directionToLight);
    float NDotH = saturate(dot(halfvec, INPUT.normal));
    float3 specularColor = saturate(light.specular.rgb * pow(NDotH, SPEC_EXP) * light.specularIntensity);
    
    float3 finalcolor = saturate((diffuseColor + specularColor + ambientColor) * color);
    
    return finalcolor;
}

float4 main(OutputVertex input) : SV_TARGET
{
    return float4(CalcDirectionalLight(float3(1, 0, 1), -lights[0].position.xyz, input, lights[0]), 1);
}