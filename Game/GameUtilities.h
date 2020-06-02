#pragma once

#include <string>

#include "Spyro.h"
#include "Golem.h"
#include "GameObjectClasses.h"
#include "ResourceManager.h"
#include "GameEffects.h"
#include "ADAI.h"

class GameUtilities
{
public:
	static ADUtils::SHADER shader;
	//static Spyro* LoadSpyroFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	//static Trigger* LoadCollectableFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	//static Hitbox* LoadCollectableFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Trigger* AddTinyEssenceFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Trigger* AddEndGameTriggerFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Trigger* AddHitbox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Destructable* AddDestructableFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static ADAI::AIUnit* AttachMinionAI(Destructable* _destructable, ADAI::FlockingGroup* _commandGroup, ADAI::FlockingGroup* _idleGroup);
	//static Destructable* LoadCollectableFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	//static Enemy* LoadCollectableFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Renderable* AddColliderBox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Renderable* AddPBRStaticAsset(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);

	static Golem* LoadGolemFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);

	static Renderable* AddSimpleAsset(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Renderable* AddSimpleAnimAsset(std::string modelname, std::string materials, std::vector<std::string> AnimationFiles, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Renderable* AddRenderableCollider(ADPhysics::Collider*);

public:
	// Game object shit
	static void AddGameObject(GameObject* obj, bool has_mesh = true);
};