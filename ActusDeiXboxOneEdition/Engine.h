#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fstream>

#include "Types.h"

using namespace DirectX;
using namespace Microsoft::WRL;

using namespace ADResource::ADRenderer;

class Engine
{
public:
	Engine() = default;
	~Engine() = default;

	bool Initialize();
	bool Update();
	bool Render();
	bool ShutDown();

public:
	ResourceManager* GetResourceManager();
	void SetCamera(XMFLOAT3 position, float yaw, float pitch, float fov);

private:
	ADResource::ADRenderer::PBRRenderer* GetPBRRenderer();

private:
	Model m_model;
	WVP WORLD;
	std::string write_path;
	std::string read_path;

	std::vector<Light> lights;

	FPSCamera* camera;

private:
	// New shit
	ADResource::ADRenderer::PBRRenderer pbr;
	ResourceManager rmanager;
};