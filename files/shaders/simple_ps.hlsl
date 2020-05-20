struct OutputVertex
{
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tanget : TANGENT;
};

float4 main(OutputVertex v) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}