#include "pch.h"
#include "Engine.h"

#include "DDSTextureLoader.h"

#include "Utils.h"

// Shaders
#include "base_vs.csh"
#include "base_ps.csh"

Engine::Engine()
{
	// Don't delete
	ADUtils::InitializeFileSystem();
	// Don't delete
}

// Shaders

bool Engine::Initialize()
{
	// Initialize PBR Renderer
	pbr.Initialize(); // Needs error checking

	// Initialize unified buffers
	ResourceManager::ConfigureUnifiedBuffers(pbr.GetPBRRendererResources()->device);

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


DirectX::XMFLOAT3 Engine::GetCameraPosition()
{
	return camera->GetPosition();
}

void Engine::MoveCamera(XMFLOAT3 move_direction)
{
	camera->Move(move_direction);
}


void Engine::RotateCamera(float yaw, float pitch)
{
	camera->Rotate(yaw, pitch);
}

ADResource::ADRenderer::PBRRenderer* Engine::GetPBRRenderer()
{
	return &pbr;
}