#include "pch.h"
#include "Engine.h"

#include "DDSTextureLoader.h"

#include "Utils.h"

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

	// Start timer
	engine_time = XTime();
	engine_time.Restart();

	userInterface->Initialize(pbr.GetPBRRendererResources()->device.Get(), pbr.GetPBRRendererResources()->context.Get());

	return true;
}

bool Engine::Update()
{
	// Tick timer
	engine_time.Signal();
	delta_time_sd = engine_time.SmoothDelta();
	delta_time_sf = static_cast<float>(delta_time_sd);

	if (userInterface->GetUIState() == 0)
	{

		// For each game object, call update
		int OBJ_COUNT =  ResourceManager::GetGameObjectCount();
		ADResource::ADGameplay::GameObject** OBJS = ResourceManager::GetGameObjectPtr();

		for (int i = 0; i < OBJ_COUNT; i++)
		{
			OBJS[i]->Update(delta_time_sf);
		}

		//pbr.Update(camera, ocamera); //  Needs error checking
	}
	else if (userInterface->GetUIState() == 2)
	{
		return false;
	}

	// Move the light
	/*ResourceManager::GetLightPtr()[1].position.x += .1 * lightdir;
	if (fabs(ResourceManager::GetLightPtr()[1].position.x) > 10)
		lightdir *= -1;*/

	userInterface->Update(delta_time_sf);
	return true;
}

bool Engine::Render()
{
	// Loop over all game objects with a mesh and render
	int OBJ_COUNT = ResourceManager::GetGameObjectCount();
	ADResource::ADGameplay::GameObject** OBJS = ResourceManager::GetGameObjectPtr();
	for (int i = 0; i < OBJ_COUNT; i++)
	{
		OBJS[i]->Render();
	}

	pbr.Render(camera, ocamera);
	userInterface->Render(pbr.GetPBRRendererResources()->context.Get(), pbr.GetPBRRendererResources()->render_target_view.Get());
	pbr.Frame();

	return true;
}

bool Engine::ShutDown()
{
	pbr.ShutDown();
	userInterface->ShutDown();
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

	ocamera = new OrbitCamera;
	ocamera->Rotate(yaw, pitch);
	ocamera->SetFOV(fov);
}


DirectX::XMFLOAT3 Engine::GetCameraPosition()
{
	return camera->GetPosition();
}


FPSCamera* Engine::GetCamera()
{
	return camera;
}


OrbitCamera* Engine::GetOrbitCamera()
{
	return ocamera;
}

void Engine::MoveCamera(XMFLOAT3 move_direction)
{
	camera->Move(move_direction);
}


void Engine::RotateCamera(float yaw, float pitch)
{
	camera->Rotate(yaw, pitch);
}

void Engine::SetupUserInterface(AD_UI::ADUI* _uiSetup)
{
	userInterface = _uiSetup;
}
//
//AD_UI::ADUI* Engine::GetUserInterface()
//{
//	return nullptr;
//}

ADResource::ADRenderer::PBRRenderer* Engine::GetPBRRenderer()
{
	return &pbr;
}