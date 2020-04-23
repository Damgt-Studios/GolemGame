#include "pchgame.h"
#include "Spyro.h"

void ADResource::ADGameplay::Spyro::Update(float delta_time)
{
	HandleInput(delta_time);

	// GAMEPLAY
	// Jumping
	//if (jumping)
	//{
	//	jump_count_down += delta_time;

	//	if (jump_count_down < jump_time_length)
	//	{
	//		if (jump_count_down <= jump_time_up)
	//		{
	//			// Jump completion ratio
	//			float jump_up_ratio = jump_count_down / jump_time_up;

	//			transform.r[3].m128_f32[1] =
	//				lerp(GetPosition().y, jump_height, jump_up_ratio);
	//		}
	//		else
	//		{
	//			transform.r[3].m128_f32[1] = GetPosition().y - (gravity * delta_time);
	//		}
	//	}

	//	if (GetPosition().y <= 0)
	//	{
	//		jumping = false;
	//		jump_count_down = 0;
	//		transform.r[3].m128_f32[1] = 0;
	//		og_y_pos = GetPosition().y;
	//	}
	//}

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
	Manifold m;

	if (AabbToAabbCollision(collider, item, m))
	{
		XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
		PhysicsMaterial temp(0, 0, 0);
		VelocityImpulse(Velocity, mat, tempV, temp, m);
		PositionalCorrection((XMFLOAT4&)transform.r[3], mat, tempV, temp, m);

		float Dot = VectorDot(XMFLOAT3(collider.Pos.x - item.Pos.x, collider.Pos.y - item.Pos.y, collider.Pos.z - item.Pos.z), XMFLOAT3(0,1,0));

		if (Dot > 0.5f)
			jumping = false;;
	}
}

void ADResource::ADGameplay::Spyro::CheckCollision(Plane& item)
{
	Manifold m;

	if (AabbToPlaneCollision(collider, item, m))
	{
		XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
		PhysicsMaterial temp(0, 0, 0);
		VelocityImpulse(tempV, temp, Velocity, mat, m);
		PositionalCorrection(tempV, temp, (XMFLOAT4&)transform.r[3], mat, m);
		jumping = false;
	}
}

void ADResource::ADGameplay::Spyro::HandleInput(float delta_time)
{
	XMFLOAT3 pos(0, 0, 0);

	if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::UP)
	{
		Velocity.z += spyro_move_speed * delta_time;
		
	}
	else if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::DOWN)
	{
		Velocity.z += -spyro_move_speed * delta_time;
	}

	if (Input::QueryThumbSticLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::LEFT)
	{
		Velocity.x += -spyro_move_speed * delta_time;
	}
	else if (Input::QueryThumbSticLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) == (int)Input::DIRECTION::RIGHT)
	{
		Velocity.x += spyro_move_speed * delta_time;
	}

	if (Velocity.y > maxDownwardVelocity)
		Velocity.y += Gravity * delta_time * floatiness;
	else
		Velocity.y = maxDownwardVelocity;

	AddToPositionVector((XMFLOAT3&)Velocity);

	Velocity.x = 0;
	Velocity.z = 0;

	// Actions
	if (Input::QueryButtonDown(GamepadButtons::A) && !jumping)
	{
		/*jumping = true;
		og_y_pos = GetPosition().y;*/
		jumping = true;
		Velocity.y = (-Gravity * delta_time * floatiness) * 20;
	}

}