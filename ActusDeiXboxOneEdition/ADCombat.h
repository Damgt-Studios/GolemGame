#pragma once
#include "Renderable.h"
#include <AnimationStateMachine.h>
#include <unordered_map>
#include "ADParticles.h"


namespace ADResource
{
	namespace ADGameplay
	{
		enum OBJECT_TAG
		{
			PLAYER = 0,
			COMMAND_MARKER,
			UNTYPED_MINION,
			WOOD_MINION,
			WATER_MINION,
			FIRE_MINION,
			STONE_MINION,
			PEASANT,
			HOUSE_LVL1,
			HOUSE_LVL2,
			BALLISTA,
			CATAPULT,
			MEGA_SHOT,
			WATER_TOWER,
			ELECTTRIC_TOWER,
			FIRE_CATAPULT,
			MISC_STRUCTURE,
			MISC_DESTRUCTABLE,
			SMALL_ESSENCE,
			SPAWNER,
			FIRE_OBJECT,
			ALLY_HITBOX,
			ENEMY_HITBOX,
			CONSUMPTION_HITBOX,
			PROJECTILE,
			EVENT_TRIGGER
		};

		enum GAME_ELEMENTS
		{
			STONE = 0,
			WATER,
			FIRE,
			WOOD,
			ELECTRIC
		};

		class HitBox : public ADGameplay::Renderable
		{

		public:
			XMFLOAT3 colScale = { 1,1,1 };
			XMFLOAT3 modelScale = { 1,1,1 };
			XMFLOAT3 vel = { 0,0,0 };

			bool isDeactivateOnFirstApplication = false;
			float offsetX = 0;
			float offsetZ = 0;
			float offsetY = 0;
			float lifespan = 0;
			XMFLOAT3 rotation = { 0,0,0 };
			
			ADPhysics::OBB collider;

			ADResource::ADGameplay::GameObject* target = nullptr;
			std::string eventName= "";
			std::string modelName= "";
			std::string matName = "";

			HitBox() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::TRIGGER;	gamePlayType = ADResource::ADGameplay::ALLY_HITBOX; colliderPtr->trigger = true; };
			~HitBox() = default;

			HitBox* Clone()
			{
				HitBox* nBox = new HitBox();
				XMFLOAT3 scale = modelScale; //XMFLOAT3(1, 1, 1);
				nBox->SetScale(scale);
				nBox->SetRotation(scale);
				nBox->SetPosition(scale);
				nBox->active = false;

				int cnt = 0;
				for (auto& effect : effects)
				{
					nBox->effects.push_back(effect->clone());
					nBox->effects[cnt].get()->sourceID = ResourceManager::GenerateEffectID();
					cnt++;
				}
				nBox->offsetX = offsetX;
				nBox->offsetZ = offsetZ;
				nBox->offsetY = offsetY;
				nBox->colScale = colScale;
				nBox->modelScale = modelScale;
				nBox->modelName = modelName;
				nBox->matName = matName;
				nBox->vel = vel;
				nBox->lifespan = lifespan;
				nBox->team = team;

				if (nBox->team == 3)
				{
					nBox->gamePlayType = ADResource::ADGameplay::CONSUMPTION_HITBOX;
				}
				nBox->isDeactivateOnFirstApplication = isDeactivateOnFirstApplication;
				nBox->eventName = eventName;

				nBox->rotation = rotation;
				if (nBox->modelName != "")
				{
					AD_ULONG id = ResourceManager::AddSimpleModel(nBox->modelName, nBox->matName, XMFLOAT3(1, 1, 1), nBox->modelScale, { 0,0,0 }); 
					nBox->SetMeshID(id);
				}

				nBox->rotation = rotation;
				nBox->transform = XMMatrixRotationX(nBox->rotation.x);
				XMMATRIX matrix1 = XMMatrixTranslation(nBox->offsetX, nBox->offsetY, nBox->offsetZ);
				nBox->collider = ADPhysics::OBB(nBox->transform * matrix1, nBox->colScale);


				nBox->SetScale(nBox->modelScale);	
				nBox->collider.trigger = true;
				nBox->colliderPtr = &nBox->collider;

				ResourceManager::AddGameObject(nBox);
				return nBox;
			}

			void Enable()
			{
				active = true;
				for (int i = 0; i < effects.size(); ++i)
				{
					effects[i].get()->instanceID++;
				}
			}

			virtual void Update(float _deltaTime)
			{

				if (active)
				{
					if (target)
					{
						XMVECTOR vel = XMLoadFloat3(&target->GetPosition()) - XMLoadFloat3(&GetPosition());
						vel = XMVector3Normalize(vel) * 5;
						XMStoreFloat4(&Velocity, vel);
					}
					// Physics
					collider = ADPhysics::OBB(transform, colScale); 
					colliderPtr = &collider;
					collider.trigger = true;

					if (lifespan > 0)
					{
						lifespan -= _deltaTime;
						if (lifespan <= 0)
						{
							active = false;
						}
					}

					AddToPositionVector((XMFLOAT3&)Velocity);
				}
			}


			void CheckCollision(GameObject* obj) override
			{
				if (active && obj->active)
				{
					if (!obj->colliderPtr->trigger)
					{
						if (!target || target == obj)
						{
							ADPhysics::Manifold m;
							if (obj->colliderPtr->isCollision(&collider, m))
							{
								if (obj->has_stats)
								{
									if (obj->team != team && obj->colliderPtr->type != ADPhysics::ColliderType::Plane)
									{
										if (gamePlayType != CONSUMPTION_HITBOX || obj->gamePlayType >= WOOD_MINION && obj->gamePlayType <= STONE_MINION)
										{
											PassEffects(obj);
										}
									}
								}
							}
						}
						
					}
				}
			}

			void PassEffects(GameObject* obj)
			{
				for (int i = 0; i < effects.size(); ++i)
				{
					if (!obj->HasEffectID(effects[i].get()->sourceID, effects[i].get()->instanceID))
					{
						obj->effects.push_back(effects[i].get()->clone());
						obj->effects[obj->effects.size() - 1].get()->OnApply(obj->GetStatSheet());
						XMFLOAT3 hbpos = GetPosition();
						ADEvents::ADEventSystem::Instance()->SendEvent(eventName, (void*)&hbpos);
						if (isDeactivateOnFirstApplication)
						{
							active = false;
						}
					}
				}
			}
		};


		class Action
		{
		public:
			bool active = false;
			float cooldownDuration = 0.f;
			float cooldownTimer = 0.f;
			float attackDuration = 0.f;
			float attackTimer = 0.f;
			float hitboxDelay = 0.f;
			float scaleCorrection = 1.f;
			std::vector<HitBox*> hitboxes;
			UINT currentHitBox = 0;
			UINT hitboxCount = 0;

			bool removeHbIfEnd = true;
			bool movesToPlayer = true;
			std::vector<bool> hitboxFired;

			std::vector<bool> eventFired;
			std::vector<float> eventDelay;
			std::vector<std::string> eventName;

			XMFLOAT3* hbpos = nullptr;

			//If the attack doesn't own the hitbox this needs to change.
			~Action()
			{
				for (auto& hb : hitboxes)
				{
					delete hb;
				}
				hitboxes.clear();
			};

			bool StartAction(XMMATRIX* _casterTransform)
			{
				if (cooldownTimer <= 0 && attackTimer <= 0)
				{
					currentHitBox++;
					if (currentHitBox >= hitboxCount)
					{
						currentHitBox = 0;
						for (auto& hb : hitboxFired)
						{
							hb = false;
						}
					}
					if (hitboxes.size() > 0 && movesToPlayer)
					{
						if (hitboxes[currentHitBox]->modelName == "")
						{
							hitboxes[currentHitBox]->transform = XMMatrixMultiply(XMMatrixScaling(hitboxes[currentHitBox]->colScale.x, hitboxes[currentHitBox]->colScale.y, hitboxes[currentHitBox]->colScale.z), (*_casterTransform * scaleCorrection));
						}
						else
						{
							hitboxes[currentHitBox]->transform = XMMatrixMultiply(XMMatrixScaling(hitboxes[currentHitBox]->modelScale.x, hitboxes[currentHitBox]->modelScale.y, hitboxes[currentHitBox]->modelScale.z), (*_casterTransform));
							
							hitboxes[currentHitBox]->SetRotation(hitboxes[currentHitBox]->rotation, RotationType::yxz);
						}
						XMVECTOR castSideNormal = _casterTransform->r[0];
						XMVECTOR castUpNormal = _casterTransform->r[1];
						XMVECTOR castHeadingNormal = _casterTransform->r[2];
						XMVECTOR targetLocation = _casterTransform->r[3];
						hitboxes[currentHitBox]->transform.r[3] = targetLocation;

						castUpNormal = XMVector4Normalize(castUpNormal);
						XMFLOAT3 casterUN;
						XMStoreFloat3(&casterUN, castUpNormal);

						castSideNormal = XMVector4Normalize(castSideNormal);
						XMFLOAT3 casterSN;
						XMStoreFloat3(&casterSN, castSideNormal);


						castHeadingNormal = XMVector4Normalize(castHeadingNormal);
						XMFLOAT3 casterFN;
						XMStoreFloat3(&casterFN, castHeadingNormal);
						XMFLOAT3 targetPos;
						XMStoreFloat3(&targetPos, targetLocation);

						targetPos.x += (casterFN.x * hitboxes[currentHitBox]->offsetZ) + (casterUN.x * hitboxes[currentHitBox]->offsetY) + (casterSN.x * hitboxes[currentHitBox]->offsetX);
						targetPos.y += (casterFN.y * hitboxes[currentHitBox]->offsetZ) + (casterUN.y * hitboxes[currentHitBox]->offsetY) + (casterSN.y * hitboxes[currentHitBox]->offsetX);
						targetPos.z += (casterFN.z * hitboxes[currentHitBox]->offsetZ) + (casterUN.z * hitboxes[currentHitBox]->offsetY) + (casterSN.z * hitboxes[currentHitBox]->offsetX);

						hitboxes[currentHitBox]->SetPosition(targetPos);
						hitboxes[currentHitBox]->Velocity.x = (casterFN.x * hitboxes[currentHitBox]->vel.z) + (casterUN.x * hitboxes[currentHitBox]->vel.y) + (casterSN.x * hitboxes[currentHitBox]->vel.x);
						hitboxes[currentHitBox]->Velocity.y = (casterFN.y * hitboxes[currentHitBox]->vel.z) + (casterUN.y * hitboxes[currentHitBox]->vel.y) + (casterSN.y * hitboxes[currentHitBox]->vel.x);
						hitboxes[currentHitBox]->Velocity.z = (casterFN.z * hitboxes[currentHitBox]->vel.z) + (casterUN.z * hitboxes[currentHitBox]->vel.y) + (casterSN.z * hitboxes[currentHitBox]->vel.x);
						
					}
					for (int i = 0; i < eventDelay.size(); i++)
					{
						eventFired[i] = false;
					}
					if (hitboxes.size() > 0)
					{
						cooldownTimer = cooldownDuration;
						attackTimer = attackDuration;
						if (hitboxDelay <= 0)
						{
							hitboxFired[currentHitBox] = true;
							hitboxes[currentHitBox]->Enable();
						}
						active = true;
						return true;
					}
				}
				return false;
			}
			void Update(float _deltaTime)
			{
				if (cooldownTimer > 0)
				{
					cooldownTimer -= _deltaTime;

				}
				if (active)
				{
					if (hitboxFired[currentHitBox] == false && attackDuration - attackTimer > hitboxDelay)
					{
						hitboxFired[currentHitBox] = true;
						hitboxes[currentHitBox]->Enable();
					}
					if (attackTimer > 0)
					{
						for (int i = 0; i < eventDelay.size(); i++)
						{
							if (eventFired[i] == false && attackDuration - attackTimer > eventDelay[i])
							{
								if (hbpos)
								{
									delete hbpos;
								}
								hbpos = new XMFLOAT3(hitboxes[currentHitBox]->GetPosition());
								ADEvents::ADEventSystem::Instance()->SendEvent(eventName[i], (void*)hbpos);
								eventFired[i] = true;
							}
						}
						attackTimer -= _deltaTime;
						if (attackTimer <= 0)
						{
							EndAction();
						}
					}
					else
					{
						EndAction();
					}

					hitboxes[currentHitBox]->collider.Pos = hitboxes[currentHitBox]->GetPosition();
				}

			}

			void EndAction()
			{
				//Some hit boxes would turn off this way, others require they burn out or collide.
				if (hitboxes[currentHitBox])
				{
					active = false;
					if (removeHbIfEnd)
					{
						hitboxes[currentHitBox]->active = false;
						hitboxFired[currentHitBox] = false;
					}
				}
			}

			Action* Clone()
			{
				Action* action = new Action();
				for (auto& hb : hitboxes)
				{
					action->hitboxes.push_back(hb->Clone());
					action->hitboxFired.push_back(false);
				}
				action->hitboxCount = hitboxCount;
				action->cooldownDuration = cooldownDuration;
				action->attackDuration = attackDuration;
				action->removeHbIfEnd = removeHbIfEnd;
				action->scaleCorrection = scaleCorrection;


				for (auto& evnt : eventName)
				{
					action->eventName.push_back(evnt);
					action->eventFired.push_back(false);
				}
				for (auto& evnt : eventDelay)
				{
					action->eventDelay.push_back(evnt);
				}
				action->hitboxDelay = hitboxDelay;
				return action;
			}
		};

		class Destructable : public Renderable
		{
		protected:
			StatSheet* stats = nullptr;
		public:
			XMFLOAT3 colScale = { 0,0,0 };
			AnimationStateMachine* anim_controller = nullptr;
			ADPhysics::AABB collider;
			std::string deathEvent = "";
			BloodEmitter* villagerBlood = nullptr;

			Destructable()
			{
				colliderPtr = &collider;
				physicsType = OBJECT_PHYSICS_TYPE::COLLIDABLE;
				anim_controller = new AnimationStateMachine();
			}
			~Destructable() override
			{
				delete stats;
			}

			virtual void Update(float _deltaTime)
			{
				if (active)
				{
					ProcessEffects(_deltaTime);
					collider = ADPhysics::AABB(VectorToFloat3(transform.r[3]), colScale);
					colliderPtr = &collider;
					physicsType = OBJECT_PHYSICS_TYPE::COLLIDABLE;

					anim_controller->SetModel_To_CurrentAnimation();
				}
			};

			void ApplyEffect(ADResource::ADGameplay::Effect* _effect)
			{
				if (active)
				{
					effects.push_back(_effect->clone());
					effects[effects.size() - 1].get()->OnApply(GetStatSheet());
				}
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
						if (!obj->colliderPtr->trigger && obj->team != team)
						{
							collisionQueue.push(CollisionPacket(this, obj, m));
						}
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
					Death();
				}
			}

			void Death()
			{
				ADEvents::ADEventSystem::Instance()->SendEvent(deathEvent, (void*)(gamePlayType));
				//Death Time
				if (villagerBlood)
				{
					if (!villagerBlood[0].GetActive())
						villagerBlood[0].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else if (!villagerBlood[1].GetActive())
						villagerBlood[1].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else if (!villagerBlood[2].GetActive())
						villagerBlood[2].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else if (!villagerBlood[3].GetActive())
						villagerBlood[3].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else if (!villagerBlood[4].GetActive())
						villagerBlood[4].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else if (!villagerBlood[5].GetActive())
						villagerBlood[5].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else if (!villagerBlood[6].GetActive())
						villagerBlood[6].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else if (!villagerBlood[7].GetActive())
						villagerBlood[7].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else if (!villagerBlood[8].GetActive())
						villagerBlood[8].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
					else
						villagerBlood[9].Activate({ transform.r[3].m128_f32[0],transform.r[3].m128_f32[1] + 10,transform.r[3].m128_f32[2],transform.r[3].m128_f32[4] });
				}
				DropLoot();
				Remove();
			}

			void Remove()
			{
				active = false;
			}

			void DropLoot()
			{

			}
		};

		class Trigger : public Renderable
		{
		public:
			XMFLOAT3 colScale;

			bool isDeactivateOnFirstApplication = false;
			float offsetX = 0;
			float offsetZ = 0;
			float offsetY = 0;
			ADPhysics::AABB collider;


			Trigger() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::TRIGGER; gamePlayType = ADResource::ADGameplay::EVENT_TRIGGER; colliderPtr->trigger = true; };

			void Enable()
			{
				active = true;
				for (int i = 0; i < effects.size(); ++i)
				{
					effects[i].get()->instanceID++;
				}
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
					if (!obj->colliderPtr->trigger)
					{
						ADPhysics::Manifold m;
						if (obj->colliderPtr->isCollision(&collider, m))
						{
							if (obj->team != team && obj->colliderPtr->type != ADPhysics::ColliderType::Plane)
							{
								if (gamePlayType == EVENT_TRIGGER && obj->gamePlayType == PLAYER)
								{
									CallEvent();
									if (isDeactivateOnFirstApplication)
									{
										active = false;
									}
								}
								else
								{
									PassEffects(obj);
								}
							}
						}
					}
				}
			}

			void PassEffects(GameObject* obj)
			{
				for (int i = 0; i < effects.size(); ++i)
				{
					if (!obj->HasEffectID(effects[i].get()->sourceID, effects[i].get()->instanceID))
					{
						obj->effects.push_back(effects[i].get()->clone());
						obj->effects[obj->effects.size() - 1].get()->OnApply(obj->GetStatSheet());
						if (isDeactivateOnFirstApplication)
						{
							active = false;
						}
					}
				}
			}

			void CallEvent()
			{
				//Never used.
			}
		};
	}
}


class DefinitionDatabase
{
	DefinitionDatabase() {};
	~DefinitionDatabase() { Shutdown(); };
	DefinitionDatabase(const DefinitionDatabase& _rhs) {};
	DefinitionDatabase& operator =(const DefinitionDatabase& _rhs) {};

public:

	std::unordered_map<std::string, ADResource::ADGameplay::Effect*>	effectsDatabase;
	std::unordered_map<std::string, ADResource::ADGameplay::StatSheet*> statsheetDatabase;
	std::unordered_map<std::string, ADResource::ADGameplay::HitBox*>	hitboxDatabase;
	std::unordered_map<std::string, ADResource::ADGameplay::Action*>	actionDatabase;


	static DefinitionDatabase* Instance()
	{
		static DefinitionDatabase instance;
		return &instance;
	};

	void Shutdown()
	{
		for (auto it = effectsDatabase.begin(), itEnd = effectsDatabase.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
		effectsDatabase.clear();
		for (auto it = statsheetDatabase.begin(), itEnd = statsheetDatabase.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
		statsheetDatabase.clear();
		for (auto it = hitboxDatabase.begin(), itEnd = hitboxDatabase.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
		hitboxDatabase.clear();
		for (auto it = actionDatabase.begin(), itEnd = actionDatabase.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
		actionDatabase.clear();
	};
};