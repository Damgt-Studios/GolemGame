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
	//ResourceManager::ConfigureUnifiedBuffers(pbr.GetRendererResources()->device);
	
	// Start timer
	engine_time = XTime();
	engine_time.Restart();

	userInterface.Initialize(pbr.GetRendererResources()->device, pbr.GetRendererResources()->context, pbr.GetRendererResources()->render_target_view, &pbr.GetRendererResources()->viewport);
	bigCloud.Initialize(pbr.renderer_resources.device.Get(), 4000, { 0,25,25,1 }, L"files/textures/Particle_Dust.dds");
	recoveryEmitter.Initialize(pbr.renderer_resources.device.Get(), 300, { 0,25,25,1 }, 15.0f, L"files/textures/Particle_Health.dds");
	smallCloud.Initialize(pbr.renderer_resources.device.Get(), 4000, { 0,25,25,1 }, L"files/textures/Particle_Dust.dds");
	waterWave.Initialize(pbr.renderer_resources.device.Get(), 4000, { 0,25,25,1 }, L"files/textures/Particle_Water.dds");
	ironSkin.Initialize(pbr.renderer_resources.device.Get(), 4000, { 0,25,25,1 }, L"files/textures/Particle_Stone.dds");
	fireball.Initialize(pbr.renderer_resources.device.Get(), 4000, { 0,25,25,1 }, L"files/textures/Particle_Fireball.dds");
	bigWoodPuff.Initialize(pbr.renderer_resources.device.Get(), { 0,25,25,1 }, L"files/textures/Particle_ElementalPuff_Sheet.dds", { 0.06f, 0.57f, 0.18f, 1.0f });
	bigFirePuff.Initialize(pbr.renderer_resources.device.Get(), { 0,25,25,1 }, L"files/textures/Particle_ElementalPuff_Sheet.dds", { 0.9f, 0.45f, 0.2f, 1.0f });
	bigWaterPuff.Initialize(pbr.renderer_resources.device.Get(), { 0,25,25,1 }, L"files/textures/Particle_ElementalPuff_Sheet.dds", { 0.07f, 0.48f, 0.73f, 1.0f });
	bigStonePuff.Initialize(pbr.renderer_resources.device.Get(), { 0,25,25,1 }, L"files/textures/Particle_ElementalPuff_Sheet.dds", { 0.5f, 0.52f, 0.53f, 1.0f });
	woodCylinder.Initialize(pbr.renderer_resources.device.Get(), 100, { 0,25,25,1 }, 15.0f, L"files/textures/Particle_Grass.dds");
	fireCylinder.Initialize(pbr.renderer_resources.device.Get(), 100, { 0,25,25,1 }, 15.0f, L"files/textures/Particle_Fire_Cylinder.dds");
	waterCylinder.Initialize(pbr.renderer_resources.device.Get(), 100, { 0,25,25,1 }, 15.0f, L"files/textures/Particle_Water.dds");
	stoneCylinder.Initialize(pbr.renderer_resources.device.Get(), 100, { 0,25,25,1 }, 15.0f, L"files/textures/Particle_Stone.dds");
	destructionCloud.Initialize(pbr.renderer_resources.device.Get(), 4000, { 0,25,25,1 }, L"files/textures/Particle_Smoke.dds");
	destructionCloud2.Initialize(pbr.renderer_resources.device.Get(), 4000, { 0,25,25,1 }, L"files/textures/Particle_Smoke.dds");
	for (int i = 0; i < 10; ++i)
	{
		bloodEmitters[i].Initialize(pbr.renderer_resources.device.Get(), { 0,25,25,1 }, L"files/textures/Particle_Blood_Sheet.dds");
	}

	woodCylinder.Activate(15.0f, { 0,-1000,0,0 }, 15.0f);
	fireCylinder.Activate(15.0f, { 0,-1000,0,0 }, 15.0f);
	waterCylinder.Activate(15.0f, { 0,-1000,0,0 }, 15.0f);
	stoneCylinder.Activate(15.0f, { 0,-1000,0,0 }, 15.0f);
	recoveryEmitter.Activate(15.0f, { 0,-1000,0,0 }, 15.0f);

	return true;
}

bool Engine::Update(float _delta)
{
	// Tick timer
	engine_time.Signal();
	delta_time_sd = engine_time.SmoothDelta();
	delta_time_sf = static_cast<float>(delta_time_sd);

	if (*userInterface.GetUIState() == 0)
	{
		engine_time_sf = delta_time_sf;
		// For each game object, call update
		int OBJ_COUNT = ResourceManager::GetGameObjectCount();
		ADResource::ADGameplay::GameObject** OBJS = ResourceManager::GetGameObjectPtr();

		for (int i = 0; i < OBJ_COUNT; i++)
		{
			OBJS[i]->Update(engine_time_sf);


		}

		Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
		float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;
		XMFLOAT4X4 view;
		XMFLOAT4X4 proj;
		XMFLOAT4 camPos;
		XMMATRIX temp;
		ocamera->GetViewMatrix(temp);
		XMStoreFloat4x4(&view, temp);
		temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, 0.1f, 3000);
		XMStoreFloat4x4(&proj, temp);
		camPos = XMFLOAT4(ocamera->GetPosition().x, ocamera->GetPosition().y, ocamera->GetPosition().z, 1);
		bigCloud.UpdateParticles		(engine_time_sf, view, proj, camPos);
		recoveryEmitter.UpdateParticles	(engine_time_sf, view, proj, camPos);
		smallCloud.UpdateParticles		(engine_time_sf, view, proj, camPos);
		waterWave.UpdateParticles		(engine_time_sf, view, proj, camPos);
		ironSkin.UpdateParticles		(engine_time_sf, view, proj, camPos);
		fireball.UpdateParticles		(engine_time_sf, view, proj, camPos);
		bigWoodPuff.UpdateParticles		(engine_time_sf, view, proj, camPos);
		bigFirePuff.UpdateParticles		(engine_time_sf, view, proj, camPos);
		bigWaterPuff.UpdateParticles	(engine_time_sf, view, proj, camPos);
		bigStonePuff.UpdateParticles	(engine_time_sf, view, proj, camPos);
		woodCylinder.UpdateParticles	(engine_time_sf, view, proj, camPos);
		fireCylinder.UpdateParticles	(engine_time_sf, view, proj, camPos);
		waterCylinder.UpdateParticles	(engine_time_sf, view, proj, camPos);
		stoneCylinder.UpdateParticles	(engine_time_sf, view, proj, camPos);
		destructionCloud.UpdateParticles(engine_time_sf, view, proj, camPos);
		destructionCloud2.UpdateParticles(engine_time_sf, view, proj, camPos);

		//Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
		//float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;
		//XMFLOAT4X4 view;
		//XMFLOAT4X4 proj;
		//XMFLOAT4 camPos;
		//XMMATRIX temp;
		//ocamera->GetViewMatrix(temp);
		//XMStoreFloat4x4(&view, temp);
		//temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, 0.1f, 3000);
		//XMStoreFloat4x4(&proj, temp);
		//camPos = XMFLOAT4(ocamera->GetPosition().x, ocamera->GetPosition().y, ocamera->GetPosition().z, 1);
		//bigCloud.UpdateParticles(delta_time_sf, view, proj, camPos);
		//pbr.Update(camera, ocamera); //  Needs error checking
	}
	else if (*userInterface.GetUIState() == 2)
	{
		return false;
	}
	else
	{
		engine_time_sf = 0;
	}

	// Move the light
	/*ResourceManager::GetLightPtr()[1].position.x += .1 * lightdir;
	if (fabs(ResourceManager::GetLightPtr()[1].position.x) > 10)
		lightdir *= -1;*/

	userInterface.Update(_delta);
	/*Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4 camPos;
	XMMATRIX temp;
	ocamera->GetViewMatrix(temp);
	XMStoreFloat4x4(&view, temp);
	temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, 0.1f, 3000);
	XMStoreFloat4x4(&proj, temp);
	camPos = XMFLOAT4(ocamera->GetPosition().x, ocamera->GetPosition().y, ocamera->GetPosition().z, 1);
	bigCloud.UpdateParticles(engine_time.SmoothDelta(), view, proj, camPos);*/

	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4 camPos;
	XMMATRIX temp;
	ocamera->GetViewMatrix(temp);
	XMStoreFloat4x4(&view, temp);
	temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, 0.1f, 3000);
	XMStoreFloat4x4(&proj, temp);
	camPos = XMFLOAT4(ocamera->GetPosition().x, ocamera->GetPosition().y, ocamera->GetPosition().z, 1);
	bigCloud.UpdateParticles(delta_time_sf, view, proj, camPos);
	recoveryEmitter.UpdateParticles(delta_time_sf, view, proj, camPos);
	smallCloud.UpdateParticles(delta_time_sf, view, proj, camPos);
	waterWave.UpdateParticles(delta_time_sf, view, proj, camPos);
	ironSkin.UpdateParticles(delta_time_sf, view, proj, camPos);
	fireball.UpdateParticles(delta_time_sf, view, proj, camPos);
	bigWoodPuff.UpdateParticles(delta_time_sf, view, proj, camPos);
	bigFirePuff.UpdateParticles(delta_time_sf, view, proj, camPos);
	bigWaterPuff.UpdateParticles(delta_time_sf, view, proj, camPos);
	bigStonePuff.UpdateParticles(delta_time_sf, view, proj, camPos);
	woodCylinder.UpdateParticles(delta_time_sf, view, proj, camPos);
	fireCylinder.UpdateParticles(delta_time_sf, view, proj, camPos);
	waterCylinder.UpdateParticles(delta_time_sf, view, proj, camPos);
	stoneCylinder.UpdateParticles(delta_time_sf, view, proj, camPos);
	destructionCloud.UpdateParticles(delta_time_sf, view, proj, camPos);
	destructionCloud2.UpdateParticles(delta_time_sf, view, proj, camPos);
	for (int i = 0; i < 10; ++i)
	{
		bloodEmitters[i].UpdateParticles(delta_time_sf, view, proj, camPos);
	}

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

	pbr.Render(camera, ocamera, engine_time_sf);
	bigCloud.RenderParticles(pbr.renderer_resources.context.Get());
	recoveryEmitter.RenderParticles(pbr.renderer_resources.context.Get());
	smallCloud.RenderParticles(pbr.renderer_resources.context.Get());
	waterWave.RenderParticles(pbr.renderer_resources.context.Get());
	ironSkin.RenderParticles(pbr.renderer_resources.context.Get());
	fireball.RenderParticles(pbr.renderer_resources.context.Get());
	bigWoodPuff.RenderParticles(pbr.renderer_resources.context.Get());
	bigFirePuff.RenderParticles(pbr.renderer_resources.context.Get());
	bigWaterPuff.RenderParticles(pbr.renderer_resources.context.Get());
	bigStonePuff.RenderParticles(pbr.renderer_resources.context.Get());
	woodCylinder.RenderParticles(pbr.renderer_resources.context.Get());
	fireCylinder.RenderParticles(pbr.renderer_resources.context.Get());
	waterCylinder.RenderParticles(pbr.renderer_resources.context.Get());
	stoneCylinder.RenderParticles(pbr.renderer_resources.context.Get());
	destructionCloud.RenderParticles(pbr.renderer_resources.context.Get());
	destructionCloud2.RenderParticles(pbr.renderer_resources.context.Get());
	for (int i = 0; i < 10; ++i)
	{
		bloodEmitters[i].RenderParticles(pbr.renderer_resources.context.Get());
	}
	userInterface.Render();
	pbr.Frame();

	return true;
}

bool Engine::ShutDown()
{
	pbr.ShutDown();
	userInterface.Shutdown();
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

ADUI::ADUI* Engine::GetUI()
{
	return &userInterface;
}

ADResource::ADRenderer::PBRRenderer* Engine::GetPBRRenderer()
{
	return &pbr;
}

float Engine::GetEngineDeltaTime()
{
	return engine_time_sf;
}
