#include "pchgame.h"
#include "Spyro.h"

void ADResource::ADGameplay::Spyro::Update(float delta_time)
{
	HandleInput(delta_time);

	// GAMEPLAY
	// Jumping
	if (jumping)
	{
		jump_count_down += delta_time;

		if (jump_count_down < jump_time_length)
		{
			if (jump_count_down <= jump_time_up)
			{
				// Jump completion ratio
				float jump_up_ratio = jump_count_down / jump_time_up;

				transform.r[3].m128_f32[1] =
					lerp(GetPosition().y, jump_height, jump_up_ratio);
			}
			else
			{
				transform.r[3].m128_f32[1] = GetPosition().y - (gravity * delta_time);
			}
		}

		if (GetPosition().y <= 0)
		{
			jumping = false;
			jump_count_down = 0;
			transform.r[3].m128_f32[1] = 0;
			og_y_pos = GetPosition().y;
		}
	}

	// Physics
	collider = AABB(GetPosition(), XMFLOAT3(2, 2, 2));
}

void ADResource::ADGameplay::Spyro::Damage(DAMAGE_TYPE d_type)
{

}

void ADResource::ADGameplay::Spyro::Remove()
{

}

void ADResource::ADGameplay::Spyro::CheckCollision(AABB& item)
{
	if (AabbToAabbCollision(collider, item))
	{
		jumping = false;
		jump_count_down = 0;
		og_y_pos = og_y_pos = GetPosition().y;
	}
}

void ADResource::ADGameplay::Spyro::HandleInput(float delta_time)
{
	XMFLOAT3 pos(0, 0, 0);

	if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::UP)
	{
		pos.z += spyro_move_speed * delta_time;
		
	}
	else if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::DOWN)
	{
		pos.z += -spyro_move_speed * delta_time;
	}

	if (Input::QueryThumbSticLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::LEFT)
	{
		pos.x += -spyro_move_speed * delta_time;
	}
	else if (Input::QueryThumbSticLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::RIGHT)
	{
		pos.x += spyro_move_speed * delta_time;
	}

	AddToPositionVector(pos);

	// Actions
	if (Input::QueryButtonDown(GamepadButtons::A) && !jumping)
	{
		jumping = true;
		og_y_pos = GetPosition().y;
	}

}