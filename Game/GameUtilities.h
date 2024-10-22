#pragma once

#include "Golem.h"
#include "GameplayAI.h"
#include "GameObjectClasses.h"
#include "GameEffects.h"
#include "ADAI.h"
#include "Engine.h"

#include <string>

class GameUtilities
{
public:
	static ADUtils::SHADER shader;
	static Trigger* AddTriggerFromModelFile(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static MessageTrigger* AddMessageTrigger(ADUI::UIMessage _message, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Destructable* AddDestructableFromModelFile(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static ADAI::VillagerAI* AttachVillagerAI(ADResource::ADGameplay::Destructable* _destructable, ADAI::VillagerGroup* _localGroup, std::vector< ADResource::ADGameplay::GameObject*>* _fearGroup, std::vector< ADResource::ADGameplay::Building*>* _safetyGroup, Engine* _engine);
	static ADAI::MinionAI* AttachMinionAI(ADResource::ADGameplay::Destructable* _destructable, ADAI::MinionGroup* _localGroup, std::vector< ADResource::ADGameplay::GameObject*>* _killGroup, OBJECT_TAG _minionType, ADAI::PathersQueue* _pathingQueue);
	static ADAI::TowerAI* AttachTowerAI(Building* _destructable, std::vector< ADResource::ADGameplay::GameObject*>* _killGroup);

	static Golem* LoadGolemFromModelFile(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);

	static Renderable* AddSimpleAsset(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool instanced = false, RotationType type = RotationType::xyz);
	static Renderable* AddSimpleAnimAsset(std::string modelname, std::string materials, std::vector<std::string> AnimationFiles, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static Renderable* AddRenderableCollider();

	static std::vector<Renderable*> GenerateHouse1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateHouse2(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateHouse3(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateHouse4(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateBarn1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateBarn2(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateGateway(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateScaffolding(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateScaffoldWallX(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateScaffoldWallZ(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateWall(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateWell(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateTavern(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateTree(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateLadder(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRock1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateLamp(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateMeetingHall(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateTower(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateCart(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateStraw(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateDebris1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRubble1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRubble2(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRubble3(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateAttackTower(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateBallista(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRockWall1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRockWall2(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRockWall3(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRockWall4(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateRockWall5(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GeneratePathway1(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GeneratePathway2(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GeneratePathway3(XMFLOAT3 pos, XMFLOAT3 rotation);
	static std::vector<Renderable*> GenerateBanner(XMFLOAT3 pos, XMFLOAT3 rotation);

public:
	// Game object shit
	static void AddGameObject(GameObject* obj, bool has_mesh = true);
};