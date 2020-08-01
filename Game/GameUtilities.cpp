#include "pchgame.h"
#include "GameUtilities.h"


ADUtils::SHADER GameUtilities::shader;

ADResource::ADGameplay::Golem* GameUtilities::LoadGolemFromModelFile(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Golem* golem = new ADResource::ADGameplay::Golem;

	shader = { 0 };
	strcpy_s(shader.vshader, "files\\shaders\\animated_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\animated_ps.hlsl");


	//Model
	golem->SetScale(scale);
	golem->SetRotation(rotation);
	golem->SetPosition(position);
	std::vector<std::string> animations;
	animations.push_back("files/models/Golem_3_Idle.animfile");
	animations.push_back("files/models/Golem_3_Born.animfile");
	animations.push_back("files/models/Golem_3_Run.animfile");
	animations.push_back("files/models/Golem_3_Command.animfile");
	animations.push_back("files/models/Golem_3_Death.animfile");
	animations.push_back("files/models/Golem_3_Eat.animfile");
	animations.push_back("files/models/Golem_3_GroundSlam.animfile");
	animations.push_back("files/models/Golem_3_Heavy_Hit_Back.animfile");
	animations.push_back("files/models/Golem_3_Heavy_Hit_Front.animfile");
	animations.push_back("files/models/Golem_3_Heavy_Hit_Left.animfile");
	animations.push_back("files/models/Golem_3_Heavy_Hit_Right.animfile");
	animations.push_back("files/models/Golem_3_Idle.animfile");
	animations.push_back("files/models/Golem_3_IdleLook.animfile");
	animations.push_back("files/models/Golem_3_Kick.animfile");
	animations.push_back("files/models/Golem_3_Run.animfile");
	animations.push_back("files/models/Golem_3_Special.animfile");
	animations.push_back("files/models/Golem_3_SummonMinions.animfile");
	animations.push_back("files/models/Golem_3_TowerPunch.animfile");
	AD_ULONG id = ResourceManager::InitializeAnimatedModel("files/models/Golem_3.AnimMesh", "files/textures/Golem_3.mat", animations, position, scale, rotation, shader);
	golem->meshIDs[STONE] = id;

	std::vector<std::string> animations2;
	animations2.push_back("files/models/Golem_4_Idle.animfile");
	animations2.push_back("files/models/Golem_4_Born.animfile");
	animations2.push_back("files/models/Golem_4_Run.animfile");
	animations2.push_back("files/models/Golem_4_Command.animfile");
	animations2.push_back("files/models/Golem_4_Death.animfile");
	animations2.push_back("files/models/Golem_4_Eat.animfile");
	animations2.push_back("files/models/Golem_4_GroundSlam.animfile");
	animations2.push_back("files/models/Golem_4_Heavy_Hit_Back.animfile");
	animations2.push_back("files/models/Golem_4_Heavy_Hit_Front.animfile");
	animations2.push_back("files/models/Golem_4_Heavy_Hit_Left.animfile");
	animations2.push_back("files/models/Golem_4_Heavy_Hit_Right.animfile");
	animations2.push_back("files/models/Golem_4_Idle.animfile");
	animations2.push_back("files/models/Golem_4_IdleLook.animfile");
	animations2.push_back("files/models/Golem_4_Kick.animfile");
	animations2.push_back("files/models/Golem_4_Run.animfile");
	animations2.push_back("files/models/Golem_4_Special.animfile");
	animations2.push_back("files/models/Golem_4_SummonMinions.animfile");
	animations2.push_back("files/models/Golem_4_TowerPunch.animfile");
	id = ResourceManager::InitializeAnimatedModel("files/models/Golem_4.AnimMesh", "files/textures/Golem_4.mat", animations2, position, scale, rotation, shader);
	golem->meshIDs[WATER] = id;

	std::vector<std::string> animations3;
	animations3.push_back("files/models/Golem_2_Idle.animfile");
	animations3.push_back("files/models/Golem_2_Born.animfile");
	animations3.push_back("files/models/Golem_2_Run.animfile");
	animations3.push_back("files/models/Golem_2_Command.animfile");
	animations3.push_back("files/models/Golem_2_Death.animfile");
	animations3.push_back("files/models/Golem_2_Eat.animfile");
	animations3.push_back("files/models/Golem_2_GroundSlam.animfile");
	animations3.push_back("files/models/Golem_2_Heavy_Hit_Back.animfile");
	animations3.push_back("files/models/Golem_2_Heavy_Hit_Front.animfile");
	animations3.push_back("files/models/Golem_2_Heavy_Hit_Left.animfile");
	animations3.push_back("files/models/Golem_2_Heavy_Hit_Right.animfile");
	animations3.push_back("files/models/Golem_2_Idle.animfile");
	animations3.push_back("files/models/Golem_2_IdleLook.animfile");
	animations3.push_back("files/models/Golem_2_Kick.animfile");
	animations3.push_back("files/models/Golem_2_Run.animfile");
	animations3.push_back("files/models/Golem_2_Special.animfile");
	animations3.push_back("files/models/Golem_2_SummonMinions.animfile");
	animations3.push_back("files/models/Golem_2_TowerPunch.animfile");
	id = ResourceManager::InitializeAnimatedModel("files/models/Golem_2.AnimMesh", "files/textures/Golem_2.mat", animations3, position, scale, rotation, shader);
	golem->meshIDs[FIRE] = id;

	std::vector<std::string> animations4;
	animations4.push_back("files/models/Golem_1_Idle.animfile");
	animations4.push_back("files/models/Golem_1_Born.animfile");
	animations4.push_back("files/models/Golem_1_Run.animfile");
	animations4.push_back("files/models/Golem_1_Command.animfile");
	animations4.push_back("files/models/Golem_1_Death.animfile");
	animations4.push_back("files/models/Golem_1_Eat.animfile");
	animations4.push_back("files/models/Golem_1_GroundSlam.animfile");
	animations4.push_back("files/models/Golem_1_Heavy_Hit_Back.animfile");
	animations4.push_back("files/models/Golem_1_Heavy_Hit_Front.animfile");
	animations4.push_back("files/models/Golem_1_Heavy_Hit_Left.animfile");
	animations4.push_back("files/models/Golem_1_Heavy_Hit_Right.animfile");
	animations4.push_back("files/models/Golem_1_Idle.animfile");
	animations4.push_back("files/models/Golem_1_IdleLook.animfile");
	animations4.push_back("files/models/Golem_1_Kick.animfile");
	animations4.push_back("files/models/Golem_1_Run.animfile");
	animations4.push_back("files/models/Golem_1_Special.animfile");
	animations4.push_back("files/models/Golem_1_SummonMinions.animfile");
	animations4.push_back("files/models/Golem_1_TowerPunch.animfile");
	id = ResourceManager::InitializeAnimatedModel("files/models/Golem_1.AnimMesh", "files/textures/Golem_1.mat", animations4, position, scale, rotation, shader);
	golem->meshIDs[WOOD] = id;

	golem->SetMeshID(golem->meshIDs[STONE]);


	//Get Data Driven Components
	golem->stats = DefinitionDatabase::Instance()->statsheetDatabase["GreatGolem"];
	golem->has_stats = true;

	golem->consume = DefinitionDatabase::Instance()->actionDatabase["GolemConsume"];
	golem->consume->scaleCorrection = 10;

	golem->gActions[STONE].kick = DefinitionDatabase::Instance()->actionDatabase["StoneGolemKick"];
	golem->gActions[STONE].kick->scaleCorrection = 10;
	golem->gActions[STONE].punch = DefinitionDatabase::Instance()->actionDatabase["StoneGolemPunch"];
	golem->gActions[STONE].punch->scaleCorrection = 10;
	golem->gActions[STONE].slam = DefinitionDatabase::Instance()->actionDatabase["StoneGolemSlam"];
	golem->gActions[STONE].slam->scaleCorrection = 10;
	golem->gActions[STONE].special = DefinitionDatabase::Instance()->actionDatabase["GolemTaunt"];
	golem->gActions[STONE].special->scaleCorrection = 10;
	golem->gActions[STONE].nextForm = DefinitionDatabase::Instance()->actionDatabase["StoneNextForm"];
	golem->gActions[STONE].nextForm->scaleCorrection = 10;
	golem->gActions[STONE].prevForm = DefinitionDatabase::Instance()->actionDatabase["StonePreviousForm"];
	golem->gActions[STONE].prevForm->scaleCorrection = 10;

	golem->gActions[WATER].kick = DefinitionDatabase::Instance()->actionDatabase["WaterGolemKick"];
	golem->gActions[WATER].kick->scaleCorrection = 10;
	golem->gActions[WATER].punch = DefinitionDatabase::Instance()->actionDatabase["WaterGolemPunch"];
	golem->gActions[WATER].punch->scaleCorrection = 10;
	golem->gActions[WATER].slam = DefinitionDatabase::Instance()->actionDatabase["WaterGolemSlam"];
	golem->gActions[WATER].slam->scaleCorrection = 10;
	golem->gActions[WATER].special = DefinitionDatabase::Instance()->actionDatabase["GolemWaterWave"];
	golem->gActions[WATER].special->scaleCorrection = 5;
	golem->gActions[WATER].nextForm = DefinitionDatabase::Instance()->actionDatabase["WaterNextForm"];
	golem->gActions[WATER].nextForm->scaleCorrection = 10;
	golem->gActions[WATER].prevForm = DefinitionDatabase::Instance()->actionDatabase["WaterPreviousForm"];
	golem->gActions[WATER].prevForm->scaleCorrection = 10;

	golem->gActions[FIRE].kick = DefinitionDatabase::Instance()->actionDatabase["FireGolemKick"];
	golem->gActions[FIRE].kick->scaleCorrection = 10;
	golem->gActions[FIRE].punch = DefinitionDatabase::Instance()->actionDatabase["FireGolemPunch"];
	golem->gActions[FIRE].punch->scaleCorrection = 10;
	golem->gActions[FIRE].slam = DefinitionDatabase::Instance()->actionDatabase["FireGolemSlam"];
	golem->gActions[FIRE].slam->scaleCorrection = 10;
	golem->gActions[FIRE].special = DefinitionDatabase::Instance()->actionDatabase["GolemFireball"];
	golem->gActions[FIRE].special->scaleCorrection = 10;
	golem->gActions[FIRE].nextForm = DefinitionDatabase::Instance()->actionDatabase["FireNextForm"];
	golem->gActions[FIRE].nextForm->scaleCorrection = 10;
	golem->gActions[FIRE].prevForm = DefinitionDatabase::Instance()->actionDatabase["FirePreviousForm"];
	golem->gActions[FIRE].prevForm->scaleCorrection = 10;

	golem->gActions[WOOD].kick = DefinitionDatabase::Instance()->actionDatabase["WoodGolemKick"];
	golem->gActions[WOOD].kick->scaleCorrection = 10;
	golem->gActions[WOOD].punch = DefinitionDatabase::Instance()->actionDatabase["WoodGolemPunch"];
	golem->gActions[WOOD].punch->scaleCorrection = 10;
	golem->gActions[WOOD].slam = DefinitionDatabase::Instance()->actionDatabase["WoodGolemSlam"];
	golem->gActions[WOOD].slam->scaleCorrection = 10;
	golem->gActions[WOOD].special = DefinitionDatabase::Instance()->actionDatabase["GolemRooting"];
	golem->gActions[WOOD].special->scaleCorrection = 10;
	golem->gActions[WOOD].nextForm = DefinitionDatabase::Instance()->actionDatabase["WoodNextForm"];
	golem->gActions[WOOD].nextForm->scaleCorrection = 10;
	golem->gActions[WOOD].prevForm = DefinitionDatabase::Instance()->actionDatabase["WoodPreviousForm"];
	golem->gActions[WOOD].prevForm->scaleCorrection = 10;

	golem->consume->active = false;
	for (int i = 0; i < 4; ++i)
	{
		golem->gActions[i].kick->active = false;
		golem->gActions[i].punch->active = false;
		golem->gActions[i].slam->active = false;
		golem->gActions[i].special->active = false;
		golem->gActions[i].nextForm->active = false;
		golem->gActions[i].prevForm->active = false;
	}

	golem->InitializeController();

	GameUtilities::AddGameObject(dynamic_cast<GameObject*>(golem));

	return golem;
};


Trigger* GameUtilities::AddTriggerFromModelFile(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Trigger* temp = new ADResource::ADGameplay::Trigger;

	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);


	AD_ULONG id = ResourceManager::AddSimpleModel(modelname, materials, position, scale, rotation);
	temp->SetMeshID(id);
	temp->team = 0;

	temp->colScale = scale;
	temp->collider = ADPhysics::AABB(position, temp->colScale);
	temp->colliderPtr = &temp->collider;

	return temp;
}

MessageTrigger* GameUtilities::AddMessageTrigger(ADUI::UIMessage _message, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::MessageTrigger* temp = new ADResource::ADGameplay::MessageTrigger;


	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);
	temp->team = 1;
	temp->colScale = scale;
	temp->collider = ADPhysics::AABB(position, temp->colScale);
	temp->colliderPtr = &temp->collider;
	temp->eventUIMessage = _message;
	temp->has_mesh = false;
	temp->gamePlayType = EVENT_TRIGGER;
	return temp;
}

Destructable* GameUtilities::AddDestructableFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Destructable* temp = new ADResource::ADGameplay::Destructable;

	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);

	AD_ULONG id = ResourceManager::AddAnimatedModel(modelname, materials, animations, position, scale, rotation);
	temp->SetMeshID(id);
	temp->anim_controller->Initialize(temp);

	scale.x *= (1.f / scale.x) * 1.2;
	scale.y *= (1.f / scale.y) * 1.2;
	scale.z *= (1.f / scale.z) * 1.2;
	temp->colScale = scale;
	temp->collider = ADPhysics::AABB(position, temp->colScale);
	temp->colliderPtr = &temp->collider;

	//replace when all objects have their own sheets attached.
	temp->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["Villager"]));

	return temp;
}


ADAI::VillagerAI* GameUtilities::AttachVillagerAI(ADResource::ADGameplay::Destructable* _destructable, ADAI::VillagerGroup* _localGroup, std::vector< ADResource::ADGameplay::GameObject*>* _fearGroup, std::vector< ADResource::ADGameplay::Building*>* _safetyGroup, Engine* _engine)
{
	_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["Villager"]));
	_destructable->gamePlayType = OBJECT_TAG::PEASANT;
	_destructable->deathEvent = "VillagerDeath";
	_destructable->team = 1;
	_destructable->colScale.x *= 5;
	_destructable->colScale.y *= 5;
	_destructable->colScale.z *= 5;
	_destructable->anim_controller->SetNamebyIndex(0, "Idle");
	_destructable->anim_controller->SetNamebyIndex(1, "Run");
	_destructable->anim_controller->SetNamebyIndex(2, "Cower");
	_destructable->anim_controller->SetNamebyIndex(3, "Death");
	_destructable->safeRadius = 5.f;
	_destructable->attackRadius = 8.f;
	_destructable->villagerBlood = _engine->bloodEmitters;
	_destructable->desirability = 1.2f;

	ADAI::VillagerAI* temp = new ADAI::VillagerAI;
	temp->mySSM.gameObject = _destructable;
	temp->destructable = _destructable;

	ADAI::FlockingState* waypoint = new ADAI::FlockingState();
	waypoint->mySSM = &temp->mySSM;
	temp->mySSM.states.push_back(waypoint);

	ADAI::FleeingState* fleeing = new ADAI::FleeingState();
	fleeing->mySSM = &temp->mySSM;
	fleeing->villager = temp;
	fleeing->fearedObjects = _fearGroup;

	for (auto& unit : *_safetyGroup)
	{
		fleeing->safetyStructures.push_back(unit);
	}
	temp->mySSM.states.push_back(fleeing);
	temp->mySSM.currentState = fleeing;
	_localGroup->AddUnitToGroup(temp, fleeing);

	ADAI::AttackingState* attacking = new ADAI::AttackingState();
	attacking->mySSM = &temp->mySSM;
	attacking->returnIndex = 1;
	attacking->myAttack = DefinitionDatabase::Instance()->actionDatabase["VillagerCower"];
	temp->mySSM.states.push_back(attacking);


	return temp;
}

ADAI::MinionAI* GameUtilities::AttachMinionAI(ADResource::ADGameplay::Destructable* _destructable, ADAI::MinionGroup* _localGroup, std::vector<ADResource::ADGameplay::GameObject*>* _killGroup, OBJECT_TAG _minionType, ADAI::PathersQueue* _pathingQueue)
{
	_destructable->gamePlayType = _minionType;
	_destructable->deathEvent = "MinionDeath";
	_destructable->desirability = 1.0f;
	_destructable->team = 0;
	_destructable->colScale.x *= 8;
	_destructable->colScale.y *= 10;
	_destructable->colScale.z *= 8;
	_destructable->anim_controller->SetNamebyIndex(0, "Idle");
	_destructable->anim_controller->SetNamebyIndex(1, "Run");
	_destructable->anim_controller->SetNamebyIndex(2, "Attack");
	_destructable->anim_controller->SetNamebyIndex(3, "Born");
	_destructable->anim_controller->SetNamebyIndex(4, "Flinch");
	_destructable->anim_controller->SetNamebyIndex(5, "Death");


	ADAI::MinionAI* ai = new ADAI::MinionAI;
	ai->mySSM.gameObject = _destructable;
	ai->destructable = _destructable;


	ADAI::FlockingState* idling = new ADAI::FlockingState();
	idling->mySSM = &ai->mySSM;
	ai->mySSM.states.push_back(idling);

	ADAI::CommandState* charging = new ADAI::CommandState();
	charging->mySSM = &ai->mySSM;
	charging->minion = ai;
	charging->targets = _killGroup;
	charging->pathingQueue = _pathingQueue;
	//charging->myGroup = _localGroup;
	ai->mySSM.states.push_back(charging);

	ADAI::AttackingState* attacking = new ADAI::AttackingState();
	attacking->mySSM = &ai->mySSM;
	attacking->returnIndex = 1;
	ai->mySSM.states.push_back(attacking);

	switch (_destructable->gamePlayType)
	{
	case STONE_MINION:
		_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["StoneMinion"]));
		ai->myAttack = DefinitionDatabase::Instance()->actionDatabase["StoneMinionPunch"]->Clone();
		ai->myAttack->scaleCorrection = 1 / ai->mySSM.gameObject->transform.r[0].m128_f32[0];
		attacking->myAttack = ai->myAttack;
		ai->moveSpeed = _destructable->GetStatSheet()->RequestStats("MovementSpeed")->currentValue;
		ai->maxSpeed = ai->moveSpeed * 3;
		break;
	case WATER_MINION:
		_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["WaterMinion"]));
		ai->myAttack = DefinitionDatabase::Instance()->actionDatabase["WaterMinionPunch"]->Clone();
		ai->myAttack->scaleCorrection = 1 / ai->mySSM.gameObject->transform.r[0].m128_f32[0];
		attacking->myAttack = ai->myAttack;
		ai->moveSpeed = _destructable->GetStatSheet()->RequestStats("MovementSpeed")->currentValue;
		ai->maxSpeed = ai->moveSpeed * 3;
		break;
	case FIRE_MINION:
		_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["FireMinion"]));
		ai->myAttack = DefinitionDatabase::Instance()->actionDatabase["FireMinionPunch"]->Clone();
		ai->myAttack->scaleCorrection = 1 / ai->mySSM.gameObject->transform.r[0].m128_f32[0];
		attacking->myAttack = ai->myAttack;
		ai->moveSpeed = _destructable->GetStatSheet()->RequestStats("MovementSpeed")->currentValue;
		ai->maxSpeed = ai->moveSpeed * 3;
		break;
	case WOOD_MINION:
		_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["WoodMinion"]));
		ai->myAttack = DefinitionDatabase::Instance()->actionDatabase["WoodMinionPunch"]->Clone();
		ai->myAttack->scaleCorrection = 1 / ai->mySSM.gameObject->transform.r[0].m128_f32[0];
		attacking->myAttack = ai->myAttack;
		ai->moveSpeed = _destructable->GetStatSheet()->RequestStats("MovementSpeed")->currentValue;
		ai->maxSpeed = ai->moveSpeed * 3;
		break;
	}

	ai->mySSM.currentState = charging;

	_localGroup->AddUnitToGroup(ai, charging);

	return ai;
}

ADAI::TowerAI* GameUtilities::AttachTowerAI(Building* _tower, std::vector<ADResource::ADGameplay::GameObject*>* _killGroup)
{
	_tower->gamePlayType = BALLISTA;
	_tower->deathEvent = "BuildingDeath";
	_tower->desirability = 2.0f;
	_tower->team = 1;

	ADAI::TowerAI* temp = new ADAI::TowerAI;
	temp->mySSM.gameObject = _tower;
	temp->building = _tower;

	ADAI::TowerState* targeting = new ADAI::TowerState();
	targeting->tower = temp;
	targeting->targets = _killGroup;
	temp->mySSM.states.push_back(targeting);

	ADAI::AttackingState* attacking = new ADAI::AttackingState();
	attacking->mySSM = &temp->mySSM;
	temp->myAttack = DefinitionDatabase::Instance()->actionDatabase["BallistaBoltFire"]->Clone();
	temp->myAttack->scaleCorrection = 10;
	attacking->myAttack = temp->myAttack;
	attacking->returnIndex = 0;
	temp->mySSM.states.push_back(attacking);

	temp->mySSM.currentState = targeting;

	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddSimpleAsset(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool instanced, RotationType type)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	temp->SetScale(scale);
	temp->SetRotation(rotation, type);
	if (instanced)
	{
		temp->SetPosition({ 0,0,0 });
	}
	else
	{
		temp->SetPosition(position);
	}

	AD_ULONG id = ResourceManager::AddSimpleModel(modelname, materials, position, scale, rotation, instanced);
	temp->SetMeshID(id);

	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddSimpleAnimAsset(std::string modelname, std::string materials, std::vector<std::string> AnimationFiles, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);

	AD_ULONG id = ResourceManager::AddAnimatedModel(modelname, materials, AnimationFiles, position, scale, rotation);
	temp->SetMeshID(id);
	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddRenderableCollider()
{

	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	AD_ULONG id = ResourceManager::AddRenderableCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
	temp->SetMeshID(id);

	return temp;
}


//Template
/*
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif


	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
*/

std::vector<Renderable*> GameUtilities::GenerateHouse1(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif
	temp.push_back(AddSimpleAsset("files/models/House_1_WoodenFrame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_1_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_1_Structure.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_1_StoneDetails.mesh", "files/textures/Stones2B.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateHouse2(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/House_2_WoodenFrame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_2_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_2_Structure.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_2_StoneDetails.mesh", "files/textures/Stones2B.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateHouse3(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/House_3_WoodenFrame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_3_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_3_Structure.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_3_StoneDetails.mesh", "files/textures/Stones2A.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateHouse4(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/House_4_WoodenFrame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_4_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), XMFLOAT3(90, rotation.y, 0), false, RotationType::yzx));
	temp.push_back(AddSimpleAsset("files/models/House_4_Structure.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_4_StoneDetails.mesh", "files/textures/Stones2A.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateBarn1(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Barn_01_WoodenFrame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_01_Planks.mesh", "files/textures/Planks.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_01_Roof.mesh", "files/textures/Roof_02.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateBarn2(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Barn_02_WoodenFrame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_02_Structure.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_02_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_02_StoneDetails.mesh", "files/textures/Stones2A.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateGateway(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/GateWayOpenning.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayTowers.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayStoneDetails.mesh", "files/textures/Stones1.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayStoneWindows.mesh", "files/textures/Stones2A.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayWooden.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayWoodenStructure.mesh", "files/textures/Planks.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayLattice.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y + 5, pos.z), XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateScaffolding(XMFLOAT3 pos, XMFLOAT3 rotation) {

#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateScaffoldWallX(XMFLOAT3 pos, XMFLOAT3 rotation) {

#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateScaffoldWallZ(XMFLOAT3 pos, XMFLOAT3 rotation) {

#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateWall(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/WallBase.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/WallRoof.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), XMFLOAT3(90, 0, -rotation.y)));
	temp.push_back(AddSimpleAsset("files/models/WallDetails.mesh", "files/textures/Stones1.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateWell(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Well_Wooden.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y + 0.01f, pos.z), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Well_Hole.mesh", "files/textures/Brick4.mat", XMFLOAT3(pos.x, pos.y + 0.01f, pos.z), XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateTavern(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Tavern_Frame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tavern_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tavern_Structure.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateTree(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Tree_Bark.mesh", "files/textures/Bark.mat", pos, XMFLOAT3(0.025, 0.02, 0.025), rotation, true));
	temp.push_back(AddSimpleAsset("files/models/Tree_Branch.mesh", "files/textures/Branch.mat", pos, XMFLOAT3(0.025, 0.02, 0.025), rotation, true));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateLadder(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Ladder.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateRock1(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Rock_1.mesh", "files/textures/Rock.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation, true));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateLamp(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Lamp.mesh", "files/textures/Lamp.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateMeetingHall(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/MeetingHall_WoodenFrame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/MeetingHall_Structure.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/MeetingHall_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/MeetingHall_StoneDetails.mesh", "files/textures/Stones2B.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/MeetingHall_BrickDetails.mesh", "files/textures/Brick_rock1.mat", pos, XMFLOAT3(25, 25, 25), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateTower(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Tower_WoodenFrame.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_Structure.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_StoneDetails.mesh", "files/textures/Stones2A.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_BrickDetails.mesh", "files/textures/Brick_rock1.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateCart(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Cart.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateStraw(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Straw_01.mesh", "files/textures/Straw.mat", pos, XMFLOAT3(0.03, 0.03, 0.03), rotation, true));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateDebris1(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Debris_01.mesh", "files/textures/Debris.mat", pos, XMFLOAT3(0.25, 0.25, 0.25), rotation, true));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateRubble1(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Rubble_01_Stone.mesh", "files/textures/Debris.mat", pos, XMFLOAT3(0.05, 0.05, 0.05), rotation));
	temp.push_back(AddSimpleAsset("files/models/Rubble_01_Wood.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y - 6.5, pos.z - 4.2), XMFLOAT3(50, 50, 50), XMFLOAT3(14.030, 0.585 + rotation.y, 7.898)));
	//temp.push_back(AddSimpleAsset("files/models/Rubble_01_Wood.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y - 0.15, pos.z / 2 - 0.1f), XMFLOAT3(50, 50, 50), XMFLOAT3(14.030, 0.585 + rotation.y, 7.898)));

	//for (size_t i = 0; i < temp.size(); i++)
	//{
	//	AddGameObject(temp[i]);
	//}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateRubble2(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Rubble_02_Stone.mesh", "files/textures/Debris.mat", pos, XMFLOAT3(0.05, 0.05, 0.05), rotation, true));
	temp.push_back(AddSimpleAsset("files/models/Rubble_02_Wood.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.05, 0.05, 0.05), rotation, true));

	//for (size_t i = 0; i < temp.size(); i++)
	//{
	//	AddGameObject(temp[i]);
	//}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateRubble3(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Rubble_03_Stone.mesh", "files/textures/Debris.mat", pos, XMFLOAT3(0.05, 0.05, 0.05), rotation, true));
	temp.push_back(AddSimpleAsset("files/models/Rubble_03_Wood.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.05, 0.05, 0.05), rotation, true));

	//for (size_t i = 0; i < temp.size(); i++)
	//{
	//	AddGameObject(temp[i]);
	//}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateAttackTower(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/AttackTower.mesh", "files/textures/Wood_01.mat", pos, XMFLOAT3(0.025, 0.025, 0.025), rotation, false));
	temp.push_back(AddSimpleAsset("files/models/Shed.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y + 25, pos.z), XMFLOAT3(25, 25, 25), rotation, false));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateBallista(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Ballista.mesh", "files/textures/Ballista.mat", pos, XMFLOAT3(0.05, 0.05, 0.05), rotation, false));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateRockWall1(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Cliff_1.mesh", "files/textures/Cliff_1.mat", XMFLOAT3(pos.x, pos.y + 16.67, pos.z), XMFLOAT3(0.075, 0.075, 0.075), rotation, false));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateRockWall2(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Cliff_2.mesh", "files/textures/Cliff_2.mat", XMFLOAT3(pos.x, pos.y + 16.67, pos.z), XMFLOAT3(0.075, 0.075, 0.075), rotation, false));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateRockWall3(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Cliff_3.mesh", "files/textures/Cliff_3.mat", XMFLOAT3(pos.x, pos.y + 16.67, pos.z), XMFLOAT3(0.075, 0.075, 0.075), rotation, false));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateRockWall4(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Cliff_4.mesh", "files/textures/Cliff_4.mat", XMFLOAT3(pos.x, pos.y + 16.67, pos.z), XMFLOAT3(0.075, 0.075, 0.075), rotation, false));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateRockWall5(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Cliff_5.mesh", "files/textures/Cliff_5.mat", XMFLOAT3(pos.x, pos.y + 10, pos.z), XMFLOAT3(0.075, 0.075, 0.075), rotation, false));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GeneratePathway1(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Pathway1.mesh", "files/textures/Pathway.mat", XMFLOAT3(pos.x, pos.y + 0.1, pos.z), XMFLOAT3(0.1, 0.1, 0.1), rotation, true));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GeneratePathway2(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Pathway2.mesh", "files/textures/Pathway.mat", XMFLOAT3(pos.x, pos.y + 0.1, pos.z), XMFLOAT3(0.1, 0.1, 0.1), rotation, true));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GeneratePathway3(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Pathway3.mesh", "files/textures/Pathway.mat", XMFLOAT3(pos.x, pos.y + 0.1, pos.z), XMFLOAT3(0.125, 0.125, 0.125), rotation, true));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

std::vector<Renderable*> GameUtilities::GenerateBanner(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Banner.mesh", "files/textures/Banner.mat", XMFLOAT3(pos.x, pos.y + 20, pos.z), XMFLOAT3(0.075, 0.075, 0.075), rotation, false));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}

void GameUtilities::AddGameObject(GameObject* obj, bool has_mesh)
{
	obj->has_mesh = has_mesh;

	ResourceManager::AddGameObject(obj);
}

