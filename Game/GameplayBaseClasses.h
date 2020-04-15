#pragma once
#include "Types.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Camera.h"

namespace ADResource
{
	namespace ADGameplay
	{
		class Renderable : public GameObject
		{
		public:
			void Render()
			{
				ResourceManager::AddModelToRenderQueue(dynamic_cast<GameObject*>(this));
			}
		};
	}
}