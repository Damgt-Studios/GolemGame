#include "pchgame.h"
#include "GameUtilities.h"


ADUtils::SHADER GameUtilities::shader;

ADResource::ADGameplay::Golem* GameUtilities::LoadGolemFromModelFile(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
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
	animations.push_back("files/models/Golem_1_Idle.animfile");
	animations.push_back("files/models/Golem_1_Born.animfile");
	animations.push_back("files/models/Golem_1_Run.animfile");
	animations.push_back("files/models/Golem_1_Death.animfile");
	animations.push_back("files/models/Golem_1_Kick.animfile");
	AD_ULONG id = ResourceManager::InitializeAnimatedModel(modelname, materials, animations, position, scale, rotation, shader);
	golem->SetMeshID(id);


	//Get Data Driven Components
	golem->stats = DefinitionDatabase::Instance()->statsheetDatabase["GreatGolem"];
	golem->golemPunch = DefinitionDatabase::Instance()->actionDatabase["WoodGolemPunch"];
	golem->golemKick = DefinitionDatabase::Instance()->actionDatabase["WoodGolemKick"];
	golem->golemSlam = DefinitionDatabase::Instance()->actionDatabase["WoodGolemSlam"];
	golem->golemConsume = DefinitionDatabase::Instance()->actionDatabase["GolemConsume"];
	golem->golemWaterWave = DefinitionDatabase::Instance()->actionDatabase["GolemWaterWave"];
	golem->golemFireball = DefinitionDatabase::Instance()->actionDatabase["GolemFireball"];
	golem->golemTaunt = DefinitionDatabase::Instance()->actionDatabase["GolemTaunt"];
	golem->golemRoot = DefinitionDatabase::Instance()->actionDatabase["GolemRooting"];

	golem->golemPunch->active = false;
	golem->golemKick->active = false;
	golem->golemSlam->active = false;
	golem->golemConsume->active = false;
	golem->golemWaterWave->active = false;
	golem->golemFireball->active = false;
	golem->golemTaunt->active = false;
	golem->golemRoot->active = false;

	golem->InitializeController();

	GameUtilities::AddGameObject(dynamic_cast<GameObject*>(golem));

	return golem;
};
//
//Trigger* GameUtilities::AddTinyEssenceFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
//{
//	//ADResource::ADGameplay::Trigger* temp = DefinitionDatabase::Instance()->hitboxDatabase["GolemPunchHB"];
//
//	//// Transform data
//	//temp->SetScale(scale);
//	//temp->SetRotation(rotation);
//	//temp->SetPosition(position);
//
//	//AD_ULONG id = ResourceManager::AddAnimatedModel(modelname, materials, animations, position, scale, rotation);
//	//temp->SetMeshID(id);
//
//	//temp->active = false;
//	//temp->team = 1;
//
//	//scale.x *= 500.0f;
//	//scale.y *= 500.0f;
//	//scale.z *= 500.0f;
//	//temp->collider = ADPhysics::AABB(position, scale);
//	//temp->collider.trigger = true;
//	//temp->colliderPtr = &temp->collider;
//
//	//return temp;
//	return nullptr;
//};
//
//Trigger* GameUtilities::AddEndGameTriggerFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
//{
//	ADResource::ADGameplay::Trigger* temp = new ADResource::ADGameplay::Trigger;
//
//	// Transform data
//	temp->SetScale(scale);
//	temp->SetRotation(rotation);
//	temp->SetPosition(position);
//
//	AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
//	temp->SetMeshID(id);
//
//	scale.x *= 1.8f;
//	scale.y *= 1.8f;
//	scale.z *= 1.8f;
//	temp->colScale = scale;
//	temp->collider = ADPhysics::AABB(position, temp->colScale);
//	temp->collider.trigger = true;
//	temp->gamePlayType = EVENT_TRIGGER;
//
//	//Using UI as a event Manager.  Temp solution.
//	//temp->eventUIMessage.controllerID = 1;
//	//temp->eventUIMessage.messageType = ADUI::UIMessageTypes::ExternalMsg;
//	//temp->eventUIMessage.number = 3;
//
//
//	return temp;
//}

//
//HitBox* GameUtilities::AddHitbox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
//{
//	ADResource::ADGameplay::HitBox* temp = new ADResource::ADGameplay::HitBox;
//
//	// Transform data
//	temp->SetScale(scale);
//	temp->SetRotation(rotation);
//	temp->SetPosition(position);
//
//	AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
//	temp->SetMeshID(id);
//
//	scale.x *= 1.8f;
//	scale.y *= 1.8f;
//	scale.z *= 1.8f;
//	temp->colScale = scale;
//	temp->collider = ADPhysics::OBB(temp->transform, temp->colScale);
//	temp->collider.trigger = true;
//	temp->isDeactivateOnFirstApplication = true;
//	temp->active = false;
//	temp->gamePlayType = ENEMY_HITBOX;
//	temp->team = 1;
//
//	temp->effects.push_back(std::unique_ptr<EssenceEffect>(new EssenceEffect()));
//	temp->effects[0].get()->sourceID = ResourceManager::GenerateEffectID();
//
//	return temp;
//}


Destructable* GameUtilities::AddDestructableFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Destructable* temp = new ADResource::ADGameplay::Destructable;

	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);

	//AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
	//temp->SetMeshID(id);

	AD_ULONG id = ResourceManager::AddAnimatedModel(modelname, materials, animations, position, scale, rotation);
	temp->SetMeshID(id);

	scale.x *= 100.0f;
	scale.y *= 100.0f;
	scale.z *= 100.0f;
	temp->colScale = scale;
	temp->collider = ADPhysics::AABB(position, temp->colScale);
	temp->colliderPtr = &temp->collider;

	//replace when all objects have their own sheets attached.
	temp->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["Villager"]));

	return temp;
}



ADAI::AIUnit* GameUtilities::AttachMinionAI(Destructable* _destructable, ADAI::FlockingGroup* _commandGroup, OBJECT_TAG _minionType)
{
	ADAI::AIUnit* temp = new ADAI::AIUnit;
	temp->owner = _destructable;
	_destructable->gamePlayType = _minionType;
	_destructable->deathEvent = "MinionDeath";
	switch (_destructable->gamePlayType)
	{
	case STONE_MINION:
		_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["StoneMinion"]));
		break;
	case WATER_MINION:
		_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["WaterMinion"]));
		break;
	case FIRE_MINION:
		_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["FireMinion"]));
		break;
	case WOOD_MINION:
		_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["WoodMinion"]));
		break;
	}
	_destructable->team = 0;
	ADAI::IdleState* idling = new ADAI::IdleState();
	ADAI::FlockingState* charging = new ADAI::FlockingState();
	charging->owner = _destructable;
	temp->states.push_back(idling);
	temp->states.push_back(charging);
	temp->currentState = idling;

	_commandGroup->AddUnitToGroup(temp, charging);

	return temp;
}
//
//void GameUtilities::InitializeMinionModels()
//{
//	//std::vector<std::string> stoneMinionAnimations;
//	//stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");
//	//std::vector<std::string> waterMinionAnimations;
//	//waterMinionAnimations.push_back("files/models/Minion_4_Idle.animfile");
//	//std::vector<std::string> fireMinionAnimations;
//	//fireMinionAnimations.push_back("files/models/Minion_2_Idle.animfile");
//	//std::vector<std::string> woodMinionAnimations;
//	//woodMinionAnimations.push_back("files/models/Minion_1_Idle.animfile");
//	//
//	//XMFLOAT3 position = { 0,0,0 };
//	//XMFLOAT3 scale = { 0,0,0 };
//	//XMFLOAT3 rotation = { 0,0,0 };
//
//	//DefinitionDatabase::Instance()->stoneMinionMeshid = ResourceManager::InitializeAnimatedModel("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, position, scale, rotation, shader);
//	//DefinitionDatabase::Instance()->waterMinionMeshid = ResourceManager::InitializeAnimatedModel("files/models/Minion_4.AnimMesh", "files/textures/Minion_4.mat", waterMinionAnimations, position, scale, rotation, shader);
//	//DefinitionDatabase::Instance()->fireMinionMeshid  = ResourceManager::InitializeAnimatedModel("files/models/Minion_2.AnimMesh", "files/textures/Minion_2.mat", fireMinionAnimations, position, scale, rotation, shader);
//	//DefinitionDatabase::Instance()->woodMinionMeshid  = ResourceManager::InitializeAnimatedModel("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, position, scale, rotation, shader);
//	//DefinitionDatabase::Instance()->villagerMeshid    = ResourceManager::InitializeAnimatedModel("files/models/Minion_2.AnimMesh", "files/textures/Minion_2.mat", fireMinionAnimations, position, scale, rotation, shader);
//
//
//	//for (int i = 0; i < 10; i++)
//	//{
//		//stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, XMFLOAT3(-130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
//	//	stoneMinionsAI.push_back(GameUtilities::AttachMinionAI(stoneMinions[i], golem->flockingGroups[STONE], STONE_MINION));
//	//	waterMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_4.AnimMesh", "files/textures/Minion_4.mat", waterMinionAnimations, XMFLOAT3(-130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
//	//	waterMinionsAI.push_back(GameUtilities::AttachMinionAI(waterMinions[i], golem->flockingGroups[WATER], WATER_MINION));
//	//	fireMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_2.AnimMesh", "files/textures/Minion_2.mat", fireMinionAnimations, XMFLOAT3(130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
//	//	fireMinionsAI.push_back(GameUtilities::AttachMinionAI(fireMinions[i], golem->flockingGroups[FIRE], FIRE_MINION));
//	//	woodMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
//	//	woodMinionsAI.push_back(GameUtilities::AttachMinionAI(woodMinions[i], golem->flockingGroups[WOOD], WOOD_MINION));
//	//}
//	//for (int i = 0; i < 10; i++)
//	//{
//	//	villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, XMFLOAT3((i - 5) * 10, 5, (i - 5) * 10), XMFLOAT3(0.015f, 0.03f, 0.015f), XMFLOAT3(0, 0, 0)));
//	//	villagersAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock1));
//	//}
//
//	//Destructable* m1 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", stoneMinionAnimations, XMFLOAT3(0, 5, 0), XMFLOAT3(0.02f, 1.02f, 0.02f), XMFLOAT3(0, 0, 0));
//	//golem->targetMarker = m1;
//
//	//for (int i = 0; i < 10; i++)
//	//{
//	//	GameUtilities::AddGameObject(stoneMinions[i]);
//	//	GameUtilities::AddGameObject(waterMinions[i]);
//	//	GameUtilities::AddGameObject(fireMinions[i]);
//	//	GameUtilities::AddGameObject(woodMinions[i]);
//	//	GameUtilities::AddGameObject(villagers[i]);
//
//	//}
//	//GameUtilities::AddGameObject(m1);
//}
//
//ADAI::AIUnit* GameUtilities::BirthStoneMinion(ADAI::FlockingGroup* _commandGroup)
//{
//	//ADResource::ADGameplay::Destructable* fMinion = new ADResource::ADGameplay::Destructable;
//
//	//XMFLOAT3 position = { -130, 5, -130 };
//	//XMFLOAT3 scale = { 0.03f, 0.03f, 0.03f };
//	//XMFLOAT3 rotation = { 0,0,0 };
//
//
//	//// Transform data
//	//fMinion->SetScale(scale);
//	//fMinion->SetRotation(rotation);
//	//fMinion->SetPosition(position);
//
//
//	////fMinion->SetMeshID(DefinitionDatabase::Instance()->stoneMinionMeshid);
//	//fMinion->team = 0;
//
//	//scale.x *= 100.0f;
//	//scale.y *= 100.0f;
//	//scale.z *= 100.0f;
//	//fMinion->colScale = scale;
//	//fMinion->collider = ADPhysics::AABB(position, fMinion->colScale);
//	//fMinion->colliderPtr = &fMinion->collider;
//	//fMinion->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["GreatGolem"]));
//
//	//ADAI::AIUnit* fAI = new ADAI::AIUnit;
//	//fAI->owner = fMinion;
//	//fMinion->gamePlayType = FIRE_MINION;
//	//fMinion->deathEvent = "StoneMinionCountChange";
//	//fMinion->team = 0;
//	//ADAI::IdleState* idling = new ADAI::IdleState();
//	//ADAI::FlockingState* charging = new ADAI::FlockingState();
//	//fAI->states.push_back(idling);
//	//fAI->states.push_back(charging);
//
//	////DefinitionDatabase::Instance()->stoneMinions.push_back(fMinion);
//	//_commandGroup->AddUnitToGroup(fMinion, charging);
//
//	//return fAI;
//	return nullptr;
//}

ADAI::AIUnit* GameUtilities::AttachVillagerAI(Destructable* _destructable, ADAI::FlockingGroup* _fearGroup)
{
	ADAI::AIUnit* temp = new ADAI::AIUnit;
	temp->owner = _destructable;
	_destructable->gamePlayType = OBJECT_TAG::PEASANT;
	_destructable->deathEvent = "VillagerDeath";
	_destructable->team = 1;
	ADAI::IdleState* idling = new ADAI::IdleState();
	ADAI::FlockingState* fleeing = new ADAI::FlockingState();
	temp->states.push_back(idling);
	temp->states.push_back(fleeing);
	temp->currentState = idling;

	_fearGroup->AddUnitToGroup(temp, fleeing);
	_destructable->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["Villager"]));

	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddSimpleAsset(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, RotationType type)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	temp->SetPosition(position);
	temp->SetRotation(rotation, type);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::AddSimpleModel(modelname, materials, position, scale, rotation);
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
	temp.push_back(AddSimpleAsset("files/models/House_1_WoodenFrame.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
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

	temp.push_back(AddSimpleAsset("files/models/House_2_WoodenFrame.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_2_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_2_Structure.mesh", "files/textures/Brick4.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
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

	temp.push_back(AddSimpleAsset("files/models/House_3_WoodenFrame.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_3_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_3_Structure.mesh", "files/textures/Brick4.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
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

	temp.push_back(AddSimpleAsset("files/models/House_4_WoodenFrame.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_4_Roof.mesh", "files/textures/Roof_03.mat", pos, XMFLOAT3(25, 25, 25), XMFLOAT3(90, rotation.y, 0), RotationType::yzx));
	temp.push_back(AddSimpleAsset("files/models/House_4_Structure.mesh", "files/textures/Brick4.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
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

	temp.push_back(AddSimpleAsset("files/models/Barn_01_WoodenFrame.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_01_Planks.mesh", "files/textures/Planks.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
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

	temp.push_back(AddSimpleAsset("files/models/Barn_02_WoodenFrame.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
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
	temp.push_back(AddSimpleAsset("files/models/GateWayLattice.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y + .366f, pos.z), XMFLOAT3(25, 25, 25), rotation));

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
	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x + 0.5f, pos.y, pos.z), XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x - 0.5f, pos.y, pos.z), XMFLOAT3(25, 25, 25), rotation));

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
	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y, pos.z + 0.5f), XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y, pos.z - 0.5f), XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Well_Wooden.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Well_Hole.mesh", "files/textures/Brick4.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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
	temp.push_back(AddSimpleAsset("files/models/Tavern_Structure.mesh", "files/textures/Brick4.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Tree_Bark.mesh", "files/textures/Bark.mat", XMFLOAT3(pos.x * 1000, pos.y * 1200, pos.z * 1000), XMFLOAT3(0.025, 0.02, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tree_Branch.mesh", "files/textures/Branch.mat", XMFLOAT3(pos.x * 1000, pos.y * 1200, pos.z * 1000), XMFLOAT3(0.025, 0.02, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Ladder.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000 + 250, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Rock_1.mesh", "files/textures/Rock.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Lamp.mesh", "files/textures/Lamp.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/MeetingHall_WoodenFrame.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
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

	temp.push_back(AddSimpleAsset("files/models/Tower_WoodenFrame.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_Structure.mesh", "files/textures/Brick4.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_StoneDetails.mesh", "files/textures/Stones2A.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_BrickDetails.mesh", "files/textures/Brick_rock1.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Cart.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Straw_01.mesh", "files/textures/Straw.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Debris_01.mesh", "files/textures/Debris.mat", XMFLOAT3(pos.x * 100, pos.y * 100, pos.z * 100), XMFLOAT3(0.25, 0.25, 0.25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Rubble_01_Stone.mesh", "files/textures/Debris.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));
	temp.push_back(AddSimpleAsset("files/models/Rubble_01_Wood.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x, pos.y - 0.15, pos.z / 2 - 0.1f), XMFLOAT3(50, 50, 50), XMFLOAT3(14.030, 0.585 + rotation.y, 7.898)));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateRubble2(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Rubble_02_Stone.mesh", "files/textures/Debris.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));
	temp.push_back(AddSimpleAsset("files/models/Rubble_02_Wood.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));

	for (size_t i = 0; i < temp.size(); i++)
	{
		AddGameObject(temp[i]);
	}

	return temp;
}
std::vector<Renderable*> GameUtilities::GenerateRubble3(XMFLOAT3 pos, XMFLOAT3 rotation) {
#ifndef MEMORY_MANAGER
	std::vector<Renderable*> temp;
#else
	ADVector<Renderable*> temp;
#endif

	temp.push_back(AddSimpleAsset("files/models/Rubble_03_Stone.mesh", "files/textures/Debris.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));
	temp.push_back(AddSimpleAsset("files/models/Rubble_03_Wood.mesh", "files/textures/Wood_01.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));

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

