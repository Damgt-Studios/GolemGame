#pragma once
#include "ResourceManager.h"
#include "../Game/GameUtilities.h"
#include "../Game/GameObjectClasses.h"
#include "../Game/GameplayAI.h"
#include "MeshLoader.h"
#include "ADCombat.h"
#include "ADEventSystem.h"
#include "ADUserInterface.h"
#include "Engine.h"

#include <iostream>
#include <fstream>

#include <vector>
//#include "../Game/GameUserInterface.h"


namespace ADGameplay
{
	class Scene
	{
	private:

		struct PBRArguments
		{
			std::array<char, 260> Model;
			std::array<char, 260> Texture;
			XMFLOAT3 position = { 0,0,0 }, scale = { 0,0,0 }, rotation = { 0,0,0 };
		};
		struct PhysicsArgs
		{
			std::array<char, 260> Model;
			std::array<char, 260> Texture;
			XMFLOAT3 position = { 0,0,0 }, scale = { 0,0,0 }, rotation = { 0,0,0 };
			ADResource::ADGameplay::OBJECT_PHYSICS_TYPE type;
		};

		std::vector<Light> sceneLights;
		PBRArguments GolemArguments;
		ADResource::ADGameplay::Golem* golem;
		PBRArguments PlaneArguments;
		Renderable* Plane;
		PBRArguments SkyboxArguments;
		std::vector<PhysicsArgs> renderableArguments;
		std::vector<Renderable*> renderables;

		//Minion Arguments
		std::vector<PBRArguments> stoneMinionArguments;
		std::vector<PBRArguments> waterMinionArguments;
		std::vector<PBRArguments> fireMinionArguments;
		std::vector<PBRArguments> woodMinionArguments;

		std::vector<PBRArguments> villagerArguments;

		//Minions
		std::vector<Destructable*> stoneMinions;
		std::vector<Destructable*> waterMinions;
		std::vector<Destructable*> fireMinions;
		std::vector<Destructable*> woodMinions;

		std::vector<Destructable*> villagers;
		std::vector<ADAI::TowerAI*> towers;
		std::vector<Building*> buildings;


		std::vector<ADResource::ADGameplay::GameObject*> villagerThreats;
		std::vector<ADResource::ADGameplay::GameObject*> minionTargets;
		std::vector<ADResource::ADGameplay::GameObject*> minionAvoids;

		//Minion AI
		std::vector<ADAI::MinionAI*> stoneMinionsAI;
		std::vector<ADAI::MinionAI*> waterMinionsAI;
		std::vector<ADAI::MinionAI*> fireMinionsAI;
		std::vector<ADAI::MinionAI*> woodMinionsAI;

		std::vector<ADAI::VillagerAI*> villagerAI;

		//Animation vecters
		std::vector<std::string> animations;
		std::vector<std::string> stoneMinionAnimations;
		std::vector<std::string> waterMinionAnimations;
		std::vector<std::string> fireMinionAnimations;
		std::vector<std::string> woodMinionAnimations;

		std::vector<std::string> villagerAnimations;


		//Target
		Trigger* m1;

		//Events
		UINT stoneMinionCount = 10;
		UINT waterMinionCount = 10;
		UINT fireMinionCount = 10;
		UINT woodMinionCount = 10;
		UINT totalMinionCount = 40;
		UINT villagerCount = 50;

		//String stuff for the events.
		std::string* stoneMinionCountstr;
		std::string* waterMinionCountstr;
		std::string* fireMinionCountstr;
		std::string* woodMinionCountstr;
		std::string* allMinionCountstr;

		std::string* villagerCountstr;
		ADAI::VillagerGroup villageFlock1;
		ADAI::VillagerGroup villageFlock2;
		ADAI::VillagerGroup villageFlock3;
		ADAI::VillagerGroup villageFlock4;
		ADAI::VillagerGroup villageFlock5;

		void AddRubble(ADResource::ADGameplay::Building* _building, int _rubbleID, XMFLOAT3 rotation, XMFLOAT3 collider_scale, XMFLOAT3 offset, Engine* _engine, bool _townhall = false)
		{
			Building* rubble;
			switch (_rubbleID)
			{
			case 1:
				rubble = new Building(_building->GetPosition(), rotation, collider_scale, offset, GameUtilities::GenerateRubble1, "Rubble");
				break;													  
			case 2:														  
				rubble = new Building(_building->GetPosition(), rotation, collider_scale, offset, GameUtilities::GenerateRubble2, "Rubble");
				break;													  
			case 3:														  
				rubble = new Building(_building->GetPosition(), rotation, collider_scale, offset, GameUtilities::GenerateRubble3, "Rubble");
				break;
			default:
				return;
				break;
			}
			rubble->active = false;
			GameUtilities::AddGameObject(rubble);
			_building->SetRubble(rubble, golem, _townhall);
			_building->destructionEmitter = &_engine->destructionCloud;
			_building->destructionEmitter2 = &_engine->destructionCloud;
			//I don't feel like pasting this 3000s times.... so here it is.
			AddBuilding(_building);
		}

		void AddBallista()
		{

		}

		void InitializeScene(Engine* _engine)
		{


			villageFlock1.groupCohesionStrength = 0.01f;
			villageFlock1.groupSeparationStrength = 1.0;
			villageFlock2.groupCohesionStrength = 0.01f;
			villageFlock2.groupSeparationStrength = 1.0;
			villageFlock3.groupCohesionStrength = 0.01f;
			villageFlock3.groupSeparationStrength = 1.0;
			villageFlock3.groupCohesionStrength = 0.01f;
			villageFlock3.groupSeparationStrength = 1.0;
			villageFlock4.groupCohesionStrength = 0.01f;
			villageFlock4.groupSeparationStrength = 1.0;
			villageFlock5.groupCohesionStrength = 0.01f;
			villageFlock5.groupSeparationStrength = 1.0;

			std::vector<std::string> bucketheadAnimations;
			bucketheadAnimations.push_back("files/models/Bucket_Idle.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Run.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Fear.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Death.animfile");

			

#pragma region Level
			
			//=========================================================Tutorial Stage
			
			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(435.5 + (i * 40.5), 0, -900), XMFLOAT3(0, 0, 0), XMFLOAT3(42, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(435.5 + (i * 40.5), 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(42, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(435.5 + (i * 40.5), 0, -300), XMFLOAT3(0, 0, 0), XMFLOAT3(42, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(435.5 + (i * 40.5), 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(42, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				minionAvoids.push_back(wall);
			}

			//===================================================Castle

			Building* gate1 = new Building(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway, "Gate");
			gate1->AddObject(XMFLOAT3(5,5,9.5), XMFLOAT3(0,180,0), GameUtilities::GenerateBanner);
			gate1->AddObject(XMFLOAT3(-5,5,9.5), XMFLOAT3(0,180,0), GameUtilities::GenerateBanner);
			gate1->AddObject(XMFLOAT3(5,5,-9.5), XMFLOAT3(0,0,0), GameUtilities::GenerateBanner);
			gate1->AddObject(XMFLOAT3(-5,5,-9.5), XMFLOAT3(0,0,0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(gate1);
			minionTargets.push_back(gate1);

			Building* gate2 = new Building(XMFLOAT3(-474, 0, -595.5), XMFLOAT3(0, 90, 0), XMFLOAT3(40, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway, "Gate");
			gate2->AddObject(XMFLOAT3(-483.5, 5, -590.5), XMFLOAT3(0, 90, 0), GameUtilities::GenerateBanner);
			gate2->AddObject(XMFLOAT3(-464.5, 5, -590.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			gate2->AddObject(XMFLOAT3(-483.5, 5, -600.5), XMFLOAT3(0, 90, 0), GameUtilities::GenerateBanner);
			gate2->AddObject(XMFLOAT3(-464.5, 5, -600.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(gate2);
			minionTargets.push_back(gate2);

			float safeRad = 15;
			float avoidRad= 50;
			Building* wall1 = new Building(XMFLOAT3(40.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall1);
			wall1->safeRadius = safeRad;
			wall1->avoidRadius = avoidRad;
			minionAvoids.push_back(wall1);

			Building* wall2 = new Building(XMFLOAT3(81, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall2);
			wall2->safeRadius = safeRad;
			wall2->avoidRadius = avoidRad;
			minionAvoids.push_back(wall2);

			Building* wall3 = new Building(XMFLOAT3(121.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall3);
			wall3->safeRadius = safeRad;
			wall3->avoidRadius = avoidRad;
			minionAvoids.push_back(wall3);

			Building* wall4 = new Building(XMFLOAT3(162, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall4);
			wall4->safeRadius = safeRad;
			wall4->avoidRadius = avoidRad;
			minionAvoids.push_back(wall4);

			Building* wall5 = new Building(XMFLOAT3(202.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall5);
			wall5->safeRadius = safeRad;
			wall5->avoidRadius = avoidRad;
			minionAvoids.push_back(wall5);

			Building* wall6 = new Building(XMFLOAT3(243, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall6);
			wall6->safeRadius = safeRad;
			wall6->avoidRadius = avoidRad;
			minionAvoids.push_back(wall6);

			for (unsigned int i = 0; i < 20; i++)
			{
				Building* wall = new Building(XMFLOAT3(271.5, 0, -28.5f - (i * 40.5f)), XMFLOAT3(0, 90, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				wall->safeRadius = safeRad;
				wall->avoidRadius = avoidRad;
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 18; i++)
			{
				Building* wall = new Building(XMFLOAT3(243 - (i * 40.5), 0, -826.5), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				wall->safeRadius = safeRad;
				wall->avoidRadius = avoidRad;
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(-474, 0, -798 + (i * 40.5)), XMFLOAT3(0, 90, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				wall->safeRadius = safeRad;
				wall->avoidRadius = avoidRad;
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(-474, 0, -555 + (i * 40.5)), XMFLOAT3(0, 90, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				wall->safeRadius = safeRad;
				wall->avoidRadius = avoidRad;
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(-445.5 + (i * 40.5), 0, -364.5), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				wall->safeRadius = safeRad;
				wall->avoidRadius = avoidRad;
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(-255, 0, -336 + (i * 40.5)), XMFLOAT3(0, 90, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				wall->safeRadius = safeRad;
				wall->avoidRadius = avoidRad;
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 15; i++)
			{
				Building* wall = new Building(XMFLOAT3(-40.5 - (i * 40.5f), 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				wall->safeRadius = safeRad;
				wall->avoidRadius = avoidRad;
				minionAvoids.push_back(wall);
			}

			for (unsigned int i = 0; i < 9; i++)
			{
				Building* wall = new Building(XMFLOAT3(-283.5 - (i * 40.5), 0, -145.5), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 35, 20), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
				wall->safeRadius = safeRad;
				wall->avoidRadius = avoidRad;
				minionAvoids.push_back(wall);
			}

			
			Building* tower1 = new Building(XMFLOAT3(271.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			tower1->AddObject(XMFLOAT3(271.5, 20, 9), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			tower1->AddObject(XMFLOAT3(280.5, 20, 0), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(tower1);
			AddRubble(tower1, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), _engine);

			Building* tower2 = new Building(XMFLOAT3(271.5, 0, -826.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			tower2->AddObject(XMFLOAT3(271.5, 20, -835.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBanner);
			tower2->AddObject(XMFLOAT3(280.5, 20, -826.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(tower2);
			AddRubble(tower2, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), _engine);

			Building* tower3 = new Building(XMFLOAT3(-474, 0, -826.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			tower3->AddObject(XMFLOAT3(-474, 20, -835.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBanner);
			tower3->AddObject(XMFLOAT3(-483, 20, -826.5), XMFLOAT3(0, 90, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(tower3);
			AddRubble(tower3, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), _engine);

			Building* tower4 = new Building(XMFLOAT3(-474, 0, -364.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			tower4->AddObject(XMFLOAT3(-483, 20, -364.5), XMFLOAT3(0, 90, 0), GameUtilities::GenerateBanner);
			tower4->AddObject(XMFLOAT3(-474, 20, -355.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(tower4);
			AddRubble(tower4, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), _engine);

			Building* tower5 = new Building(XMFLOAT3(-255, 0, -364.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			tower5->AddObject(XMFLOAT3(-246, 20, -364.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			tower5->AddObject(XMFLOAT3(-255, 20, -373.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(tower5);
			AddRubble(tower5, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), _engine);

			Building* tower6 = new Building(XMFLOAT3(-255, 0, -145.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			tower6->AddObject(XMFLOAT3(-246, 20, -145.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			tower6->AddObject(XMFLOAT3(-255, 20, -136.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(tower6);
			AddRubble(tower6, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), _engine);

			//========================================================Houses

			Building* tutorial_home1 = new Building(XMFLOAT3(570, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(tutorial_home1);
			AddRubble(tutorial_home1, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home2 = new Building(XMFLOAT3(570, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home2);
			AddRubble(tutorial_home2, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home3 = new Building(XMFLOAT3(460, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(tutorial_home3);
			AddRubble(tutorial_home3, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home4 = new Building(XMFLOAT3(460, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(tutorial_home4);
			AddRubble(tutorial_home4, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home5 = new Building(XMFLOAT3(570, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home5);
			AddRubble(tutorial_home5, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home6 = new Building(XMFLOAT3(570, 0, -850), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(tutorial_home6);
			AddRubble(tutorial_home6, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home7 = new Building(XMFLOAT3(460, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(tutorial_home7);
			AddRubble(tutorial_home7, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home8 = new Building(XMFLOAT3(460, 0, -850), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home8);
			AddRubble(tutorial_home8, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home9 = new Building(XMFLOAT3(570, 0, -100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home9);
			AddRubble(tutorial_home9, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home10 = new Building(XMFLOAT3(570, 0, -200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(tutorial_home10);
			AddRubble(tutorial_home10, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home11 = new Building(XMFLOAT3(460, 0, -100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(tutorial_home11);
			AddRubble(tutorial_home11, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* tutorial_home12 = new Building(XMFLOAT3(460, 0, -200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(tutorial_home12);
			AddRubble(tutorial_home12, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);
			
			Building* home1 = new Building(XMFLOAT3(575, 0, 200), XMFLOAT3(0, -60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home1);
			AddRubble(home1, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home2 = new Building(XMFLOAT3(525, 0, 250), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home2);
			AddRubble(home2, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home3 = new Building(XMFLOAT3(475, 0, 300), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home3);
			AddRubble(home3, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home4 = new Building(XMFLOAT3(475, 0, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home4);
			AddRubble(home4, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home5 = new Building(XMFLOAT3(475, 0, 400), XMFLOAT3(0, 30, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home5);
			AddRubble(home5, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home6 = new Building(XMFLOAT3(525, 0, 450), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home6);
			AddRubble(home6, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home7 = new Building(XMFLOAT3(575, 0, 500), XMFLOAT3(0, 60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home7);
			AddRubble(home7, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home8 = new Building(XMFLOAT3(200, 0, 575), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home8);
			AddRubble(home8, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home9 = new Building(XMFLOAT3(250, 0, 525), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home9);
			AddRubble(home9, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home10 = new Building(XMFLOAT3(300, 0, 475), XMFLOAT3(0, -60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home10);
			AddRubble(home10, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home11 = new Building(XMFLOAT3(350, 0, 475), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home11);
			AddRubble(home11, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home12 = new Building(XMFLOAT3(400, 0, 475), XMFLOAT3(0, 60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home12);
			AddRubble(home12, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home13 = new Building(XMFLOAT3(450, 0, 525), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home13);
			AddRubble(home13, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home14 = new Building(XMFLOAT3(500, 0, 575), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home14);
			AddRubble(home14, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home15 = new Building(XMFLOAT3(440, 0, 80), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home15);
			AddRubble(home15, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home16 = new Building(XMFLOAT3(400, 0, 120), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home16);
			AddRubble(home16, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home17 = new Building(XMFLOAT3(360, 0, 160), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home17);
			AddRubble(home17, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home18 = new Building(XMFLOAT3(320, 0, 200), XMFLOAT3(0, 135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home18);
			AddRubble(home18, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home19 = new Building(XMFLOAT3(280, 0, 240), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home19);
			AddRubble(home19, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home20 = new Building(XMFLOAT3(240, 0, 280), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home20);
			AddRubble(home20, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home21 = new Building(XMFLOAT3(180, 0, 275), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home21);
			AddRubble(home21, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home22 = new Building(XMFLOAT3(120, 0, 270), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home22);
			AddRubble(home22, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home23 = new Building(XMFLOAT3(60, 0, 265), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home23);
			AddRubble(home23, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home24 = new Building(XMFLOAT3(0, 0, 260), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home24);
			AddRubble(home24, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home25 = new Building(XMFLOAT3(-60, 0, 255), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home25);
			AddRubble(home25, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home26 = new Building(XMFLOAT3(-120, 0, 250), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home26);
			AddRubble(home26, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home27 = new Building(XMFLOAT3(180, 0, 500), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home27);
			AddRubble(home27, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home28 = new Building(XMFLOAT3(120, 0, 495), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home28);
			AddRubble(home28, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home29 = new Building(XMFLOAT3(60, 0, 490), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home29);
			AddRubble(home29, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home30 = new Building(XMFLOAT3(0, 0, 485), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home30);
			AddRubble(home30, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home31 = new Building(XMFLOAT3(-60, 0, 480), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home31);
			AddRubble(home31, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home32 = new Building(XMFLOAT3(-120, 0, 475), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home32);
			AddRubble(home32, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home33 = new Building(XMFLOAT3(-180, 0, 465), XMFLOAT3(0, -135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home33);
			AddRubble(home33, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home34 = new Building(XMFLOAT3(-220, 0, 435), XMFLOAT3(0, -225, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home34);
			AddRubble(home34, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home35 = new Building(XMFLOAT3(-250, 0, 395), XMFLOAT3(0, 60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home35);
			AddRubble(home35, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home36 = new Building(XMFLOAT3(-270, 0, 345), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home36);
			AddRubble(home36, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home37 = new Building(XMFLOAT3(-270, 0, 165), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home37);
			AddRubble(home37, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home38 = new Building(XMFLOAT3(-250, 0, 115), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home38);
			AddRubble(home38, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home39 = new Building(XMFLOAT3(-220, 0, 75), XMFLOAT3(0, 30, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home39);
			AddRubble(home39, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home40 = new Building(XMFLOAT3(-180, 0, 45), XMFLOAT3(0, 30, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home40);
			AddRubble(home40, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home41 = new Building(XMFLOAT3(300, 0, -1000), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home41);
			AddRubble(home41, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home42 = new Building(XMFLOAT3(250, 0, -1050), XMFLOAT3(0, 60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home42);
			AddRubble(home42, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home43 = new Building(XMFLOAT3(200, 0, -1100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home43);
			AddRubble(home43, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home44 = new Building(XMFLOAT3(150, 0, -1050), XMFLOAT3(0, -60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home44);
			AddRubble(home44, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home45 = new Building(XMFLOAT3(100, 0, -1000), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home45);
			AddRubble(home45, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home46 = new Building(XMFLOAT3(0, 0, -1000), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home46);
			AddRubble(home46, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home47 = new Building(XMFLOAT3(-50, 0, -1050), XMFLOAT3(0, 60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home47);
			AddRubble(home47, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home48 = new Building(XMFLOAT3(-100, 0, -1100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home48);
			AddRubble(home48, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home49 = new Building(XMFLOAT3(-150, 0, -1050), XMFLOAT3(0, -60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home49);
			AddRubble(home49, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home50 = new Building(XMFLOAT3(-200, 0, -1000), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home50);
			AddRubble(home50, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home51 = new Building(XMFLOAT3(-300, 0, -1000), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home51);
			AddRubble(home51, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home52 = new Building(XMFLOAT3(-350, 0, -1050), XMFLOAT3(0, 60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home52);
			AddRubble(home52, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home53 = new Building(XMFLOAT3(-400, 0, -1100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home53);
			AddRubble(home53, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home54 = new Building(XMFLOAT3(-450, 0, -1050), XMFLOAT3(0, -60, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home54);
			AddRubble(home54, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home55 = new Building(XMFLOAT3(-500, 0, -1000), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home55);
			AddRubble(home55, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);
			
			Building* home56 = new Building(XMFLOAT3(-200, 0, -30), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home56);
			AddRubble(home56, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home57 = new Building(XMFLOAT3(-125, 0, -30), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(2, 0.5, 0), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home57);
			AddRubble(home57, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home58 = new Building(XMFLOAT3(-50, 0, -30), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home58);
			AddRubble(home58, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home59 = new Building(XMFLOAT3(50, 0, -30), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home59);
			AddRubble(home59, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home60 = new Building(XMFLOAT3(125, 0, -30), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home60);
			AddRubble(home60, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home61 = new Building(XMFLOAT3(200, 0, -30), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home61);
			AddRubble(home61, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home62 = new Building(XMFLOAT3(-275, 0, -30), XMFLOAT3(0, -90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home62);
			AddRubble(home62, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home63 = new Building(XMFLOAT3(-350, 0, -30), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home63);
			AddRubble(home63, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home64 = new Building(XMFLOAT3(-275, 0, -125), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home64);
			AddRubble(home64, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home65 = new Building(XMFLOAT3(-350, 0, -125), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home65);
			AddRubble(home65, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home66 = new Building(XMFLOAT3(225, 0, -75), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home66);
			AddRubble(home66, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home67 = new Building(XMFLOAT3(225, 0, -150), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home67);
			AddRubble(home67, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home68 = new Building(XMFLOAT3(225, 0, -225), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home68);
			AddRubble(home68, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home69 = new Building(XMFLOAT3(225, 0, -300), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home69);
			AddRubble(home69, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home70 = new Building(XMFLOAT3(225, 0, -375), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home70);
			AddRubble(home70, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home71 = new Building(XMFLOAT3(225, 0, -450), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home71);
			AddRubble(home71, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home72 = new Building(XMFLOAT3(225, 0, -525), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home72);
			AddRubble(home72, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home73 = new Building(XMFLOAT3(225, 0, -600), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home73);
			AddRubble(home73, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home74 = new Building(XMFLOAT3(225, 0, -675), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home74);
			AddRubble(home74, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home75 = new Building(XMFLOAT3(225, 0, -750), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home75);
			AddRubble(home75, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home76 = new Building(XMFLOAT3(-225, 0, -150), XMFLOAT3(0, 135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home76);
			AddRubble(home76, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home77 = new Building(XMFLOAT3(-187.5, 0, -187.5), XMFLOAT3(0, 135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home77);
			AddRubble(home77, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home78 = new Building(XMFLOAT3(-150, 0, -225), XMFLOAT3(0, 135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home78);
			AddRubble(home78, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home79 = new Building(XMFLOAT3(-112.5, 0, -262.5), XMFLOAT3(0, 135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home79);
			AddRubble(home79, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home80 = new Building(XMFLOAT3(-75, 0, -300), XMFLOAT3(0, 135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home80);
			AddRubble(home80, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home81 = new Building(XMFLOAT3(-225, 0, -350), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home81);
			AddRubble(home81, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home82 = new Building(XMFLOAT3(-187.5, 0, -387.5), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home82);
			AddRubble(home82, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home83 = new Building(XMFLOAT3(-150, 0, -425), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home83);
			AddRubble(home83, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home84 = new Building(XMFLOAT3(-112.5, 0, -462.5), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home84);
			AddRubble(home84, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home85 = new Building(XMFLOAT3(-75, 0, -500), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home85);
			AddRubble(home85, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home86 = new Building(XMFLOAT3(150, 0, -550), XMFLOAT3(0, 200, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home86);
			AddRubble(home86, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home87 = new Building(XMFLOAT3(125, 0, -600), XMFLOAT3(0, 20, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home87);
			AddRubble(home87, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home88 = new Building(XMFLOAT3(100, 0, -650), XMFLOAT3(0, 200, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home88);
			AddRubble(home88, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home89 = new Building(XMFLOAT3(75, 0, -700), XMFLOAT3(0, 20, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home89);
			AddRubble(home89, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home90 = new Building(XMFLOAT3(50, 0, -750), XMFLOAT3(0, 20, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home90);
			AddRubble(home90, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home91 = new Building(XMFLOAT3(-25, 0, -375), XMFLOAT3(0, 30, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home91);
			AddRubble(home91, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home92 = new Building(XMFLOAT3(12.5, 0, -337.5), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home92);
			AddRubble(home92, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home93 = new Building(XMFLOAT3(50, 0, -300), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home93);
			AddRubble(home93, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home94 = new Building(XMFLOAT3(87.5, 0, -337.5), XMFLOAT3(0, 135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home94);
			AddRubble(home94, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home95 = new Building(XMFLOAT3(125, 0, -375), XMFLOAT3(0, 150, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home95);
			AddRubble(home95, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home96 = new Building(XMFLOAT3(-200, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home96);
			AddRubble(home96, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home97 = new Building(XMFLOAT3(-350, 0, -600), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home97);
			AddRubble(home97, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home98 = new Building(XMFLOAT3(-275, 0, -525), XMFLOAT3(0, 270, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home98);
			AddRubble(home98, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home99 = new Building(XMFLOAT3(-275, 0, -675), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home99);
			AddRubble(home99, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home100 = new Building(XMFLOAT3(-225, 0, -550), XMFLOAT3(0, 315, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home100);
			AddRubble(home100, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home101 = new Building(XMFLOAT3(-325, 0, -550), XMFLOAT3(0, 225, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home101);
			AddRubble(home101, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home102 = new Building(XMFLOAT3(-225, 0, -650), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home102);
			AddRubble(home102, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home103 = new Building(XMFLOAT3(-325, 0, -650), XMFLOAT3(0, 135, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home103);
			AddRubble(home103, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home104 = new Building(XMFLOAT3(0, 0, -650), XMFLOAT3(0, 70, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home104);
			AddRubble(home104, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home105 = new Building(XMFLOAT3(-75, 0, -675), XMFLOAT3(0, 70, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home105);
			AddRubble(home105, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home106 = new Building(XMFLOAT3(-150, 0, -700), XMFLOAT3(0, 70, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home106);
			AddRubble(home106, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home107 = new Building(XMFLOAT3(-225, 0, -725), XMFLOAT3(0, 70, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home107);
			AddRubble(home107, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home108 = new Building(XMFLOAT3(-300, 0, -750), XMFLOAT3(0, 70, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home108);
			AddRubble(home108, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home109 = new Building(XMFLOAT3(-375, 0, -775), XMFLOAT3(0, 70, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home109);
			AddRubble(home109, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home110 = new Building(XMFLOAT3(-275, 0, -400), XMFLOAT3(0, 270, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse3, "House3");
			GameUtilities::AddGameObject(home110);
			AddRubble(home110, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home111 = new Building(XMFLOAT3(-325, 0, -425), XMFLOAT3(0, 240, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse4, "House4");
			GameUtilities::AddGameObject(home111);
			AddRubble(home111, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home112 = new Building(XMFLOAT3(-375, 0, -475), XMFLOAT3(0, 210, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home112);
			AddRubble(home112, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			Building* home113 = new Building(XMFLOAT3(-425, 0, -550), XMFLOAT3(0, 180, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse2, "House2");
			GameUtilities::AddGameObject(home113);
			AddRubble(home113, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), _engine);

			//========================================Large Buildings
			float safeRad2 = 25;
			Building* tavern1 = new Building(XMFLOAT3(-400, 0, 250), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 50), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			tavern1->safeRadius = safeRad2;
			tavern1->attackRadius = 18.f;
			GameUtilities::AddGameObject(tavern1);
			AddRubble(tavern1, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), _engine, true);

			Building* tavern2 = new Building(XMFLOAT3(-400, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 50), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			tavern2->safeRadius = safeRad2;
			tavern2->attackRadius = 18.f;
			GameUtilities::AddGameObject(tavern2);
			AddRubble(tavern2, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), _engine, true);

			Building* tavern3 = new Building(XMFLOAT3(200, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 50), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			tavern3->safeRadius = safeRad2;
			tavern3->attackRadius = 18.f;
			GameUtilities::AddGameObject(tavern3);
			AddRubble(tavern3, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), _engine, true);

			Building* tavern4 = new Building(XMFLOAT3(200, 0, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 50), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			tavern4->safeRadius = safeRad2;
			tavern4->attackRadius = 18.f;
			GameUtilities::AddGameObject(tavern4);
			AddRubble(tavern4, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), _engine, true);

			Building* meeting_hall1 = new Building(XMFLOAT3(-100, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			meeting_hall1->safeRadius = safeRad2;
			GameUtilities::AddGameObject(meeting_hall1);
			AddRubble(meeting_hall1, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), _engine, true);

			Building* meeting_hall2 = new Building(XMFLOAT3(50, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			meeting_hall2->safeRadius = safeRad2;
			GameUtilities::AddGameObject(meeting_hall2);
			AddRubble(meeting_hall2, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), _engine, true);

			Building* meeting_hall3 = new Building(XMFLOAT3(-275, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			meeting_hall3->safeRadius = safeRad2;
			GameUtilities::AddGameObject(meeting_hall3);
			AddRubble(meeting_hall3, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), _engine, true);

			Building* meeting_hall4 = new Building(XMFLOAT3(-475, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			meeting_hall4->safeRadius = safeRad2;
			GameUtilities::AddGameObject(meeting_hall4);
			AddRubble(meeting_hall4, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), _engine, true);

			//===================================================Ballista Towers
			
			Building* attackTower1 = new Building(XMFLOAT3(450, 0, -350), XMFLOAT3(0, 180, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower1->AddObject(XMFLOAT3(450, 2, -355.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower1);

			Building* ballista1 = new Building(XMFLOAT3(450, 20, -350), XMFLOAT3(0, 180, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista1);
			Building* rubble1 = new Building(XMFLOAT3(450, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			rubble1->active = false;
			GameUtilities::AddGameObject(rubble1);
			attackTower1->SetRubble(rubble1, golem);
			attackTower1->SetTurret(ballista1);
			AddBuilding(attackTower1);

			Building* attackTower2 = new Building(XMFLOAT3(580, 0, -350), XMFLOAT3(0, 180, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower2->AddObject(XMFLOAT3(580, 2, -355.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower2);

			Building* ballista2 = new Building(XMFLOAT3(580, 20, -350), XMFLOAT3(0, 180, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista2);
			Building* rubble2 = new Building(XMFLOAT3(580, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble2);
			attackTower2->SetRubble(rubble2, golem);
			attackTower2->SetTurret(ballista2);
			AddBuilding(attackTower2);

			Building* attackTower3 = new Building(XMFLOAT3(515, 0, -400), XMFLOAT3(0, 180, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower3->AddObject(XMFLOAT3(515, 2, -405.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower3);

			Building* ballista3 = new Building(XMFLOAT3(515, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista3);	
			Building* rubble3 = new Building(XMFLOAT3(515, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble3);
			attackTower3->SetRubble(rubble3, golem);
			attackTower3->SetTurret(ballista3);
			AddBuilding(attackTower3);

			Building* attackTower4 = new Building(XMFLOAT3(550, 0, 350), XMFLOAT3(0, 270, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower4->AddObject(XMFLOAT3(544.5, 2, 350), XMFLOAT3(0, 90, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower4);

			Building* ballista4 = new Building(XMFLOAT3(550, 20, 350), XMFLOAT3(0, 270, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista4);
			Building* rubble4 = new Building(XMFLOAT3(550, 0, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble4);
			attackTower4->SetRubble(rubble4, golem);
			attackTower4->SetTurret(ballista4);
			AddBuilding(attackTower4);

			Building* attackTower5 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 180, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower5->AddObject(XMFLOAT3(350, 2, 544.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower5);

			Building* ballista5 = new Building(XMFLOAT3(350, 20, 550), XMFLOAT3(0, 180, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista5);
			Building* rubble5 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble5);
			attackTower5->SetRubble(rubble5, golem);
			attackTower5->SetTurret(ballista5);
			AddBuilding(attackTower5);

			Building* attackTower6 = new Building(XMFLOAT3(250, 0, 375), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower6->AddObject(XMFLOAT3(255.5, 2, 375), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower6);

			Building* ballista6 = new Building(XMFLOAT3(250, 20, 375), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista6);
			Building* rubble6 = new Building(XMFLOAT3(250, 0, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble6);
			attackTower6->SetRubble(rubble6, golem);
			attackTower6->SetTurret(ballista6);
			AddBuilding(attackTower6);

			Building* attackTower7 = new Building(XMFLOAT3(-25, 0, 450), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower7->AddObject(XMFLOAT3(-19.5, 2, 450), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower7);

			Building* ballista7 = new Building(XMFLOAT3(-25, 20, 450), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista7);
			Building* rubble7 = new Building(XMFLOAT3(-25, 0, 450), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble7);
			attackTower7->SetRubble(rubble7, golem);
			attackTower7->SetTurret(ballista7);
			AddBuilding(attackTower7);

			Building* attackTower8 = new Building(XMFLOAT3(-25, 0, 300), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower8->AddObject(XMFLOAT3(-19.5, 2, 300), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower8);

			Building* ballista8 = new Building(XMFLOAT3(-25, 20, 300), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista8);
			Building* rubble8 = new Building(XMFLOAT3(-25, 0, 300), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble8);
			attackTower8->SetRubble(rubble8, golem);
			attackTower8->SetTurret(ballista8);
			AddBuilding(attackTower8);

			Building* attackTower9 = new Building(XMFLOAT3(-275, 0, 212.5), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower9->AddObject(XMFLOAT3(-269.5, 2, 212.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower9);

			Building* ballista9 = new Building(XMFLOAT3(-275, 20, 212.5), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista9);
			Building* rubble9 = new Building(XMFLOAT3(-275, 0, 212.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble9);
			attackTower9->SetRubble(rubble9, golem);
			attackTower9->SetTurret(ballista9);
			AddBuilding(attackTower9);

			Building* attackTower10 = new Building(XMFLOAT3(-275, 0, 287.5), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower10->AddObject(XMFLOAT3(-269.5, 2, 287.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower10);

			Building* ballista10 = new Building(XMFLOAT3(-275, 20, 287.5), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista10);
			Building* rubble10 = new Building(XMFLOAT3(-275, 0, 287.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble10);
			attackTower10->SetRubble(rubble10, golem);
			attackTower10->SetTurret(ballista10);
			AddBuilding(attackTower10);

			Building* attackTower11 = new Building(XMFLOAT3(-25, 0, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower11->AddObject(XMFLOAT3(-25, 2, 30.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower11);

			Building* ballista11 = new Building(XMFLOAT3(-25, 20, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista11);
			Building* rubble11 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble11);
			attackTower11->SetRubble(rubble11, golem);
			attackTower11->SetTurret(ballista11);
			AddBuilding(attackTower11);

			Building* attackTower12 = new Building(XMFLOAT3(25, 0, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower12->AddObject(XMFLOAT3(25, 2, 30.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower12);

			Building* ballista12 = new Building(XMFLOAT3(25, 20, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista12);
			Building* rubble12 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble12);
			attackTower12->SetRubble(rubble12, golem);
			attackTower12->SetTurret(ballista12);
			AddBuilding(attackTower12);

			Building* attackTower13 = new Building(XMFLOAT3(-450, 0, -75), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower13->AddObject(XMFLOAT3(-444.5, 2, -75), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower13);

			Building* ballista13 = new Building(XMFLOAT3(-450, 20, -75), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista13);
			Building* rubble13 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble13);
			attackTower13->SetRubble(rubble13, golem);
			attackTower13->SetTurret(ballista13);
			AddBuilding(attackTower13);

			Building* attackTower14 = new Building(XMFLOAT3(-50, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower14->AddObject(XMFLOAT3(-50, 2, -394.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower14);

			Building* ballista14 = new Building(XMFLOAT3(-50, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista14);
			Building* rubble14 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble14);
			attackTower14->SetRubble(rubble14, golem);
			attackTower14->SetTurret(ballista14);
			AddBuilding(attackTower14);

			Building* attackTower15 = new Building(XMFLOAT3(150, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower15->AddObject(XMFLOAT3(150, 2, -394.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower15);

			Building* ballista15 = new Building(XMFLOAT3(150, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista15);
			Building* rubble15 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble15);
			attackTower15->SetRubble(rubble15, golem);
			attackTower15->SetTurret(ballista15);
			AddBuilding(attackTower15);

			Building* attackTower16 = new Building(XMFLOAT3(-449, 0, -570.5), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower16->AddObject(XMFLOAT3(-443.5, 2, -570.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower16);

			Building* ballista16 = new Building(XMFLOAT3(-449, 20, -570.5), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista16);
			Building* rubble16 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble16);
			attackTower16->SetRubble(rubble16, golem);
			attackTower16->SetTurret(ballista16);
			AddBuilding(attackTower16);

			Building* attackTower17 = new Building(XMFLOAT3(-449, 0, -620.5), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower17->AddObject(XMFLOAT3(-443.5, 2, -620.5), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower17);

			Building* ballista17 = new Building(XMFLOAT3(-449, 20, -620.5), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista17);
			Building* rubble17 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble17);
			attackTower17->SetRubble(rubble17, golem);
			attackTower17->SetTurret(ballista17);
			AddBuilding(attackTower17);

			Building* attackTower18 = new Building(XMFLOAT3(-212.5, 0, -400), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower18->AddObject(XMFLOAT3(-207, 2, -400), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower18);

			Building* ballista18 = new Building(XMFLOAT3(-212.5, 20, -400), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista18);
			Building* rubble18 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble18);
			attackTower18->SetRubble(rubble18, golem);
			attackTower18->SetTurret(ballista18);
			AddBuilding(attackTower18);

			Building* attackTower19 = new Building(XMFLOAT3(-175, 0, -550), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower19->AddObject(XMFLOAT3(-169.5, 2, -550), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower19);

			Building* ballista19 = new Building(XMFLOAT3(-175, 20, -550), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista19);
			Building* rubble19 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble19);
			attackTower19->SetRubble(rubble19, golem);
			attackTower19->SetTurret(ballista19);
			AddBuilding(attackTower19);

			Building* attackTower20 = new Building(XMFLOAT3(-175, 0, -650), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower20->AddObject(XMFLOAT3(-169.5, 2, -650), XMFLOAT3(0, 270, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower20);

			Building* ballista20 = new Building(XMFLOAT3(-175, 20, -650), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista20);
			Building* rubble20 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble20);
			attackTower20->SetRubble(rubble20, golem);
			attackTower20->SetTurret(ballista20);
			AddBuilding(attackTower20);

			Building* attackTower21 = new Building(XMFLOAT3(150, 0, -775), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower21->AddObject(XMFLOAT3(150, 2, -769.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower21);

			Building* ballista21 = new Building(XMFLOAT3(150, 20, -775), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista21);
			Building* rubble21 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble21);
			attackTower21->SetRubble(rubble21, golem);
			attackTower21->SetTurret(ballista21);
			AddBuilding(attackTower21);

			Building* attackTower22 = new Building(XMFLOAT3(-350, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower22->AddObject(XMFLOAT3(-350, 2, -919.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower22);

			Building* ballista22 = new Building(XMFLOAT3(-350, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista22);
			Building* rubble22 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble22);
			attackTower22->SetRubble(rubble22, golem);
			attackTower22->SetTurret(ballista22);
			AddBuilding(attackTower22);

			Building* attackTower23 = new Building(XMFLOAT3(-450, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower23->AddObject(XMFLOAT3(-450, 2, -919.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower23);

			Building* ballista23 = new Building(XMFLOAT3(-450, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista23);
			Building* rubble23 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble23);
			attackTower23->SetRubble(rubble23, golem);
			attackTower23->SetTurret(ballista23);
			AddBuilding(attackTower23);

			Building* attackTower24 = new Building(XMFLOAT3(-50, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower24->AddObject(XMFLOAT3(-50, 2, -919.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower24);

			Building* ballista24 = new Building(XMFLOAT3(-50, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista24);
			Building* rubble24 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble24);
			attackTower24->SetRubble(rubble24, golem);
			attackTower24->SetTurret(ballista24);
			AddBuilding(attackTower24);

			Building* attackTower25 = new Building(XMFLOAT3(-150, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower25->AddObject(XMFLOAT3(-150, 2, -919.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower25);

			Building* ballista25 = new Building(XMFLOAT3(-150, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista25);
			Building* rubble25 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble25);
			attackTower25->SetRubble(rubble25, golem);
			attackTower25->SetTurret(ballista25);
			AddBuilding(attackTower25);

			Building* attackTower26 = new Building(XMFLOAT3(150, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower26->AddObject(XMFLOAT3(150, 2, -919.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower26);

			Building* ballista26 = new Building(XMFLOAT3(150, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista26);
			Building* rubble26 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble26);
			attackTower26->SetRubble(rubble26, golem);
			attackTower26->SetTurret(ballista26);
			AddBuilding(attackTower26);

			Building* attackTower27 = new Building(XMFLOAT3(250, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower27->AddObject(XMFLOAT3(250, 2, -919.5), XMFLOAT3(0, 180, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower27);

			Building* ballista27 = new Building(XMFLOAT3(250, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista27);
			Building* rubble27 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble27);
			attackTower27->SetRubble(rubble27, golem);
			attackTower27->SetTurret(ballista27);
			AddBuilding(attackTower27);


			Building* attackTower28 = new Building(XMFLOAT3(-50, 0, -75), XMFLOAT3(0, 45, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower28->AddObject(XMFLOAT3(-46, 2, -71), XMFLOAT3(0, 225, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower28);

			Building* ballista28 = new Building(XMFLOAT3(-50, 20, -75), XMFLOAT3(0, 45, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista28);
			Building* rubble28 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble28);
			attackTower28->SetRubble(rubble28, golem);
			attackTower28->SetTurret(ballista28);
			AddBuilding(attackTower28);

			Building* attackTower29 = new Building(XMFLOAT3(50, 0, -75), XMFLOAT3(0, -45, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			attackTower29->AddObject(XMFLOAT3(46, 2, -71), XMFLOAT3(0, 135, 0), GameUtilities::GenerateBanner);
			GameUtilities::AddGameObject(attackTower29);

			Building* ballista29 = new Building(XMFLOAT3(50, 20, -75), XMFLOAT3(0, -45, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista29);
			Building* rubble29 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble29);
			attackTower29->SetRubble(rubble29, golem);
			attackTower29->SetTurret(ballista29);
			AddBuilding(attackTower29);

			//================Details / Lights / Etc.

			Light light;
			ZeroMemory(&light, sizeof(Light));
			light.lightType = (int)LIGHTTYPE::DIRECTIONAL;
			light.diffuse = XMFLOAT4(0.5, 0.5, 0.5, 1);
			light.diffuseIntensity = 1;
			light.ambient = XMFLOAT4(0.156f, 0.003f, 0.215f, 1);
			light.ambientIntensity = 3;
			light.lightDirection = XMFLOAT4(0, -0.5f, -1, 1);
			light.position = XMFLOAT4(0, 0, 0, 1);

			Building* lamp1 = new Building(XMFLOAT3(27.5, 0, 295), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp1);

			// Point light
			Light light1;
			ZeroMemory(&light1, sizeof(Light));
			light1.lightType = (int)LIGHTTYPE::POINT;
			light1.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light1.diffuseIntensity = 4;
			light1.position = XMFLOAT4(27.5, 7.5, 297, 1);
			light1.lightRadius = 50;

			Building* lamp2 = new Building(XMFLOAT3(35, 0, 430), XMFLOAT3(0, 180, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp2);

			Light light2;
			ZeroMemory(&light2, sizeof(Light));
			light2.lightType = (int)LIGHTTYPE::POINT;
			light2.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light2.diffuseIntensity = 4;
			light2.position = XMFLOAT4(35, 7.5, 428, 1);
			light2.lightRadius = 50;

			Building* lamp3 = new Building(XMFLOAT3(315, 0, 255), XMFLOAT3(0, 45, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp3);

			Light light3;
			ZeroMemory(&light3, sizeof(Light));
			light3.lightType = (int)LIGHTTYPE::POINT;
			light3.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light3.diffuseIntensity = 4;
			light3.position = XMFLOAT4(316, 7.5, 256, 1);
			light3.lightRadius = 50;

			Building* lamp4 = new Building(XMFLOAT3(355, 0, 425), XMFLOAT3(0, 180, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp4);

			Light light4;
			ZeroMemory(&light4, sizeof(Light));
			light4.lightType = (int)LIGHTTYPE::POINT;
			light4.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light4.diffuseIntensity = 4;
			light4.position = XMFLOAT4(355, 5, 423, 1);
			light4.lightRadius = 50;

			Building* lamp5 = new Building(XMFLOAT3(-240, 0, 245), XMFLOAT3(0, 180, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp5);

			Light light5;
			ZeroMemory(&light5, sizeof(Light));
			light5.lightType = (int)LIGHTTYPE::POINT;
			light5.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light5.diffuseIntensity = 4;
			light5.position = XMFLOAT4(-240, 7.5, 243, 1);
			light5.lightRadius = 50;

			Building* lamp6 = new Building(XMFLOAT3(-180, 0, -55), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp6);
			
			Light light6;
			ZeroMemory(&light6, sizeof(Light));
			light6.lightType = (int)LIGHTTYPE::POINT;
			light6.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light6.diffuseIntensity = 4;
			light6.position = XMFLOAT4(-180, 7.5, -53, 1);
			light6.lightRadius = 50;

			Building* lamp7 = new Building(XMFLOAT3(120, 0, -57), XMFLOAT3(0, 180, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp7);

			Light light7;
			ZeroMemory(&light7, sizeof(Light));
			light7.lightType = (int)LIGHTTYPE::POINT;
			light7.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light7.diffuseIntensity = 4;
			light7.position = XMFLOAT4(120, 7.5, -59, 1);
			light7.lightRadius = 50;

			Building* lamp8 = new Building(XMFLOAT3(-360, 0, -110), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp8);
			
			Light light8;
			ZeroMemory(&light8, sizeof(Light));
			light8.lightType = (int)LIGHTTYPE::POINT;
			light8.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light8.diffuseIntensity = 4;
			light8.position = XMFLOAT4(-360, 7.5, -108, 1);
			light8.lightRadius = 50;

			Building* lamp9 = new Building(XMFLOAT3(192.5, 0, -160), XMFLOAT3(0, -90, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp9);
			
			Light light9;
			ZeroMemory(&light9, sizeof(Light));
			light9.lightType = (int)LIGHTTYPE::POINT;
			light9.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light9.diffuseIntensity = 4;
			light9.position = XMFLOAT4(190.5, 7.5, -160, 1);
			light9.lightRadius = 50;

			Building* lamp10 = new Building(XMFLOAT3(191, 0, -410), XMFLOAT3(0, -30, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp10);

			Light light10;
			ZeroMemory(&light10, sizeof(Light));
			light10.lightType = (int)LIGHTTYPE::POINT;
			light10.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light10.diffuseIntensity = 4;
			light10.position = XMFLOAT4(190.75, 7.5, -409.25, 1);
			light10.lightRadius = 50;

			Building* lamp11 = new Building(XMFLOAT3(-9, 0, -312.5), XMFLOAT3(0, -45, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp11);
			
			Light light11;
			ZeroMemory(&light11, sizeof(Light));
			light11.lightType = (int)LIGHTTYPE::POINT;
			light11.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light11.diffuseIntensity = 4;
			light11.position = XMFLOAT4(-10, 7.5, -311.5, 1);
			light11.lightRadius = 50;

			Building* lamp12 = new Building(XMFLOAT3(-120, 0, -390), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp12);

			Light light12;
			ZeroMemory(&light12, sizeof(Light));
			light12.lightType = (int)LIGHTTYPE::POINT;
			light12.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light12.diffuseIntensity = 4;
			light12.position = XMFLOAT4(-120, 7.5, -388, 1);
			light12.lightRadius = 50;
			
			Building* lamp13 = new Building(XMFLOAT3(120, 0, -650), XMFLOAT3(0, 45, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp13);
			
			Light light13;
			ZeroMemory(&light13, sizeof(Light));
			light13.lightType = (int)LIGHTTYPE::POINT;
			light13.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light13.diffuseIntensity = 4;
			light13.position = XMFLOAT4(121, 7.5, -649, 1);
			light13.lightRadius = 50;

			Building* lamp14 = new Building(XMFLOAT3(-50, 0, -550), XMFLOAT3(0, 180, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp14);
			
			Light light14;
			ZeroMemory(&light14, sizeof(Light));
			light14.lightType = (int)LIGHTTYPE::POINT;
			light14.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light14.diffuseIntensity = 4;
			light14.position = XMFLOAT4(-50, 7.5, -552, 1);
			light14.lightRadius = 50;

			Building* lamp15 = new Building(XMFLOAT3(-215, 0, -390), XMFLOAT3(0, 180, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp15);
			
			Light light15;
			ZeroMemory(&light15, sizeof(Light));
			light15.lightType = (int)LIGHTTYPE::POINT;
			light15.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light15.diffuseIntensity = 4;
			light15.position = XMFLOAT4(-215, 7.5, -392, 1);
			light15.lightRadius = 50;

			Building* lamp16 = new Building(XMFLOAT3(-190, 0, -700), XMFLOAT3(0, -25, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp16);
			
			Light light16;
			ZeroMemory(&light16, sizeof(Light));
			light16.lightType = (int)LIGHTTYPE::POINT;
			light16.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light16.diffuseIntensity = 4;
			light16.position = XMFLOAT4(-190.25, 7.5, -699.25, 1);
			light16.lightRadius = 50;

			Building* lamp17 = new Building(XMFLOAT3(-382, 0, -904.75), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp17);
			
			Light light17;
			ZeroMemory(&light17, sizeof(Light));
			light17.lightType = (int)LIGHTTYPE::POINT;
			light17.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light17.diffuseIntensity = 4;
			light17.position = XMFLOAT4(-382, 7.5, -903, 1);
			light17.lightRadius = 50;

			Building* lamp18 = new Building(XMFLOAT3(-102, 0, -905), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp18);
			
			Light light18;
			ZeroMemory(&light18, sizeof(Light));
			light18.lightType = (int)LIGHTTYPE::POINT;
			light18.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light18.diffuseIntensity = 4;
			light18.position = XMFLOAT4(-102, 7.5, -903, 1);
			light18.lightRadius = 50;

			Building* lamp19 = new Building(XMFLOAT3(178, 0, -905), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Inv");
			GameUtilities::AddGameObject(lamp19);

			Light light19;
			ZeroMemory(&light19, sizeof(Light));
			light19.lightType = (int)LIGHTTYPE::POINT;
			light19.diffuse = XMFLOAT4(1, 0.807, 0, 1);
			light19.diffuseIntensity = 4;
			light19.position = XMFLOAT4(178, 7.5, -903, 1);
			light19.lightRadius = 50;

			ResourceManager::AddLight(light);
			ResourceManager::AddLight(light1);
			ResourceManager::AddLight(light2);
			ResourceManager::AddLight(light3);
			ResourceManager::AddLight(light4);
			ResourceManager::AddLight(light5);
			ResourceManager::AddLight(light6);
			ResourceManager::AddLight(light7);
			ResourceManager::AddLight(light8);
			ResourceManager::AddLight(light9);
			ResourceManager::AddLight(light10);
			ResourceManager::AddLight(light11);
			ResourceManager::AddLight(light12);
			ResourceManager::AddLight(light13);
			ResourceManager::AddLight(light14);
			ResourceManager::AddLight(light15);
			ResourceManager::AddLight(light16);
			ResourceManager::AddLight(light17);
			ResourceManager::AddLight(light18);
			ResourceManager::AddLight(light19);

			Building* well1 = new Building(XMFLOAT3(525, 0, 525), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell, "Well");
			GameUtilities::AddGameObject(well1);
			
			Building* well2 = new Building(XMFLOAT3(225, 0, 306.25), XMFLOAT3(0, -45, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell, "Well");
			GameUtilities::AddGameObject(well2);
			
			Building* well3 = new Building(XMFLOAT3(-200, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell, "Well");
			GameUtilities::AddGameObject(well3);
			
			Building* well4 = new Building(XMFLOAT3(160, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell, "Well");
			GameUtilities::AddGameObject(well4);
			
			Building* well5 = new Building(XMFLOAT3(185, 0, -390), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 5, 3), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell, "Well");
			GameUtilities::AddGameObject(well5);

			GameUtilities::GeneratePathway2(XMFLOAT3(0, 0, 10), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-17.5, 0, 62.5), XMFLOAT3(0, 180, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-60, 0, 87.5), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-110, 0, 87.5), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-162, 0, 110), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-199.5, 0, 147.5), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-237, 0, 185), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-249.5, 0, 232.5), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-249.5, 0, 283), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-235, 0, 330), XMFLOAT3(0, 30, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-207.5, 0, 374.5), XMFLOAT3(0, 30, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-174, 0, 415), XMFLOAT3(0, 45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-125, 0, 430), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-73, 0, 430), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-21, 0, 430), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(32, 0, 423), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(89, 0, 423), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(141, 0, 423), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(193, 0, 423), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(245, 0, 423), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(297, 0, 423), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(350, 0, 417), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(404, 0, 395), XMFLOAT3(0, 180, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(420, 0, 340), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(420, 0, 288), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(433, 0, 246), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(474, 0, 226), XMFLOAT3(0, -90, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(437, 0, 432), XMFLOAT3(0, -135, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(474, 0, 469), XMFLOAT3(0, -135, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(232, 0, 396), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(250, 0, 334), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(285, 0, 294), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(322.5, 0, 256.5), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(357, 0, 214), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(394, 0, 177), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(431, 0, 140), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(468, 0, 103), XMFLOAT3(0, -45, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(232, 0, 459), XMFLOAT3(0, 0, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(32, 0, 392), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(32, 0, 340), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(32, 0, 300), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-21, 0, 300), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-73, 0, 300), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-125, 0, 300), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-186, 0, 282.5), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-226, 0, 245), XMFLOAT3(0, 45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-277, 0, 250), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-330, 0, 251), XMFLOAT3(0, 90, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(95, 0, 292.5), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(147, 0, 292.5), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(201, 0, 292.5), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(255, 0, 297.5), XMFLOAT3(0, 80, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(18, 0, -35), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(62, 0, -55), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(116, 0, -61), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(170, 0, -82), XMFLOAT3(0, 180, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(185, 0, -132), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(190, 0, -182.5), XMFLOAT3(0, -20, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -233), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -287), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -341), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -395), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -449), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -503), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -557), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -611), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -665), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(200, 0, -719), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(175, 0, -770), XMFLOAT3(0, -90, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(158, 0, -676), XMFLOAT3(0, -45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(190, 0, -716), XMFLOAT3(0, -25, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(178, 0, -350), XMFLOAT3(0, 45, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(178, 0, -270), XMFLOAT3(0, 45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(138, 0, -306), XMFLOAT3(0, 45, 0));


			GameUtilities::GeneratePathway2(XMFLOAT3(190, 0, -503), XMFLOAT3(0, 22.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(169, 0, -551), XMFLOAT3(0, 22.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(148, 0, -600), XMFLOAT3(0, 22.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(127, 0, -649), XMFLOAT3(0, 22.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(106, 0, -698), XMFLOAT3(0, 22.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(85 , 0, -747), XMFLOAT3(0, 22.5, 0));
			
			GameUtilities::GeneratePathway2(XMFLOAT3(123 , 0, -785), XMFLOAT3(0, 95, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(73 , 0, -780), XMFLOAT3(0, 95, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(23 , 0, -775), XMFLOAT3(0, 95, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-27 , 0, -770), XMFLOAT3(0, 95, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-79 , 0, -742), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-106 , 0, -685), XMFLOAT3(0, -20, 0));
			
			GameUtilities::GeneratePathway2(XMFLOAT3(47 , 0, -609), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-4 , 0, -626), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-55 , 0, -643), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-106 , 0, -660), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-157 , 0, -677), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-208 , 0, -694), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-259 , 0, -711), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-310 , 0, -728), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-361 , 0, -745), XMFLOAT3(0, 251, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-403 , 0, -728), XMFLOAT3(0, 0, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(-18, 0, -35), XMFLOAT3(0, 45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-62, 0, -55), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-116, 0, -55), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(-177, 0, -49), XMFLOAT3(0, -90, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-239, 0, -70), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-293, 0, -80), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-345, 0, -80), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-397, 0, -80), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-245, 0, -105), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-205, 0, -142), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-165.5, 0, -179), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-126, 0, -216), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-86.5, 0, -253), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-47, 0, -290), XMFLOAT3(0, -47.5, 0));
			
			GameUtilities::GeneratePathway2(XMFLOAT3(-5, 0, -298.5), XMFLOAT3(0, 42.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(40, 0, -280), XMFLOAT3(0, 80, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(75, 0, -292.5), XMFLOAT3(0, 315, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(112, 0, -330.5), XMFLOAT3(0, 315, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(149, 0, -370.5), XMFLOAT3(0, 320, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(183, 0, -413.5), XMFLOAT3(0, 320, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(-42, 0, -338), XMFLOAT3(0, 42.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-80.5, 0, -376), XMFLOAT3(0, 42.5, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-120.5, 0, -382.5), XMFLOAT3(0, -45, 0));
			
			GameUtilities::GeneratePathway2(XMFLOAT3(-175.5, 0, -352.5), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-215, 0, -315.5), XMFLOAT3(0, -47.5, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(-230, 0, -231.5), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-230, 0, -268.5), XMFLOAT3(0, 0, 0));


			GameUtilities::GeneratePathway2(XMFLOAT3(-90.5, 0, -335.5), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-130, 0, -298.5), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-169.5, 0, -259), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-209, 0, -219.5), XMFLOAT3(0, -47.5, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(-105.5, 0, -418), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-68, 0, -455), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-30.5, 0, -492), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(7, 0, -529), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(44.5, 0, -566), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(82, 0, -603), XMFLOAT3(0, -47.5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(119.5, 0, -640), XMFLOAT3(0, -47.5, 0));

			GameUtilities::GeneratePathway2(XMFLOAT3(-25, 0, -535), XMFLOAT3(0, 45, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-1, 0, -610), XMFLOAT3(0, 135, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-39, 0, -571), XMFLOAT3(0, 135, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-77, 0, -532), XMFLOAT3(0, 135, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-115, 0, -493), XMFLOAT3(0, 135, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-153, 0, -454), XMFLOAT3(0, 135, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-209, 0, -422), XMFLOAT3(0, 135, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(-265, 0, -434), XMFLOAT3(0, 60, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-315, 0, -455), XMFLOAT3(0, 60, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-356, 0, -490), XMFLOAT3(0, 30, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-385, 0, -537), XMFLOAT3(0, 30, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-420, 0, -575), XMFLOAT3(0, 55, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-434.5, 0, -614), XMFLOAT3(0, -20, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-422.5, 0, -667.), XMFLOAT3(0, -5, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-468, 0, -595.5), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-530, 0, -613), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-555, 0, -658), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-555, 0, -712), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-555, 0, -766), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-555, 0, -820), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-538.5, 0, -880), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-488, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-434, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(-380, 0, -902.5), XMFLOAT3(0, 265, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-316, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-262, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-208, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-154, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(-100, 0, -902.5), XMFLOAT3(0, 265, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-36, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(18, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(72, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(126, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway1(XMFLOAT3(180, 0, -902.5), XMFLOAT3(0, 265, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(234, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(288, 0, -905), XMFLOAT3(0, 90, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(330, 0, -930), XMFLOAT3(0, 180, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(347.5, 0, -982), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(347.5, 0, -1036), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(347.5, 0, -1090), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(322.5, 0, -1144), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(262, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(208, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(154, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(100, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(46, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-8, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-62, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-116, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-170, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-224, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-278, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-332, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-386, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-440, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-494, 0, -1161), XMFLOAT3(0, 270, 0));
			GameUtilities::GeneratePathway3(XMFLOAT3(-538, 0, -1136.5), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-555, 0, -1078), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-555, 0, -1024), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-555, 0, -970), XMFLOAT3(0, 0, 0));
			GameUtilities::GeneratePathway2(XMFLOAT3(-555, 0, -916), XMFLOAT3(0, 0, 0));

			
			
			//Trees Collidable and Non-collidable
			XMFLOAT3 renderable_tree_positions[] = {
				XMFLOAT3(-340, 0, 25),
				XMFLOAT3(-370, 0, 25),
				XMFLOAT3(-400, 0, 25),
				XMFLOAT3(-430, 0, 25),
				XMFLOAT3(-460, 0, 25),
				XMFLOAT3(-490, 0, 25),
				XMFLOAT3(-520, 0, 25),
				XMFLOAT3(-550, 0, 25),
				XMFLOAT3(-580, 0, 25),
				XMFLOAT3(-610, 0, 25),
				XMFLOAT3(-340, 0, 50),
				XMFLOAT3(-370, 0, 50),
				XMFLOAT3(-400, 0, 50),
				XMFLOAT3(-430, 0, 50),
				XMFLOAT3(-460, 0, 50),
				XMFLOAT3(-490, 0, 50),
				XMFLOAT3(-520, 0, 50),
				XMFLOAT3(-550, 0, 50),
				XMFLOAT3(-580, 0, 50),
				XMFLOAT3(-610, 0, 50),
				XMFLOAT3(-340, 0, 75),
				XMFLOAT3(-370, 0, 75),
				XMFLOAT3(-400, 0, 75),
				XMFLOAT3(-430, 0, 75),
				XMFLOAT3(-460, 0, 75),
				XMFLOAT3(-490, 0, 75),
				XMFLOAT3(-520, 0, 75),
				XMFLOAT3(-550, 0, 75),
				XMFLOAT3(-580, 0, 75),
				XMFLOAT3(-610, 0, 75),
				XMFLOAT3(-340, 0, 100),
				XMFLOAT3(-370, 0, 100),
				XMFLOAT3(-400, 0, 100),
				XMFLOAT3(-430, 0, 100),
				XMFLOAT3(-460, 0, 100),
				XMFLOAT3(-490, 0, 100),
				XMFLOAT3(-520, 0, 100),
				XMFLOAT3(-550, 0, 100),
				XMFLOAT3(-580, 0, 100),
				XMFLOAT3(-610, 0, 100),
				XMFLOAT3(-340, 0, 125),
				XMFLOAT3(-370, 0, 125),
				XMFLOAT3(-400, 0, 125),
				XMFLOAT3(-430, 0, 125),
				XMFLOAT3(-460, 0, 125),
				XMFLOAT3(-490, 0, 125),
				XMFLOAT3(-520, 0, 125),
				XMFLOAT3(-550, 0, 125),
				XMFLOAT3(-580, 0, 125),
				XMFLOAT3(-610, 0, 125),
				XMFLOAT3(-340, 0, 150),
				XMFLOAT3(-370, 0, 150),
				XMFLOAT3(-400, 0, 150),
				XMFLOAT3(-430, 0, 150),
				XMFLOAT3(-460, 0, 150),
				XMFLOAT3(-490, 0, 150),
				XMFLOAT3(-520, 0, 150),
				XMFLOAT3(-550, 0, 150),
				XMFLOAT3(-580, 0, 150),
				XMFLOAT3(-610, 0, 150),
				XMFLOAT3(-340, 0, 175),
				XMFLOAT3(-370, 0, 175),
				XMFLOAT3(-400, 0, 175),
				XMFLOAT3(-430, 0, 175),
				XMFLOAT3(-460, 0, 175),
				XMFLOAT3(-490, 0, 175),
				XMFLOAT3(-520, 0, 175),
				XMFLOAT3(-550, 0, 175),
				XMFLOAT3(-580, 0, 175),
				XMFLOAT3(-610, 0, 175),
				XMFLOAT3(-460, 0, 225),
				XMFLOAT3(-490, 0, 225),
				XMFLOAT3(-520, 0, 225),
				XMFLOAT3(-550, 0, 225),
				XMFLOAT3(-580, 0, 225),
				XMFLOAT3(-610, 0, 225),
				XMFLOAT3(-460, 0, 275),
				XMFLOAT3(-490, 0, 275),
				XMFLOAT3(-520, 0, 275),
				XMFLOAT3(-550, 0, 275),
				XMFLOAT3(-580, 0, 275),
				XMFLOAT3(-610, 0, 275),
				XMFLOAT3(-340, 0, 325),
				XMFLOAT3(-370, 0, 325),
				XMFLOAT3(-400, 0, 325),
				XMFLOAT3(-430, 0, 325),
				XMFLOAT3(-460, 0, 325),
				XMFLOAT3(-490, 0, 325),
				XMFLOAT3(-520, 0, 325),
				XMFLOAT3(-550, 0, 325),
				XMFLOAT3(-580, 0, 325),
				XMFLOAT3(-610, 0, 325),
				XMFLOAT3(-340, 0, 350),
				XMFLOAT3(-370, 0, 350),
				XMFLOAT3(-400, 0, 350),
				XMFLOAT3(-430, 0, 350),
				XMFLOAT3(-460, 0, 350),
				XMFLOAT3(-490, 0, 350),
				XMFLOAT3(-520, 0, 350),
				XMFLOAT3(-550, 0, 350),
				XMFLOAT3(-580, 0, 350),
				XMFLOAT3(-610, 0, 350),
				XMFLOAT3(-340, 0, 375),
				XMFLOAT3(-370, 0, 375),
				XMFLOAT3(-400, 0, 375),
				XMFLOAT3(-430, 0, 375),
				XMFLOAT3(-460, 0, 375),
				XMFLOAT3(-490, 0, 375),
				XMFLOAT3(-520, 0, 375),
				XMFLOAT3(-550, 0, 375),
				XMFLOAT3(-580, 0, 375),
				XMFLOAT3(-610, 0, 375),
				XMFLOAT3(-340, 0, 400),
				XMFLOAT3(-370, 0, 400),
				XMFLOAT3(-400, 0, 400),
				XMFLOAT3(-430, 0, 400),
				XMFLOAT3(-460, 0, 400),
				XMFLOAT3(-490, 0, 400),
				XMFLOAT3(-520, 0, 400),
				XMFLOAT3(-550, 0, 400),
				XMFLOAT3(-580, 0, 400),
				XMFLOAT3(-610, 0, 400),
				XMFLOAT3(-340, 0, 425),
				XMFLOAT3(-370, 0, 425),
				XMFLOAT3(-400, 0, 425),
				XMFLOAT3(-430, 0, 425),
				XMFLOAT3(-460, 0, 425),
				XMFLOAT3(-490, 0, 425),
				XMFLOAT3(-520, 0, 425),
				XMFLOAT3(-550, 0, 425),
				XMFLOAT3(-580, 0, 425),
				XMFLOAT3(-610, 0, 425),
				XMFLOAT3(-340, 0, 450),
				XMFLOAT3(-370, 0, 450),
				XMFLOAT3(-400, 0, 450),
				XMFLOAT3(-430, 0, 450),
				XMFLOAT3(-460, 0, 450),
				XMFLOAT3(-490, 0, 450),
				XMFLOAT3(-520, 0, 450),
				XMFLOAT3(-550, 0, 450),
				XMFLOAT3(-580, 0, 450),
				XMFLOAT3(-610, 0, 450),
				XMFLOAT3(-340, 0, 475),
				XMFLOAT3(-370, 0, 475),
				XMFLOAT3(-400, 0, 475),
				XMFLOAT3(-430, 0, 475),
				XMFLOAT3(-460, 0, 475),
				XMFLOAT3(-490, 0, 475),
				XMFLOAT3(-520, 0, 475),
				XMFLOAT3(-550, 0, 475),
				XMFLOAT3(-580, 0, 475),
				XMFLOAT3(-610, 0, 475),
				XMFLOAT3(-340, 0, 500),
				XMFLOAT3(-370, 0, 500),
				XMFLOAT3(-400, 0, 500),
				XMFLOAT3(-430, 0, 500),
				XMFLOAT3(-460, 0, 500),
				XMFLOAT3(-490, 0, 500),
				XMFLOAT3(-520, 0, 500),
				XMFLOAT3(-550, 0, 500),
				XMFLOAT3(-580, 0, 500),
				XMFLOAT3(-610, 0, 500),
				XMFLOAT3(-340, 0, 525),
				XMFLOAT3(-370, 0, 525),
				XMFLOAT3(-400, 0, 525),
				XMFLOAT3(-430, 0, 525),
				XMFLOAT3(-460, 0, 525),
				XMFLOAT3(-490, 0, 525),
				XMFLOAT3(-520, 0, 525),
				XMFLOAT3(-550, 0, 525),
				XMFLOAT3(-580, 0, 525),
				XMFLOAT3(-610, 0, 525),
				XMFLOAT3(-340, 0, 550),
				XMFLOAT3(-370, 0, 550),
				XMFLOAT3(-400, 0, 550),
				XMFLOAT3(-430, 0, 550),
				XMFLOAT3(-460, 0, 550),
				XMFLOAT3(-490, 0, 550),
				XMFLOAT3(-520, 0, 550),
				XMFLOAT3(-550, 0, 550),
				XMFLOAT3(-580, 0, 550),
				XMFLOAT3(-610, 0, 550),
				XMFLOAT3(-340, 0, 575),
				XMFLOAT3(-370, 0, 575),
				XMFLOAT3(-400, 0, 575),
				XMFLOAT3(-430, 0, 575),
				XMFLOAT3(-460, 0, 575),
				XMFLOAT3(-490, 0, 575),
				XMFLOAT3(-520, 0, 575),
				XMFLOAT3(-550, 0, 575),
				XMFLOAT3(-580, 0, 575),
				XMFLOAT3(-610, 0, 575),
				XMFLOAT3(-340, 0, 600),
				XMFLOAT3(-370, 0, 600),
				XMFLOAT3(-400, 0, 600),
				XMFLOAT3(-430, 0, 600),
				XMFLOAT3(-460, 0, 600),
				XMFLOAT3(-490, 0, 600),
				XMFLOAT3(-520, 0, 600),
				XMFLOAT3(-550, 0, 600),
				XMFLOAT3(-580, 0, 600),
				XMFLOAT3(-610, 0, 600),
				XMFLOAT3(-320, 0, -175),
				XMFLOAT3(-350, 0, -175),
				XMFLOAT3(-380, 0, -175),
				XMFLOAT3(-410, 0, -175),
				XMFLOAT3(-440, 0, -175),
				XMFLOAT3(-470, 0, -175),
				XMFLOAT3(-500, 0, -175),
				XMFLOAT3(-530, 0, -175),
				XMFLOAT3(-560, 0, -175),
				XMFLOAT3(-590, 0, -175),
				XMFLOAT3(-620, 0, -175),
				XMFLOAT3(-320, 0, -200),
				XMFLOAT3(-350, 0, -200),
				XMFLOAT3(-380, 0, -200),
				XMFLOAT3(-410, 0, -200),
				XMFLOAT3(-440, 0, -200),
				XMFLOAT3(-470, 0, -200),
				XMFLOAT3(-500, 0, -200),
				XMFLOAT3(-530, 0, -200),
				XMFLOAT3(-560, 0, -200),
				XMFLOAT3(-590, 0, -200),
				XMFLOAT3(-620, 0, -200),
				XMFLOAT3(-320, 0, -225),
				XMFLOAT3(-350, 0, -225),
				XMFLOAT3(-380, 0, -225),
				XMFLOAT3(-410, 0, -225),
				XMFLOAT3(-440, 0, -225),
				XMFLOAT3(-470, 0, -225),
				XMFLOAT3(-500, 0, -225),
				XMFLOAT3(-530, 0, -225),
				XMFLOAT3(-560, 0, -225),
				XMFLOAT3(-590, 0, -225),
				XMFLOAT3(-620, 0, -225),
				XMFLOAT3(-320, 0, -250),
				XMFLOAT3(-350, 0, -250),
				XMFLOAT3(-380, 0, -250),
				XMFLOAT3(-410, 0, -250),
				XMFLOAT3(-440, 0, -250),
				XMFLOAT3(-470, 0, -250),
				XMFLOAT3(-500, 0, -250),
				XMFLOAT3(-530, 0, -250),
				XMFLOAT3(-560, 0, -250),
				XMFLOAT3(-590, 0, -250),
				XMFLOAT3(-620, 0, -250),
				XMFLOAT3(-320, 0, -275),
				XMFLOAT3(-350, 0, -275),
				XMFLOAT3(-380, 0, -275),
				XMFLOAT3(-410, 0, -275),
				XMFLOAT3(-440, 0, -275),
				XMFLOAT3(-470, 0, -275),
				XMFLOAT3(-500, 0, -275),
				XMFLOAT3(-530, 0, -275),
				XMFLOAT3(-560, 0, -275),
				XMFLOAT3(-590, 0, -275),
				XMFLOAT3(-620, 0, -275),
				XMFLOAT3(-320, 0, -300),
				XMFLOAT3(-350, 0, -300),
				XMFLOAT3(-380, 0, -300),
				XMFLOAT3(-410, 0, -300),
				XMFLOAT3(-440, 0, -300),
				XMFLOAT3(-470, 0, -300),
				XMFLOAT3(-500, 0, -300),
				XMFLOAT3(-530, 0, -300),
				XMFLOAT3(-560, 0, -300),
				XMFLOAT3(-590, 0, -300),
				XMFLOAT3(-620, 0, -300),
				XMFLOAT3(-320, 0, -325),
				XMFLOAT3(-350, 0, -325),
				XMFLOAT3(-380, 0, -325),
				XMFLOAT3(-410, 0, -325),
				XMFLOAT3(-440, 0, -325),
				XMFLOAT3(-470, 0, -325),
				XMFLOAT3(-500, 0, -325),
				XMFLOAT3(-530, 0, -325),
				XMFLOAT3(-560, 0, -325),
				XMFLOAT3(-590, 0, -325),
				XMFLOAT3(-620, 0, -325),
				XMFLOAT3(-520, 0, -350),
				XMFLOAT3(-550, 0, -350),
				XMFLOAT3(-580, 0, -350),
				XMFLOAT3(-610, 0, -350),
				XMFLOAT3(-520, 0, -375),
				XMFLOAT3(-550, 0, -375),
				XMFLOAT3(-580, 0, -375),
				XMFLOAT3(-610, 0, -375),
				XMFLOAT3(-520, 0, -400),
				XMFLOAT3(-550, 0, -400),
				XMFLOAT3(-580, 0, -400),
				XMFLOAT3(-610, 0, -400),
				XMFLOAT3(-520, 0, -425),
				XMFLOAT3(-550, 0, -425),
				XMFLOAT3(-580, 0, -425),
				XMFLOAT3(-610, 0, -425),
				XMFLOAT3(320, 0, -25),
				XMFLOAT3(350, 0, -25),
				XMFLOAT3(380, 0, -25),
				XMFLOAT3(320, 0, -50),
				XMFLOAT3(350, 0, -50),
				XMFLOAT3(380, 0, -50),
				XMFLOAT3(320, 0, -75),
				XMFLOAT3(350, 0, -75),
				XMFLOAT3(380, 0, -75),
				XMFLOAT3(320, 0, -100),
				XMFLOAT3(350, 0, -100),
				XMFLOAT3(380, 0, -100),
				XMFLOAT3(320, 0, -125),
				XMFLOAT3(350, 0, -125),
				XMFLOAT3(380, 0, -125),
				XMFLOAT3(320, 0, -150),
				XMFLOAT3(350, 0, -150),
				XMFLOAT3(380, 0, -150),
				XMFLOAT3(320, 0, -175),
				XMFLOAT3(350, 0, -175),
				XMFLOAT3(380, 0, -175),
				XMFLOAT3(320, 0, -200),
				XMFLOAT3(350, 0, -200),
				XMFLOAT3(380, 0, -200),
				XMFLOAT3(320, 0, -225),
				XMFLOAT3(350, 0, -225),
				XMFLOAT3(380, 0, -225),
				XMFLOAT3(350, 0, -250),
				XMFLOAT3(380, 0, -250),
				XMFLOAT3(320, 0, -275),
				XMFLOAT3(350, 0, -275),
				XMFLOAT3(380, 0, -275),
				XMFLOAT3(320, 0, -300),
				XMFLOAT3(350, 0, -300),
				XMFLOAT3(380, 0, -300),
				XMFLOAT3(320, 0, -325),
				XMFLOAT3(350, 0, -325),
				XMFLOAT3(380, 0, -325),
				XMFLOAT3(320, 0, -350),
				XMFLOAT3(350, 0, -350),
				XMFLOAT3(380, 0, -350),
				XMFLOAT3(320, 0, -375),
				XMFLOAT3(350, 0, -375),
				XMFLOAT3(380, 0, -375),
				XMFLOAT3(320, 0, -400),
				XMFLOAT3(350, 0, -400),
				XMFLOAT3(380, 0, -400),
				XMFLOAT3(320, 0, -425),
				XMFLOAT3(350, 0, -425),
				XMFLOAT3(380, 0, -425),
				XMFLOAT3(320, 0, -450),
				XMFLOAT3(350, 0, -450),
				XMFLOAT3(380, 0, -450),
				XMFLOAT3(320, 0, -475),
				XMFLOAT3(350, 0, -475),
				XMFLOAT3(380, 0, -475),
				XMFLOAT3(320, 0, -500),
				XMFLOAT3(350, 0, -500),
				XMFLOAT3(380, 0, -500),
				XMFLOAT3(320, 0, -525),
				XMFLOAT3(350, 0, -525),
				XMFLOAT3(380, 0, -525),
				XMFLOAT3(320, 0, -550),
				XMFLOAT3(350, 0, -550),
				XMFLOAT3(380, 0, -550),
				XMFLOAT3(320, 0, -575),
				XMFLOAT3(350, 0, -575),
				XMFLOAT3(380, 0, -575),
				XMFLOAT3(320, 0, -600),
				XMFLOAT3(350, 0, -600),
				XMFLOAT3(380, 0, -600),
				XMFLOAT3(320, 0, -625),
				XMFLOAT3(350, 0, -625),
				XMFLOAT3(380, 0, -625),
				XMFLOAT3(320, 0, -650),
				XMFLOAT3(350, 0, -650),
				XMFLOAT3(380, 0, -650),
				XMFLOAT3(320, 0, -650),
				XMFLOAT3(350, 0, -650),
				XMFLOAT3(380, 0, -650),
				XMFLOAT3(320, 0, -675),
				XMFLOAT3(350, 0, -675),
				XMFLOAT3(380, 0, -675),
				XMFLOAT3(320, 0, -700),
				XMFLOAT3(350, 0, -700),
				XMFLOAT3(380, 0, -700),
				XMFLOAT3(320, 0, -725),
				XMFLOAT3(350, 0, -725),
				XMFLOAT3(380, 0, -725),
				XMFLOAT3(320, 0, -750),
				XMFLOAT3(350, 0, -750),
				XMFLOAT3(380, 0, -750),
				XMFLOAT3(320, 0, -775),
				XMFLOAT3(350, 0, -775),
				XMFLOAT3(380, 0, -775),
				XMFLOAT3(320, 0, -800),
				XMFLOAT3(350, 0, -800),
				XMFLOAT3(380, 0, -800),
				XMFLOAT3(320, 0, -825),
				XMFLOAT3(350, 0, -825),
				XMFLOAT3(380, 0, -825),
			};

			XMFLOAT3 collidable_tree_positions[] = {
				XMFLOAT3(-300, 0, 12.5),
				XMFLOAT3(-320, 0, 12.5),
				XMFLOAT3(-340, 0, 12.5),
				XMFLOAT3(-360, 0, 12.5),
				XMFLOAT3(-380, 0, 12.5),
				XMFLOAT3(-400, 0, 12.5),
				XMFLOAT3(-420, 0, 12.5),
				XMFLOAT3(-440, 0, 12.5),
				XMFLOAT3(-460, 0, 12.5),
				XMFLOAT3(-480, 0, 12.5),
				XMFLOAT3(-500, 0, 12.5),
				XMFLOAT3(-520, 0, 12.5),
				XMFLOAT3(-540, 0, 12.5),
				XMFLOAT3(-560, 0, 12.5),
				XMFLOAT3(-580, 0, 12.5),
				XMFLOAT3(-600, 0, 12.5),
				XMFLOAT3(-620, 0, 12.5),
				XMFLOAT3(-310, 0, 25),
				XMFLOAT3(-300, 0, 37.5),
				XMFLOAT3(-310, 0, 50),
				XMFLOAT3(-300, 0, 62.5),
				XMFLOAT3(-310, 0, 75),
				XMFLOAT3(-300, 0, 87.5),
				XMFLOAT3(-310, 0, 100),
				XMFLOAT3(-300, 0, 112.5),
				XMFLOAT3(-310, 0, 125),
				XMFLOAT3(-300, 0, 137.5),
				XMFLOAT3(-310, 0, 150),
				XMFLOAT3(-300, 0, 162.5),
				XMFLOAT3(-310, 0, 175),
				XMFLOAT3(-300, 0, 187.5),
				XMFLOAT3(-310, 0, 200),
				XMFLOAT3(-330, 0, 200),
				XMFLOAT3(-350, 0, 200),
				XMFLOAT3(-370, 0, 200),
				XMFLOAT3(-390, 0, 200),
				XMFLOAT3(-410, 0, 200),
				XMFLOAT3(-430, 0, 200),
				XMFLOAT3(-450, 0, 200),
				XMFLOAT3(-470, 0, 200),
				XMFLOAT3(-490, 0, 200),
				XMFLOAT3(-510, 0, 200),
				XMFLOAT3(-530, 0, 200),
				XMFLOAT3(-550, 0, 200),
				XMFLOAT3(-570, 0, 200),
				XMFLOAT3(-590, 0, 200),
				XMFLOAT3(-440, 0, 212.5),
				XMFLOAT3(-430, 0, 225),
				XMFLOAT3(-440, 0, 237.5),
				XMFLOAT3(-430, 0, 250),
				XMFLOAT3(-440, 0, 262.5),
				XMFLOAT3(-430, 0, 275),
				XMFLOAT3(-440, 0, 287.5),
				XMFLOAT3(-310, 0, 300),
				XMFLOAT3(-330, 0, 300),
				XMFLOAT3(-350, 0, 300),
				XMFLOAT3(-370, 0, 300),
				XMFLOAT3(-390, 0, 300),
				XMFLOAT3(-410, 0, 300),
				XMFLOAT3(-430, 0, 300),
				XMFLOAT3(-450, 0, 300),
				XMFLOAT3(-470, 0, 300),
				XMFLOAT3(-490, 0, 300),
				XMFLOAT3(-510, 0, 300),
				XMFLOAT3(-530, 0, 300),
				XMFLOAT3(-550, 0, 300),
				XMFLOAT3(-570, 0, 300),
				XMFLOAT3(-590, 0, 300),
				XMFLOAT3(-300, 0, 312.5),
				XMFLOAT3(-310, 0, 325),
				XMFLOAT3(-300, 0, 337.5),
				XMFLOAT3(-310, 0, 350),
				XMFLOAT3(-300, 0, 362.5),
				XMFLOAT3(-310, 0, 375),
				XMFLOAT3(-300, 0, 387.5),
				XMFLOAT3(-310, 0, 400),
				XMFLOAT3(-300, 0, 412.5),
				XMFLOAT3(-310, 0, 425),
				XMFLOAT3(-300, 0, 437.5),
				XMFLOAT3(-310, 0, 450),
				XMFLOAT3(-300, 0, 462.5),
				XMFLOAT3(-310, 0, 475),
				XMFLOAT3(-300, 0, 487.5),
				XMFLOAT3(-310, 0, 500),
				XMFLOAT3(-300, 0, 512.5),
				XMFLOAT3(-310, 0, 525),
				XMFLOAT3(-300, 0, 537.5),
				XMFLOAT3(-310, 0, 550),
				XMFLOAT3(-300, 0, 562.5),
				XMFLOAT3(-310, 0, 575),
				XMFLOAT3(-300, 0, 587.5),
				XMFLOAT3(-310, 0, 600),
				XMFLOAT3(-300, 0, 612.5),
				XMFLOAT3(-300, 0, -162.5),
				XMFLOAT3(-320, 0, -162.5),
				XMFLOAT3(-340, 0, -162.5),
				XMFLOAT3(-360, 0, -162.5),
				XMFLOAT3(-380, 0, -162.5),
				XMFLOAT3(-400, 0, -162.5),
				XMFLOAT3(-420, 0, -162.5),
				XMFLOAT3(-440, 0, -162.5),
				XMFLOAT3(-460, 0, -162.5),
				XMFLOAT3(-480, 0, -162.5),
				XMFLOAT3(-500, 0, -162.5),
				XMFLOAT3(-520, 0, -162.5),
				XMFLOAT3(-540, 0, -162.5),
				XMFLOAT3(-560, 0, -162.5),
				XMFLOAT3(-580, 0, -162.5),
				XMFLOAT3(-600, 0, -162.5),
				XMFLOAT3(-290, 0, -175),
				XMFLOAT3(-300, 0, -187.5),
				XMFLOAT3(-290, 0, -200),
				XMFLOAT3(-300, 0, -212.5),
				XMFLOAT3(-290, 0, -225),
				XMFLOAT3(-300, 0, -237.5),
				XMFLOAT3(-290, 0, -250),
				XMFLOAT3(-300, 0, -262.5),
				XMFLOAT3(-290, 0, -275),
				XMFLOAT3(-300, 0, -287.5),
				XMFLOAT3(-290, 0, -300),
				XMFLOAT3(-300, 0, -312.5),
				XMFLOAT3(-290, 0, -325),
				XMFLOAT3(-300, 0, -337.5),
				XMFLOAT3(-320, 0, -337.5),
				XMFLOAT3(-340, 0, -337.5),
				XMFLOAT3(-360, 0, -337.5),
				XMFLOAT3(-380, 0, -337.5),
				XMFLOAT3(-400, 0, -337.5),
				XMFLOAT3(-420, 0, -337.5),
				XMFLOAT3(-440, 0, -337.5),
				XMFLOAT3(-460, 0, -337.5),
				XMFLOAT3(-480, 0, -337.5),
				XMFLOAT3(-500, 0, -337.5),
				XMFLOAT3(-520, 0, -337.5),
				XMFLOAT3(-540, 0, -337.5),
				XMFLOAT3(-560, 0, -337.5),
				XMFLOAT3(-580, 0, -337.5),
				XMFLOAT3(-600, 0, -337.5),
				XMFLOAT3(-490, 0, -350),
				XMFLOAT3(-500, 0, -362.5),
				XMFLOAT3(-490, 0, -375),
				XMFLOAT3(-500, 0, -387.5),
				XMFLOAT3(-490, 0, -400),
				XMFLOAT3(-500, 0, -412.5),
				XMFLOAT3(-490, 0, -425),
				XMFLOAT3(-500, 0, -437.5),
				XMFLOAT3(-490, 0, -450),
				XMFLOAT3(-510, 0, -450),
				XMFLOAT3(-530, 0, -450),
				XMFLOAT3(-550, 0, -450),
				XMFLOAT3(-570, 0, -450),
				XMFLOAT3(-590, 0, -450),
				XMFLOAT3(-610, 0, -450),
				XMFLOAT3(290, 0, 0),
				XMFLOAT3(310, 0, 0),
				XMFLOAT3(330, 0, 0),
				XMFLOAT3(350, 0, 0),
				XMFLOAT3(370, 0, 0),
				XMFLOAT3(390, 0, 0),
				XMFLOAT3(300, 0, -12.5),
				XMFLOAT3(330, 0, -12.5),
				XMFLOAT3(360, 0, -12.5),
				XMFLOAT3(390, 0, -12.5),
				XMFLOAT3(290, 0, -25),
				XMFLOAT3(300, 0, -37.5),
				XMFLOAT3(290, 0, -50),
				XMFLOAT3(300, 0, -62.5),
				XMFLOAT3(290, 0, -75),
				XMFLOAT3(300, 0, -87.5),
				XMFLOAT3(290, 0, -100),
				XMFLOAT3(300, 0, -112.5),
				XMFLOAT3(290, 0, -125),
				XMFLOAT3(300, 0, -137.5),
				XMFLOAT3(290, 0, -150),
				XMFLOAT3(300, 0, -162.5),
				XMFLOAT3(290, 0, -175),
				XMFLOAT3(300, 0, -187.5),
				XMFLOAT3(290, 0, -200),
				XMFLOAT3(300, 0, -212.5),
				XMFLOAT3(290, 0, -225),
				XMFLOAT3(300, 0, -237.5),
				XMFLOAT3(290, 0, -250),
				XMFLOAT3(300, 0, -262.5),
				XMFLOAT3(290, 0, -275),
				XMFLOAT3(300, 0, -287.5),
				XMFLOAT3(290, 0, -300),
				XMFLOAT3(300, 0, -312.5),
				XMFLOAT3(290, 0, -325),
				XMFLOAT3(300, 0, -337.5),
				XMFLOAT3(290, 0, -350),
				XMFLOAT3(300, 0, -362.5),
				XMFLOAT3(290, 0, -375),
				XMFLOAT3(300, 0, -387.5),
				XMFLOAT3(290, 0, -400),
				XMFLOAT3(300, 0, -412.5),
				XMFLOAT3(290, 0, -425),
				XMFLOAT3(300, 0, -437.5),
				XMFLOAT3(290, 0, -450),
				XMFLOAT3(300, 0, -462.5),
				XMFLOAT3(290, 0, -475),
				XMFLOAT3(300, 0, -487.5),
				XMFLOAT3(290, 0, -500),
				XMFLOAT3(300, 0, -512.5),
				XMFLOAT3(290, 0, -525),
				XMFLOAT3(300, 0, -537.5),
				XMFLOAT3(290, 0, -550),
				XMFLOAT3(300, 0, -562.5),
				XMFLOAT3(290, 0, -575),
				XMFLOAT3(300, 0, -587.5),
				XMFLOAT3(290, 0, -600),
				XMFLOAT3(300, 0, -612.5),
				XMFLOAT3(290, 0, -625),
				XMFLOAT3(300, 0, -637.5),
				XMFLOAT3(290, 0, -650),
				XMFLOAT3(300, 0, -662.5),
				XMFLOAT3(290, 0, -675),
				XMFLOAT3(300, 0, -687.5),
				XMFLOAT3(290, 0, -700),
				XMFLOAT3(300, 0, -712.5),
				XMFLOAT3(290, 0, -725),
				XMFLOAT3(300, 0, -737.5),
				XMFLOAT3(290, 0, -750),
				XMFLOAT3(300, 0, -762.5),
				XMFLOAT3(290, 0, -775),
				XMFLOAT3(300, 0, -787.5),
				XMFLOAT3(290, 0, -800),
				XMFLOAT3(300, 0, -812.5),
				XMFLOAT3(290, 0, -825),
				XMFLOAT3(300, 0, -837.5),
				XMFLOAT3(320, 0, -837.5),
				XMFLOAT3(340, 0, -837.5),
				XMFLOAT3(360, 0, -837.5),
				XMFLOAT3(380, 0, -837.5),
			};

			for (auto& position : renderable_tree_positions)
			{
				GameUtilities::GenerateTree(position, XMFLOAT3(0, 0, 0));
			}

			for (auto& position : collidable_tree_positions)
			{
				GameUtilities::AddGameObject(new Building(position, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 9, 12.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateTree, "Inv"));
			}
			
			towers.push_back(GameUtilities::AttachTowerAI(ballista1, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista2, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista3, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista4, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista5, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista6, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista7, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista8, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista9, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista10, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista11, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista12, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista13, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista14, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista15, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista16, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista17, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista18, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista19, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista20, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista21, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista22, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista23, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista24, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista25, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista26, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista27, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista28, &villagerThreats));
			towers.push_back(GameUtilities::AttachTowerAI(ballista29, &villagerThreats));
#pragma endregion


			for (int i = 0; i < 10; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -245), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock1, &villagerThreats, &buildings, _engine));
			}
			for (int i = 10; i < 20; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -400), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock2, &villagerThreats, &buildings, _engine));
			}
			for (int i = 20; i < 30; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -700), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock3, &villagerThreats, &buildings, _engine));
			}
			for (int i = 30; i < 40; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -900), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock4, &villagerThreats, &buildings, _engine));
			}
			for (int i = 40; i < 50; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3(0.f, 0.1f, (0.f+(i*-25.f))), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock5, &villagerThreats, &buildings, _engine));
			}

			villagerThreats.push_back(golem);

			for (int i = 0; i < 10; i++)
			{
				villagerThreats.push_back(stoneMinions[i]);
				villagerThreats.push_back(waterMinions[i]);
				villagerThreats.push_back(fireMinions[i]);
				villagerThreats.push_back(woodMinions[i]);
			}

			for (int i = 0; i < 50; i++)
			{
				minionTargets.push_back(villagers[i]);
				GameUtilities::AddGameObject(villagers[i]);
			}

			for (auto& building : buildings)
			{
				minionTargets.push_back(building);
			}

			ADAI::PathersQueue* pq = new ADAI::PathersQueue();
			ADAI::CohesionGroup* cg = new ADAI::CohesionGroup();
			ADAI::SeperationGroup* sg = new ADAI::SeperationGroup();
			ADAI::TargetingGroup* tg = new ADAI::TargetingGroup();

			for (int i = 0; i < 10; i++)
			{
				cg->members.push_back(stoneMinions[i]);
				sg->members.push_back(stoneMinions[i]);

				cg->members.push_back(waterMinions[i]);
				sg->members.push_back(waterMinions[i]);

				cg->members.push_back(fireMinions[i]);
				sg->members.push_back(fireMinions[i]);

				cg->members.push_back(woodMinions[i]);
				sg->members.push_back(woodMinions[i]);
			}


			golem->minionGroups[STONE]->avoidanceGroup = &minionAvoids;
			golem->minionGroups[STONE]->myGroups.push_back(cg);
			golem->minionGroups[STONE]->myGroups.push_back(sg);
			golem->minionGroups[STONE]->myTargetGroups.push_back(tg);

			golem->minionGroups[WATER]->avoidanceGroup = &minionAvoids;
			golem->minionGroups[WATER]->myGroups.push_back(cg);
			golem->minionGroups[WATER]->myGroups.push_back(sg);
			golem->minionGroups[WATER]->myTargetGroups.push_back(tg);

			golem->minionGroups[FIRE]->avoidanceGroup = &minionAvoids;
			golem->minionGroups[FIRE]->myGroups.push_back(cg);
			golem->minionGroups[FIRE]->myGroups.push_back(sg);
			golem->minionGroups[FIRE]->myTargetGroups.push_back(tg);

			golem->minionGroups[WOOD]->avoidanceGroup = &minionAvoids;
			golem->minionGroups[WOOD]->myGroups.push_back(cg);
			golem->minionGroups[WOOD]->myGroups.push_back(sg);
			golem->minionGroups[WOOD]->myTargetGroups.push_back(tg);

			golem->minionGroups[STONE]->pathingQueue = pq;
			golem->minionGroups[WATER]->pathingQueue = pq;
			golem->minionGroups[FIRE]->pathingQueue = pq;
			golem->minionGroups[WOOD]->pathingQueue = pq;

			for (int i = 0; i < 10; i++)
			{
				stoneMinionsAI.push_back(GameUtilities::AttachMinionAI(stoneMinions[i], golem->minionGroups[STONE], &minionTargets, STONE_MINION, pq));
				waterMinionsAI.push_back(GameUtilities::AttachMinionAI(waterMinions[i], golem->minionGroups[WATER], &minionTargets, WATER_MINION, pq));
				fireMinionsAI.push_back(GameUtilities::AttachMinionAI(fireMinions[i], golem->minionGroups[FIRE], &minionTargets, FIRE_MINION, pq));
				woodMinionsAI.push_back(GameUtilities::AttachMinionAI(woodMinions[i], golem->minionGroups[WOOD], &minionTargets, WOOD_MINION, pq));
			}
		}



	public:
		void LoadScene(const char* filename, Engine* _engine)
		{
			//InitializeScene();
			std::fstream file{ filename, std::ios_base::in | std::ios_base::binary };

			assert(file.is_open());

			uint32_t NumLights;
			file.read((char*)&NumLights, sizeof(uint32_t));
			sceneLights.resize(NumLights);
			file.read((char*)sceneLights.data(), sizeof(Light) * NumLights);

			file.read((char*)&PlaneArguments, sizeof(PBRArguments));
			file.read((char*)&GolemArguments, sizeof(PBRArguments));
			file.read((char*)&SkyboxArguments, sizeof(PBRArguments));

			uint32_t NumAnimations;
			file.read((char*)&NumAnimations, sizeof(uint32_t));
			for (uint32_t i = 0; i < NumAnimations; i++)
			{
				uint32_t tempNum;
				file.read((char*)&tempNum, sizeof(uint32_t));
				string temp;
				temp.resize(tempNum);
				file.read((char*)temp.c_str(), tempNum);
				animations.push_back(temp);
			}

			uint32_t NumStoneAnimations;
			file.read((char*)&NumStoneAnimations, sizeof(uint32_t));
			for (uint32_t i = 0; i < NumStoneAnimations; i++)
			{
				uint32_t tempNum;
				file.read((char*)&tempNum, sizeof(uint32_t));
				string temp;
				temp.resize(tempNum);
				file.read((char*)temp.c_str(), tempNum);
				stoneMinionAnimations.push_back(temp);
			}
			stoneMinionAnimations.push_back("files/models/Minion_3_Run.animfile");
			stoneMinionAnimations.push_back("files/models/Minion_3_Punch.animfile");
			stoneMinionAnimations.push_back("files/models/Minion_3_Born.animfile");
			stoneMinionAnimations.push_back("files/models/Minion_3_Flinch.animfile");
			stoneMinionAnimations.push_back("files/models/Minion_3_Death.animfile");

			uint32_t NumWaterAnimations;
			file.read((char*)&NumWaterAnimations, sizeof(uint32_t));
			for (uint32_t i = 0; i < NumWaterAnimations; i++)
			{
				uint32_t tempNum;
				file.read((char*)&tempNum, sizeof(uint32_t));
				string temp;
				temp.resize(tempNum);
				file.read((char*)temp.c_str(), tempNum);
				waterMinionAnimations.push_back(temp);
			}
			waterMinionAnimations.push_back("files/models/Minion_4_Run.animfile");
			waterMinionAnimations.push_back("files/models/Minion_4_Punch.animfile");
			waterMinionAnimations.push_back("files/models/Minion_4_Born.animfile");
			waterMinionAnimations.push_back("files/models/Minion_4_Flinch.animfile");
			waterMinionAnimations.push_back("files/models/Minion_4_Death.animfile");

			uint32_t NumFireAnimations;
			file.read((char*)&NumFireAnimations, sizeof(uint32_t));
			for (uint32_t i = 0; i < NumFireAnimations; i++)
			{
				uint32_t tempNum;
				file.read((char*)&tempNum, sizeof(uint32_t));
				string temp;
				temp.resize(tempNum);
				file.read((char*)temp.c_str(), tempNum);
				fireMinionAnimations.push_back(temp);
			}
			fireMinionAnimations.push_back("files/models/Minion_2_Run.animfile");
			fireMinionAnimations.push_back("files/models/Minion_2_Punch.animfile");
			fireMinionAnimations.push_back("files/models/Minion_2_Born.animfile");
			fireMinionAnimations.push_back("files/models/Minion_2_Flinch.animfile");
			fireMinionAnimations.push_back("files/models/Minion_2_Death.animfile");

			uint32_t NumWoodAnimations;
			file.read((char*)&NumWoodAnimations, sizeof(uint32_t));
			for (uint32_t i = 0; i < NumWoodAnimations; i++)
			{
				uint32_t tempNum;
				file.read((char*)&tempNum, sizeof(uint32_t));
				string temp;
				temp.resize(tempNum);
				file.read((char*)temp.c_str(), tempNum);
				woodMinionAnimations.push_back(temp);
			}
			woodMinionAnimations.push_back("files/models/Minion_1_Run.animfile");
			woodMinionAnimations.push_back("files/models/Minion_1_Punch.animfile");
			woodMinionAnimations.push_back("files/models/Minion_1_Born.animfile");
			woodMinionAnimations.push_back("files/models/Minion_1_Flinch.animfile");
			woodMinionAnimations.push_back("files/models/Minion_1_Death.animfile");

			uint32_t NumStone;
			file.read((char*)&NumStone, sizeof(uint32_t));
			stoneMinionArguments.resize(NumStone);
			file.read((char*)stoneMinionArguments.data(), sizeof(PBRArguments) * NumStone);

			uint32_t NumWater;
			file.read((char*)&NumWater, sizeof(uint32_t));
			waterMinionArguments.resize(NumWater);
			file.read((char*)waterMinionArguments.data(), sizeof(PBRArguments) * NumWater);

			uint32_t NumFire;
			file.read((char*)&NumFire, sizeof(uint32_t));
			fireMinionArguments.resize(NumFire);
			file.read((char*)fireMinionArguments.data(), sizeof(PBRArguments) * NumFire);

			uint32_t NumWood;
			file.read((char*)&NumWood, sizeof(uint32_t));
			woodMinionArguments.resize(NumWood);
			file.read((char*)woodMinionArguments.data(), sizeof(PBRArguments) * NumWood);

			uint32_t NumRenderable;
			file.read((char*)&NumRenderable, sizeof(uint32_t));
			renderableArguments.resize(NumRenderable);
			file.read((char*)renderableArguments.data(), sizeof(PhysicsArgs) * NumRenderable);

			file.close();


			Plane = GameUtilities::AddSimpleAsset(PlaneArguments.Model.data(), PlaneArguments.Texture.data(), PlaneArguments.position, PlaneArguments.scale, PlaneArguments.rotation);
			ResourceManager::AddSkybox(SkyboxArguments.Model.data(), SkyboxArguments.Texture.data(), SkyboxArguments.position, SkyboxArguments.scale, SkyboxArguments.rotation);

			golem = GameUtilities::LoadGolemFromModelFile({ 500.f, 0.1f, -1150.f }, GolemArguments.scale, GolemArguments.rotation);

			for (int i = 0; i < stoneMinionCount; i++)
			{

				stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile(stoneMinionArguments[i].Model.data(), stoneMinionArguments[i].Texture.data(), stoneMinionAnimations, { 500.f + i * 5, 0.1f, -1150.f }, stoneMinionArguments[i].scale, stoneMinionArguments[i].rotation));
				waterMinions.push_back(GameUtilities::AddDestructableFromModelFile(waterMinionArguments[i].Model.data(), waterMinionArguments[i].Texture.data(), waterMinionAnimations, { 500.f + i * 5, 0.1f, -1150.f }, waterMinionArguments[i].scale, waterMinionArguments[i].rotation));
				fireMinions.push_back(GameUtilities::AddDestructableFromModelFile(fireMinionArguments[i].Model.data(), fireMinionArguments[i].Texture.data(), fireMinionAnimations, { 500.f, 0.1f, -1150.f + i * 5 }, fireMinionArguments[i].scale, fireMinionArguments[i].rotation));
				woodMinions.push_back(GameUtilities::AddDestructableFromModelFile(woodMinionArguments[i].Model.data(), woodMinionArguments[i].Texture.data(), woodMinionAnimations, { 500.f, 0.1f, -1150.f + i * 5 }, woodMinionArguments[i].scale, woodMinionArguments[i].rotation));
			}



			for (int i = 0; i < stoneMinionCount; i++)
			{
				GameUtilities::AddGameObject(stoneMinions[i]);
				GameUtilities::AddGameObject(waterMinions[i]);
				GameUtilities::AddGameObject(fireMinions[i]);
				GameUtilities::AddGameObject(woodMinions[i]);
			}


			m1 = GameUtilities::AddTriggerFromModelFile("files/models/Target.mesh", "files/textures/Target.mat", XMFLOAT3(-145, 10, -145), XMFLOAT3(3, 3, 3), XMFLOAT3(0, 0, 0));
			m1->desirability = 10;

			golem->targetMarker = m1;
			m1->gamePlayType = ADResource::ADGameplay::COMMAND_MARKER;
			GameUtilities::AddGameObject(m1);

			stoneMinionCountstr = new std::string();
			waterMinionCountstr = new std::string();
			fireMinionCountstr = new std::string();
			woodMinionCountstr = new std::string();
			allMinionCountstr = new std::string();
			villagerCountstr = new std::string();

			InitializeScene(_engine);
		}

		ADResource::ADGameplay::Golem* GetGolem()
		{
			return golem;
		}

		Renderable* GetPlane()
		{
			return Plane;
		}

		void GetMinions(std::vector<Destructable*>* stone, std::vector<Destructable*>* water, std::vector<Destructable*>* fire, std::vector<Destructable*>* wood)
		{
			*stone = stoneMinions;
			*water = waterMinions;
			*fire = fireMinions;
			*wood = woodMinions;
		}

		void destroy()
		{
			sceneLights.clear();
			delete stoneMinionCountstr;
			delete waterMinionCountstr;
			delete fireMinionCountstr;
			delete woodMinionCountstr;
			delete allMinionCountstr;
			delete villagerCountstr;

		}

		void Update(float _delta_time)
		{
			villageFlock1.Update(_delta_time);
			villageFlock2.Update(_delta_time);
			villageFlock3.Update(_delta_time);
			villageFlock4.Update(_delta_time);
			villageFlock5.Update(_delta_time);

			for (int i = 0; i < 4; ++i)
			{
				golem->minionGroups[i]->Update(_delta_time);
			}
			for (auto& stoneMinion : stoneMinionsAI)
			{
				stoneMinion->Update(_delta_time);
			}
			for (auto& waterMinion : waterMinionsAI)
			{
				waterMinion->Update(_delta_time);
			}
			for (auto& fireMinion : fireMinionsAI)
			{
				fireMinion->Update(_delta_time);
			}
			for (auto& woodMinion : woodMinionsAI)
			{
				woodMinion->Update(_delta_time);
			}
			for (auto& villager : villagerAI)
			{
				villager->Update(_delta_time);
			}
			for (auto& tower : towers)
			{
				tower->Update(_delta_time);
			}
		}

		void UpdateVillagerCount()
		{
			villagerCount--;
			villagerCountstr->clear();
			villagerCountstr->append(to_string(villagerCount));
			ADEvents::ADEventSystem::Instance()->SendEvent("VillagerCountChanged", static_cast<void*>(villagerCountstr));

			if (villagerCount <= 0)
			{
				//Using UI as a event Manager.  Temp solution.
				ADUI::UIMessage eventUIMessage;
				eventUIMessage.targetID = 0;
				eventUIMessage.externalMsg = true;
				eventUIMessage.commandID = 0;
				ADUI::MessageReceiver::SendMessage(&eventUIMessage);
			}
		}

		void UpdateMinionCounts(UINT _minionType)
		{
			switch (_minionType)
			{
			case ADResource::ADGameplay::OBJECT_TAG::STONE_MINION:
			{
				stoneMinionCount = 0;
				for (auto& minion : stoneMinions)
				{
					if (minion->active)
					{
						++stoneMinionCount;
					}
				}
				stoneMinionCountstr->clear();
				stoneMinionCountstr->append(to_string(stoneMinionCount));
				ADEvents::ADEventSystem::Instance()->SendEvent("StoneMinionCountChanged", static_cast<void*>(stoneMinionCountstr));
				break;
			}
			case ADResource::ADGameplay::OBJECT_TAG::WATER_MINION:
			{
				waterMinionCount = 0;
				for (auto& minion : waterMinions)
				{
					if (minion->active)
					{
						++waterMinionCount;
					}
				}
				//waterMinionCount--;
				waterMinionCountstr->clear();
				waterMinionCountstr->append(to_string(waterMinionCount));
				ADEvents::ADEventSystem::Instance()->SendEvent("WaterMinionCountChanged", static_cast<void*>(waterMinionCountstr));
				break;
			}
			case ADResource::ADGameplay::OBJECT_TAG::FIRE_MINION:
			{
				fireMinionCount = 0;
				for (auto& minion : fireMinions)
				{
					if (minion->active)
					{
						++fireMinionCount;
					}
				}
				//fireMinionCount--;
				fireMinionCountstr->clear();
				fireMinionCountstr->append(to_string(fireMinionCount));
				ADEvents::ADEventSystem::Instance()->SendEvent("FireMinionCountChanged", static_cast<void*>(fireMinionCountstr));
				break;
			}
			case ADResource::ADGameplay::OBJECT_TAG::WOOD_MINION:
			{
				woodMinionCount = 0;
				for (auto& minion : woodMinions)
				{
					if (minion->active)
					{
						++woodMinionCount;
					}
				}
				//woodMinionCount--;
				woodMinionCountstr->clear();
				woodMinionCountstr->append(to_string(woodMinionCount));
				ADEvents::ADEventSystem::Instance()->SendEvent("WoodMinionCountChanged", static_cast<void*>(woodMinionCountstr));
				break;
			}
			}
			//totalMinionCount--;
			totalMinionCount = stoneMinionCount + waterMinionCount + fireMinionCount + woodMinionCount;
			allMinionCountstr->clear();
			allMinionCountstr->append(to_string(totalMinionCount));
			ADEvents::ADEventSystem::Instance()->SendEvent("MinionCountChanged", static_cast<void*>(allMinionCountstr));
		}




		//REplace me!
		void AddBuilding(Building* _building)
		{
			buildings.push_back(_building);
			if (_building->HasTurret())
			{
				towers.push_back(GameUtilities::AttachTowerAI(_building->GetTurret(), &villagerThreats));
			}

			if (_building->HasRubble())
			{
				_building->GetRubble()->active = false;
				minionAvoids.push_back(_building->GetRubble());
			}
		}
	};
}

