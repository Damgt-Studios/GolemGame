#pragma pack_matrix(row_major)

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ambient_occlusion : register(t4);

// Samplers
SamplerState t_sampler : register(s0);
SamplerState t_normal_sampler : register(s1);

struct OutputVertex
{
    float4 position : SV_Position;
    float3 campos : CAM;
    float3 posxyz : POSXYZ;
    float3 worldposition : WORDLPOS;
    float2 tex : TEXCOORD;
    float3 tangent : TANGENT;
    float3 bitangent : BITAN;
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
#define SPEC_EXP 100
#define PI 3.14159265359

// Cook-Torrance specular BRDF term
// D(H) * F(L, H) * G(L, V, H)
// -------------------------
//  4(dot(N, L)) * dot(N, V)

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float3 CalcAmbient(Light light, float3 normal, float3 color)
{
    // Convert from [-1, 1] to [0, 1]
    float up = normal.y * 0.5 + 0.5;
    // Calculate the ambient value
    float3 Ambient = (lights[0].ambientDown.xyz * light.ambientIntensityDown) + up * (light.ambientUp.xyz * light.ambientIntensityUp);

    // Apply the ambient value to the color
    return Ambient * color;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float3 CalcDirectionalLight(float3 color, float3 directionToLight, OutputVertex INPUT, Light light)
{    
    color = pow(color, 2.2);
    //Load normal from normal map
    float3 normalMap = t_normal.Sample(t_normal_sampler, INPUT.tex).rgb;
    //Change normal map range from [0, 1] to [-1, 1]
    normalMap = (2.0f * normalMap) - 1.0f;
    
    // Ambient
    float3 ambientColor = CalcAmbient(light, normalMap, color);
    
    // Diffuse
    float NDotL = saturate(dot(normalize(directionToLight), normalMap));
    float3 diffuseColor = saturate(light.diffuse.rgb * NDotL);
    diffuseColor = saturate(diffuseColor * light.diffuseIntensity);
    
    // Specular
    float3 dirToEye = normalize(INPUT.campos - INPUT.worldposition);
    float3 halfvec = normalize(dirToEye + directionToLight);
    float NDotH = saturate(dot(halfvec, normalMap));
    float3 specularColor = saturate(light.specular.rgb * pow(NDotH, SPEC_EXP) * light.specularIntensity);
    
    float3 finalcolor = saturate((diffuseColor + specularColor + ambientColor) * color);
    
    return pow(finalcolor, 1.0/2.2);
}

float4 main(OutputVertex input) : SV_TARGET
{
    // test
    //return float4(CalcDirectionalLight(t_albedo.Sample(t_sampler, input.tex).rgb, -lights[0].lightDirection.xyz, input, lights[0]), 1);
    // test
    
    //Load normal from normal map
    float3 normalMap = t_normal.Sample(t_normal_sampler, input.tex).rgb;
    //Change normal map range from [0, 1] to [-1, 1]
    normalMap = (2.0f * normalMap) - 1.0f;
    float3x3 TBN = float3x3(input.tangent, input.bitangent, input.normal);
    normalMap = normalize(mul(normalMap, TBN));
    
    float3 directionToLight = -lights[0].lightDirection.xyz;
    
    float3 albedo = pow(t_albedo.Sample(t_sampler, input.tex).rgb, 2.2);
    float metallic = t_metallic.Sample(t_sampler, input.tex).r;
    float roughness = t_metallic.Sample(t_sampler, input.tex).g;
    float ambient_occlusion = t_metallic.Sample(t_sampler, input.tex).b;
    float emissive_strength = 1 - t_albedo.Sample(t_sampler, input.tex).a;
    
    // BDRF
    float3 N = normalize(normalMap);
    float3 V = normalize(input.campos - input.worldposition);
    float3 F0 = float3(0.04, 0.04, 0.04); // Hard coded fresnel value for dielectrics
    F0 = lerp(F0, albedo, metallic);
    
    // Reflectance equation
    // Calculate per-light radiance
    float3 L = normalize(directionToLight);
    float3 H = normalize(V + L);
    // Distance is 1 for dlights
    float distance = 1; // Length(lightpos - input.worldpos)
    //float attenuation = 1.0 / (distance * distance);
    float attenuation = 1;
    float3 radience = lights[0].diffuse * attenuation;
    
    // Cook-torrence BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float3 F = fresnelSchlick(max(dot(H, V), 0), F0);
    
    float3 KS = F;
    float3 KD = float3(1.0, 1.0, 1.0) - KS;
    KD *= 1.0 - metallic;
    
    float3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0);
    float3 specular = numerator / max(denominator, 0.001);
    float NDotL = max(dot(N, L), 0.0);
    float3 LO = (KD * albedo / PI + specular) * radience * NDotL; // Change to += on loop
    
    // After loop
    float3 ambient = float3(0.03, .03, .03) * albedo * ambient_occlusion;
    float3 _color = ambient + LO;
    
    _color = _color / (_color + float3(1.0, 1.0, 1.0));
    _color = pow(_color, 1.0 / 2.2); // Gamma correction
    
    // Add emissive color
    //_color = saturate(_color + (albedo * emissive_strength));
    //_color = saturate(_color + (albedo));
    //_color = float3(emissive_strength, emissive_strength, emissive_strength);
    
    return float4(_color, 1.0);
    // BDRF
}