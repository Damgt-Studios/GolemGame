
//// Use row major matrices
//#pragma pack_matrix(row_major)

//struct InputVertex
//{
//    float2 position : POSITION;
//    float2 tex : TEX;
//    float4 color : COL;
//};

//struct InputPixel
//{
//    float4 color : COLOR;
//    float4 position : SV_POSITION;
//    float2 tex : TEXCOORD;
//};

//InputPixel main(InputVertex input : InputVertex)
//{
//    InputPixel output;
//    output.position = (input.position, 0, 0);
//    return output;
//}


// Use row major matrices
#pragma pack_matrix(row_major)

struct InputVertex
{
    float4 color : COLOR;
    float3 position : POSITION;
    float2 tex : TEXCOORD;
};

cbuffer SHADER_VARIABLES : register(b0)
{
    float4x4 projectionMatrix;
};

struct OutputVertex
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

OutputVertex main(InputVertex input)
{
    OutputVertex output;
    
    output.color = input.color;
    output.position = mul(float4(input.position, 1), projectionMatrix);
    output.tex = input.tex;
    
    return output;
}