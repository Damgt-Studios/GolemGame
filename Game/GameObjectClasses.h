#pragma once
#include "ADUserInterface.h"
#include "GameplayBaseClasses.h"
#include <Types.h>

namespace ADResource
{
	namespace ADGameplay
	{
		enum class ObjectTag
		{
			Spyro = 0,
			Gem,

		};

		class Collectable : public Renderable
		{
		public:
			ADPhysics::AABB collider;

			Collectable() { 
				colliderPtr = &collider; type = OBJECT_TYPE::GEM; colliderPtr->trigger = true; GemCount = 1;
			};



			virtual void CheckCollision(GameObject* obj) 
			{
				if (this->active)
				{
					ADPhysics::Manifold m;
					obj->colliderPtr->isCollision(&collider, m);
				}
			}
		};

		class Trigger : public Renderable
		{
		public:
			ADPhysics::AABB collider;

			Trigger() { colliderPtr = &collider; type = OBJECT_TYPE::TRIGGER; colliderPtr->trigger = true; };

			virtual void CheckCollision(GameObject* obj)
			{
				if (this->active)
				{
					ADPhysics::Manifold m;
					obj->colliderPtr->isCollision(&collider, m);
				}
			}
		};

		class Hitboxe : public Renderable
		{

		};

		class Destructable : public Renderable
		{

		};

		class Enemy : public Renderable
		{
			/*ADResource::AD_AI::AI ai;
			int health;
			void Update()
			{
				ai.Update();
			}
			void Damage(ADResource::ADGameplay::DAMAGE_TYPE damageType) override
			{
				if (defenseType != ADResource::ADGameplay::INVULNERABLE && defenseType != damageType)
				{
					health--;
					if (health < 1)
					{
						Remove();
					}
				}
			};*/

		public:
			ADPhysics::AABB collider;

			Enemy() { colliderPtr = &collider; type = OBJECT_TYPE::ENEMY; };

			virtual void CheckCollision(GameObject* obj)
			{
				if (this->active)
				{
					ADPhysics::Manifold m;
					obj->colliderPtr->isCollision(&collider, m);
				}
			}

			void Damage(DAMAGE_TYPE damageType)
			{
				if (defenseType != INVULNERABLE && defenseType != damageType)
				{
					Remove();
				}
			};
		};
	}
}