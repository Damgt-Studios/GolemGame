#define amountPos 4096

cbuffer Shader_Vars : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 camPos;
    float4 time;
    float4 dimensions;
};

cbuffer particlePos : register(b1)
{
    float4 particlePositions[amountPos];
}

struct InputVertex
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct OutputVertex
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

OutputVertex main(InputVertex input, uint InstanceID : SV_InstanceID)
{
    OutputVertex output = (OutputVertex) 0;
    output.pos = particlePositions[InstanceID % amountPos];
    output.pos = mul(worldMatrix, output.pos);
    output.color = input.color;
    return output;
}