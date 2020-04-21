
// Use row major matrices
#pragma pack_matrix(row_major)

struct InputVertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
};

cbuffer SHADER_VARIABLES : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 cameraPosition;
};

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

OutputVertex main(InputVertex input)
{
    OutputVertex output;
    
    output.position = mul(float4(input.position, 1), worldMatrix);
    output.worldposition = output.position.xyz;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.posxyz = input.position.xyz;
    output.campos = cameraPosition;
    output.tex = input.tex;
    
    output.normal = normalize(mul(float4(input.normal, 0), worldMatrix).xyz);
    output.tangent = normalize(mul(float4(input.tangent, 0), worldMatrix).xyz);
    output.bitangent = normalize(cross(output.normal, output.tangent));
    
    // Readjust
    output.tangent = normalize(output.tangent - dot(output.tangent, output.normal) * output.normal);
    
    return output;
}