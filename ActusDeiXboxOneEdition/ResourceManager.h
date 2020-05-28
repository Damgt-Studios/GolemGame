#pragma once

#include "Types.h"
#include "Utils.h"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <queue>

namespace 
{
	// Maps
	std::unordered_map<AD_ULONG, unsigned int> light_map;
	std::unordered_map<AD_ULONG, unsigned int> pbrmodel_map;

	std::unordered_map<AD_ULONG, unsigned int> gameObject;

	std::unordered_map<AD_ULONG, unsigned int> fbxmodel_map;
	//std::unordered_map<AD_ULONG, unsigned int> animated_fbxmodel_map;

#ifdef AD_MEMORY_DEFAULT
	std::vector<ADResource::ADRenderer::Vertex> pbrVertexData;
	std::vector<unsigned int> pbrIndxData;

	std::vector<ADResource::ADRenderer::Light> lights;
	std::vector<ADResource::ADRenderer::Model> pbrmodels;

	std::vector<ADResource::ADRenderer::SimpleModel*> fbxmodels;
	//std::vector<ADResource::ADRenderer::SimpleAnimModel> animated_fbxmodels;

	std::vector<ADResource::ADGameplay::GameObject> passables;
	std::vector<ADResource::ADGameplay::GameObject> collidables;

	std::vector<ADResource::ADGameplay::GameObject*> gameobjects;

	std::queue<ADResource::ADGameplay::GameObject*> render_queue;
#else
	ADVector<ADResource::ADRenderer::Vertex> pbrVertexData;
	ADVector<unsigned int> pbrIndxData;

	ADVector<ADResource::ADRenderer::Light> lights;
	ADVector<ADResource::ADRenderer::Model> pbrmodels;

	ADVector<ADResource::ADGameplay::GameObject> passables;
	ADVector<ADResource::ADGameplay::GameObject> collidables;

	ADVector<ADResource::ADGameplay::GameObject*> gameobjects;

	ADQueue<ADResource::ADGameplay::GameObject*> render_queue;
#endif

	// On/ Offs
	ADResource::ADRenderer::SimpleStaticModel skybox;

	// Unified buffers
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
}

// Note: Update the resource manager to load models and store mesh data in same draw call in contiguous memory

class ResourceManager
{
public:

	static AD_ULONG AddPBRModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe = false);

	static AD_ULONG AddSimpleModel(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe = false);
	static AD_ULONG AddAnimatedModel(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe = false);

	static AD_ULONG AddColliderBox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe = false);
	static AD_ULONG AddLight(ADResource::ADRenderer::Light& light);

	static AD_ULONG AddSpyro(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static void AddSkybox(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);

	// Utils
	static AD_ULONG GenerateUniqueID();
	static AD_ULONG GenerateEffectID();

	static AD_ULONG InitializePBRModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, ADUtils::SHADER& shader);

	static AD_ULONG InitializeSimpleModel(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, ADUtils::SHADER& shader);
	static AD_ULONG InitializeAnimatedModel(std::string modelname, std::string materials, std::vector<string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, ADUtils::SHADER& shader);

public:
	// Rendering init shit
	static void ConfigureUnifiedBuffers(ComPtr<ID3D11Device1> device);
	static ComPtr<ID3D11Buffer> GetVertexBuffer();
	static ComPtr<ID3D11Buffer> GetIndexBuffer();
	static void AddModelToRenderQueue(ADResource::ADGameplay::GameObject*);
	static bool RenderQueueEmpty();
	static ADResource::ADGameplay::GameObject* PopFromRenderQueue();

public:
	// Game object shit
	static int AddGameObject(ADResource::ADGameplay::GameObject* OBJ);

public:
	// Getters/ setters
	static int GetLightCount();
	static char* GetLightDataPtr();
	
	static char* GetPBRDataPtr();
	static ADResource::ADRenderer::Model* GetPBRPtr();
	static ADResource::ADRenderer::SimpleStaticModel* GetSkybox();
	static ADResource::ADRenderer::Light* GetLightPtr();
	static int GetPBRModelCount();
	static int GetPBRVertexCount();

	static int GetGameObjectCount();
	static ADResource::ADGameplay::GameObject** GetGameObjectPtr();

	static ADResource::ADRenderer::Model* GetModelPtrFromMeshId(AD_ULONG mesh_id);
	static ADResource::ADRenderer::SimpleModel** GetSimpleModelPtrFromMeshId(AD_ULONG mesh_id);
	static ADResource::ADRenderer::SimpleAnimModel* GetSimpleAnimModelPtrFromMeshId(AD_ULONG mesh_id);

private:
	static AD_ULONG current_id;
	static AD_ULONG effect_id;
};

// Note: Update uuid to use long long