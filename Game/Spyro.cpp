#include "pchgame.h"
#include "Spyro.h"

void ADResource::ADGameplay::Spyro::Update(float delta_time)
{
	HandleInput(delta_time);

	// Physics
	collider = AABB(GetPosition(), XMFLOAT3(2, 2, 2));
}

void ADResource::ADGameplay::Spyro::Damage(DAMAGE_TYPE d_type)
{

}

void ADResource::ADGameplay::Spyro::Remove()
{

}

void ADResource::ADGameplay::Spyro::OnTrigger(GameObject* other)
{
	//Do whatever we need upon trigger
}

void ADResource::ADGameplay::Spyro::OnCollision(GameObject* other) 
{
	//Do whatever we need upon collision
	jumping = false;
}

void ADResource::ADGameplay::Spyro::CheckCollision(GameObject* obj) 
{
	Manifold m;

	if (obj->active) 
	{
		if (obj->collider->isCollision(&collider, m))
		{
			if (obj->collider->trigger)
			{
				OnTrigger(obj);
			}
			else
			{
				float Dot = VectorDot(XMFLOAT3(collider.Pos.x - obj->collider->Pos.x, collider.Pos.y - obj->collider->Pos.y, collider.Pos.z - obj->collider->Pos.z), XMFLOAT3(0, 1, 0));

				if (Dot > 0.5f)
					jumping = false;

				CollisionPacket	temp;
				temp.A = this;
				temp.B = obj;
				temp.m = m;
				collisionQueue.push(temp);
				OnCollision(obj);
			}
		}
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