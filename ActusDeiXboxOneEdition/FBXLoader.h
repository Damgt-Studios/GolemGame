//#pragma once
//#include "pch.h"
//using namespace std;
//using namespace DirectX;
//
//
//class FBXLoader
//{
//	struct SimpleVertex
//	{
//
//		XMFLOAT3 Position;
//		XMFLOAT3 Tex;
//		XMFLOAT3 Normal;
//		XMFLOAT3 Tangent;
//	};
//	struct SimpleMesh
//	{
//		vector<SimpleVertex> vertexList;
//		vector<int> indicesList;
//	};
//	// Simple material definition
//	struct material_t
//	{
//	public:
//		enum e_component { EMISSIVE = 0, DIFFUSE, SPECULAR, SHININESS, COUNT };
//		struct component_t
//		{
//			float value[3] = { 0.0f, 0.0f, 0.0f };
//			float factor = 0.0f;
//			int64_t input = -1;
//		};
//		component_t& operator[](int i) { return components[i]; }
//		const component_t& operator[](int i)const { return components[i]; }
//	private:
//		component_t components[COUNT];
//	};
//
//
//
//	vector<XMFLOAT3> positionList;
//	vector<XMFLOAT3> normalList;
//	vector<XMFLOAT3> uvList;
//
//	vector< int > positionIndices, uvIndices, normalIndices;
//
//	int numVertices = 0;
//	int* indices;
//	int numIndices = 0;
//	float scale = 1.0f;
//	SimpleVertex* vertices;
//	std::vector<file_path_t> paths;
//
//
//
//	void FBX_InitLoad(const char* fbxfile, const char* meshfile, const char* matPath = "", const char* matfile = "");
//	void ProcessFbxMesh(FbxNode* Node, const char* meshfile, const char* matpath = "", const char* matfile = "");
//	void Compactify(SimpleVertex* verticesCompact, const char* meshfile);
//	void ComputeTangent(SimpleMesh& simpleMesh);
//
//	void Load_FBX(const char* meshFileName, SimpleMesh& mesh);
//
//};
//
