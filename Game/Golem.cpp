#include "pchgame.h"
#include "Golem.h"

// Public Methods
//Constructor and Destructor
ADResource::ADGameplay::Golem::Golem() {
	collider = OBB(transform * translateToMiddle, XMFLOAT3(20, 60, 20));
	colliderPtr = &collider;

	stats = DefinitionDatabase::Instance()->statsheetDatabase["GreatGolem"];
	InitAnims();
	InitActions();

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
	consume->Update(delta_time);
	for (int i = 0; i < 4; ++i)
	{
		gActions[i].kick->Update(delta_time);
		gActions[i].punch->Update(delta_time);
		gActions[i].slam->Update(delta_time);
		gActions[i].special->Update(delta_time);
	}
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
	if (stats->RequestStats("Health")->currentValue <= 0)
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

StatSheet* ADResource::ADGameplay::Golem::GetStatSheet()
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
	if (Input::QueryButtonDown(GamepadButtons::X) && !isActing && responseTimer < 0)
	{
		TowerPunch();
	}

	// Golem Ground Slam
	if (Input::QueryButtonDown(GamepadButtons::A) && !isActing && responseTimer < 0)
	{
		GroundSlam();
	}

	// Golem Kick
	if (Input::QueryButtonDown(GamepadButtons::B) && !isActing && responseTimer < 0)
	{
		Kick();
	}

	// Golem Special Move
	if (Input::QueryButtonDown(GamepadButtons::Y) && !isActing && responseTimer < 0 && stats->RequestStats("Token")->currentValue > stats->RequestStats("Token")->minValue)
	{
		PerformSpecial();
	}

	// Increment Player Element
	if (Input::QueryButtonDown(GamepadButtons::RightShoulder) && responseTimer < 0)
	{
		ChangeElement(true);
	}

	// Decrement Player Element
	if (Input::QueryButtonDown(GamepadButtons::LeftShoulder) && responseTimer < 0)
	{
		ChangeElement(false);
	}

	// Move up minions list
	if (Input::QueryButtonDown(GamepadButtons::DPadDown) && responseTimer < 0)
	{
		ChangeMinionGroup(true);
	}

	// Move down minions list
	if (Input::QueryButtonDown(GamepadButtons::DPadUp) && responseTimer < 0)
	{
		ChangeMinionGroup(false);
	}

	// Consume Minion
	if (Input::QueryButtonDown(GamepadButtons::DPadLeft) && !isActing && responseTimer < 0)
	{
		ConsumeMinion();
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
	anims[STONE].born = "Golem_3_Born";
	anims[STONE].command = "Golem_3_Command";
	anims[STONE].death = "Golem_3_Death";
	anims[STONE].eat = "Golem_3_Eat";
	anims[STONE].groundSlam = "Golem_3_GroundSlam";
	anims[STONE].heavyHitBack = "Golem_3_Heavy_Hit_Back";
	anims[STONE].heavyHitFront = "Golem_3_Heavy_Hit_Front";
	anims[STONE].heavyHitLeft = "Golem_3_Heavy_Hit_Left";
	anims[STONE].heavyHitRight = "Golem_3_Heavy_Hit_Right";
	anims[STONE].idle = "Golem_3_Idle";
	anims[STONE].idleLook = "Golem_3_IdleLook";
	anims[STONE].kick = "Golem_3_Kick";
	anims[STONE].run = "Golem_3_Run";
	anims[STONE].special = "Golem_3_Special";
	anims[STONE].summonMinions = "Golem_3_SummonMinions";
	anims[STONE].towerPunch = "Golem_3_TowerPunch";

	anims[WATER].born = "Golem_4_Born";
	anims[WATER].command = "Golem_4_Command";
	anims[WATER].death = "Golem_4_Death";
	anims[WATER].eat = "Golem_4_Eat";
	anims[WATER].groundSlam = "Golem_4_GroundSlam";
	anims[WATER].heavyHitBack = "Golem_4_Heavy_Hit_Back";
	anims[WATER].heavyHitFront = "Golem_4_Heavy_Hit_Front";
	anims[WATER].heavyHitLeft = "Golem_4_Heavy_Hit_Left";
	anims[WATER].heavyHitRight = "Golem_4_Heavy_Hit_Right";
	anims[WATER].idle = "Golem_4_Idle";
	anims[WATER].idleLook = "Golem_4_IdleLook";
	anims[WATER].kick = "Golem_4_Kick";
	anims[WATER].run = "Golem_4_Run";
	anims[WATER].special = "Golem_4_Special";
	anims[WATER].summonMinions = "Golem_4_SummonMinions";
	anims[WATER].towerPunch = "Golem_4_TowerPunch";

	anims[FIRE].born = "Golem_2_Born";
	anims[FIRE].command = "Golem_2_Command";
	anims[FIRE].death = "Golem_2_Death";
	anims[FIRE].eat = "Golem_2_Eat";
	anims[FIRE].groundSlam = "Golem_2_GroundSlam";
	anims[FIRE].heavyHitBack = "Golem_2_Heavy_Hit_Back";
	anims[FIRE].heavyHitFront = "Golem_2_Heavy_Hit_Front";
	anims[FIRE].heavyHitLeft = "Golem_2_Heavy_Hit_Left";
	anims[FIRE].heavyHitRight = "Golem_2_Heavy_Hit_Right";
	anims[FIRE].idle = "Golem_2_Idle";
	anims[FIRE].idleLook = "Golem_2_IdleLook";
	anims[FIRE].kick = "Golem_2_Kick";
	anims[FIRE].run = "Golem_2_Run";
	anims[FIRE].special = "Golem_2_Special";
	anims[FIRE].summonMinions = "Golem_2_SummonMinions";
	anims[FIRE].towerPunch = "Golem_2_TowerPunch";

	anims[WOOD].born = "Golem_1_Born";
	anims[WOOD].command = "Golem_1_Command";
	anims[WOOD].death = "Golem_1_Death";
	anims[WOOD].eat = "Golem_1_Eat";
	anims[WOOD].groundSlam = "Golem_1_GroundSlam";
	anims[WOOD].heavyHitBack = "Golem_1_Heavy_Hit_Back";
	anims[WOOD].heavyHitFront = "Golem_1_Heavy_Hit_Front";
	anims[WOOD].heavyHitLeft = "Golem_1_Heavy_Hit_Left";
	anims[WOOD].heavyHitRight = "Golem_1_Heavy_Hit_Right";
	anims[WOOD].idle = "Golem_1_Idle";
	anims[WOOD].idleLook = "Golem_1_IdleLook";
	anims[WOOD].kick = "Golem_1_Kick";
	anims[WOOD].run = "Golem_1_Run";
	anims[WOOD].special = "Golem_1_Special";
	anims[WOOD].summonMinions = "Golem_1_SummonMinions";
	anims[WOOD].towerPunch = "Golem_1_TowerPunch";
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
	responseTimer = 0.2f;
	gActions[playerElement].special->StartAction(&transform);

	stats->RequestStats("Token")->currentValue--;
	if (stats->RequestStats("Token")->currentValue < stats->RequestStats("Token")->minValue)
	{
		stats->RequestStats("Token")->currentValue = 0;
	}
}

void ADResource::ADGameplay::Golem::TowerPunch()
{
	anim_controller->PlayAnimationByName(anims[playerElement].towerPunch.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].towerPunch.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
	responseTimer = 0.2f;
	gActions[playerElement].punch->StartAction(&transform);
}

void ADResource::ADGameplay::Golem::GroundSlam()
{
	anim_controller->PlayAnimationByName(anims[playerElement].groundSlam.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].groundSlam.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
	responseTimer = 0.2f;
	gActions[playerElement].slam->StartAction(&transform);
}

void ADResource::ADGameplay::Golem::Kick()
{
	anim_controller->PlayAnimationByName(anims[playerElement].kick.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].kick.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
	responseTimer = 0.2f;
	gActions[playerElement].kick->StartAction(&transform);
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
		responseTimer = 0.2f;
		++playerElement;
		if (playerElement == 4)
			playerElement = 0;
	}
	else
	{
		idleTime = 0.0;
		responseTimer = 0.2f;
		--playerElement;
		if (playerElement < 0)
			playerElement = 3;
	}
}

void ADResource::ADGameplay::Golem::ChangeMinionGroup(bool nextElement)
{
	if (nextElement)
	{
		idleTime = 0.0;
		responseTimer = 0.2f;
		++commandTargetGroup;
		if (commandTargetGroup == 5)
			commandTargetGroup = 4;
	}
	else
	{
		idleTime = 0.0;
		responseTimer = 0.2f;
		--commandTargetGroup;
		if (commandTargetGroup < 0)
			commandTargetGroup = 0;
	}
}

void ADResource::ADGameplay::Golem::ConsumeMinion()
{
	anim_controller->PlayAnimationByName(anims[playerElement].eat.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].eat.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
	responseTimer = 0.2f;
	consume->StartAction(&transform);
}

void ADResource::ADGameplay::Golem::SummonMinions()
{
	anim_controller->PlayAnimationByName(anims[playerElement].summonMinions.c_str());
	currentAnimTime = anim_controller->GetDurationByName(anims[playerElement].summonMinions.c_str()) / 2700.0;
	isActing = true;
	idleTime = 0.0;
}

void ADResource::ADGameplay::Golem::InitActions()
{
	consume = DefinitionDatabase::Instance()->actionDatabase["GolemConsume"];

	gActions[STONE].kick = DefinitionDatabase::Instance()->actionDatabase["StoneGolemKick"];
	gActions[STONE].punch = DefinitionDatabase::Instance()->actionDatabase["StoneGolemPunch"];
	gActions[STONE].slam = DefinitionDatabase::Instance()->actionDatabase["StoneGolemSlam"];
	gActions[STONE].special = DefinitionDatabase::Instance()->actionDatabase["GolemTaunt"];

	gActions[WATER].kick = DefinitionDatabase::Instance()->actionDatabase["WaterGolemKick"];
	gActions[WATER].punch = DefinitionDatabase::Instance()->actionDatabase["WaterGolemPunch"];
	gActions[WATER].slam = DefinitionDatabase::Instance()->actionDatabase["WaterGolemSlam"];
	gActions[WATER].special = DefinitionDatabase::Instance()->actionDatabase["GolemWaterWave"];

	gActions[FIRE].kick = DefinitionDatabase::Instance()->actionDatabase["FireGolemKick"];
	gActions[FIRE].punch = DefinitionDatabase::Instance()->actionDatabase["FireGolemPunch"];
	gActions[FIRE].slam = DefinitionDatabase::Instance()->actionDatabase["FireGolemSlam"];
	gActions[FIRE].special = DefinitionDatabase::Instance()->actionDatabase["GolemFireball"];

	gActions[WOOD].kick = DefinitionDatabase::Instance()->actionDatabase["WoodGolemKick"];
	gActions[WOOD].punch = DefinitionDatabase::Instance()->actionDatabase["WoodGolemPunch"];
	gActions[WOOD].slam = DefinitionDatabase::Instance()->actionDatabase["WoodGolemSlam"];
	gActions[WOOD].special = DefinitionDatabase::Instance()->actionDatabase["GolemRooting"];

	consume->active = false;
	for (int i = 0; i < 4; ++i)
	{
		gActions[i].kick->active = false;
		gActions[i].punch->active = false;
		gActions[i].slam->active = false;
		gActions[i].special->active = false;
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