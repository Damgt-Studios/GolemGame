#pragma once
#include "pch.h"

using namespace std;
using namespace DirectX;
using file_path_t = std::array<char, 260>; // Simple max size file path string

int numVertices = 0;
int* indices;
int numIndices = 0;
float scale = 1.0f;
struct SimpleVertex
{

	XMFLOAT3 Position;
	XMFLOAT3 Tex;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;
};
SimpleVertex* vertices;


struct SimpleMesh
{
	vector<SimpleVertex> vertexList;
	vector<int> indicesList;
};
// Simple material definition






void Load_FBX(const char* meshFileName, SimpleMesh& mesh);




