#pragma once

#include <string>
#include <fstream>

#include "Types.h"
#include "Camera.h"
#include "ResourceManager.h"

using namespace ADResource::ADRenderer;

namespace
{
	PBRRendererResources pbr;
}

namespace ADResource
{
	namespace ADRenderer
	{
		class PBRRenderer
		{
		public:
			PBRRenderer() = default;
			~PBRRenderer() = default;

			// Lifecycle Methods
			bool Initialize();
			bool Update(FPSCamera* camera);
			bool Frame();
			bool ShutDown();

		public:
			static PBRRendererResources* GetPBRRendererResources();

			// Temp
		private:
			//Model m_model;
			WVP WORLD;
		};
	};
}