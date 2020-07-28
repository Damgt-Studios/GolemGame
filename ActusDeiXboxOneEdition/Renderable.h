#pragma once
#include "Types.h"
#include "ResourceManager.h"

namespace ADResource
{
	namespace ADGameplay
	{
		class Renderable : public GameObject
		{
		public:
			void Render()
			{
				if (this->active && this->has_mesh) 
				{
					ResourceManager::AddModelToRenderQueue(dynamic_cast<GameObject*>(this));
					ResourceManager::AddModelToShadowQueue(dynamic_cast<GameObject*>(this));
				}
			}
		};

	}
}