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

		class Building : public GameObject
		{
		public:
			Building(Building&) = delete;
			Building(const Building&) = delete;
			Building(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 collider_scale, XMFLOAT3 offset, std::vector<Renderable*>(*Generator)(XMFLOAT3, XMFLOAT3))
			{
				pos = position; rot = rotation;	colliderScale = collider_scale;	off = offset;
				models = Generator(position, rotation);

				for (size_t i = 0; i < models.size(); i++)
				{
					models[i]->colliderPtr = nullptr;
				}

				collider = ADPhysics::OBB(XMMatrixRotationY(XMConvertToRadians(rot.y)) * XMMatrixTranslation(pos.x + off.x, pos.y + off.y, pos.z + off.z), colliderScale);
				physicsType = COLLIDABLE;
				colliderPtr = &collider;
			}

			virtual void Update(float delta_time)
			{
				collider = ADPhysics::OBB(XMMatrixRotationY(XMConvertToRadians(rot.y)) * XMMatrixTranslation(pos.x + off.x, pos.y + off.y, pos.z + off.z), colliderScale);
				collider.Pos = VectorToFloat3(XMVector3Transform(Float3ToVector(collider.Pos), XMMatrixScaling(25, 25, 25)));

				physicsType = COLLIDABLE;
				colliderPtr = &collider;
			}

			XMMATRIX GetColliderInfo() 
			{
				XMMATRIX temp;
				temp.r[0] = XMVector3Normalize(Float3ToVector(collider.AxisX));
				temp.r[1] = XMVector3Normalize(Float3ToVector(collider.AxisY));
				temp.r[2] = XMVector3Normalize(Float3ToVector(collider.AxisZ));
				temp.r[3] = Float3ToVector(collider.Pos);
				temp.r[3].m128_f32[3] = 1;

				temp = XMMatrixScaling(collider.GetWidth() + 10, collider.GetHeight() + 10, collider.GetLength() + 10) * temp;;
				
				temp.r[3].m128_f32[0] += off.x;
				temp.r[3].m128_f32[1] += off.y;
				temp.r[3].m128_f32[2] += off.z;

				temp.r[3].m128_f32[3] = 1;

				return temp;
			}
		private:
			XMFLOAT3 pos, rot, off, colliderScale;
			std::vector<Renderable*> models;
			ADPhysics::OBB collider;
		};
	}
}