#pragma once

#include "Types.h"

#include <unordered_map>
#include <string>
#include <vector>

namespace 
{
	// Maps
	std::unordered_map<AD_ULONG, unsigned int> light_map;
	std::unordered_map<AD_ULONG, unsigned int> pbrmodel_map;

	std::unordered_map<AD_ULONG, unsigned int> gameObject;

	std::vector<ADResource::ADRenderer::Light> lights;
	std::vector<ADResource::ADRenderer::Model> pbrmodels;

	std::vector<ADResource::ADGameplay::GameObject> passables;
	std::vector<ADResource::ADGameplay::GameObject> collidables;
}

// Note: Update the resource manager to load models and store mesh data in same draw call in contiguous memory

class ResourceManager
{
public:
	ResourceManager() = default;
	~ResourceManager() = default;

	AD_ULONG AddPBRModel(std::string modelname);
	AD_ULONG AddLight(ADResource::ADRenderer::Light& light);

	// Utils
	AD_ULONG GenerateUniqueID();

public:
	// Getters/ setters
	static int GetLightCount();
	static char* GetLightDataPtr();
	
	static char* GetPBRDataPtr();
	static ADResource::ADRenderer::Model* GetPBRPtr();
	static int GetPBRVertexCount();


private:
	AD_ULONG current_id = 0;
};

// Note: Update uuid to use long long