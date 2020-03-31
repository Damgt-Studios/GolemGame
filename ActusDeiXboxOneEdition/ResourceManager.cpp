#include "pch.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Utils.h"

AD_ULONG ResourceManager::AddPBRModel(std::string modelname)
{
	ADResource::ADRenderer::Model temp;
	ADUtils::LoadWobjectMesh(modelname.c_str(), temp, ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()->device);
	temp.position = XMFLOAT3(0, 0, 0);

	// grab id and add stuff
	AD_ULONG id = GenerateUniqueID();
	unsigned int index = pbrmodels.size();
	pbrmodel_map.insert(std::pair<AD_ULONG, unsigned int>(id, index));
	pbrmodels.push_back(temp);
	
	return id;
}

AD_ULONG ResourceManager::AddLight(ADResource::ADRenderer::Light& light)
{
	ADResource::ADRenderer::Light temp;
	memcpy(&temp, &light, sizeof(ADResource::ADRenderer::Light));

	// Generate id and add that shit
	AD_ULONG id = GenerateUniqueID();
	unsigned int index = lights.size();
	light_map.insert(std::pair<AD_ULONG, unsigned int>(id, index));
	lights.push_back(temp);

	return id;
}

AD_ULONG ResourceManager::GenerateUniqueID()
{
	return current_id++;
}

int ResourceManager::GetLightCount()
{
	return lights.size();
}

char* ResourceManager::GetLightDataPtr()
{
	return (char*)lights.data();
}

char* ResourceManager::GetPBRDataPtr()
{
	return (char*)pbrmodels.data();
}


ADResource::ADRenderer::Model* ResourceManager::GetPBRPtr()
{
	return pbrmodels.data();
}

int ResourceManager::GetPBRVertexCount()
{
	int count = 0;

	for (int i = 0; i < pbrmodels.size(); i++)
	{
		count += pbrmodels[i].vertices.size();
	}

	return count;
}
