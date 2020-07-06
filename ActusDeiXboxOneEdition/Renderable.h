#pragma once
//#include "Types.h"
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
				if (this->active)
					ResourceManager::AddModelToRenderQueue(dynamic_cast<GameObject*>(this));
			}
		};

	}
}