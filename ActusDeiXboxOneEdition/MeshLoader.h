#pragma once
#include "pch.h"

using namespace std;
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Position = { 0,0,0 };
	XMFLOAT3 Tex = { 0,0,0 };
	XMFLOAT3 Normal = { 0,0,0 };
	XMFLOAT3 Tangent = { 0,0,0 };
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
	XMFLOAT3 Position = { 0,0,0 };
	XMFLOAT3 Tex = { 0,0,0 };
	XMFLOAT3 Normal = { 0,0,0 };
	XMFLOAT3 Tangent = { 0,0,0 };

	XMINT4 Joint = { 0,0,0,0 };
	XMFLOAT4 Weight = { 0,0,0,0 };

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
	double time = 0; vector<XMMATRIX> jointsMatrix; vector<int> parents;
};


struct anim_clip
{
	string animfilename = "";
	double duration = 0;
	vector<keyframe> frames;
};


void Load_Mesh(const char* meshFileName, SimpleMesh& mesh);
void Load_AnimMesh(const char* meshFileName, SimpleMeshAnim& mesh);
void Load_AnimFile(const char* animFileName, vector<bones>& skeleton, vector<XMMATRIX>& inverse, anim_clip& clip);




