#include "pch.h"
#include "Engine.h"

#include "DDSTextureLoader.h"

#include "Utils.h"

// Shaders
#include "base_vs.csh"
#include "base_ps.csh"
// Shaders

bool Engine::Initialize()
{
	// Don't delete
	ADUtils::InitializeFileSystem();
	// Don't delete

	// Model
	/*camera = new FPSCamera(XMFLOAT3(0, 0, -25));
	camera->Rotate(0, 15);
	camera->SetFOV(30);*/
	// Model

	// Lights
	Light light;
	ZeroMemory(&light, sizeof(Light));
	light.lightType = (int)LIGHTTYPE::DIRECTIONAL;
	light.ambientUp = XMFLOAT4(1, 1, 1, 1);
	light.ambientDown = XMFLOAT4(1, 1, 1, 1);
	light.ambientIntensityDown = .2;
	light.ambientIntensityUp = .4;
	light.diffuse = XMFLOAT4(1, 1, 1, 1);
	light.lightDirection = XMFLOAT4(2, -1, 3, 1);
	light.diffuseIntensity = 1;
	light.specular = XMFLOAT4(1, 1, 1, 1);
	light.specularIntensity = .2;
	lights.push_back(light);
	// Lights

	// Initialize PBR Renderer
	pbr.Initialize(); // Needs error checking

	return true;
}

bool Engine::Update()
{
	pbr.Update(camera); //  Needs error checking

	return true;
}

bool Engine::Render()
{
	pbr.Frame();

	return true;
}

bool Engine::ShutDown()
{
	pbr.ShutDown();

	return true;
}

ResourceManager* Engine::GetResourceManager()
{
	return &rmanager;
}


void Engine::SetCamera(XMFLOAT3 position, float yaw, float pitch, float fov)
{
	camera = new FPSCamera(position);
	camera->Rotate(yaw, pitch);
	camera->SetFOV(fov);
}

ADResource::ADRenderer::PBRRenderer* Engine::GetPBRRenderer()
{
	return &pbr;
}