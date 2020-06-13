#include "pchgame.h"
#include "Golem.h"

// Public Methods
//Constructor and Destructor
ADResource::ADGameplay::Golem::Golem() {
	collider = OBB(transform * translateToMiddle, XMFLOAT3(20, 60, 20));
	colliderPtr = &collider;

	stats = new StatSheet();
	InitAnims();

	flockingGroups = new ADAI::FlockingGroup * [5];
	for (int i = 0; i < 5; ++i)
	{
		flockingGroups[i] = new ADAI::FlockingGroup();
		flockingGroups[i]->groupTarget = &transform;
	}
}

ADResource::ADGameplay::Golem::~Golem()
{
	for (int i = 0; i < 5; ++i)
	{
		delete flockingGroups[i];
	}
	delete[] flockingGroups;
}


// Update Methods
void ADResource::ADGameplay::Golem::Update(float delta_time)
{
	HandleInput(delta_time);
	ProcessEffects(delta_time);

	for (int i = 0; i < 5; ++i)
	{
		flockingGroups[i]->Update(delta_time);
	}

	// Physics
	XMMATRIX colliderLocation = transform;
	colliderLocation.r[3].m128_f32[1] += 15;

	colliderLocation.r[0] = XMVector3Normalize(transform.r[0]);
	colliderLocation.r[1] = XMVector3Normalize(transform.r[1]);
	colliderLocation.r[2] = XMVector3Normalize(transform.r[2]);

	collider = OBB(colliderLocation, XMFLOAT3(20, 60, 20));
	colliderPtr = &collider;

	if (invulnerableTimer > 0)
	{
		invulnerableTimer -= delta_time;
		if (invulnerableTimer <= 0)
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

//Checks collision, If collides with a trigger, calls OnTrigger, if colliders with a collider, calls OnCollision
void ADResource::ADGameplay::Golem::CheckCollision(GameObject* obj)
{
	Manifold m;

	if (obj->active)
	{
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
				collisionQueue.push(CollisionPacket(obj, this, m));
				OnCollision(obj);
			}
		}
	}
}

void ADResource::ADGameplay::Golem::OnCollision(GameObject* other)
{
	//Sample of what to do with what we have right now
	if (other->colliderPtr->type == ColliderType::Plane || other->colliderPtr->type == ColliderType::Aabb)
	{
		float Dot = VectorDot(XMFLOAT3(
			collider.Pos.x - other->colliderPtr->Pos.x,
			collider.Pos.y - other->colliderPtr->Pos.y,
			collider.Pos.z - other->colliderPtr->Pos.z), XMFLOAT3(0, 1, 0));
	}
}

void ADResource::ADGameplay::Golem::Remove()
{

}


// Accessors
void ADResource::ADGameplay::Golem::GetView(XMMATRIX& view)
{
	camera = view;
}

void ADResource::ADGameplay::Golem::GetAnimationController(AnimationStateMachine& controller)
{
	anim_controller = &controller;
}

XMMATRIX ADResource::ADGameplay::Golem::GetColliderInfo()
{
	XMMATRIX temp;
	temp.r[0] = XMVector3Normalize(Float3ToVector(collider.AxisX));
	temp.r[1] = XMVector3Normalize(Float3ToVector(collider.AxisY));
	temp.r[2] = XMVector3Normalize(Float3ToVector(collider.AxisZ));
	temp.r[3] = Float3ToVector(collider.Pos);
	temp.r[3].m128_f32[3] = 1;

	temp = XMMatrixScaling(collider.GetWidth() / 2, collider.GetHeight() / 2, collider.GetWidth() / 2) * temp;

	return temp;
}

iStatSheet* ADResource::ADGameplay::Golem::GetStatSheet()
{
	return stats;
}

int ADResource::ADGameplay::Golem::GetCurrentElement()
{
	return playerElement;
}


// Private Methods
void ADResource::ADGameplay::Golem::HandleInput(float delta_time)
{
	playerElement = 3;
	// Idle Animation
	if (!isActing)
	{
		if (idleTime > 10.0)
			anim_controller->PlayAnimationByName(anims[playerElement].idleLook.c_str());
		else
			anim_controller->PlayAnimationByName(anims[playerElement].idle.c_str());
		idleTime += delta_time;
	}

	XMFLOAT3 pos(0, 0, 0);

	responseTimer -= delta_time;
	currentAnimTime -= delta_time;

	if (currentAnimTime <= 0.0)
		isActing = false;

	// Golem Tower Punch
	if (Input::QueryButtonDown(GamepadButtons::X) && !isActing)
	{
		TowerPunch();
	}

	// Golem Ground Slam
	if (Input::QueryButtonDown(GamepadButtons::A) && !isActing)
	{
		GroundSlam();
	}

	// Golem Kick
	if (Input::QueryButtonDown(GamepadButtons::B) && !isActing)
	{
		Kick();
	}

	// Golem Special Move
	if (Input::QueryButtonDown(GamepadButtons::Y) && !isActing && specialCoins > 0)
	{
		PerformSpecial();
	}

	// Increment Player Element
	if (Input::QueryButtonDown(GamepadButtons::RightShoulder))
	{
		ChangeElement(true);
	}

	// Decrement Player Element
	if (Input::QueryButtonDown(GamepadButtons::LeftShoulder))
	{
		ChangeElement(false);
	}

	// Move up minions list
	if (Input::QueryButtonDown(GamepadButtons::DPadDown))
	{
		ChangeMinionGroup(true);
	}

	// Move down minions list
	if (Input::QueryButtonDown(GamepadButtons::DPadUp))
	{
		ChangeMinionGroup(false);
	}

	// Recall Minions
	if (Input::QueryTriggerUpDown(Input::TRIGGERS::LEFT_TRIGGER, 0.1f))
	{
		RecallMinions();
	}

	// Send Minions Ahead
	if (Input::QueryTriggerUpDown(Input::TRIGGERS::RIGHT_TRIGGER, 0.1f) && !isActing)
	{
		CommandMinions();
	}

	// Golem Movement
	XMFLOAT4 forward;
	XMStoreFloat4(&forward, Golem::transform.r[2]);
	if ((Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) || Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK)) && !isActing)
	{
		MoveGolem(forward, delta_time);
	}

	AddToPositionVector((XMFLOAT3&)Velocity);

	Velocity.x = 0;
	Velocity.z = 0;
}

void ADResource::ADGameplay::Golem::InitAnims()
{
	anims[0].command = "Golem_3_Command";
	anims[0].death = "Golem_3_Death";
	anims[0].eat = "Golem_3_Eat";
	anims[0].groundSlam = "Golem_3_GroundSlam";
	anims[0].heavyHitBack = "Golem_3_Heavy_Hit_Back";
	anims[0].heavyHitFront = "Golem_3_Heavy_Hit_Front";
	anims[0].heavyHitLeft = "Golem_3_Heavy_Hit_Left";
	anims[0].heavyHitRight = "Golem_3_Heavy_Hit_Right";
	anims[0].idle = "Golem_3_Idle";
	anims[0].idleLook = "Golem_3_IdleLook";
	anims[0].kick = "Golem_3_Kick";
	anims[0].run = "Golem_3_Run";
	anims[0].special = "Golem_3_Special";
	anims[0].summonMinions = "Golem_3_SummonMinions";
	anims[0].towerPunch = "Golem_3_TowerPunch";

	anims[1].command = "Golem_4_Command";
	anims[1].death = "Golem_4_Death";
	anims[1].eat = "Golem_4_Eat";
	anims[1].groundSlam = "Golem_4_GroundSlam";
	anims[1].heavyHitBack = "Golem_4_Heavy_Hit_Back";
	anims[1].heavyHitFront = "Golem_4_Heavy_Hit_Front";
	anims[1].heavyHitLeft = "Golem_4_Heavy_Hit_Left";
	anims[1].heavyHitRight = "Golem_4_Heavy_Hit_Right";
	anims[1].idle = "Golem_4_Idle";
	anims[1].idleLook = "Golem_4_IdleLook";
	anims[1].kick = "Golem_4_Kick";
	anims[1].run = "Golem_4_Run";
	anims[1].special = "Golem_4_Special";
	anims[1].summonMinions = "Golem_4_SummonMinions";
	anims[1].towerPunch = "Golem_4_TowerPunch";

	anims[2].command = "Golem_2_Command";
	anims[2].death = "Golem_2_Death";
	anims[2].eat = "Golem_2_Eat";
	anims[2].groundSlam = "Golem_2_GroundSlam";
	anims[2].heavyHitBack = "Golem_2_Heavy_Hit_Back";
	anims[2].heavyHitFront = "Golem_2_Heavy_Hit_Front";
	anims[2].heavyHitLeft = "Golem_2_Heavy_Hit_Left";
	anims[2].heavyHitRight = "Golem_2_Heavy_Hit_Right";
	anims[2].idle = "Golem_2_Idle";
	anims[2].idleLook = "Golem_2_IdleLook";
	anims[2].kick = "Golem_2_Kick";
	anims[2].run = "Golem_2_Run";
	anims[2].special = "Golem_2_Special";
	anims[2].summonMinions = "Golem_2_SummonMinions";
	anims[2].towerPunch = "Golem_2_TowerPunch";

	anims[3].command = "Golem_1_Command";
	anims[3].death = "Golem_1_Death";
	anims[3].eat = "Golem_1_Eat";
	anims[3].groundSlam = "Golem_1_GroundSlam";
	anims[3].heavyHitBack = "Golem_1_Heavy_Hit_Back";
	anims[3].heavyHitFront = "Golem_1_Heavy_Hit_Front";
	anims[3].heavyHitLeft = "Golem_1_Heavy_Hit_Left";
	anims[3].heavyHitRight = "Golem_1_Heavy_Hit_Right";
	anims[3].idle = "Golem_1_Idle";
	anims[3].idleLook = "Golem_1_IdleLook";
	anims[3].kick = "Golem_1_Kick";
	anims[3].run = "Golem_1_Run";
	anims[3].special = "Golem_1_Special";
	anims[3].summonMinions = "Golem_1_SummonMinions";
	anims[3].towerPunch = "Golem_1_TowerPunch";
}

void ADResource::ADGameplay::Golem::MoveGolem(XMFLOAT4& forward, float delta_time)
{
	idleTime = 0.0;
	float angle = atan2(Input::QueryThumbStickValueExactX(Input::THUMBSTICKS::LEFT_THUMBSTICK),
		Input::QueryThumbStickValueExactY(Input::THUMBSTICKS::LEFT_THUMBSTICK));

	Golem::RotationYBasedOnView(camera, angle, WMATH_PI);
	anim_controller->PlayAnimationByName(anims[playerElement].run.c_str());


	Velocity.x += forward.x * delta_time * golemMoveSpeed;
	Velocity.y += forward.y * delta_time * golemMoveSpeed;
	Velocity.z += forward.z * delta_time * golemMoveSpeed;

	transform = XMMatrixScaling(0.1, 0.1, 0.1) * transform;
}

void ADResource::ADGameplay::Golem::PerformSpecial()
{
	anim_controller->PlayAnimationByName(anims[playerElement].special.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].special.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
	if (responseTimer < 0)
	{
		responseTimer = 0.2f;
		stats->token.currentValue--;
		if (stats->token.currentValue < stats->token.minValue)
			stats->token.currentValue = 0;
		--specialCoins;
	}
}

void ADResource::ADGameplay::Golem::TowerPunch()
{
	anim_controller->PlayAnimationByName(anims[playerElement].towerPunch.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].towerPunch.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
}

void ADResource::ADGameplay::Golem::GroundSlam()
{
	anim_controller->PlayAnimationByName(anims[playerElement].groundSlam.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].groundSlam.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
}

void ADResource::ADGameplay::Golem::Kick()
{
	anim_controller->PlayAnimationByName(anims[playerElement].kick.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].kick.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
}

void ADResource::ADGameplay::Golem::CommandMinions()
{
	anim_controller->PlayAnimationByName(anims[playerElement].command.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].command.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
	if (commandTargetGroup == 4)
	{
		for (int i = 0; i < 3; ++i)
		{
			flockingGroups[i]->SetCommandDirection(camera);
		}
		targetMarker->SetPosition(flockingGroups[3]->SetCommandDirection(camera));
	}
	else
	{
		targetMarker->SetPosition(flockingGroups[commandTargetGroup]->SetCommandDirection(camera));
	}
}

void ADResource::ADGameplay::Golem::RecallMinions()
{
	idleTime = 0.0;
	if (commandTargetGroup == 4)
	{
		for (int i = 0; i < 4; ++i)
		{
			flockingGroups[i]->ReturnCall();
		}
	}
	else
	{
		flockingGroups[commandTargetGroup]->ReturnCall();
	}
}

void ADResource::ADGameplay::Golem::ChangeElement(bool nextElement)
{
	if (nextElement)
	{
		idleTime = 0.0;
		if (responseTimer < 0)
		{
			responseTimer = 0.2f;
			++playerElement;
			if (playerElement == 4)
				playerElement = 0;
		}
	}
	else
	{
		idleTime = 0.0;
		if (responseTimer < 0)
		{
			responseTimer = 0.2f;
			--playerElement;
			if (playerElement < 0)
				playerElement = 3;
		}
	}
}

void ADResource::ADGameplay::Golem::ChangeMinionGroup(bool nextElement)
{
	if (nextElement)
	{
		idleTime = 0.0;
		if (responseTimer < 0)
		{
			responseTimer = 0.2f;
			++commandTargetGroup;
			if (commandTargetGroup == 5)
				commandTargetGroup = 4;
		}
	}
	else
	{
		if (responseTimer < 0)
		{
			responseTimer = 0.2f;
			--commandTargetGroup;
			if (commandTargetGroup < 0)
				commandTargetGroup = 0;
		}
	}
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