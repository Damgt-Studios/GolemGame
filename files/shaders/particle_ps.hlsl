Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer Shader_Vars : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 camPos;
    float4 time;
    float4 dimensions;
};

struct OutputGeometry
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(OutputGeometry inputPixel) : SV_TARGET
{
    float4 finalColor = tex.Sample(samp, inputPixel.uv);
    if (finalColor.a <= 0.5f)
        discard;
    return finalColor;
}