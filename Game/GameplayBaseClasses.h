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
				if(this->active)
					ResourceManager::AddModelToRenderQueue(dynamic_cast<GameObject*>(this));
			}
		};

		class Building 
		{
		public:
			Building(Building&) = delete;
			Building(const Building&) = delete;
			Building(int health, int essence, XMFLOAT3 position, XMFLOAT3 rotation, std::vector<Renderable*>(*Generator)(XMFLOAT3, XMFLOAT3))
			{
				_health = health; _essence = essence;
				pos = position; rot = rotation;
				models = Generator(position, rotation);

				for (size_t i = 0; i < models.size(); i++)
				{
					models[i]->colliderPtr = nullptr;
				}
			}

			void Update()
			{
				collider = &ADPhysics::OBB(XMMatrixRotationY(XMConvertToRadians(rot.y)) * XMMatrixTranslation(pos.x, pos.y, pos.z), XMFLOAT3(1,1,1));
			}
		private:
			int _health, _essence;
			XMFLOAT3 pos, rot;
			std::vector<Renderable*> models;
			ADPhysics::OBB* collider = nullptr;
		};
	}
}