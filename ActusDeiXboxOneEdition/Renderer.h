#pragma once

#include <string>
#include <fstream>

#include "Types.h"
#include "Camera.h"

using namespace ADResource::ADRenderer;

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

		// Temp
	private:
		std::vector<Light> lights;
		Model m_model;
		WVP WORLD;

	private:
		PBRRendererResources pbr;
	};
};