#pragma once
#include "ADUserInterface.h"
#include "GameplayBaseClasses.h"
#include <Types.h>

namespace ADResource
{
	namespace ADGameplay
	{
		class Collectable : public Renderable
		{
		public:
			ADPhysics::AABB collider;

			virtual void CheckCollision(GameObject* obj) 
			{
				if (this->active)
				{
					ADPhysics::Manifold m;

					if (obj->colliderPtr->isCollision(&collider, m))
					{
							ADResource::AD_UI::UIMessage message;
							message.controllerID = 1;
							message.messageType = 2;
							message.number = 1;
						
							ADUI::MessageReceiver::SendMessage(&message);
							Remove();
					}
				}
				

			}
		};

		class Trigger : public Renderable
		{
		public:
			ADPhysics::AABB collider;

			virtual void CheckCollision(GameObject* obj)
			{
				if (this->active)
				{
					ADPhysics::Manifold m;

					if (obj->colliderPtr->isCollision(&collider, m))
					{
						ADResource::AD_UI::UIMessage message;
						message.controllerID = 1;
						message.messageType = 2;
						message.number = 3;

						ADUI::MessageReceiver::SendMessage(&message);
						Remove();
					}
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

			virtual void CheckCollision(GameObject* obj)
			{
				if (this->active)
				{
					ADPhysics::Manifold m;

					if (obj->colliderPtr->isCollision(&collider, m))
					{
						ADResource::AD_UI::UIMessage message;
						message.controllerID = 1;
						message.messageType = 2;
						message.number = 2;

						ADUI::MessageReceiver::SendMessage(&message);
						Remove();
					}
				}
			}
		};
	}
}