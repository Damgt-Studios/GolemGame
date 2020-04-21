#pragma once

#include "Types.h"
#include "Utils.h"

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

	std::vector<ADResource::ADRenderer::Vertex> pbrVertexData;
	std::vector<unsigned int> pbrIndxData;

	std::vector<ADResource::ADRenderer::Light> lights;
	std::vector<ADResource::ADRenderer::Model> pbrmodels;

	std::vector<ADResource::ADGameplay::GameObject> passables;
	std::vector<ADResource::ADGameplay::GameObject> collidables;

	std::vector<ADResource::ADGameplay::GameObject*> gameobjects;

	std::queue<ADResource::ADGameplay::GameObject*> render_queue;

	// On/ Offs
	ADResource::ADRenderer::Model skybox;

	// Unified buffers
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
}

// Note: Update the resource manager to load models and store mesh data in same draw call in contiguous memory

class ResourceManager
{
public:

	static AD_ULONG AddPBRModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe = false);
	static AD_ULONG AddColliderBox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe = false);
	static AD_ULONG AddLight(ADResource::ADRenderer::Light& light);

	static AD_ULONG AddSpyro(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static void AddSkybox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);

	// Utils
	static AD_ULONG GenerateUniqueID();

	static AD_ULONG InitializePBRModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, ADUtils::SHADER& shader);

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
	static ADResource::ADRenderer::Model* GetSkybox();
	static ADResource::ADRenderer::Light* GetLightPtr();
	static int GetPBRModelCount();
	static int GetPBRVertexCount();

	static int GetGameObjectCount();
	static ADResource::ADGameplay::GameObject** GetGameObjectPtr();

	static ADResource::ADRenderer::Model* GetModelPtrFromMeshId(AD_ULONG mesh_id);

private:
	static AD_ULONG current_id;
};

// Note: Update uuid to use long long