#include "pchgame.h"
#include "Spyro.h"

ADResource::ADGameplay::Spyro::Spyro()
{
	collider = &colliderSpecific;
	typeID = ADResource::ADGameplay::GAME_OBJECT_TAGS::SPYRO;
}

void ADResource::ADGameplay::Spyro::Update(float delta_time)
{
	HandleInput(delta_time);

	// Physics
	colliderSpecific = AABB(GetPosition(), XMFLOAT3(2, 2, 2));
}

void ADResource::ADGameplay::Spyro::Damage(DAMAGE_TYPE d_type)
{

}

void ADResource::ADGameplay::Spyro::Remove()
{

}

void ADResource::ADGameplay::Spyro::CheckCollision(GameObject& _other)
{
	ADPhysics::Manifold m;
	if (_other.collider->ColliderCollision(&colliderSpecific, m))
	{
		ResolveCollision(_other, m);

		_other.ResolveCollision(*this, m);
	}
}

void ADResource::ADGameplay::Spyro::ResolveCollision(GameObject& _other, Manifold& m)
{
	if (!_other.isTrigger)
	{
		XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
		PhysicsMaterial temp(0, 0, 0);
		VelocityImpulse(Velocity, mat, tempV, temp, m);
		PositionalCorrection((XMFLOAT4&)transform.r[3], mat, tempV, temp, m);

		float Dot = VectorDot(XMFLOAT3(colliderSpecific.Pos.x - _other.collider->Pos.x, colliderSpecific.Pos.y - _other.collider->Pos.y, colliderSpecific.Pos.z - _other.collider->Pos.z), XMFLOAT3(0, 1, 0));

		if (Dot > 0.5f)
			jumping = false;
	}
};


void ADResource::ADGameplay::Spyro::CheckCollision(AABB& item)
{
	Manifold m;

	if (AabbToAabbCollision(colliderSpecific, item, m))
	{
		XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
		PhysicsMaterial temp(0, 0, 0);
		VelocityImpulse(Velocity, mat, tempV, temp, m);
		PositionalCorrection((XMFLOAT4&)transform.r[3], mat, tempV, temp, m);

		float Dot = VectorDot(XMFLOAT3(colliderSpecific.Pos.x - item.Pos.x, colliderSpecific.Pos.y - item.Pos.y, colliderSpecific.Pos.z - item.Pos.z), XMFLOAT3(0,1,0));

		if (Dot > 0.5f)
			jumping = false;;
	}
}

void ADResource::ADGameplay::Spyro::CheckCollision(Plane& item)
{
	Manifold m;

	if (AabbToPlaneCollision(colliderSpecific, item, m))
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