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

	temp->transform;

	AD_ULONG id = ResourceManager::InitializeAnimatedModel(modelname, materials, animations, position, scale, rotation, shader);
	temp->SetMeshID(id);
	
	return temp;
};

Trigger* GameUtilities::AddTinyEssenceFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
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
	temp->collider = ADPhysics::AABB(position, scale);
	temp->collider.trigger = true;
	temp->isDeactivateOnFirstApplication = true;

	temp->effects.push_back(std::unique_ptr<EssenceEffect>(new EssenceEffect()));
	temp->effects[0].get()->sourceID = ResourceManager::GenerateEffectID();

	return temp;
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
	temp->collider = ADPhysics::AABB(position, scale);
	temp->collider.trigger = true;
	temp->gamePlayType = EVENT_TRIGGER;

	//Using UI as a event Manager.  Temp solution.
	//temp->eventUIMessage.controllerID = 1;
	//temp->eventUIMessage.messageType = ADUI::UIMessageTypes::ExternalMsg;
	//temp->eventUIMessage.number = 3;


	return temp;
}

Trigger* GameUtilities::AddHitbox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
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
	temp->collider = ADPhysics::AABB(position, scale);
	temp->collider.trigger = true;
	temp->isDeactivateOnFirstApplication = true;
	temp->active = false;
	temp->gamePlayType = ALLY_HITBOX;
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

	scale.x *= 5.f;
	scale.y *= 5.f;
	scale.z *= 5.f;
	//scale.x *= 0.5f;
	//scale.y *= 0.5f;
	//scale.z *= 0.5f;
	temp->collider = ADPhysics::AABB(position, scale);

	return temp;
}

ADAI::AIUnit* GameUtilities::AttachMinionAI(Destructable* _destructable, ADAI::FlockingGroup* _commandGroup)
{
	ADAI::AIUnit* temp = new ADAI::AIUnit;
	temp->owner = _destructable;
	ADAI::IdleState* idling = new ADAI::IdleState();
	ADAI::FlockingState* charging = new ADAI::FlockingState();
	temp->states.push_back(idling);
	temp->states.push_back(charging);

	_commandGroup->AddUnitToGroup(_destructable, charging);

	return nullptr;
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

ADResource::ADGameplay::Renderable* GameUtilities::AddRenderableCollider(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotation)
{

	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;
	temp->SetPosition(pos);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::AddRenderableCollider(pos, scale, rotation);
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
	temp.push_back(AddSimpleAsset("files/models/House_1_WoodenFrame.mesh", "files/textures/House_1_WoodenFrame.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_1_Roof.mesh", "files/textures/House_1_Roof.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_1_Structure.mesh", "files/textures/House_1_Structure.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_1_StoneDetails.mesh", "files/textures/House_1_StoneDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/House_2_WoodenFrame.mesh", "files/textures/House_2_WoodenFrame.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_2_Roof.mesh", "files/textures/House_2_Roof.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_2_Structure.mesh", "files/textures/House_2_Structure.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_2_StoneDetails.mesh", "files/textures/House_2_StoneDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/House_3_WoodenFrame.mesh", "files/textures/House_3_WoodenFrame.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_3_Roof.mesh", "files/textures/House_3_Roof.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_3_Structure.mesh", "files/textures/House_3_Structure.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_3_StoneDetails.mesh", "files/textures/House_3_StoneDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/House_4_WoodenFrame.mesh", "files/textures/House_4_WoodenFrame.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_4_Roof.mesh", "files/textures/House_4_Roof.mat", pos, XMFLOAT3(25, 25, 25), XMFLOAT3(90, rotation.y, 0), RotationType::yzx));
	temp.push_back(AddSimpleAsset("files/models/House_4_Structure.mesh", "files/textures/House_4_Structure.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/House_4_StoneDetails.mesh", "files/textures/House_4_StoneDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Barn_01_WoodenFrame.mesh", "files/textures/Barn_01_WoodenFrame.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_01_Planks.mesh", "files/textures/Barn_01_Planks.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_01_Roof.mesh", "files/textures/Barn_01_Roof.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Barn_02_WoodenFrame.mesh", "files/textures/Barn_02_WoodenFrame.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_02_Structure.mesh", "files/textures/Barn_02_Structure.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_02_Roof.mesh", "files/textures/Barn_02_Roof.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Barn_02_StoneDetails.mesh", "files/textures/Barn_02_StoneDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/GateWayOpenning.mesh", "files/textures/GateWayOpenning.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayTowers.mesh", "files/textures/GateWayOpenning.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayStoneDetails.mesh", "files/textures/GateWayStoneDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayStoneWindows.mesh", "files/textures/GateWayStoneWindows.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayWooden.mesh", "files/textures/GateWayWooden.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayWoodenStructure.mesh", "files/textures/GateWayWoodenStructure.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/GateWayLattice.mesh", "files/textures/GateWayLattice.mat", XMFLOAT3(pos.x, pos.y + .366f, pos.z), XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Scaffolding.mesh", "files/textures/Scaffolding.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/WallBase.mesh", "files/textures/WallBase.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/WallRoof.mesh", "files/textures/WallRoof.mat", pos, XMFLOAT3(25, 25, 25), XMFLOAT3(90, 0, -rotation.y)));
	temp.push_back(AddSimpleAsset("files/models/WallDetails.mesh", "files/textures/WallDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Well_Wooden.mesh", "files/textures/Well_Wooden.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Well_Hole.mesh", "files/textures/Well_Hole.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Tavern_Frame.mesh", "files/textures/Tavern_Frame.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tavern_Roof.mesh", "files/textures/Tavern_Roof.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tavern_Structure.mesh", "files/textures/Tavern_Structure.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Tree_Bark.mesh", "files/textures/Tree_Bark.mat", XMFLOAT3(pos.x * 1000, pos.y * 1200, pos.z * 1000), XMFLOAT3(0.025, 0.02, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tree_Branch.mesh", "files/textures/Tree_Branch.mat", XMFLOAT3(pos.x * 1000, pos.y * 1200, pos.z * 1000), XMFLOAT3(0.025, 0.02, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Ladder.mesh", "files/textures/Ladder.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000 + 250, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Rock_1.mesh", "files/textures/Rock_1.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/MeetingHall_WoodenFrame.mesh", "files/textures/MeetingHall_WoodenFrame.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/MeetingHall_Structure.mesh", "files/textures/MeetingHall_Structure.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/MeetingHall_Roof.mesh", "files/textures/MeetingHall_Roof.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/MeetingHall_StoneDetails.mesh", "files/textures/MeetingHall_StoneDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));
	temp.push_back(AddSimpleAsset("files/models/MeetingHall_BrickDetails.mesh", "files/textures/MeetingHall_BrickDetails.mat", pos, XMFLOAT3(25, 25, 25), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Tower_WoodenFrame.mesh", "files/textures/Tower_WoodenFrame.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_Structure.mesh", "files/textures/Tower_Structure.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_StoneDetails.mesh", "files/textures/Tower_StoneDetails.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));
	temp.push_back(AddSimpleAsset("files/models/Tower_BrickDetails.mesh", "files/textures/Tower_BrickDetails.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Cart.mesh", "files/textures/Cart.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Straw_01.mesh", "files/textures/Straw_01.mat", XMFLOAT3(pos.x * 1000, pos.y * 1000, pos.z * 1000), XMFLOAT3(0.025, 0.025, 0.025), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Rubble_01_Stone.mesh", "files/textures/Rubble_01_Stone.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));
	temp.push_back(AddSimpleAsset("files/models/Rubble_01_Wood.mesh", "files/textures/Rubble_01_Wood.mat", XMFLOAT3(pos.x, pos.y - 0.15, pos.z / 2 - 0.1f), XMFLOAT3(50, 50, 50), XMFLOAT3(14.030, 0.585 + rotation.y, 7.898)));

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

	temp.push_back(AddSimpleAsset("files/models/Rubble_02_Stone.mesh", "files/textures/Rubble_02_Stone.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));
	temp.push_back(AddSimpleAsset("files/models/Rubble_02_Wood.mesh", "files/textures/Rubble_02_Wood.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));

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

	temp.push_back(AddSimpleAsset("files/models/Rubble_03_Stone.mesh", "files/textures/Rubble_03_Stone.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));
	temp.push_back(AddSimpleAsset("files/models/Rubble_03_Wood.mesh", "files/textures/Rubble_03_Wood.mat", XMFLOAT3(pos.x * 500, pos.y * 500, pos.z * 500), XMFLOAT3(0.05, 0.05, 0.05), rotation));

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