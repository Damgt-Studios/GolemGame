#pragma once

#include <string>
#include <fstream>

#include "Types.h"
#include "Camera.h"
#include "ResourceManager.h"

using namespace ADResource::ADRenderer;

namespace
{
	PBRRendererResources pbr_renderer_resources;
}

namespace ADResource
{
	namespace ADRenderer
	{
		class PBRRenderer
		{
		public:
			PBRRenderer();
			~PBRRenderer() = default;

			// Lifecycle Methods
			bool Initialize();
			bool Update(FPSCamera* camera, OrbitCamera* ocamera);
			bool Render(FPSCamera* camera, OrbitCamera* ocamera, float delta_time);
			bool Frame();
			bool ShutDown();

		public:
			static PBRRendererResources* GetPBRRendererResources();

			// Temp
		private:
			WVP WORLD;

			// Temp
			ComPtr<IDXGIDevice1> dxgiDevice;
		};
	};
}