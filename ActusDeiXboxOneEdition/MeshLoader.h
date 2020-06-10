#pragma once
#include "pch.h"

using namespace std;
using namespace DirectX;

struct SimpleVertex
{

	XMFLOAT3 Position;
	XMFLOAT3 Tex;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;
};


struct SimpleMesh
{
	vector<SimpleVertex> vertexList;
	vector<int> indicesList;
};
// Simple material definition

//Animation
struct SimpleVertexAnim
{
	XMFLOAT3 Position;
	XMFLOAT3 Tex;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;

	XMINT4 Joint;
	XMFLOAT4 Weight;

};
struct SimpleMeshAnim
{
	vector<SimpleVertexAnim> vertexList;
	vector<int> indicesList;
};

struct bones
{
	XMMATRIX global_xform; int parent_index; int childCount;

};
struct keyframe
{
	double time; vector<XMMATRIX> jointsMatrix; vector<int> parents;
};


struct anim_clip
{
	string animfilename;
	double duration;
	vector<keyframe> frames;
};


void Load_Mesh(const char* meshFileName, SimpleMesh& mesh);
void Load_AnimMesh(const char* meshFileName, SimpleMeshAnim& mesh);
void Load_AnimFile(const char* animFileName, vector<bones>& skeleton, vector<XMMATRIX>& inverse, anim_clip& clip);




