#include "pch.h"
#include "ResourceManager.h"
#include "Renderer.h"

// Static private members
AD_ULONG ResourceManager::current_id = 0;

AD_ULONG ResourceManager::AddPBRModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe)
{
	ADUtils::SHADER shader = { 0 };

	if (!wireframe)
	{
		strcpy_s(shader.vshader, "files\\shaders\\base_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\base_ps.hlsl");
	}
	else
	{
		strcpy_s(shader.vshader, "files\\shaders\\debug_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\debug_ps.hlsl");
	}
	
	shader.wireframe = wireframe;
	
	return InitializePBRModel(modelname, position, scale, rotation, shader);
}

AD_ULONG ResourceManager::AddColliderBox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe /*= false*/)
{
	ADUtils::SHADER shader = { 0 };

	strcpy_s(shader.vshader, "files\\shaders\\pink_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\pink_ps.hlsl");

	shader.wireframe = wireframe;

	return InitializePBRModel(modelname, position, scale, rotation, shader);
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


AD_ULONG ResourceManager::AddSpyro(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADUtils::SHADER shader = { 0 };
	strcpy_s(shader.vshader, "files\\shaders\\spyro_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\spyro_ps.hlsl");

	return InitializePBRModel(modelname, position, scale, rotation, shader);
}


void ResourceManager::AddSkybox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADUtils::SHADER shader = { 0 };
	strcpy_s(shader.vshader, "files\\shaders\\skybox_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\skybox_ps.hlsl");

	ADUtils::LoadWobjectMesh(modelname.c_str(), skybox, ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()->device, shader);
	skybox.position = position;
	skybox.scale = scale;
	skybox.rotation = rotation;
}

AD_ULONG ResourceManager::GenerateUniqueID()
{
	return ResourceManager::current_id++;
}

AD_ULONG ResourceManager::InitializePBRModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, ADUtils::SHADER& shader)
{
	ADResource::ADRenderer::Model temp;
	ADUtils::LoadWobjectMesh(modelname.c_str(), temp, ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()->device, shader);
	temp.position = position;
	temp.scale = scale;
	temp.rotation = rotation;

	// Eh
	// Add the stuff to the PBR vertex data
	int pre_resize_vertex_end_index = pbrVertexData.size();
	pbrVertexData.resize(pbrVertexData.size() + temp.vertices.size());
	memcpy((char*)&pbrVertexData[pre_resize_vertex_end_index], temp.vertices.data(), sizeof(Vertex) * temp.vertices.size());

	// Add stuff into the PBR index data
	int pre_resize_index_end = pbrIndxData.size();
	pbrIndxData.resize(pbrIndxData.size() + temp.indices.size());
	memcpy((char*)&pbrIndxData[pre_resize_index_end], temp.indices.data(), sizeof(unsigned int) * temp.indices.size());

	ADResource::ADRenderer::PBRVertexBufferDesc desc = { 0 };
	desc.base_vertex_location = pre_resize_vertex_end_index;
	desc.index_count = pbrIndxData.size() - pre_resize_index_end;
	desc.vertex_count = pbrVertexData.size() - pre_resize_vertex_end_index;
	desc.index_start = pre_resize_index_end;

	temp.desc = desc;
	temp.desc.wireframe_mode = shader.wireframe;
	// Eh

	// grab id and add stuff
	AD_ULONG id = GenerateUniqueID();
	unsigned int index = pbrmodels.size();
	pbrmodel_map.insert(std::pair<AD_ULONG, unsigned int>(id, index));
	pbrmodels.push_back(temp);

	return id;
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
	bufferDesc.ByteWidth = sizeof(unsigned int) * pbrIndxData.size();

	D3D11_SUBRESOURCE_DATA idata;
	ZeroMemory(&idata, sizeof(D3D11_SUBRESOURCE_DATA));
	idata.pSysMem = pbrIndxData.data();
	result = device->CreateBuffer(&bufferDesc, &idata, &indexBuffer);
	assert(!FAILED(result));
}


Microsoft::WRL::ComPtr<ID3D11Buffer> ResourceManager::GetVertexBuffer()
{
	return vertexBuffer;
}


Microsoft::WRL::ComPtr<ID3D11Buffer> ResourceManager::GetIndexBuffer()
{
	return indexBuffer;
}

int ResourceManager::AddGameObject(ADResource::ADGameplay::GameObject* OBJ)
{
	int temp = gameobjects.size();

	gameobjects.push_back(OBJ);

	return temp;
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

ADResource::ADRenderer::Model* ResourceManager::GetSkybox()
{
	return &skybox;
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

ADResource::ADRenderer::Model* ResourceManager::GetModelPtrFromMeshId(AD_ULONG mesh_id)
{
	Model* temp = nullptr;
	std::unordered_map<AD_ULONG, unsigned int>::const_iterator iter = pbrmodel_map.find(mesh_id);

	if (iter != pbrmodel_map.end())
	{
		// Found
		temp = &pbrmodels[iter->second];
	}

	return temp;
}