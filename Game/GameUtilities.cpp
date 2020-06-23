#include "pchgame.h"
#include "GameUtilities.h"


//ADResource::ADGameplay::Spyro* GameUtilities::LoadSpyroFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
//{
//	ADResource::ADGameplay::Spyro* temp = new ADResource::ADGameplay::Spyro;
//
//	ADUtils::SHADER shader = { 0 };
//	strcpy_s(shader.vshader, "files\\shaders\\spyro_vs.hlsl");
//	strcpy_s(shader.pshader, "files\\shaders\\spyro_ps.hlsl");
//
//	// Transform data
//	temp->SetPosition(position);
//	temp->SetRotation(rotation);
//	temp->SetScale(scale);
//
//	AD_ULONG id = ResourceManager::InitializePBRModel(modelname, position, scale, rotation, shader);
//	temp->SetMeshID(id);
//
//	return temp;
//}

ADUtils::SHADER GameUtilities::shader;

ADResource::ADGameplay::Golem* GameUtilities::LoadGolemFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Golem* temp = new ADResource::ADGameplay::Golem;

	shader = { 0 };
	strcpy_s(shader.vshader, "files\\shaders\\animated_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\animated_ps.hlsl");

	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);


	AD_ULONG id = ResourceManager::InitializeAnimatedModel(modelname, materials, animations, position, scale, rotation, shader);
	temp->SetMeshID(id);
	
	return temp;
};

Trigger* GameUtilities::AddTinyEssenceFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	//ADResource::ADGameplay::Trigger* temp = DefinitionDatabase::Instance()->hitboxDatabase["GolemPunchHB"];

	//// Transform data
	//temp->SetScale(scale);
	//temp->SetRotation(rotation);
	//temp->SetPosition(position);

	//AD_ULONG id = ResourceManager::AddAnimatedModel(modelname, materials, animations, position, scale, rotation);
	//temp->SetMeshID(id);

	//temp->active = false;
	//temp->team = 1;

	//scale.x *= 500.0f;
	//scale.y *= 500.0f;
	//scale.z *= 500.0f;
	//temp->collider = ADPhysics::AABB(position, scale);
	//temp->collider.trigger = true;
	//temp->colliderPtr = &temp->collider;

	//return temp;
	return nullptr;
};

Trigger* GameUtilities::AddEndGameTriggerFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Trigger* temp = new ADResource::ADGameplay::Trigger;

	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);

	AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
	temp->SetMeshID(id);

	scale.x *= 1.8f;
	scale.y *= 1.8f;
	scale.z *= 1.8f;
	temp->colScale = scale;
	temp->collider = ADPhysics::AABB(position, temp->colScale);
	temp->collider.trigger = true;
	temp->gamePlayType = EVENT_TRIGGER;

	//Using UI as a event Manager.  Temp solution.
	//temp->eventUIMessage.controllerID = 1;
	//temp->eventUIMessage.messageType = ADUI::UIMessageTypes::ExternalMsg;
	//temp->eventUIMessage.number = 3;


	return temp;
}

HitBox* GameUtilities::AddHitbox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::HitBox* temp = new ADResource::ADGameplay::HitBox;

	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);

	AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
	temp->SetMeshID(id);

	scale.x *= 1.8f;
	scale.y *= 1.8f;
	scale.z *= 1.8f;
	temp->colScale = scale;
	temp->collider = ADPhysics::OBB(temp->transform, temp->colScale);
	temp->collider.trigger = true;
	temp->isDeactivateOnFirstApplication = true;
	temp->active = false;
	temp->gamePlayType = ENEMY_HITBOX;
	temp->team = 1;

	temp->effects.push_back(std::unique_ptr<EssenceEffect>(new EssenceEffect()));
	temp->effects[0].get()->sourceID = ResourceManager::GenerateEffectID();

	return temp;
}

Destructable* GameUtilities::AddDestructableFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Destructable* temp = new ADResource::ADGameplay::Destructable;

	
	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);

	//AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
	//temp->SetMeshID(id);

	AD_ULONG id = ResourceManager::InitializeAnimatedModel(modelname, materials, animations, position, scale, rotation, shader);
	temp->SetMeshID(id);
	temp->team = 0;

	scale.x *= 100.0f;
	scale.y *= 100.0f;
	scale.z *= 100.0f;
	//scale.x *= 0.5f;
	//scale.y *= 0.5f;
	//scale.z *= 0.5f;
	temp->colScale = scale;
	temp->collider = ADPhysics::AABB(position, temp->colScale);
	temp->colliderPtr = &temp->collider;
	temp->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["GreatGolem"]));


	return temp;
}

ADAI::AIUnit* GameUtilities::AttachMinionAI(Destructable* _destructable, ADAI::FlockingGroup* _commandGroup, OBJECT_TAG _minionType)
{
	ADAI::AIUnit* temp = new ADAI::AIUnit;
	temp->owner = _destructable;
	_destructable->gamePlayType = _minionType;
	ADAI::IdleState* idling = new ADAI::IdleState();
	ADAI::FlockingState* charging = new ADAI::FlockingState();
	temp->states.push_back(idling);
	temp->states.push_back(charging);

	_commandGroup->AddUnitToGroup(_destructable, charging);

	return temp;
}

ADAI::AIUnit* GameUtilities::AttachVillagerAI(Destructable* _destructable, ADAI::FlockingGroup* _fearGroup)
{
	ADAI::AIUnit* temp = new ADAI::AIUnit;
	temp->owner = _destructable;
	_destructable->gamePlayType = OBJECT_TAG::PEASANT;
	ADAI::IdleState* idling = new ADAI::IdleState();
	ADAI::FlockingState* fleeing = new ADAI::FlockingState();
	temp->states.push_back(idling);
	temp->states.push_back(fleeing);

	//_commandGroup->AddUnitToGroup(_destructable, charging);

	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddColliderBox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);

	AD_ULONG id = ResourceManager::AddColliderBox(modelname, position, scale, rotation);
	temp->SetMeshID(id);

	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddPBRStaticAsset(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	// Transform data
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetPosition(position);

	AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
	temp->SetMeshID(id);

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

	AD_ULONG id = ResourceManager::AddRenderableCollider(XMFLOAT3(0,0,0), XMFLOAT3(1,1,1), XMFLOAT3(0,0,0));
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