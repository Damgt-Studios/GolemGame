#include "pch.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Utils.h"

AD_ULONG ResourceManager::AddPBRModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADRenderer::Model temp;
	ADUtils::LoadWobjectMesh(modelname.c_str(), temp, ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()->device);
	temp.position = position;
	temp.scale = scale;
	temp.rotation = rotation;

	// Eh
	// Add the stuff to the PBR vertex data
	int pre_resize_vertex_end_index = pbrVertexData.size();
	pbrVertexData.resize(pbrVertexData.size() + temp.vertices.size());
	memcpy((char*)&pbrVertexData[pre_resize_vertex_end_index], temp.vertices.data(), sizeof(Vertex) * temp.vertices.size());

	// Add stuff into the PBR index data
	int pre_resize_index_end = pbrIndexData.size();
	pbrIndexData.resize(pbrIndexData.size() + temp.indices.size());
	memcpy((char*)&pbrIndexData[pre_resize_index_end], temp.indices.data(), sizeof(unsigned int) * temp.indices.size());

	ADResource::ADRenderer::PBRVertexBufferDesc desc = { 0 };
	desc.base_vertex_location = pre_resize_vertex_end_index;
	desc.index_count = temp.indices.size();
	desc.vertex_count = temp.vertices.size();
	desc.index_start = pre_resize_index_end;

	temp.desc = desc;
	// Eh

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


void ResourceManager::ConfigureUnifiedBuffers(ComPtr<ID3D11Device1> device)
{
	// Setup Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA vdata;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&vdata, sizeof(D3D11_SUBRESOURCE_DATA));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(Vertex) * pbrVertexData.size();
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vdata.pSysMem = pbrVertexData.data();

	HRESULT result = device->CreateBuffer(&bufferDesc, &vdata, &vertexBuffer);
	assert(!FAILED(result));

	// Index buffer
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(int) * pbrIndexData.size();

	D3D11_SUBRESOURCE_DATA idata;
	ZeroMemory(&idata, sizeof(D3D11_SUBRESOURCE_DATA));
	idata.pSysMem = pbrIndexData.data();
	result = device->CreateBuffer(&bufferDesc, &idata, &indexBuffer);
	assert(!FAILED(result));
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


int ResourceManager::GetPBRModelCount()
{
	return pbrmodels.size();
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
