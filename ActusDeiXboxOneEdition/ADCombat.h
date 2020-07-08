#pragma once
#include "Renderable.h"
#include <unordered_map>


namespace ADResource
{
	namespace ADGameplay
	{
		enum OBJECT_TAG
		{
			PLAYER = 0,
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
			XMFLOAT3 colScale;
			XMFLOAT3 vel;

			bool isDeactivateOnFirstApplication = false;
			float offsetX;
			float offsetZ;
			float offsetY = 0;
			float lifespan;
			ADPhysics::OBB collider;

			std::string eventName;

			HitBox() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::TRIGGER; colliderPtr->trigger = true; };
			~HitBox() = default;


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
				// Physics
				collider = ADPhysics::OBB(transform, XMFLOAT3(1, 1, 1));
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


			void CheckCollision(GameObject* obj) override
			{
				if (active && obj->active)
				{
					ADPhysics::Manifold m;
					if (obj->colliderPtr->isCollision(&collider, m))
					{
						if (obj->team != team && obj->colliderPtr->type != ADPhysics::ColliderType::Plane)
						{
							if (gamePlayType != CONSUMPTION_HITBOX || obj->gamePlayType >= WOOD_MINION && obj->gamePlayType <= STONE_MINION)
								PassEffects(obj);
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
			bool active;
			float cooldownDuration;
			float cooldownTimer;
			float attackDuration;
			float attackTimer;
			float hitboxDelay = 0;
			HitBox* hitbox;
			UINT hitboxCount;

			bool removeHbIfEnd = true;
			bool movesToPlayer = true;
			bool hitboxFired = false;

			std::vector<bool> eventFired;
			std::vector<float> eventDelay;
			std::vector<std::string> eventName;

			XMFLOAT3* hbpos = nullptr;

			//If the attack doesn't own the hitbox this needs to change.
			~Action()
			{
				delete hitbox;
				if (hbpos)
				{
					delete hbpos;
				}
			};

			bool StartAction(XMMATRIX* _casterTransform)
			{
				if (movesToPlayer)
				{
					hitbox->transform = *_casterTransform;
					hitbox->transform = XMMatrixMultiply(XMMatrixScaling(hitbox->colScale.x * 10, hitbox->colScale.y * 10, hitbox->colScale.z * 10), hitbox->transform);
					XMVECTOR castSideNormal = _casterTransform->r[0];
					XMVECTOR castUpNormal = _casterTransform->r[1];
					XMVECTOR castHeadingNormal = _casterTransform->r[2];
					XMVECTOR targetLocation = _casterTransform->r[3];

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

					targetPos.x += (casterFN.x * hitbox->offsetZ) + (casterUN.x * hitbox->offsetY) + (casterSN.x * hitbox->offsetX);
					targetPos.y += (casterFN.y * hitbox->offsetZ) + (casterUN.y * hitbox->offsetY) + (casterSN.y * hitbox->offsetX);
					targetPos.z += (casterFN.z * hitbox->offsetZ) + (casterUN.z * hitbox->offsetY) + (casterSN.z * hitbox->offsetX);

					hitbox->SetPosition(targetPos);

					hitbox->Velocity.x = (casterFN.x * hitbox->vel.z) + (casterUN.x * hitbox->vel.y) + (casterSN.x * hitbox->vel.x);
					hitbox->Velocity.y = (casterFN.y * hitbox->vel.z) + (casterUN.y * hitbox->vel.y) + (casterSN.y * hitbox->vel.x);
					hitbox->Velocity.z = (casterFN.z * hitbox->vel.z) + (casterUN.z * hitbox->vel.y) + (casterSN.z * hitbox->vel.x);

				}
				if (cooldownTimer <= 0 && attackTimer <= 0)
				{
					for (int i = 0; i < eventDelay.size(); i++)
					{
						eventFired[i] = false;
					}
					if (hitbox)
					{
						cooldownTimer = cooldownDuration;
						attackTimer = attackDuration;
						for (int i = 0; i < hitboxCount; ++i)
						{
							if (hitboxDelay <= 0)
							{
								hitboxFired = true;
								hitbox[i].Enable();
							}
							active = true;
							return true;
						}
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
					if (attackTimer > 0)
					{
						if (hitboxFired == false && attackDuration - attackTimer > hitboxDelay)
						{
							for (int i = 0; i < hitboxCount; ++i)
							{
								hitboxFired = true;
								hitbox[i].Enable();
							}
						}
						for (int i = 0; i < eventDelay.size(); i++)
						{
							if (eventFired[i] == false && attackDuration - attackTimer > eventDelay[i])
							{
								if (hbpos != nullptr)
								{
									delete hbpos;
								}
								hbpos = new XMFLOAT3(hitbox->GetPosition());
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

					hitbox->collider.Pos = hitbox->GetPosition();
				}

			}

			void EndAction()
			{
				//Some hit boxes would turn off this way, others require they burn out or collide.
				if (hitbox && removeHbIfEnd)
				{
					active = false;
					hitboxFired = false;
					for (int i = 0; i < hitboxCount; ++i)
					{
						hitbox[i].active = false;
					}
				}
			}
		};

		class Destructable : public Renderable
		{
		protected:
			StatSheet* stats;
		public:
			XMFLOAT3 colScale;
			ADPhysics::AABB collider;
			std::string deathEvent;

			Destructable() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::COLLIDABLE; }
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
				stats = statSheet;
			};

			void CheckCollision(GameObject* obj) override
			{
				if (active && obj->active)
				{
					ADPhysics::Manifold m;
					if (obj->colliderPtr->isCollision(&collider, m))
					{
						collisionQueue.push(CollisionPacket(this, obj, m));
					}
				}
			}

			void ProcessEffects(float _deltaTime)
			{
				for (int i = 0; i < effects.size(); ++i)
				{
					effects[i].get()->Update(_deltaTime);

					//std::string msg = std::to_string(effects[i].get()->tickTimer);
					//ADUI::MessageReceiver::Log(msg);

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
			float offsetX;
			float offsetZ;
			float offsetY = 0;
			ADPhysics::AABB collider;

			//Until we have an event manager this is our solution for events.
			//ADUI::UIMessage eventUIMessage;


			Trigger() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::TRIGGER; colliderPtr->trigger = true; };

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
				// Physics
				XMFLOAT3 dang;
				XMStoreFloat3(&dang, transform.r[3]);
				collider = ADPhysics::AABB(dang, colScale);
				colliderPtr = &collider;
				collider.trigger = true;
			};

			void CheckCollision(GameObject* obj) override
			{
				if (active && obj->active)
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
				//ADUI::MessageReceiver::SendMessage(&eventUIMessage);
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

	//AD_ULONG villagerIdleMeshid;
	//AD_ULONG villagerWalkingMeshid;
	//AD_ULONG villagerRunningMeshid;


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