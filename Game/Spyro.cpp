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

void ADResource::ADGameplay::Spyro::GetView(XMMATRIX& view)
{
	camera = view;
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

		float Dot = VectorDot(XMFLOAT3(collider.Pos.x - item.Pos.x, collider.Pos.y - item.Pos.y, collider.Pos.z - item.Pos.z), XMFLOAT3(0, 1, 0));

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
	if (Input::QueryButtonDown(GamepadButtons::Y))
	{
		spyro_move_speed = 50;

	}
	else
	{

		spyro_move_speed = 30;
	}


	if ((Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) && Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK))|| Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) || Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) )
	{
		
		float angle = atan2(Input::QueryThumbStickValueExactX(Input::THUMBSTICKS::LEFT_THUMBSTICK), 
			Input::QueryThumbStickValueExactY(Input::THUMBSTICKS::LEFT_THUMBSTICK) );


		Spyro::RotationYBasedOnView(camera,angle, WMATH_PI);


	
	XMFLOAT4 forward;
	XMStoreFloat4(&forward, Spyro::transform.r[2]);



		Velocity.x += forward.x * delta_time * spyro_move_speed;
		Velocity.y += forward.y * delta_time * spyro_move_speed;
		Velocity.z += forward.z * delta_time * spyro_move_speed;


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