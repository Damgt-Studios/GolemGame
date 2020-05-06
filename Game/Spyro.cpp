#include "pchgame.h"
#include "Spyro.h"

ADResource::ADGameplay::Spyro::Spyro() {
	collider = OBB(transform, XMFLOAT3(2, 2, 2));
	colliderPtr = &collider;
}

void ADResource::ADGameplay::Spyro::Update(float delta_time)
{
	HandleInput(delta_time);

	// Physics
	collider = OBB(transform, XMFLOAT3(2,2,2));
	colliderPtr = &collider;
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

	//Function is mainly for gameplay
}

void ADResource::ADGameplay::Spyro::OnCollision(GameObject* other) 
{
	//Do whatever we need upon collision

	//Function is mainly for gameplay

	//Sample of what to do with what we have right now
	if (other->colliderPtr->type == ColliderType::Plane || other->colliderPtr->type == ColliderType::Aabb)
	{
		float Dot = VectorDot(XMFLOAT3(
			collider.Pos.x - other->colliderPtr->Pos.x,
			collider.Pos.y - other->colliderPtr->Pos.y,
			collider.Pos.z - other->colliderPtr->Pos.z), XMFLOAT3(0, 1, 0));

		if (Dot > 0.5f)
			jumping = false;
	}
}

void ADResource::ADGameplay::Spyro::CheckCollision(GameObject* obj) 
{
	Manifold m;

	if (obj->active) 
	{
		if (obj->colliderPtr->isCollision(&collider, m))
		{
			//If collision and collision object is a trigger then go to OnTrigger Function
			if (obj->colliderPtr->trigger)
			{
				OnTrigger(obj);
			}
			//If collision and collision object is a collider then go to OnCollision Function
			else
			{
				collisionQueue.push(CollisionPacket(this, obj, m));
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