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
#define SPEC_EXP 4
#define PI 3.14159265359
#define GAMMA 2.6
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

float4 computePointLight(Light plight, OutputVertex input)
{
    // Hard coded values
    float specularShininess = 50;
    
    // Texture color
    float4 color = t_albedo.Sample(t_sampler, input.tex);
    
    // Ambient color
    float3 ambientColor = saturate(color.rgb * plight.ambientIntensityDown);
    
    // Diffuse color
    float3 normal = normalize(input.normal);
    float3 lightDirection = (plight.position.xyz - input.posxyz);
    float diffuseFactor = saturate(dot(normal, lightDirection));
    float3 diffuseColor = saturate(diffuseFactor * plight.diffuseIntensity * plight.diffuse);
    
    // Specular color
    // The view direction needs to be added to the positive light direction here
    float3 viewDirection = normalize(input.campos - input.posxyz);
    float3 halfVector = normalize(lightDirection + viewDirection);
    float specularFactor = pow(max(dot(normal, halfVector), 0), specularShininess);
    float3 specularColor = saturate(specularFactor * plight.specularIntensity * plight.specular.rgb);
    
    // Attenuation
    float distance = length(plight.position.xyz - input.posxyz);
    float attenuation = 1.0 - saturate(distance / plight.lightRadius);
    attenuation *= attenuation;
    
    // Apply attenuation
    diffuseColor = saturate(diffuseColor * attenuation);
    specularColor = saturate(specularColor * attenuation);
    
    // Combine phong components
    color = saturate(float4((ambientColor + diffuseColor + specularColor) * color.rgb, color.a));
    return color;
}

float3 CalcDirectionalLight(float3 color, float3 directionToLight, OutputVertex INPUT, Light light)
{   
    //color = pow(color, GAMMA);
    //Load normal from normal map
    float3 normalMap = t_normal.Sample(t_sampler, INPUT.tex).rgb;
    //Change normal map range from [0, 1] to [-1, 1]
    normalMap = (2.0f * normalMap) - 1.0f;
    // Compute tbn
    float3x3 TBN = float3x3(INPUT.tangent, INPUT.bitangent, INPUT.normal);
    normalMap = normalize(mul(normalMap, TBN));
    
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
    
    float3 finalcolor = saturate((diffuseColor + specularColor) * color) + ambientColor;
    
    return finalcolor;
}

float4 main(OutputVertex input) : SV_TARGET
{
    // test
    //return computePointLight(lights[1], input);
    // test
    
    //Load normal from normal map
    float3 normalMap = t_normal.Sample(t_sampler, input.tex).rgb;
    //Change normal map range from [0, 1] to [-1, 1]
    // Compute tbn
    float3x3 TBN = float3x3(input.tangent, input.bitangent, input.normal);
    normalMap = mul(normalMap, TBN);
    normalMap = normalize(normalMap);
    //return float4(normalMap, 1);
    
    float3 white = float3(1, 1, 1);
    float3 directionToLight = normalize(-lights[0].lightDirection.zyx);
    //float3 directionToLight = lights[0].position.xyz - input.posxyz;
    
    float3 albedo = pow(t_albedo.Sample(t_sampler, input.tex).rgb, GAMMA);
    float ambient_occlusion = t_metallic.Sample(t_sampler, input.tex).r;
    float roughness = t_metallic.Sample(t_sampler, input.tex).g;
    float metallic = t_metallic.Sample(t_sampler, input.tex).b;
    
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
    //float distance = 1; // 
    // Attenuation
    //float distance = length(lights[0].position.xyz - input.posxyz);
    //float attenuation = 1.0 - saturate(distance / lights[0].lightRadius);
    //attenuation *= attenuation;
    float attenuation = 1;
    float3 radience = lights[0].diffuse * lights[0].diffuseIntensity * attenuation;
    
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
    _color = pow(_color, 1.0 / GAMMA); // Gamma correction
    
    return float4(_color, 1.0);
    // BDRF
}