#include "pch.h"
#include "Utils.h"

#include "DDSTextureLoader.h"

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

void ADUtils::LoadWobjectMesh(const char* meshname, Model& _model, ComPtr<ID3D11Device1> device)
{
	Header header;

	std::string smeshname = meshname;

	// Break if fbx
	assert(smeshname.find(".fbx") == std::string::npos);
	// Break if not wobj
	assert(smeshname.find(".wobj") != std::string::npos);

	std::ifstream file;
	file.open(meshname, std::ios::binary | std::ios::in);
	assert(file.is_open());

	// Read in the header
	file.read((char*)&header, sizeof(Header));
	std::string s = header.t_albedo;

	// Create a buffer to hold the data
	char* buffer = new char[header.vertexcount * (size_t)sizeof(Vertex)];

	// Read in the indices
	file.read((char*)buffer, header.indexcount * (size_t)sizeof(int));
	_model.indices.resize(header.indexcount);
	memcpy(_model.indices.data(), buffer, sizeof(int) * header.indexcount);

	// Read in the vertices
	file.read((char*)buffer, header.vertexcount * (size_t)sizeof(Vertex));
	_model.vertices.resize(header.vertexcount);
	memcpy(_model.vertices.data(), buffer, sizeof(Vertex) * header.vertexcount);

	// Free memory
	delete[] buffer;

	file.close();

	// Load textures
	LoadTextures(header, _model, device);
}

void ADUtils::LoadTextures(Header& header, Model& _model, ComPtr<ID3D11Device1> dev)
{
	HRESULT result;

	ComPtr<ID3D11Device> device;
	result = dev.As(&device);
	assert(!FAILED(result));

	std::string texture_path = READ_PATH.append("files\\textures\\");

	// Construct wide string with filename
	std::string spath = header.t_albedo;
	spath = std::string(texture_path).append(spath);
	std::wstring wpath = std::wstring(spath.begin(), spath.end());

	// Load the albedo texture
	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.albedo);
	assert(!FAILED(result));

	// Normal
	spath = header.t_normal;
	spath = std::string(texture_path).append(spath);
	wpath = std::wstring(spath.begin(), spath.end());
	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.normal);
	assert(!FAILED(result));

	// Metallic
	spath = header.t_metallic;
	spath = std::string(texture_path).append(spath);
	wpath = std::wstring(spath.begin(), spath.end());
	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.metallic);
	assert(!FAILED(result));

	// Roughness
	spath = header.t_roughness;
	spath = std::string(texture_path).append(spath);
	wpath = std::wstring(spath.begin(), spath.end());
	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.roughness);
	assert(!FAILED(result));

	// AO
	spath = header.t_ambient_occlusion;
	spath = std::string(texture_path).append(spath);
	wpath = std::wstring(spath.begin(), spath.end());
	result = CreateDDSTextureFromFile(device.Get(), wpath.c_str(), nullptr, &_model.ambient_occlusion);
	assert(!FAILED(result));

	// Create texture sample state
	D3D11_SAMPLER_DESC sdesc;
	ZeroMemory(&sdesc, sizeof(D3D11_SAMPLER_DESC));
	sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sdesc.MinLOD = 0;
	sdesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the sampler state
	result = device->CreateSamplerState(&sdesc, &_model.sampler);
	assert(!FAILED(result));
}