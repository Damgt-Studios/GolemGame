#pragma once
#include <Types.h>
#include "ADUserInterface.h"
#include "GameplayBaseClasses.h"
#include "ResourceManager.h"
#include "ADEventSystem.h"
#include "ADCombat.h"
#include "ADAI.h"
#include "ADPathfinding.h"
#include "ADParticles.h"

namespace ADResource
{
	namespace ADGameplay
	{

		class MessageTrigger : public Renderable
		{
		public:
			XMFLOAT3 colScale;

			bool isDeactivateOnFirstApplication = true;
			float offsetX;
			float offsetZ;
			float offsetY = 0;
			ADPhysics::AABB collider;

			//Until we have an event manager this is our solution for events.
			ADUI::UIMessage eventUIMessage;


			MessageTrigger() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::TRIGGER; colliderPtr->trigger = true; };

			void Enable()
			{
				active = true;
			}

			virtual void Update(float _deltaTime)
			{
				if (active)
				{
					// Physics
					XMFLOAT3 dang;
					XMStoreFloat3(&dang, transform.r[3]);
					collider = ADPhysics::AABB(dang, colScale);
					colliderPtr = &collider;
					collider.trigger = true;
				}
			};

			void CheckCollision(GameObject* obj) override
			{
				if (active && obj->active)
				{
 					if (obj->gamePlayType == PLAYER)
					{
						ADPhysics::Manifold m;
						if (obj->colliderPtr->isCollision(&collider, m))
						{
							CallEvent();
							if (isDeactivateOnFirstApplication)
							{
								active = false;
							}
						}
					}
				}
			}

			void CallEvent()
			{
				ADUI::MessageReceiver::SendMessage(&eventUIMessage);
			}
		};

		class Building : public GameObject
		{
			StatSheet* stats;
			Building* rubble = nullptr;
			Building* turret = nullptr;


			void AddToScene()
			{
				for (size_t i = 0; i < models.size(); i++)
				{
					ResourceManager::AddGameObject(models[i]);
				}
			};


		public:
			std::string name;
			std::string deathEvent;

			Building()
			{
				//team = 1;
				//stats = new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["Villager"]);
			}
			~Building() { delete stats; };
			Building(Building&) = delete;
			Building(const Building&) = delete;
			Building(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 collider_scale, XMFLOAT3 offset, std::vector<Renderable*>(*Generator)(XMFLOAT3, XMFLOAT3), std::string statSheet)
			{
				//temp->SetScale(scale);
				SetScale(collider_scale);
				SetRotation(rotation);
				SetPosition(position);
				//pmat.InvMass = 0.01;

				pos = position; rot = rotation;	colliderScale = collider_scale;	off = offset;
				models = Generator(position, rotation);

				for (size_t i = 0; i < models.size(); i++)
				{
					models[i]->colliderPtr = nullptr;
				}

				collider = ADPhysics::OBB(XMMatrixRotationY(XMConvertToRadians(rot.y)) * XMMatrixTranslation(pos.x + off.x, pos.y + off.y, pos.z + off.z), colliderScale);
				physicsType = OBJECT_PHYSICS_TYPE::STATIC;
				colliderPtr = &collider;
				team = 1;
				desirability = 1.0f;
				safeRadius = min((colliderScale.x / 2.f), (colliderScale.z / 2.f)) - 1;
				attackRadius = 12.f;

				SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase[statSheet]));
			}

			//Building(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 collider_scale, XMFLOAT3 offset, std::vector<Renderable*>(*Generator)(XMFLOAT3, XMFLOAT3), std::string statSheet)
			//{
			//	pos = position; rot = rotation;	colliderScale = collider_scale;	off = offset;
			//	models = Generator(position, rotation);

			//	for (size_t i = 0; i < models.size(); i++)
			//	{
			//		models[i]->colliderPtr = nullptr;
			//	}

			//	collider = ADPhysics::OBB(XMMatrixRotationY(XMConvertToRadians(rot.y)) * XMMatrixTranslation(pos.x + off.x, pos.y + off.y, pos.z + off.z), colliderScale);
			//	physicsType = OBJECT_PHYSICS_TYPE::STATIC;
			//	colliderPtr = &collider;
			//	team = 1;

			//	SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase[statSheet]));
			//}

			void SetRubble(Building* _rubble)
			{
				rubble = _rubble;
				rubble->safeRadius = 30.0f;
			};

			void SetTurret(Building* _turret)
			{
				turret = _turret;
			};
			
			Building* GetTurret()
			{
				return turret;
			};

			Building* GetRubble()
			{
				return rubble;
			};

			bool HasTurret()
			{
				if (turret)
				{
					return true;
				}
				return false;
			};

			bool HasRubble()
			{
				if (rubble)
				{
					return true;
				}
				return false;
			};

			void RemoveFromScene()
			{
				//desirability = -5;
				active = false;
				for (size_t i = 0; i < models.size(); i++)
				{
					ResourceManager::RemoveGameObject(models[i]);
				}
				if (turret)
				{
					if(turret->active)
					{
						turret->RemoveFromScene();
					}
				}
				if (destructionEmitter && destructionEmitter2)
				{
					//if (!destructionEmitter->GetActive())
					//	destructionEmitter->Activate(1.0f, { pos.x,pos.y,pos.z,1 });
					//else
					//	destructionEmitter2->Activate(1.0f, { pos.x,pos.y,pos.z,1 });
				}
			};

			virtual void Update(float delta_time)
			{
				collider = ADPhysics::OBB(XMMatrixRotationY(XMConvertToRadians(rot.y)) * XMMatrixTranslation(pos.x + off.x, pos.y + off.y, pos.z + off.z), colliderScale);
				SetPosition(pos);
				//collider.Pos = VectorToFloat3(XMVector3Transform(Float3ToVector(collider.Pos), XMMatrixScaling(25, 25, 25)));

				physicsType = COLLIDABLE;
				colliderPtr = &collider;

				if (active)
				{	
					if (stats != nullptr)
						ProcessEffects(delta_time);
				}
			}

			XMMATRIX GetColliderInfo()
			{
				XMMATRIX temp;
				temp.r[0] = XMVector3Normalize(Float3ToVector(collider.AxisX));
				temp.r[1] = XMVector3Normalize(Float3ToVector(collider.AxisY));
				temp.r[2] = XMVector3Normalize(Float3ToVector(collider.AxisZ));
				temp.r[3] = Float3ToVector(collider.Pos);
				temp.r[3].m128_f32[3] = 1;

				temp = XMMatrixScaling(collider.GetWidth(), collider.GetHeight(), collider.GetLength()) * temp;;

				temp.r[3].m128_f32[0] += off.x;
				temp.r[3].m128_f32[1] += off.y;
				temp.r[3].m128_f32[2] += off.z;

				temp.r[3].m128_f32[3] = 1;

				return temp;
			}

			void ApplyEffect(ADResource::ADGameplay::Effect* _effect)
			{
				if (active)
				{
					effects.push_back(_effect->clone());
					effects[effects.size() - 1].get()->OnApply(GetStatSheet());
				}
			}

			Renderable* GetModelByIndex(int _index)
			{
				return models[_index];
			}

			virtual StatSheet* GetStatSheet() override
			{
				return stats;
			};

			void SetStatSheet(StatSheet* statSheet)
			{
				has_stats = true;
				stats = statSheet;
			};

			void CheckCollision(GameObject* obj) override
			{
				if (active && obj->active)
				{
					ADPhysics::Manifold m;
					if (obj->colliderPtr->isCollision(&collider, m))
					{
						//collisionQueue.push(CollisionPacket(this, obj, m));
					}
				}
			}

			void ProcessEffects(float _deltaTime)
			{
				for (int i = 0; i < effects.size(); ++i)
				{
					effects[i].get()->Update(_deltaTime);

					if (effects[i].get()->isFinished)
					{
						effects[i].get()->OnExit();
						effects.erase(effects.begin() + i);
						i--;
					}
				}
				if (stats->RequestStats("Health")->currentValue <= 0)
				{
					ADEvents::ADEventSystem::Instance()->SendEvent(deathEvent, (void*)(gamePlayType));
					active = false;
					RemoveFromScene();
					if (rubble)
					{
						rubble->active = true;
						rubble->AddToScene();
					}
					else
					{
						ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x, pos.z })->tile->walkable = true;
						ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 12.5f, pos.z + 12.5f })->tile->walkable = true;
						ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 12.5f, pos.z - 12.5f })->tile->walkable = true;
						ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 12.5f, pos.z - 12.5f })->tile->walkable = true;
						ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 12.5f, pos.z + 12.5f })->tile->walkable = true;
						ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x, pos.z }));
						ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 15, pos.z + 15 }));
						ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 15, pos.z - 15 }));
						ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 15, pos.z - 15 }));
						ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 15, pos.z + 15 }));
						if (collider.GetWidth() > 27.5f || collider.GetLength() > 27.5f)
						{
							ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x, pos.z })->tile->walkable = true;
							ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 32.f, pos.z + 32.f })->tile->walkable = true;
							ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 32.f, pos.z - 32.f })->tile->walkable = true;
							ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 32.f, pos.z - 32.f })->tile->walkable = true;
							ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 32.f, pos.z + 32.f })->tile->walkable = true;
							ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x, pos.z }));
							ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 32.f, pos.z + 32.f }));
							ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 32.f, pos.z - 32.f }));
							ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 32.f, pos.z - 32.f }));
							ADAI::ADPathfinding::Instance()->EnableTile(ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 32.f, pos.z + 32.f }));
						}

					}
				}
			}

			UpwardCloudEmitter* destructionEmitter;
			UpwardCloudEmitter* destructionEmitter2;
		private:
			XMFLOAT3 pos, rot, off, colliderScale;
			std::vector<Renderable*> models;
			ADPhysics::OBB collider;
		};
	}
}

