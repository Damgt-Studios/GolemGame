#include "pch.h"

#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

#include "Utils.h"


//************************************
// Returns:   void
// Desc:      Initializes the READ_PATH & WRITE_PATH variables needed for filesystem to function
//************************************
void ADUtils::InitializeFileSystem()
{
	Platform::String^ appInstallFolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
	READ_PATH = std::string(appInstallFolder->Begin(), appInstallFolder->End()).append("\\");

	Platform::String^ localfolder = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
	WRITE_PATH = std::string(localfolder->Begin(), localfolder->End()).append("\\");
}

//
//void ADUtils::LoadWobjectMesh(const char* meshname, Model& _model, ComPtr<ID3D11Device1> device, SHADER& shader)
//{
//	Header header;
//
//	std::string smeshname = meshname;
//
//	// Break if fbx
//	assert(smeshname.find(".fbx") == std::string::npos);
//	// Break if not wobj
//	assert(smeshname.find(".wobj") != std::string::npos);
//
//	std::ifstream file;
//	file.open(meshname, std::ios::binary | std::ios::in);
//	assert(file.is_open());
//
//	// Read in the header
//	file.read((char*)&header, sizeof(Header));
//	std::string s = header.t_albedo;
//
//	// Create a buffer to hold the data
//	char* buffer = new char[header.vertexcount * (size_t)sizeof(Vertex)];
//
//	// Read in the indices
//	file.read((char*)buffer, header.indexcount * (size_t)sizeof(int));
//	_model.indices.resize(header.indexcount);
//	memcpy(_model.indices.data(), buffer, sizeof(int) * header.indexcount);
//
//	// Read in the vertices
//	file.read((char*)buffer, header.vertexcount * (size_t)sizeof(Vertex));
//	_model.vertices.resize(header.vertexcount);
//	memcpy(_model.vertices.data(), buffer, sizeof(Vertex) * header.vertexcount);
//
//	// Free memory
//	delete[] buffer;
//
//	file.close();
//
//	// Load textures
//	LoadTextures(header, _model, device);
//
//	// Create buffers
//	// Setup Vertex Buffer
//	D3D11_BUFFER_DESC bufferDesc;
//	D3D11_SUBRESOURCE_DATA vdata;
//	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
//	ZeroMemory(&vdata, sizeof(D3D11_SUBRESOURCE_DATA));
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.ByteWidth = sizeof(Vertex) * _model.vertices.size();
//	bufferDesc.CPUAccessFlags = 0;
//	bufferDesc.MiscFlags = 0;
//	bufferDesc.StructureByteStride = 0;
//	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
//	vdata.pSysMem = _model.vertices.data();
//
//	HRESULT result = device->CreateBuffer(&bufferDesc, &vdata, &_model.vertexBuffer);
//	assert(!FAILED(result));
//
//	// Index buffer
//	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bufferDesc.ByteWidth = sizeof(int) * _model.indices.size();
//
//	D3D11_SUBRESOURCE_DATA idata;
//	ZeroMemory(&idata, sizeof(D3D11_SUBRESOURCE_DATA));
//	idata.pSysMem = _model.indices.data();
//	result = device->CreateBuffer(&bufferDesc, &idata, &_model.indexBuffer);
//	assert(!FAILED(result));
//
//	// Load shaders
//	// Load shaders
//	ComPtr<ID3D10Blob> vertexblob;
//	ComPtr<ID3D10Blob> pixelblob;
//
//	Platform::String^ appInstallFolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
//	std::string READ_PATH = std::string(appInstallFolder->Begin(), appInstallFolder->End()).append("\\");
//
//	std::string vname(shader.vshader);
//	std::string pname(shader.pshader);
//
//	std::string v = std::string(READ_PATH.begin(), READ_PATH.end()).append(vname);
//	std::string p = std::string(READ_PATH.begin(), READ_PATH.end()).append(pname);
//
//	std::string bruh = std::string(READ_PATH.begin(), READ_PATH.end());
//
//	std::wstring vshadername(v.begin(), v.end());
//	std::wstring pshadername(p.begin(), p.end());
//
//	result = D3DCompileFromFile(vshadername.c_str(), NULL, NULL, SHADER_ENTRY_POINT, SHADER_MODEL_VS, D3DCOMPILE_DEBUG, 0, &vertexblob, nullptr);
//	assert(!FAILED(result));
//	result = D3DCompileFromFile(pshadername.c_str(), NULL, NULL, SHADER_ENTRY_POINT, SHADER_MODEL_PS, D3DCOMPILE_DEBUG, 0, &pixelblob, nullptr);
//	assert(!FAILED(result));
//
//	result = device->CreateVertexShader(vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), nullptr, &_model.vertexShader);
//	assert(!FAILED(result));
//	result = device->CreatePixelShader(pixelblob->GetBufferPointer(), pixelblob->GetBufferSize(), nullptr, &_model.pixelShader);
//	assert(!FAILED(result));
//
//	// Make input layout for vertex buffer
//	D3D11_INPUT_ELEMENT_DESC tempInputElementDesc[] =
//	{
//		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
//	};
//
//	result = device->CreateInputLayout(tempInputElementDesc, ARRAYSIZE(tempInputElementDesc), vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), &_model.vertexBufferLayout);
//	assert(!FAILED(result));
//}


void ADUtils::LoadStaticMesh(const char* modelname, SimpleStaticModel& model, ComPtr<ID3D11Device1> device, SHADER& shader)
{
	SimpleMesh mesh;
	Load_Mesh(modelname, mesh);

	model.vertices = mesh.vertexList;
	model.indices = mesh.indicesList;

	D3D11_BUFFER_DESC bdesc;
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&bdesc, sizeof(bdesc));
	ZeroMemory(&subData, sizeof(subData));

	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bdesc.ByteWidth = sizeof(SimpleVertex) * model.vertices.size();
	bdesc.CPUAccessFlags = 0;
	bdesc.MiscFlags = 0;
	bdesc.StructureByteStride = 0;
	bdesc.Usage = D3D11_USAGE_IMMUTABLE;

	subData.pSysMem = model.vertices.data();

	device->CreateBuffer(&bdesc, &subData, &model.vertexBuffer);
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdesc.ByteWidth = sizeof(unsigned int) * model.indices.size();

	subData.pSysMem = model.indices.data();

	device->CreateBuffer(&bdesc, &subData, &model.indexBuffer);

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

	result = D3DCompileFromFile(vshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_VS, D3DCOMPILE_DEBUG, 0, &vertexblob, nullptr);
	assert(!FAILED(result));
	result = D3DCompileFromFile(pshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_PS, D3DCOMPILE_DEBUG, 0, &pixelblob, nullptr);
	assert(!FAILED(result));

	result = device->CreateVertexShader(vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), nullptr, &model.vertexShader);
	assert(!FAILED(result));
	result = device->CreatePixelShader(pixelblob->GetBufferPointer(), pixelblob->GetBufferSize(), nullptr, &model.pixelShader);
	assert(!FAILED(result));

	// Make input layout for vertex buffer
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD",	1, DXGI_FORMAT_R32G32B32_FLOAT,		1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		/*{ "JOINTS",		0, DXGI_FORMAT_R32G32B32A32_SINT,	0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "WEIGHTS",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0},*/
	};

	result = device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), &model.inputLayout);
	assert(!FAILED(result));

	D3D11_SAMPLER_DESC sdesc;
	ZeroMemory(&sdesc, sizeof(sdesc));
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sdesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	sdesc.MipLODBias = 0.0f;
	sdesc.MaxLOD = D3D11_FLOAT32_MAX;
	sdesc.MinLOD = 0;

	//D3D11_SAMPLER_DESC sdesc;
	//ZeroMemory(&sdesc, sizeof(sdesc));
	//sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	////sdesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	//sdesc.MipLODBias = 0.0f;
	//sdesc.MaxLOD = D3D11_FLOAT32_MAX;
	//sdesc.MinLOD = 0;

	result = device->CreateSamplerState(&sdesc, &model.sampler);
	assert(!FAILED(result));

}

void ADUtils::LoadAnimatedMesh(const char* modelname, SimpleAnimModel& model, std::vector<std::string> animations, ComPtr<ID3D11Device1> device, SHADER& shader)
{
	SimpleMeshAnim mesh;
	Load_AnimMesh(modelname, mesh);

	model.animations.resize(animations.size());

	for (unsigned int i = 0; i < animations.size(); i++)
	{
		Load_AnimFile(animations[i].c_str(), model.skeleton, model.inverse_transforms, model.animations[i]);
	}

	model.vertices = mesh.vertexList;
	model.indices = mesh.indicesList;

	model.animated = true;

	D3D11_BUFFER_DESC bdesc;
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&bdesc, sizeof(bdesc));
	ZeroMemory(&subData, sizeof(subData));

	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bdesc.ByteWidth = sizeof(SimpleVertexAnim) * model.vertices.size();
	bdesc.CPUAccessFlags = 0;
	bdesc.MiscFlags = 0;
	bdesc.StructureByteStride = 0;
	bdesc.Usage = D3D11_USAGE_IMMUTABLE;

	subData.pSysMem = model.vertices.data();

	device->CreateBuffer(&bdesc, &subData, &model.vertexBuffer);
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdesc.ByteWidth = sizeof(unsigned int) * model.indices.size();

	subData.pSysMem = model.indices.data();

	device->CreateBuffer(&bdesc, &subData, &model.indexBuffer);

	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdesc.ByteWidth = sizeof(XMMATRIX) * model.skeleton.size();
	bdesc.Usage = D3D11_USAGE_DEFAULT;

	device->CreateBuffer(&bdesc, nullptr, &model.animationBuffer);

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

	result = D3DCompileFromFile(vshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_VS, D3DCOMPILE_DEBUG, 0, &vertexblob, nullptr);
	assert(!FAILED(result));
	result = D3DCompileFromFile(pshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_PS, D3DCOMPILE_DEBUG, 0, &pixelblob, nullptr);
	assert(!FAILED(result));

	result = device->CreateVertexShader(vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), nullptr, &model.vertexShader);
	assert(!FAILED(result));
	result = device->CreatePixelShader(pixelblob->GetBufferPointer(), pixelblob->GetBufferSize(), nullptr, &model.pixelShader);
	assert(!FAILED(result));

	// Make input layout for vertex buffer
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "JOINTS",		0, DXGI_FORMAT_R32G32B32A32_SINT,	0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "WEIGHTS",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	result = device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), &model.inputLayout);
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

	result = device->CreateSamplerState(&sdesc, &model.sampler);
	assert(!FAILED(result));

}

//void ADUtils::LoadTextures(Header& header, Model& _model, ComPtr<ID3D11Device1> dev)
//{
//	HRESULT result;
//
//	ComPtr<ID3D11Device> device;
//	result = dev.As(&device);
//	assert(!FAILED(result));
//
//	std::string texture_path = std::string(READ_PATH.begin(), READ_PATH.end()).append("files\\textures\\");
//
//	// Construct wide string with filename
//	std::string spath = header.t_albedo;
//	spath = std::string(texture_path).append(spath);
//	std::wstring wpath = std::wstring(spath.begin(), spath.end());
//
//	// Load the albedo texture
//	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.albedo);
//	assert(!FAILED(result));
//
//	// Normal
//	spath = header.t_normal;
//	spath = std::string(texture_path).append(spath);
//	wpath = std::wstring(spath.begin(), spath.end());
//	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.normal);
//	assert(!FAILED(result));
//
//	// Metallic
//	spath = header.t_metallic;
//	spath = std::string(texture_path).append(spath);
//	wpath = std::wstring(spath.begin(), spath.end());
//	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.metallic);
//	assert(!FAILED(result));
//
//	// Roughness
//	spath = header.t_roughness;
//	spath = std::string(texture_path).append(spath);
//	wpath = std::wstring(spath.begin(), spath.end());
//	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.roughness);
//	assert(!FAILED(result));
//
//	// AO
//	spath = header.t_ambient_occlusion;
//	spath = std::string(texture_path).append(spath);
//	wpath = std::wstring(spath.begin(), spath.end());
//	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.ambient_occlusion);
//	assert(!FAILED(result));
//
//	// Create texture sample state
//	D3D11_SAMPLER_DESC sdesc;
//	ZeroMemory(&sdesc, sizeof(D3D11_SAMPLER_DESC));
//	sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	sdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sdesc.MinLOD = 0;
//	sdesc.MaxLOD = D3D11_FLOAT32_MAX;
//	// Create the sampler state
//	result = device->CreateSamplerState(&sdesc, &_model.sampler);
//	assert(!FAILED(result));
//}

void ADUtils::LoadTextures(std::string filepath, SimpleModel* model, ComPtr<ID3D11Device1> device, std::unordered_map<std::string, ADTexture>& textures)
{
	if (filepath == "")
		return;

	std::ifstream file(filepath, std::ios::binary);

	assert(file.is_open());

	uint32_t count;
	using file_path_t = std::array<char, 260>;
	std::vector<file_path_t> mats;

	file.read((char*)&count, sizeof(uint32_t));

	mats.resize(count);

	file.read((char*)&mats[0], sizeof(file_path_t) * count);

	file.close();

	std::string texture_Path = std::string(ADUtils::READ_PATH.begin(), ADUtils::READ_PATH.end()).append("files\\textures\\");

	if (mats.size() == 3)
	{
		//Diffuse
		std::string textureName = std::string(texture_Path).append((char*)&mats[0]);
		std::wstring wTextureName = std::wstring(textureName.begin(), textureName.end());

		auto it = textures.find(textureName);
		if (it != textures.end())
			model->albedo = &it->second;
		else
		{
			ADTexture* tex = new ADTexture();
			CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &tex->texture);
			textures[textureName] = *tex;
			delete tex;
			model->albedo = &textures[textureName];
		}
		//CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		//	0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &model->albedo);

		//Emissive
		textureName = std::string(texture_Path).append((char*)&mats[1]);
		wTextureName = std::wstring(textureName.begin(), textureName.end());

		it = textures.find(textureName);
		if (it != textures.end())
			model->emissive = &it->second;
		else
		{
			ADTexture* tex = new ADTexture();
			CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &tex->texture);
			textures[textureName] = *tex;
			delete tex;
			model->emissive = &textures[textureName];
		}

		//CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		//	0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &model->emissive);

		//Normal
		textureName = std::string(texture_Path).append((char*)&mats[2]);
		wTextureName = std::wstring(textureName.begin(), textureName.end());

		it = textures.find(textureName);
		if (it != textures.end())
			model->normal = &it->second;
		else
		{
			ADTexture* tex = new ADTexture();
			CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &tex->texture);
			textures[textureName] = *tex;
			delete tex;
			model->normal = &textures[textureName];
		}

		//CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		//	0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &model->normal);
	}
	else if (mats.size() == 2)
	{
		model->emissive = new ADTexture();
		model->emissive->shared = false;

		//Diffuse
		std::string textureName = std::string(texture_Path).append((char*)&mats[0]);
		std::wstring wTextureName = std::wstring(textureName.begin(), textureName.end());

		auto it = textures.find(textureName);
		if (it != textures.end())
			model->albedo = &it->second;
		else
		{
			ADTexture* tex = new ADTexture();
			CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &tex->texture);
			textures[textureName] = *tex;
			delete tex;
			model->albedo = &textures[textureName];
		}
		//CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		//	0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &model->albedo);

		//Normal
		textureName = std::string(texture_Path).append((char*)&mats[1]);
		wTextureName = std::wstring(textureName.begin(), textureName.end());

		it = textures.find(textureName);
		if (it != textures.end())
			model->normal = &it->second;
		else
		{
			ADTexture* tex = new ADTexture();
			CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &tex->texture);
			textures[textureName] = *tex;
			delete tex;
			model->normal = &textures[textureName];
		}
		//CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		//	0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &model->normal);
	}
	else
	{
		model->emissive = new ADTexture();
		model->emissive->shared = false;
		model->normal = new ADTexture();
		model->normal->shared = false;
		//Diffuse
		std::string textureName = std::string(texture_Path).append((char*)&mats[0]);
		std::wstring wTextureName = std::wstring(textureName.begin(), textureName.end());

		auto it = textures.find(textureName);
		if (it != textures.end())
			model->albedo = &it->second;
		else
		{
			ADTexture* tex = new ADTexture();
			CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &tex->texture);
			textures[textureName] = *tex;
			delete tex;
			model->albedo = &textures[textureName];
		}
		//CreateDDSTextureFromFileEx(device.Get(), wTextureName.c_str(), D3D10_FLOAT32_MAX, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		//	0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, nullptr, &model->albedo);
	}
}


void ADUtils::LoadUITextures(std::string _fileName, ID3D11ShaderResourceView** _srv, ComPtr<ID3D11Device1> _dev)
{
	HRESULT result;

	ComPtr<ID3D11Device> device;
	result = _dev.As(&device);
	assert(!FAILED(result));

	std::string texture_path = std::string(READ_PATH.begin(), READ_PATH.end()).append("files\\textures\\");

	// Construct wide string with filename
	std::string spath = _fileName;
	spath = std::string(texture_path).append(spath);
	std::wstring wpath = std::wstring(spath.begin(), spath.end());
	// Load the atlas texture
	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, _srv);

	assert(!FAILED(result));
}

std::wstring ADUtils::GetTexturePath(const char* textName)
{
	std::string texture_path = std::string(READ_PATH.begin(), READ_PATH.end()).append("files\\textures\\");

	std::string spath = textName;
	spath = std::string(texture_path).append(spath);
	std::wstring wpath = std::wstring(spath.begin(), spath.end());
	return wpath;

}
