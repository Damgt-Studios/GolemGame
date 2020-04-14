#pragma once
#include "Types.h"

using namespace ADResource::ADGameplay;

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

		private:
			void HandleInput();

		private:
			int health;

			// Turning
			float spyro_turn_speed = 5;
			float spyro_move_speed = 5;

			float z_plane_cooldown = .2;
			float z_plane_timer = 0;

			float x_plane_cooldown = .2;
			float x_plane_timer = 0;
		};
	}
}