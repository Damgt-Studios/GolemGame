#pragma once
#include "ADUserInterface.h"
#include "GameplayBaseClasses.h"
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
			FIRE,
			ALLY_HITBOX,
			ENEMY_HITBOX,
			PROJECTILE,
			EVENT_TRIGGER
		};

		enum STATS
		{
			HEALTH = 1,
			TOKENS,
			STORED_ESSENCE,
			ATTACK,
			SPEED,
			ARMOR,
			STRUCTURES_DESTROYED,		//we can get more specific when it comes time
		};


		struct StatSheet : public iStatSheet
		{
			Stat health = {5,99999,0, INT_MAX};

			Stat* RequestStats(UINT _statID)
			{
				switch (_statID)
				{
				case ADResource::ADGameplay::HEALTH:
					return &health;
					break;
				case ADResource::ADGameplay::TOKENS:
					break;
				case ADResource::ADGameplay::STORED_ESSENCE:
					break;
				case ADResource::ADGameplay::ATTACK:
					break;
				case ADResource::ADGameplay::SPEED:
					break;
				case ADResource::ADGameplay::ARMOR:
					break;
				case ADResource::ADGameplay::STRUCTURES_DESTROYED:
					break;
				default:
					break;
				}
			};
		};

		class Destructable : public Renderable
		{
			StatSheet* stats;
		public:
			ADPhysics::AABB collider;

			virtual void Update(float _deltaTime) 
			{
				ProcessEffects(_deltaTime);
			};

			Destructable() { colliderPtr = &collider; physicsType = OBJECT_PHYSICS_TYPE::COLLIDABLE; stats = new StatSheet(); }
			~Destructable() override
			{ 
				delete stats; 
			}

			virtual iStatSheet* GetStatSheet() override
			{ 
				return stats;
			};

			void CheckCollision(GameObject* obj) override
			{
				if (this->active)
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
				if (stats->health.currentValue <= 0)
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

		class Trigger : public Renderable
		{
		public:
			bool isDeactivateOnFirstApplication = false;
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

			void CheckCollision(GameObject* obj) override
			{
				if (active)
				{
					ADPhysics::Manifold m;
					if (obj->colliderPtr->isCollision(&collider, m))
					{
						if (obj->team != team)
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

		class Attack
		{
		public:
			bool active;
			float cooldownDuration;
			float cooldownTimer;
			float attackDuration;
			float attackTimer;
			Trigger* hitbox;
			UINT hitboxCount;

			//If the attack doesn't own the hitbox this needs to change.
			~Attack() { delete hitbox; };

			void StartAttack()
			{
				if (cooldownTimer <= 0 && attackTimer <= 0)
				{
					if (hitbox)
					{
						cooldownTimer = cooldownDuration;
						attackTimer = attackDuration;
						for (int i = 0; i < hitboxCount; ++i)
						{
							hitbox[i].Enable();
						}
					}
				}

			}

			void Update(float _deltaTime)
			{
				if (cooldownTimer > 0)
				{
					cooldownTimer -= _deltaTime;
				}
				if (attackTimer > 0)
				{
					attackTimer -= _deltaTime;
					if (attackTimer <= 0)
					{
						EndAttack();
					}
				}
			}
			
			void EndAttack()
			{
				//Some hit boxes would turn off this way, others require they burn out or collide.
				//if (hitbox)
				//{
				//	for (int i = 0; i < hitboxCount; ++i)
				//	{
				//		hitbox[i].active = false;
				//	}
				//}
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