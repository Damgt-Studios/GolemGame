cbuffer Shader_Vars : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 camPos;
    float4 time;
    float4 dimensions;
};

struct OutputVertex
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

struct OutputGeometry
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

[maxvertexcount(4)]
void main(point OutputVertex input[1], inout TriangleStream<OutputGeometry> output)
{
    OutputGeometry verts[4] =
    {
        float4(0, 0, 0, 1), input[0].color, float2(dimensions.z / 8.0f, (dimensions.w + 1) / 4.0f),
        float4(0, 0, 0, 1), input[0].color, float2(dimensions.z / 8.0f, dimensions.w / 4.0f),
        float4(0, 0, 0, 1), input[0].color, float2((dimensions.z + 1) / 8.0f, (dimensions.w + 1) / 4.0f),
        float4(0, 0, 0, 1), input[0].color, float2((dimensions.z + 1) / 8.0f, dimensions.w / 4.0f)
    };
    float halfwidth = dimensions.x / 2.0f;
    float halfheight = dimensions.y / 2.0f;
    float3 planeNormal = input[0].pos - camPos;
    planeNormal.y = 0.0f;
    planeNormal = normalize(planeNormal);
    float3 upVector = float3(0.0f, 1.0f, 0.0f);
    float3 rightVector = normalize(cross(upVector, planeNormal));
    rightVector *= halfwidth;
    upVector = float3(0.0f, halfheight, 0.0f);
    // bottom left
    verts[0].pos = input[0].pos - float4(rightVector, 0) - float4(upVector, 0);
    // top left
    verts[1].pos = input[0].pos - float4(rightVector, 0) + float4(upVector, 0);
    // bottom right
    verts[2].pos = input[0].pos + float4(rightVector, 0) - float4(upVector, 0);
    // top right
    verts[3].pos = input[0].pos + float4(rightVector, 0) + float4(upVector, 0);
    // prep triangle for rasterization
    for (uint i = 0; i < 4; ++i)
    {
        verts[i].pos = mul(viewMatrix, verts[i].pos);
        verts[i].pos = mul(projectionMatrix, verts[i].pos);
    }
    // send verts to the rasterizer
    output.Append(verts[0]);
    output.Append(verts[1]);
    output.Append(verts[2]);
    output.Append(verts[3]);
}