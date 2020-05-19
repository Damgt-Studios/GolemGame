#pragma once
#include "pch.h"

using namespace std;
using namespace DirectX;
using file_path_t = std::array<char, 260>; // Simple max size file path string

static int numVertices = 0;
static int* indices;
static int numIndices = 0;
static float scale = 1.0f;
struct SimpleVertex
{

	XMFLOAT3 Position;
	XMFLOAT3 Tex;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;
};
static SimpleVertex* vertices;


struct SimpleMesh
{
	vector<SimpleVertex> vertexList;
	vector<int> indicesList;
};
// Simple material definition






void Load_FBX(const char* meshFileName, SimpleMesh& mesh);




