#include "pch.h"
#include "MeshLoader.h"


void Load_Mesh(const char* meshFileName, SimpleMesh& mesh)
{

	std::fstream file{ meshFileName, std::ios_base::in | std::ios_base::binary };

	assert(file.is_open());

	uint32_t player_index_count;
	file.read((char*)&player_index_count, sizeof(uint32_t));

	mesh.indicesList.resize(player_index_count);

	file.read((char*)mesh.indicesList.data(), sizeof(uint32_t) * player_index_count);

	uint32_t player_vertex_count;
	file.read((char*)&player_vertex_count, sizeof(uint32_t));

	mesh.vertexList.resize(player_vertex_count);

	file.read((char*)mesh.vertexList.data(), sizeof(SimpleVertex) * player_vertex_count);

	//Example mesh conditioning if needed - this flips handedness
	for (auto& v : mesh.vertexList)
	{
		v.Position.x = -v.Position.x;
		v.Normal.x = -v.Normal.x;
		v.Tex.y = 1.0f - v.Tex.y;

	}

	int tri_count = (int)(mesh.indicesList.size() / 3);

	for (int i = 0; i < tri_count; ++i)
	{
		int* tri = mesh.indicesList.data() + i * 3;

		int temp = tri[0];
		tri[0] = tri[2];
		tri[2] = temp;
	}
	file.close();
};

void Load_AnimMesh(const char* meshFileName, SimpleMeshAnim& mesh)
{

	std::fstream file{ meshFileName, std::ios_base::in | std::ios_base::binary };

	assert(file.is_open());

	uint32_t player_index_count;
	file.read((char*)&player_index_count, sizeof(uint32_t));

	mesh.indicesList.resize(player_index_count);

	file.read((char*)mesh.indicesList.data(), sizeof(uint32_t) * player_index_count);

	uint32_t player_vertex_count;
	file.read((char*)&player_vertex_count, sizeof(uint32_t));

	mesh.vertexList.resize(player_vertex_count);

	file.read((char*)mesh.vertexList.data(), sizeof(SimpleVertexAnim) * player_vertex_count);

	//Example mesh conditioning if needed - this flips handedness
	//for (auto& v : mesh.vertexList)
	//{
	//	v.Position.x = -v.Position.x;
	//	v.Normal.x = -v.Normal.x;
	//	v.Tex.y = 1.0f - v.Tex.y;

	//}

	//int tri_count = (int)(mesh.indicesList.size() / 3);

	//for (int i = 0; i < tri_count; ++i)
	//{
	//	int* tri = mesh.indicesList.data() + i * 3;

	//	int temp = tri[0];
	//	tri[0] = tri[2];
	//	tri[2] = temp;
	//}
	file.close();
};
void Load_AnimFile(const char* animFileName, vector<bones>& skeleton, vector<XMMATRIX>& inverse, anim_clip& clip)
{

	std::fstream file{ animFileName, std::ios_base::in | std::ios_base::binary };

	assert(file.is_open());

	uint32_t joint_Count;
	file.read((char*)&joint_Count, sizeof(uint32_t));
	uint32_t inv_joint_count;
	file.read((char*)&inv_joint_count, sizeof(uint32_t));

	double duration;
	file.read((char*)&duration, sizeof(double));
	clip.duration = duration;
	uint32_t key_joint_count;
	file.read((char*)&key_joint_count, sizeof(uint32_t));

	clip.frames.resize(key_joint_count);


	for (unsigned int i = 0; i < key_joint_count; i++)
	{

		uint32_t matrix_joint_count;
		file.read((char*)&matrix_joint_count, sizeof(uint32_t));
		clip.frames[i].jointsMatrix.resize(matrix_joint_count);
		file.read((char*)&clip.frames[i].time, sizeof(double));
		uint32_t parents_joint_count;
		file.read((char*)&parents_joint_count, sizeof(uint32_t));
		clip.frames[i].parents.resize(parents_joint_count);


		file.read((char*)clip.frames[i].parents.data(), sizeof(int) * parents_joint_count);

		file.read((char*)clip.frames[i].jointsMatrix.data(), sizeof(XMMATRIX) * matrix_joint_count);


	}
	skeleton.resize(joint_Count);
	file.read((char*)skeleton.data(), sizeof(bones) * joint_Count);

	inverse.resize(inv_joint_count);
	file.read((char*)inverse.data(), sizeof(XMMATRIX) * inv_joint_count);



	file.close();
};
