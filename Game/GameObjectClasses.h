#pragma once
#include "ADUserInterface.h"
#include "GameplayBaseClasses.h"
#include "ADEventSystem.h"
#include <Types.h>

namespace ADResource
{
	namespace ADGameplay
	{
		enum OBJECT_TAG
		{
			PLAYER = 0,
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


		class Destructable : public Renderable
		{
		protected:
			StatSheet* stats;
		public:
			XMFLOAT3 colScale;
			ADPhysics::AABB collider;

			Destructable() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::COLLIDABLE; }
			~Destructable() override
			{
				delete stats;
			}

			virtual void Update(float _deltaTime)
			{
				ProcessEffects(_deltaTime);

				// Physics
				XMFLOAT3 dang;
				XMStoreFloat3(&dang, transform.r[3]);
				collider = ADPhysics::AABB(dang, colScale);
				colliderPtr = &collider;

			};

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
					obj->colliderPtr->isCollision(&collider, m);
				}
			}

			void ProcessEffects(float _deltaTime)
			{
				for (int i = 0; i < effects.size(); ++i)
				{
					effects[i].get()->Update(_deltaTime);

					std::string msg = std::to_string(effects[i].get()->tickTimer);
					ADUI::MessageReceiver::Log(msg);

					if (effects[i].get()->isFinished)
					{
						effects[i].get()->OnExit();
						effects.erase(effects.begin() + i);
						i--;
					}
					//AD_UI::UIMessage updateHudDeleteMeOneDay;
					//updateHudDeleteMeOneDay.messageType = ADUI::UIMessageTypes::ExternalMsg;
					//updateHudDeleteMeOneDay.controllerID = 0;
					//updateHudDeleteMeOneDay.number = 1;
					//ADUI::MessageReceiver::SendMessage(&updateHudDeleteMeOneDay);
				}
				if (stats->RequestStats("Health")->currentValue <= 0)
				{
					Death();
				}
			}

			void Death()
			{
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

		class HitBox : public Renderable
		{

		public:
			XMFLOAT3 colScale;

			bool isDeactivateOnFirstApplication = false;
			float offsetX;
			float offsetZ;
			float offsetY = 0;
			ADPhysics::OBB collider;

			HitBox() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::TRIGGER; colliderPtr->trigger = true; };

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
				collider = ADPhysics::OBB(transform, XMFLOAT3(1,1,1));
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
						if (isDeactivateOnFirstApplication)
						{
							active = false;
						}
					}
				}
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
			ADUI::UIMessage eventUIMessage;


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
						obj->effects[obj->effects.size()-1].get()->OnApply(obj->GetStatSheet());
						if (isDeactivateOnFirstApplication)
						{
							active = false;
						}
					}
				}
			}

			void CallEvent()
			{
				ADUI::MessageReceiver::SendMessage(&eventUIMessage);
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
			HitBox* hitbox;
			UINT hitboxCount;

			bool removeHbIfEnd = true;
			bool movesToPlayer = true;

			std::vector<bool> eventFired;
			std::vector<float> eventDelay;
			std::vector<std::string> eventName;


			//If the attack doesn't own the hitbox this needs to change.
			~Action() { delete hitbox; };

			bool StartAction(XMMATRIX* _casterTransform)
			{
				if (movesToPlayer)
				{
					hitbox->transform = *_casterTransform;
					hitbox->transform = XMMatrixMultiply(XMMatrixScaling(hitbox->colScale.x*10, hitbox->colScale.y * 10, hitbox->colScale.z * 10), hitbox->transform);
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
							hitbox[i].Enable();
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
						for (int i = 0; i < eventDelay.size(); i++)
						{
							if (eventFired[i] == false && attackDuration - attackTimer > eventDelay[i])
							{
								XMFLOAT3 hbpos = hitbox->GetPosition();
								XMFLOAT4 hbpos2 = XMFLOAT4(1, 1, 1, 1);
								ADEvents::ADEventSystem::Instance()->SendEvent(eventName[i], (void*)&hbpos2);
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
					for (int i = 0; i < hitboxCount; ++i)
					{
						hitbox[i].active = false;
					}
				}
			}
		};


		//class Enemy : public Renderable
		//{
		//	/*ADResource::AD_AI::AI ai;
		//	int health;
		//	void Update()
		//	{
		//		ai.Update();
		//	}
		//	void Damage(ADResource::ADGameplay::DAMAGE_TYPE damageType) override
		//	{
		//		if (defenseType != ADResource::ADGameplay::INVULNERABLE && defenseType != damageType)
		//		{
		//			health--;
		//			if (health < 1)
		//			{
		//				Remove();
		//			}
		//		}
		//	};*/

		////public:
		////	ADPhysics::AABB collider;

		////	Enemy() { colliderPtr = &collider; type = OBJECT_TYPE::ENEMY; };

		////	virtual void CheckCollision(GameObject* obj)
		////	{
		////		if (this->active)
		////		{
		////			ADPhysics::Manifold m;
		////			obj->colliderPtr->isCollision(&collider, m);
		////		}
		////	}

		////	void Damage(DAMAGE_TYPE damageType)
		////	{
		////		if (defenseType != INVULNERABLE && defenseType != damageType)
		////		{
		////			Remove();
		////		}
		////	};
		//};
	}
}