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

	static Renderable* AddSimpleAsset(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, RotationType type = RotationType::xyz);
	static Renderable* AddSimpleAnimAsset(std::string modelname, std::string materials, std::vector<std::string> AnimationFiles, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Renderable* AddRenderableCollider(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotation);

	static std::vector<Renderable*>& GenerateHouse1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateHouse2(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateHouse3(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateHouse4(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateBarn1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateBarn2(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateGateway(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateScaffolding(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateWall(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateWell(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateTavern(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateTree(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateLadder(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateRock1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateLamp(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateMeetingHall(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateTower(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateCart(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateStraw(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateDebris1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateRubble1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateRubble2(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*>& GenerateRubble3(XMFLOAT3 pos, XMFLOAT3 rotation);


public:
	// Game object shit
	static void AddGameObject(GameObject* obj, bool has_mesh = true);
};