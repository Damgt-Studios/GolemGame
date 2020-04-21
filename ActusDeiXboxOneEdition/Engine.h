#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "Input.h"
#include "ResourceManager.h"
#include "XTime.h"

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fstream>

#include "Types.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Gaming::Input;

using namespace ADResource::ADRenderer;

class Engine
{
public:
	Engine();
	~Engine() = default;

	bool Initialize();
	bool Update();
	bool Render();
	bool ShutDown();

public:
	ResourceManager* GetResourceManager();
	void SetCamera(XMFLOAT3 position, float yaw, float pitch, float fov);
	XMFLOAT3 GetCameraPosition();
	FPSCamera* GetCamera();
	OrbitCamera* GetOrbitCamera();
	void MoveCamera(XMFLOAT3 move_direction);
	void RotateCamera(float yaw, float pitch);

private:
	ADResource::ADRenderer::PBRRenderer* GetPBRRenderer();

private:
	Model m_model;
	WVP WORLD;
	std::string write_path;
	std::string read_path;

	std::vector<Light> lights;

	FPSCamera* camera;
	OrbitCamera* ocamera;

private:
	// New shit
	ADResource::ADRenderer::PBRRenderer pbr;
	ResourceManager rmanager;

	// Timing
	XTime engine_time;
	float delta_time_sf;
	double delta_time_sd;

	// Temp
	int lightdir = 1;
};