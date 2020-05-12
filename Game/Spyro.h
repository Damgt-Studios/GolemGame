#pragma once
#include "GameplayBaseClasses.h"

using namespace ADResource::ADGameplay;
using namespace ADPhysics;

namespace ADResource
{
	namespace ADGameplay
	{

		class Spyro : public Renderable
		{
		
		public:
			Spyro();

			virtual void Update(float time_delta);

			void Damage(DAMAGE_TYPE d_type);

			void Remove();

			void CheckCollision(GameObject* obj);

			virtual void OnCollision(GameObject* other);
			virtual void OnTrigger(GameObject* other);

			OBB collider = OBB(transform, XMFLOAT3(2, 2, 2));
			PhysicsMaterial mat = PhysicsMaterial(1, 1, 0.5f);
			bool jumping = false;


		private:
			void HandleInput(float delta_time);

		private:
			int health;

			XMFLOAT3 spryo_movement_deltas = XMFLOAT3(0, 0, 0);


			// Gameplay
			float jump_time_up = .4;
			float jump_time_length = 1;
			float jump_count_down = 0;
			float jump_height = 15;
			float og_y_pos = 0;
			float gravity = 50;

			float floatiness = 0.25f;

			// Turning
			float spyro_turn_speed = 5;
			float spyro_move_speed = 5;
		};
	}
}

namespace ADResource
{
	namespace ADGameplay
	{
		
	}
}