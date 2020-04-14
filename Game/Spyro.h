#pragma once
#include "Types.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Camera.h"

using namespace ADResource::ADGameplay;
using namespace ADPhysics;

namespace ADResource
{
	namespace ADGameplay
	{
		class Spyro : public GameObject
		{
		
		public:
			virtual void Update(float time_delta);

			void Damage(DAMAGE_TYPE d_type);

			void Remove();

			void CheckCollision(AABB& item);

		private:
			void HandleInput(float delta_time);

		private:
			int health;

			XMFLOAT3 spryo_movement_deltas = XMFLOAT3(0, 0, 0);

			AABB collider;

			// Gameplay
			float jump_time_up = .4;
			float jump_time_length = 1;
			float jump_count_down = 0;
			float jump_height = 15;
			float og_y_pos = 0;
			float gravity = 5;
			bool jumping = false;

			// Turning
			float spyro_turn_speed = 5;
			float spyro_move_speed = 5;
		};
	}
}