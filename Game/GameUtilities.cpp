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
}

Collectable* GameUtilities::AddCollectableFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Collectable* temp = new ADResource::ADGameplay::Collectable;

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


	return temp;
}

Trigger* GameUtilities::AddTriggerFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
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

	return temp;
}

Enemy* GameUtilities::AddEnemyFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Enemy* temp = new ADResource::ADGameplay::Enemy;

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

ADResource::ADGameplay::Renderable* GameUtilities::AddSimpleAsset(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::AddSimpleModel(modelname, materials, position, scale, rotation);
	temp->SetMeshID(id);

	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddSimpleAnimAsset(std::string modelname, std::string materials, std::vector<std::string> AnimationFiles, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;

	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::AddAnimatedModel(modelname, materials, AnimationFiles, position, scale, rotation);
	temp->SetMeshID(id);

	return temp;
}

void GameUtilities::AddGameObject(GameObject* obj, bool has_mesh)
{
	obj->has_mesh = has_mesh;

	ResourceManager::AddGameObject(obj);
}