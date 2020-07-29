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


			MessageTrigger() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::TRIGGER; colliderPtr->trigger = true; gamePlayType = EVENT_TRIGGER; };

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
			Action* Essence = nullptr;
			bool tokenRecovery = false;

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
				safeRadius = min((colliderScale.x / 2.f), (colliderScale.z / 2.f));
				attackRadius = 14.f;

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

			void AddObject(XMFLOAT3 position, XMFLOAT3 rotation, std::vector<Renderable*>(*Generator)(XMFLOAT3, XMFLOAT3)) 
			{
				std::vector<Renderable*> temp = Generator(position, rotation);
				
				for (auto& r : temp)
					models.push_back(r);
			}

			void SetRubble(Building* _rubble, ADResource::ADGameplay::GameObject* _player, bool _townHall = false)
			{
				rubble = _rubble;
				rubble->active = false;
				rubble->safeRadius = 25.0f;
				rubble->avoidRadius = 10.0f;

				if (_townHall)
				{
					Essence = DefinitionDatabase::Instance()->actionDatabase["SpawnEssenceL"]->Clone();
				}
				else
				{
					Essence = DefinitionDatabase::Instance()->actionDatabase["SpawnEssenceS"]->Clone();
				}
				Essence->hitboxes[0]->target = _player;
				Essence->active = false;
			};

			void SetTurret(Building* _turret)
			{
				turret = _turret;
			};
			
			Building* GetTurret()
			{
				return turret;
			};

			XMFLOAT3 GetColliderScale()
			{
				return colliderScale;
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
					if (!destructionEmitter->GetActive())
						destructionEmitter->Activate(1.0f, { pos.x,pos.y,pos.z,1 });
					else
						destructionEmitter2->Activate(1.0f, { pos.x,pos.y,pos.z,1 });
				}

				if (Essence)
				{
					Essence->StartAction(&transform);
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
					if (actionLevel > 0)
					{
						if (actionLevel > 40)
							actionLevel -= 10 * delta_time;
						actionLevel -= delta_time;
					}
				}
				if (Essence)
				{
					Essence->Update(delta_time);
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
						ADAI::SearchNode* temp1 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x, pos.z });
						if (temp1)
						{
							temp1->tile->walkable = true;
						};
						ADAI::SearchNode* temp2 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 10.f, pos.z + 10.f });
						if (temp2)
						{
							temp2->tile->walkable = true;
						};
						ADAI::SearchNode* temp3 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 10.f, pos.z - 10.f });
						if (temp3)
						{
							temp3->tile->walkable = true;
						};
						ADAI::SearchNode* temp4 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 10.f, pos.z - 10.f });
						if (temp4)
						{
							temp4->tile->walkable = true;
						}
						ADAI::SearchNode* temp5 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 10.f, pos.z + 10.f });
						if (temp5)
						{
							temp5->tile->walkable = true;
						};
						ADAI::SearchNode* temp6 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 10.f, pos.z });
						if (temp6)
						{
							temp6->tile->walkable = true;
						};
						ADAI::SearchNode* temp7 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x , pos.z - 10.f });
						if (temp7)
						{
							temp7->tile->walkable = true;
						};
						ADAI::SearchNode* temp8 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 10.f, pos.z });
						if (temp8)
						{
							temp8->tile->walkable = true;
						}
						ADAI::SearchNode* temp9 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x, pos.z + 10.f });
						if (temp9)
						{
							temp9->tile->walkable = true;
						};

						ADAI::SearchNode* temp10 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 20.f, pos.z });
						if (temp10)
						{
							temp10->tile->walkable = true;
						};
						ADAI::SearchNode* temp11 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x , pos.z - 20.f });
						if (temp11)
						{
							temp11->tile->walkable = true;
						};
						ADAI::SearchNode* temp12 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 20.f, pos.z });
						if (temp12)
						{
							temp12->tile->walkable = true;
						}
						ADAI::SearchNode* temp14 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x, pos.z + 20.f });
						if (temp14)
						{
							temp14->tile->walkable = true;
						};
						ADAI::SearchNode* temp15 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 20.f, pos.z + 20.f });
						if (temp15)
						{
							temp15->tile->walkable = true;
						};
						ADAI::SearchNode* temp16 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 20.f, pos.z - 20.f });
						if (temp16)
						{
							temp16->tile->walkable = true;
						};
						ADAI::SearchNode* temp17 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 20.f, pos.z - 20.f });
						if (temp17)
						{
							temp17->tile->walkable = true;
						}
						ADAI::SearchNode* temp18 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 20.f, pos.z + 20.f });
						if (temp18)
						{
							temp18->tile->walkable = true;
						}
						if (temp1)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp1);
						}
						if (temp2)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp2);
						}
						if (temp3)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp3);
						}
						if (temp4)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp4);
						}
						if (temp5)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp5);
						}
						if (temp6)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp6);
						}
						if (temp7)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp7);
						}
						if (temp8)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp8);
						}
						if (temp9)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp9);
						}
						if (temp10)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp10);
						}
						if (temp11)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp11);
						}
						if (temp12)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp12); //Skipped 13 oops oh well.
						}
						if (temp14)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp14);
						}
						if (temp15)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp15);
						}
						if (temp16)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp16);
						}
						if (temp17)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp17);
						}
						if (temp18)
						{
							ADAI::ADPathfinding::Instance()->EnableTile(temp18);
						}
						if (collider.GetWidth() > 37.5f || collider.GetLength() > 37.5f)
						{
							temp1 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x, pos.z });
							if (temp1)
							{
								temp1->tile->walkable = true;
							}
							temp2 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 30.f, pos.z + 30.f });
							if (temp2)
							{
								temp2->tile->walkable = true;
							}
							temp3 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x + 30.f, pos.z - 30.f });
							if (temp3)
							{
								temp3->tile->walkable = true;
							}
							temp4 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 30.f, pos.z - 30.f });
							if (temp4)
							{
								temp4->tile->walkable = true;
							}
							temp5 = ADAI::ADPathfinding::Instance()->GetTileFromPosition({ pos.x - 30.f, pos.z + 30.f });
							if (temp5)
							{
								temp5->tile->walkable = true;
							}							
							if (temp1)
							{
								ADAI::ADPathfinding::Instance()->EnableTile(temp1);
							}
							if (temp2)
							{
								ADAI::ADPathfinding::Instance()->EnableTile(temp2);
							}
							if (temp3)
							{
								ADAI::ADPathfinding::Instance()->EnableTile(temp3);
							}
							if (temp4)
							{
								ADAI::ADPathfinding::Instance()->EnableTile(temp4);
							}
							if (temp5)
							{
								ADAI::ADPathfinding::Instance()->EnableTile(temp5);
							}
						}

					}
				}
			}

			UpwardCloudEmitter* destructionEmitter;
			UpwardCloudEmitter* destructionEmitter2;
		private: 
			XMFLOAT3 pos, off, colliderScale;
			std::vector<Renderable*> models;
			ADPhysics::OBB collider;
		};
	}
}

