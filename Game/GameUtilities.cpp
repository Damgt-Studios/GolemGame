#include "pchgame.h"
#include "GameUtilities.h"


ADResource::ADGameplay::Spyro* GameUtilities::LoadSpyroFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Spyro* temp = new ADResource::ADGameplay::Spyro;

	ADUtils::SHADER shader = { 0 };
	strcpy_s(shader.vshader, "files\\shaders\\spyro_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\spyro_ps.hlsl");

	// Transform data
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::InitializePBRModel(modelname, position, scale, rotation, shader);
	temp->SetMeshID(id);

	return temp;
};

Trigger* GameUtilities::AddTinyEssenceFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Trigger* temp = new ADResource::ADGameplay::Trigger;

	// Transform data
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

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
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

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
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

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

Destructable* GameUtilities::AddEnemyFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Destructable* temp = new ADResource::ADGameplay::Destructable;

	// Transform data
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
	temp->SetMeshID(id);

	scale.x *= 1.8f;
	scale.y *= 1.8f;
	scale.z *= 1.8f;
	temp->collider = ADPhysics::AABB(position, scale);

	return temp;
}



ADResource::ADGameplay::Renderable* GameUtilities::AddColliderBox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	// Transform data
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::AddColliderBox(modelname, position, scale, rotation);
	temp->SetMeshID(id);

	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddPBRStaticAsset(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	// Transform data
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
	temp->SetMeshID(id);

	return temp;
}

void GameUtilities::AddGameObject(GameObject* obj, bool has_mesh)
{
	obj->has_mesh = has_mesh;

	ResourceManager::AddGameObject(obj);
}