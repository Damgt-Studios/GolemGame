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

				ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.y =
					lerp(ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.y, jump_height, jump_up_ratio);
			}
			else
			{
				// Jump down ratio
				float jump_time_down = (jump_time_length - jump_time_up);
				float time_down = jump_count_down - jump_time_up;
				float jump_dn_ratio = (time_down / jump_time_down);

				ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.y =
					lerp(ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.y, og_y_pos, jump_dn_ratio);
			}
		}

		if (jump_count_down > jump_time_length)
		{
			jump_count_down = 0;
			jumping = false;

			ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.y = og_y_pos;
		}
	}

	// Physics
	collider = AABB(ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position, XMFLOAT3(2, 2, 2));
}

void ADResource::ADGameplay::Spyro::Render()
{
	ResourceManager::AddModelToRenderQueue(dynamic_cast<GameObject*>(this));
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
		og_y_pos = ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.y;
	}
}

void ADResource::ADGameplay::Spyro::HandleInput(float delta_time)
{
	XMFLOAT3 pos(0, 0, 0);

	if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::UP)
	{
		//ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.z += spyro_move_speed * delta_time;
		pos.z += spyro_move_speed * delta_time;
		transform.r[3].m128_f32[2] = spyro_move_speed * delta_time;
	}
	else if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::DOWN)
	{
		//ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.z += -spyro_move_speed * delta_time;
		pos.z += -spyro_move_speed * delta_time;
	}

	if (Input::QueryThumbSticLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::LEFT)
	{
		ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.x += -spyro_move_speed * delta_time;
	}
	else if (Input::QueryThumbSticLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::RIGHT)
	{
		ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.x += spyro_move_speed * delta_time;
	}

	//AddToPositionVector(pos);

	// Actions
	if (Input::QueryButtonDown(GamepadButtons::A) && !jumping)
	{
		jumping = true;
		og_y_pos = ResourceManager::GetModelPtrFromMeshId(GetMeshId())->position.y;
	}
}