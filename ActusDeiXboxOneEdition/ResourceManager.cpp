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

AD_ULONG ResourceManager::AddModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe) {
	ADUtils::SHADER shader = { 0 };

	if (!wireframe)
	{
		strcpy_s(shader.vshader, "files\\shaders\\simple_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\simple_ps.hlsl");
	}
	else
	{
		strcpy_s(shader.vshader, "files\\shaders\\debug_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\debug_ps.hlsl");
	}

	shader.wireframe = wireframe;

	return InitializeModel(modelname, position, scale, rotation, shader);
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

AD_ULONG ResourceManager::InitializeModel(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, ADUtils::SHADER& shader) 
{
	SimpleModel temp;
	SimpleMesh mesh;
	Load_FBX(modelname.c_str(), mesh);

	temp.vertices = mesh.vertexList;
	temp.indices = mesh.indicesList;

	temp.position = position;
	temp.scale = scale;
	temp.rotation = rotation;

	D3D11_BUFFER_DESC bdesc;
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&bdesc, sizeof(bdesc));
	ZeroMemory(&subData, sizeof(subData));

	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bdesc.ByteWidth = sizeof(SimpleVertex) * temp.vertices.size();
	bdesc.CPUAccessFlags = 0;
	bdesc.MiscFlags = 0;
	bdesc.StructureByteStride = 0;
	bdesc.Usage = D3D11_USAGE_IMMUTABLE;

	subData.pSysMem = temp.vertices.data();

	ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()->device->CreateBuffer(&bdesc, &subData, &temp.vertexBuffer);
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdesc.ByteWidth = sizeof(unsigned int) * temp.indices.size();

	subData.pSysMem = temp.indices.data();

	ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()->device->CreateBuffer(&bdesc, &subData, &temp.indexBuffer);

	// Load shaders // Thanks Whittington
	ComPtr<ID3D10Blob> vertexblob;
	ComPtr<ID3D10Blob> pixelblob;

	Platform::String^ appInstallFolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
	std::string READ_PATH = std::string(appInstallFolder->Begin(), appInstallFolder->End()).append("\\");

	std::string vname(shader.vshader);
	std::string pname(shader.pshader);

	std::string v = std::string(READ_PATH.begin(), READ_PATH.end()).append(vname);
	std::string p = std::string(READ_PATH.begin(), READ_PATH.end()).append(pname);

	//The Whittington Bruh aka The Wruh
	std::string bruh = std::string(READ_PATH.begin(), READ_PATH.end());

	std::wstring vshadername(v.begin(), v.end());
	std::wstring pshadername(p.begin(), p.end());

	HRESULT result;

	ComPtr<ID3D11Device1> device = ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()->device;

	result = D3DCompileFromFile(vshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_VS, D3DCOMPILE_DEBUG, 0, &vertexblob, nullptr);
	assert(!FAILED(result));
	result = D3DCompileFromFile(pshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_PS, D3DCOMPILE_DEBUG, 0, &pixelblob, nullptr);
	assert(!FAILED(result));

	result = device->CreateVertexShader(vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), nullptr, &temp.vertexShader);
	assert(!FAILED(result));
	result = device->CreatePixelShader(pixelblob->GetBufferPointer(), pixelblob->GetBufferSize(), nullptr, &temp.pixelShader);
	assert(!FAILED(result));

	// Make input layout for vertex buffer
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	/*{ "JOINTS", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},*/
	};

	result = device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), &temp.inputLayout);
	assert(!FAILED(result));

	D3D11_SAMPLER_DESC sdesc;
	ZeroMemory(&sdesc, sizeof(sdesc));
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sdesc.MaxLOD = D3D11_FLOAT32_MAX;
	sdesc.MinLOD = 0;

	result = device->CreateSamplerState(&sdesc, &temp.sampler);
	assert(!FAILED(result));

	// grab id and add stuff
	AD_ULONG id = GenerateUniqueID();
	unsigned int index = fbxmodels.size();
	fbxmodel_map.insert(std::pair<AD_ULONG, unsigned int>(id, index));
	fbxmodels.push_back(temp);

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

void ResourceManager::AddModelToRenderQueue(ADResource::ADGameplay::GameObject* obj)
{
	render_queue.push(obj);
}

bool ResourceManager::RenderQueueEmpty()
{
	return render_queue.empty();
}

ADResource::ADGameplay::GameObject* ResourceManager::PopFromRenderQueue()
{
	ADResource::ADGameplay::GameObject* temp = render_queue.front();
	render_queue.pop();

	return temp;
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

ADResource::ADRenderer::Light* ResourceManager::GetLightPtr()
{
	return lights.data();
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

int ResourceManager::GetGameObjectCount()
{
	return gameobjects.size();
}

ADResource::ADGameplay::GameObject** ResourceManager::GetGameObjectPtr()
{
	return gameobjects.data();
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

ADResource::ADRenderer::SimpleModel* ResourceManager::GetSimpleModelPtrFromMeshId(AD_ULONG mesh_id)
{
	SimpleModel* temp = nullptr;
	std::unordered_map<AD_ULONG, unsigned int>::const_iterator iter = fbxmodel_map.find(mesh_id);

	if (iter != fbxmodel_map.end())
	{
		// Found
		temp = &fbxmodels[iter->second];
	}

	return temp;
}