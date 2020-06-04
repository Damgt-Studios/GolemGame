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

	temp->SetPosition(position);
	temp->SetScale(scale);
	temp->SetRotation(rotation);

	AD_ULONG id = ResourceManager::InitializeAnimatedModel(modelname, materials, animations, position, scale, rotation, shader);
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

Destructable* GameUtilities::AddDestructableFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Destructable* temp = new ADResource::ADGameplay::Destructable;

	// Transform data
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	//AD_ULONG id = ResourceManager::AddPBRModel(modelname, position, scale, rotation);
	//temp->SetMeshID(id);

	AD_ULONG id = ResourceManager::InitializeAnimatedModel(modelname, materials, animations, position, scale, rotation, shader);
	temp->SetMeshID(id);

	scale.x *= 0.5f;
	scale.y *= 0.5f;
	scale.z *= 0.5f;
	temp->collider = ADPhysics::AABB(position, scale);

	return temp;
}

ADAI::AIUnit* GameUtilities::AttachMinionAI(Destructable* _destructable, ADAI::FlockingGroup* _idleGroup, ADAI::FlockingGroup* _commandGroup)
{
	ADAI::AIUnit* temp = new ADAI::AIUnit;
	temp->owner = _destructable;
	ADAI::FlockingState* idling = new ADAI::FlockingState();
	ADAI::FlockingState* charging = new ADAI::FlockingState();
	temp->states.push_back(idling);
	temp->states.push_back(charging);
	temp->states.push_back(new ADAI::TestingState());

	_idleGroup->AddUnitToGroup(_destructable, idling);
	_commandGroup->AddUnitToGroup(_destructable, charging);

	return nullptr;
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

	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetScale(scale);

	AD_ULONG id = ResourceManager::AddAnimatedModel(modelname, materials, AnimationFiles, position, scale, rotation);
	temp->SetMeshID(id);

	return temp;
}

ADResource::ADGameplay::Renderable* GameUtilities::AddRenderableCollider(ADPhysics::Collider* collider)
{
	ADResource::ADGameplay::Renderable* temp = new ADResource::ADGameplay::Renderable;
	temp->SetPosition(collider->Pos);

	if (collider->type == ColliderType::Sphere) {
		Sphere* sphere = static_cast<Sphere*>(collider);
		temp->SetRotationMatrix(XMMatrixIdentity());
		temp->SetScale(XMFLOAT3(sphere->Radius, sphere->Radius, sphere->Radius));
	}
	else if (collider->type == ColliderType::Aabb) {
		AABB* aabb = static_cast<AABB*>(collider);
		temp->SetRotationMatrix(XMMatrixIdentity());
		temp->SetScale(aabb->HalfSize);
	}
	else if (collider->type == ColliderType::Obb) 
	{
		OBB* obb = static_cast<OBB*>(collider);
		XMMATRIX Matrix;
		Matrix.r[0] = XMVectorSet(obb->AxisX.x, obb->AxisX.y, obb->AxisX.z, 0);
		Matrix.r[1] = XMVectorSet(obb->AxisY.x, obb->AxisY.y, obb->AxisY.z, 0);
		Matrix.r[2] = XMVectorSet(obb->AxisZ.x, obb->AxisZ.y, obb->AxisZ.z, 0);
		Matrix.r[3] = XMVectorSet(0, 0, 0, 1);
		temp->SetRotationMatrix(Matrix);
		temp->SetScale(obb->HalfSize);
	}
	else
	{
		delete temp;
		return nullptr;
	}

	AD_ULONG id = ResourceManager::AddRenderableCollider(collider);
	temp->SetMeshID(id);
}

void GameUtilities::AddGameObject(GameObject* obj, bool has_mesh)
{
	obj->has_mesh = has_mesh;

	ResourceManager::AddGameObject(obj);
}