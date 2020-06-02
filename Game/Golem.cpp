#include "pchgame.h"
#include "Golem.h"

ADResource::ADGameplay::Golem::Golem() {
	collider = OBB(transform, XMFLOAT3(20, 25, 20));
	colliderPtr = &collider;

	chargeCollider = OBB(transform * translatetofront, XMFLOAT3(2, 2, 2));
	chargeCollider.trigger = true;
	chargeCPtr = &chargeCollider;

	fireCollider = OBB(transform * translatetofront, XMFLOAT3(2, 2, 2));
	fireCollider.trigger = true;

	fireCPtr = &fireCollider;

	stats = new StatSheet();
}

void ADResource::ADGameplay::Golem::Update(float delta_time)
{
	HandleInput(delta_time);

	ProcessEffects(delta_time);

	// Physics
	XMMATRIX colliderLocation = transform;
	colliderLocation.r[3].m128_f32[1] += 15;
	collider = OBB(colliderLocation, XMFLOAT3(20, 20, 20));
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
			//defenseType = OBJECT_DEFENSE::NONE;
		}
	}
}

void ADResource::ADGameplay::Golem::ProcessEffects(float _deltaTime)
{
	for (int i = 0; i < effects.size(); ++i)
	{
		effects[i].get()->Update(_deltaTime);

		std::string msg = std::to_string(effects[i].get()->tickTimer);
		ADUI::MessageReceiver::Log(msg);

		if (effects[i].get()->isFinished)
		{
			effects[i].get()->OnExit();
			effects.erase(effects.begin() + i);
			i--;
		}
	}
	if (stats->health.currentValue <= 0)
	{
		//Death();
	}
}

//void ADResource::ADGameplay::Golem::Damage(DAMAGE_TYPE d_type)
//{
//	--health;
//
//	ADResource::AD_UI::UIMessage message;
//	message.controllerID = 1;
//	message.messageType = 2;
//	message.number = 2;
//
//	ADUI::MessageReceiver::SendMessage(&message);
//	invulnerable_timer = invulnerable_peroid;
//	defenseType = OBJECT_DEFENSE::INVULNERABLE;
//
//	if (audioManager)
//	{
//		playingSound = true;
//		collectionNoiseID = audioManager->PlayEffect(HurtSound);
//		if (playingSound)
//		{
//			audioManager->ResumeEffect(HurtSound, collectionNoiseID);
//		}
//	}
//}

void ADResource::ADGameplay::Golem::Remove()
{

}

void ADResource::ADGameplay::Golem::GetView(XMMATRIX& view)
{
	camera = view;
}

//void ADResource::ADGameplay::Golem::OnTrigger(GameObject* other)
//{
//	if (other->type == OBJECT_TYPE::GEM)
//	{
//		GemCount += other->GemCount;
//		other->GemCount = 0;
//
//		ADResource::AD_UI::UIMessage message;
//		message.controllerID = 1;
//		message.messageType = 2;
//		message.number = 1;
//
//		ADUI::MessageReceiver::SendMessage(&message);
//
//		if (audioManager)
//		{
//			playingSound = true;
//			collectionNoiseID = audioManager->PlayEffect(0);
//			//if (playingSound)
//			//{
//			//	audioManager->ResumeEffect(0, collectionNoiseID);
//			//}
//		}
//		other->Remove();
//	}
//	if (other->type == OBJECT_TYPE::TRIGGER)
//	{
//		if (GemCount >= 10)
//		{
//			ADResource::AD_UI::UIMessage message;
//			message.controllerID = 1;
//			message.messageType = 2;
//			message.number = 3;
//
//			ADUI::MessageReceiver::SendMessage(&message);
//
//		}
//	}
//}

void ADResource::ADGameplay::Golem::OnCollision(GameObject* other)
{

	/*if (defenseType != OBJECT_DEFENSE::INVULNERABLE)
	{
		if (other->type == OBJECT_TYPE::ENEMY && !charging)
		{
			Damage(DAMAGE_TYPE::RAM);
		}
	}*/


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
void ADResource::ADGameplay::Golem::OnTriggerCharge(GameObject* other)
{

	if (charging == true)
	{

	}
}
void ADResource::ADGameplay::Golem::OnTriggerFire(GameObject* other)
{

	/*if (fire == true)
	{
		other->Damage(DAMAGE_TYPE::FIRE);



	}*/


}


//Checks collision, If collides with a trigger, calls OnTrigger, if colliders with a collider, calls OnCollision
void ADResource::ADGameplay::Golem::CheckCollision(GameObject* obj)
{
	Manifold m;

	if (obj->active)
	{
		if (charging == true)
		{
			if (obj->colliderPtr->isCollision(&chargeCollider, m))
			{
				OnTriggerCharge(obj);
				//obj->Damage(DAMAGE_TYPE::RAM);
				/*if (audioManager)
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
				}*/
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
				//OnTrigger(obj);
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


//void ADResource::ADGameplay::Golem::SetAudio(AudioManager* _audioManager)
//{
//	audioManager = _audioManager;
//}

iStatSheet* ADResource::ADGameplay::Golem::GetStatSheet()
{
	return stats;
}

int ADResource::ADGameplay::Golem::GetCurrentElement()
{
	return playerElement;
}

void ADResource::ADGameplay::Golem::HandleInput(float delta_time)
{
	XMFLOAT3 pos(0, 0, 0);
	if (Input::QueryButtonDown(GamepadButtons::X))
	{
		spyro_move_speed = 500;
		charging = true;

	}
	else
	{
		charging = false;

		spyro_move_speed = 500;
	}

	//if ((Input::QueryButtonDown(GamepadButtons::B) || Input::QueryTriggerUpDown(Input::TRIGGERS::RIGHT_TRIGGER) == 1) && fire == false)
	//{
	//	fire = true;
	//}
	//else if (Input::QueryButtonUp(GamepadButtons::B))
	//{
	//	fire = false;
	//}
	responseTimer -= delta_time;
	if (Input::QueryButtonDown(GamepadButtons::Y))
	{
		if (responseTimer < 0)
		{
			responseTimer = 0.2f;
			stats->token.currentValue--;
			if (stats->token.currentValue < stats->token.minValue)
				stats->token.currentValue = 0;
		}
	}
	if (Input::QueryButtonDown(GamepadButtons::RightShoulder))
	{
		if (responseTimer < 0)
		{
			responseTimer = 0.2f;
			++playerElement;
			if (playerElement == 4)
				playerElement = 0;
		}

	}

	if (Input::QueryButtonDown(GamepadButtons::LeftShoulder))
	{
		if (responseTimer < 0)
		{
			responseTimer = 0.2f;
			--playerElement;
			if (playerElement < 0)
				playerElement = 3;
		}
	}


	if (Input::QueryTriggerUpDown(Input::TRIGGERS::RIGHT_TRIGGER, 0.1f))
	{
		commandGroup->SetCommandDirection(XMMatrixInverse(nullptr, camera).r[3]);
	}

	if (Input::QueryTriggerUpDown(Input::TRIGGERS::LEFT_TRIGGER, 0.1f))
	{
		commandGroup->ReturnCall();
	}

	XMFLOAT4 forward;
	XMStoreFloat4(&forward, Golem::transform.r[2]);
	if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) || Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK))
	{

		float angle = atan2(Input::QueryThumbStickValueExactX(Input::THUMBSTICKS::LEFT_THUMBSTICK),
			Input::QueryThumbStickValueExactY(Input::THUMBSTICKS::LEFT_THUMBSTICK));

		Golem::RotationYBasedOnView(camera, angle, WMATH_PI);


		Velocity.x += forward.x * delta_time * spyro_move_speed;
		Velocity.y += forward.y * delta_time * spyro_move_speed;
		Velocity.z += forward.z * delta_time * spyro_move_speed;

		transform = XMMatrixScaling(0.1, 0.1, 0.1) * transform;
	}


	//if (Input::QueryButtonDown(GamepadButtons::A) && jumping == true && gliding == false && buttonup == true)
	//{
	//	gliding = true;
	//	floatiness = 0.05f;
	//	ADPhysics::maxDownwardVelocity = -0.5f;
	//}


	//if (Input::QueryButtonUp(GamepadButtons::A))
	//{
	//	buttonup = true;
	//	if (gliding == true)
	//	{
	//		gliding = false;
	//		floatiness = 0.25f;
	//		ADPhysics::maxDownwardVelocity = -20;
	//	}

	//}

	//if (Velocity.y > maxDownwardVelocity)
	//	Velocity.y += Gravity * delta_time * floatiness;
	//else
	//	Velocity.y = maxDownwardVelocity;

	AddToPositionVector((XMFLOAT3&)Velocity);

	Velocity.x = 0;
	Velocity.z = 0;


	// Actions
	//if (Input::QueryButtonDown(GamepadButtons::A) && !jumping)
	//{
	//	//buttonup = false;
	//	//floatiness = 0.25f;
	//	//jumping = true;
	//	//Velocity.y = (-Gravity * delta_time * floatiness) * 20;




	//}







}