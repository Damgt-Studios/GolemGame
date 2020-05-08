#include "pchgame.h"
#include "Spyro.h"

ADResource::ADGameplay::Spyro::Spyro() {
	collider = OBB(transform, XMFLOAT3(2, 2, 2));
	colliderPtr = &collider;

	chargeCollider = OBB(transform * translatetofront, XMFLOAT3(2, 2, 2));
	chargeCollider.trigger = true;
	chargeCPtr = &chargeCollider;

	fireCollider = OBB(transform * translatetofront, XMFLOAT3(2, 2, 2));
	fireCollider.trigger = true;

	fireCPtr = &fireCollider;



}

void ADResource::ADGameplay::Spyro::Update(float delta_time)
{
	HandleInput(delta_time);

	// Physics
	collider = OBB(transform, XMFLOAT3(2,2,2));
	colliderPtr = &collider;

	chargeCollider = OBB(transform * translatetofront, XMFLOAT3(2, 2, 2));
	chargeCollider.trigger = true;
	chargeCPtr = &chargeCollider;

	fireCollider = OBB(transform * translatetofront, XMFLOAT3(2, 2, 2));
	fireCollider.trigger = true;

	fireCPtr = &fireCollider;

}

void ADResource::ADGameplay::Spyro::Damage(DAMAGE_TYPE d_type)
{

}

void ADResource::ADGameplay::Spyro::Remove()
{

}

void ADResource::ADGameplay::Spyro::OnTrigger(GameObject* other)
{

}
void ADResource::ADGameplay::Spyro::GetView(XMMATRIX& view)
{
	camera = view;
}


void ADResource::ADGameplay::Spyro::CheckCollision(AABB& item)
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
void ADResource::ADGameplay::Spyro::OnTriggerCharge(GameObject* other)
{

	if (charging == true)
	{
		other->Damage(DAMAGE_TYPE::RAM);



	}
}
void ADResource::ADGameplay::Spyro::OnTriggerFire(GameObject* other)
{

	if (fire == true)
	{
		other->Damage(DAMAGE_TYPE::FIRE);



	}


}


//Checks collision, If collides with a trigger, calls OnTrigger, if colliders with a collider, calls OnCollision
void ADResource::ADGameplay::Spyro::CheckCollision(GameObject* obj) 
{
	Manifold m;

	if (obj->active) 
	{
		if (charging == true)
		{
			if (obj->colliderPtr->isCollision(&chargeCollider, m))
			{
				
				
					OnTriggerCharge(obj);
				
			}
		}
		else if (fire == true)
		{
			if (obj->colliderPtr->isCollision(&fireCollider, m))
			{


				OnTriggerFire(obj);

			}
		}

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
	if (Input::QueryButtonDown(GamepadButtons::X))
	{
		spyro_move_speed = 50;
		charging = true;

	}
	else
	{
		charging = false;

		spyro_move_speed = 30;
	}

	if ((Input::QueryButtonDown(GamepadButtons::B) || Input::QueryTriggerUpDown(Input::TRIGGERS::RIGHT_TRIGGER ) == 1)&& fire == false)
	{
		fire = true;
	}
	if (Input::QueryButtonUp(GamepadButtons::B))
	{
		fire = false;

	}
	
	XMFLOAT4 forward;
	XMStoreFloat4(&forward, Spyro::transform.r[2]);
	if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) || Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK))
	{

		float angle = atan2(Input::QueryThumbStickValueExactX(Input::THUMBSTICKS::LEFT_THUMBSTICK),
			Input::QueryThumbStickValueExactY(Input::THUMBSTICKS::LEFT_THUMBSTICK));


		Spyro::RotationYBasedOnView(camera, angle, WMATH_PI);


		Velocity.x += forward.x * delta_time * spyro_move_speed;
		Velocity.y += forward.y * delta_time * spyro_move_speed;
		Velocity.z += forward.z * delta_time * spyro_move_speed;


	}


	if (Input::QueryButtonDown(GamepadButtons::A) && jumping == true && gliding == false && buttonup == true)
	{


		gliding = true;
		floatiness = 0.05f;


	}


	if (Input::QueryButtonUp(GamepadButtons::A))
	{
		buttonup = true;
		if (gliding == true)
		{
			gliding = false;
			floatiness = 0.25f;
		}

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
		buttonup = false;
		floatiness = 0.25f;
		jumping = true;
		Velocity.y = (-Gravity * delta_time * floatiness) * 20;




	}







}