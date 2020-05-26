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

	if (invulnerable_timer > 0)
	{
		invulnerable_timer -= delta_time;
		if (invulnerable_timer <= 0)
		{
			defenseType = OBJECT_DEFENSE::NONE;
		}
	}
}

void ADResource::ADGameplay::Spyro::Damage(DAMAGE_TYPE d_type)
{
	--health;

	ADResource::AD_UI::UIMessage message;
	message.controllerID = 1;
	message.messageType = 2;
	message.number = 2;

	ADUI::MessageReceiver::SendMessage(&message);
	invulnerable_timer = invulnerable_peroid;
	defenseType = OBJECT_DEFENSE::INVULNERABLE;

	if (audioManager)
	{
		playingSound = true;
		collectionNoiseID = audioManager->PlayEffect(HurtSound);
		//if (playingSound)
		//{
		//	audioManager->ResumeEffect(HurtSound, collectionNoiseID);
		//}
	}
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
}

void ADResource::ADGameplay::Spyro::OnTrigger(GameObject* other)
{
	if (other->type == OBJECT_TYPE::GEM)
	{
		GemCount += other->GemCount;
		other->GemCount = 0;

		ADResource::AD_UI::UIMessage message;
		message.controllerID = 1;
		message.messageType = 2;
		message.number = 1;

		ADUI::MessageReceiver::SendMessage(&message);

		if (audioManager)
		{
			playingSound = true;
			collectionNoiseID = audioManager->PlayEffect(0);
			//if (playingSound)
			//{
			//	audioManager->ResumeEffect(0, collectionNoiseID);
			//}
		}
		other->Remove();
	}
	if (other->type == OBJECT_TYPE::TRIGGER)
	{
		if (GemCount >= 10)
		{
			ADResource::AD_UI::UIMessage message;
			message.controllerID = 1;
			message.messageType = 2;
			message.number = 3;

			ADUI::MessageReceiver::SendMessage(&message);

		}
	}
}

void ADResource::ADGameplay::Spyro::OnCollision(GameObject* other) 
{

	if (defenseType != OBJECT_DEFENSE::INVULNERABLE)
	{
		if (other->type == OBJECT_TYPE::ENEMY && !charging)
		{
			Damage(DAMAGE_TYPE::RAM);
		}
	}
	
	
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
				obj->Damage(DAMAGE_TYPE::RAM);
				if (audioManager)
				{
					if (playingSound)
					{
						audioManager->ResumeEffect(EnemyDeathSound, collectionNoiseID);
					}
					else
					{
						playingSound = true;
						collectionNoiseID = audioManager->PlayEffect(EnemyDeathSound);
					}
				}
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


void ADResource::ADGameplay::Spyro::SetAudio(AudioManager* _audioManager)
{
	audioManager = _audioManager;
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
	else if (Input::QueryButtonUp(GamepadButtons::B))
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
		ADPhysics::maxDownwardVelocity = -0.5f;
	}


	if (Input::QueryButtonUp(GamepadButtons::A))
	{
		buttonup = true;
		if (gliding == true)
		{
			gliding = false;
			floatiness = 0.25f;
			ADPhysics::maxDownwardVelocity = -20;
		}

	}

	//if (Velocity.y > maxDownwardVelocity)
	//	Velocity.y += Gravity * delta_time * floatiness;
	//else
	//	Velocity.y = maxDownwardVelocity;

	AddToPositionVector((XMFLOAT3&)Velocity);

	Velocity.x = 0;
	Velocity.z = 0;


	// Actions
	if (Input::QueryButtonDown(GamepadButtons::A) && !jumping)
	{
		//buttonup = false;
		//floatiness = 0.25f;
		//jumping = true;
		//Velocity.y = (-Gravity * delta_time * floatiness) * 20;




	}







}