#pragma once

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fstream>

#include "Camera.h"
#include "Renderer.h"
#include "Input.h"
#include "ResourceManager.h"
#include "XTime.h"

#include "Types.h"
#include "ADUserInterface.h"
#include "ADParticles.h"

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
	bool Update(float _delta);
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
	ADUI::ADUI* GetUI();
	ADResource::ADRenderer::PBRRenderer* GetPBRRenderer();
	float GetEngineDeltaTime();


private:
	//Model m_model;
	WVP WORLD;
	std::string write_path;
	std::string read_path;

#if AD_MEMORY_DEFAULT
	std::vector<Light> lights;
#else
	ADVector<Light> lights;
#endif

	FPSCamera* camera;
	OrbitCamera* ocamera;



private:
	// New shit
	ADResource::ADRenderer::PBRRenderer pbr;
	ResourceManager rmanager;
	ADUI::ADUI userInterface;

	// Timing
	XTime engine_time;
	float delta_time_sf;
	float engine_time_sf;
	double delta_time_sd;

	// Temp
	int lightdir = 1;
public:
	BigCloudEmitter bigCloud;
	HealthEmitter recoveryEmitter;
	SmallCloudEmitter smallCloud;
	WaveEmitter waterWave;
	IronSkinEmitter ironSkin;
	FireballEmitter fireball;
	BigElementalPuffEmitter bigWoodPuff;
	BigElementalPuffEmitter bigFirePuff;
	BigElementalPuffEmitter bigWaterPuff;
	BigElementalPuffEmitter bigStonePuff;
	CylinderEmitter woodCylinder;
	CylinderEmitter fireCylinder;
	CylinderEmitter waterCylinder;
	CylinderEmitter stoneCylinder;
	UpwardCloudEmitter destructionCloud;
	UpwardCloudEmitter destructionCloud2;
	BloodEmitter bloodEmitters[10];
};