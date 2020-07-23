#pragma once
#include "ResourceManager.h"
#include "../Game/GameUtilities.h"
#include "../Game/GameObjectClasses.h"
#include "../Game/GameplayAI.h"
#include "MeshLoader.h"
#include "ADCombat.h"
#include "ADEventSystem.h"
#include "ADUserInterface.h"

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
			XMFLOAT3 position, scale, rotation;
		};
		struct PhysicsArgs
		{
			std::array<char, 260> Model;
			std::array<char, 260> Texture;
			XMFLOAT3 position, scale, rotation;
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

		void AddRubble(ADResource::ADGameplay::Building* _building, int _rubbleID, XMFLOAT3 rotation, XMFLOAT3 collider_scale, XMFLOAT3 offset)
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
			_building->SetRubble(rubble);
			//I don't feel like pasting this 3000s times.... so here it is.
			AddBuilding(_building);
		}

		void AddBallista()
		{

		}

		void InitializeScene(Engine* _engine)
		{
#pragma region oldshit

			//Light light;
			//ZeroMemory(&light, sizeof(Light));
			//light.lightType = (int)LIGHTTYPE::DIRECTIONAL;
			//light.diffuse = XMFLOAT4(0.5, 0.5, 0.5, 1);
			//light.diffuseIntensity = 1;
			//light.ambient = XMFLOAT4(0.156f, 0.003f, 0.215f, 1);
			//light.ambientIntensity = 3;
			//light.lightDirection = XMFLOAT4(0, -0.5f, -1, 1);
			//light.position = XMFLOAT4(0, 0, 0, 1);

			//// Point light
			//Light light1;
			//ZeroMemory(&light1, sizeof(Light));
			//light.lightType - (int)LIGHTTYPE::POINT;
			//light.diffuse = XMFLOAT4(1, 0.647, 0, 1);
			//light.diffuseIntensity = 1;
			//light.position = (XMFLOAT4(2.5, 0, 0, 1));
			//light.lightRadius = 20;

			//sceneLights.push_back(light);
			//sceneLights.push_back(light1);

			//float mapWidth = 1000;
			//float mapHeight = 1000;

			//PlaneArguments = { "files/models/Plane.mesh", "files/textures/Grass.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(mapWidth, 100, mapHeight), XMFLOAT3(0, 0, 0) };
			//

			//sceneLights.push_back(light);
			//sceneLights.push_back(light1);

			//float mapWidth = 1000;
			//float mapHeight = 1000;
			//PlaneArguments = { "files/models/Ground.mesh", "files/textures/Ground.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(mapWidth, 100, mapHeight), XMFLOAT3(0, 0, 0) };
			//

			//
			//animations.push_back("files/models/Golem_1_Idle.animfile");
			//animations.push_back("files/models/Golem_1_Born.animfile");
			//animations.push_back("files/models/Golem_1_Run.animfile");
			//animations.push_back("files/models/Golem_1_Death.animfile");
			//animations.push_back("files/models/Golem_1_Kick.animfile");
			//
			//stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");
			//waterMinionAnimations.push_back("files/models/Minion_4_Idle.animfile");
			//fireMinionAnimations.push_back("files/models/Minion_2_Idle.animfile");
			//woodMinionAnimations.push_back("files/models/Minion_1_Idle.animfile");

			//SkyboxArguments = { "files/models/Skybox.mesh", "files/textures/Skybox.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(-10, -10, -10), XMFLOAT3(0, 0, 0) };

			//GolemArguments = { "files/models/Golem_1.AnimMesh", "files/textures/Golem_1.mat", XMFLOAT3(10, 0.00001, 10), XMFLOAT3(0.1, 0.1, 0.1), XMFLOAT3(0, 0, 0) };
			//

			//for (int i = 0; i < 10; i++)
			//{
			//	stoneMinionArguments.push_back({ "files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", XMFLOAT3(-130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0) });
			//	waterMinionArguments.push_back({ "files/models/Minion_4.AnimMesh", "files/textures/Minion_4.mat", XMFLOAT3(-130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0) });
			//	fireMinionArguments.push_back({"files/models/Minion_2.AnimMesh", "files/textures/Minion_2.mat", XMFLOAT3(130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)});
			//	woodMinionArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0) });
			//}


			//Building* house1 = new Building(XMFLOAT3(-500, 0, 100), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 30, 30), XMFLOAT3(0, 0.5f, 0.15), GameUtilities::GenerateHouse1, "House1");
			//GameUtilities::AddGameObject(house1);
			//AddBuilding(house1);
			////house1->destructionEmitter = &engine->destructionCloud;
			////house1->destructionEmitter2 = &engine->destructionCloud2;

			//Building* rubble1 = new Building(XMFLOAT3(-500, 0, 100), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 30, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble1, "Rubble");
			//GameUtilities::AddGameObject(rubble1);
			//house1->SetRubble(rubble1);


			//for (int i = 0; i < 10; i++)
			//{
			//	Building* housey = new Building(XMFLOAT3(-200 + (i * 33), 0, -100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateHouse1, "House1");
			//	GameUtilities::AddGameObject(housey);
			//	AddBuilding(housey);
			//	//housey->destructionEmitter = &engine->destructionCloud;
			//	//housey->destructionEmitter2 = &engine->destructionCloud2;

			//	Building* rubble2 = new Building(XMFLOAT3(-200 + (i * 33), 0, -100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble2, "Rubble");
			//	GameUtilities::AddGameObject(rubble2);
			//	housey->SetRubble(rubble2);
			//}

			/*Building* attackTower = new Building(XMFLOAT3(10, 0, 200), XMFLOAT3(0, 0, 0), XMFLOAT3(15, 25, 15), XMFLOAT3(0, 0, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower);

			Building* ballista = new Building(XMFLOAT3(10, 20, 200), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista);*/

			

			//Building* gateway = new Building(XMFLOAT3(330, 0, -330), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway, "Gate");
			//GameUtilities::AddGameObject(gateway);

			//Building* scaffolding = new Building(XMFLOAT3(410, 0, -400), XMFLOAT3(0, 90, 0), XMFLOAT3(5, 25, 25), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateScaffoldWallX, "Scaffolding");
			//GameUtilities::AddGameObject(scaffolding);

			//Building* wall = new Building(XMFLOAT3(700, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall, "Wall");
			//GameUtilities::AddGameObject(wall);

			//Building* well = new Building(XMFLOAT3(1500, 0, 1500), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 5, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell, "Rubble");
			//GameUtilities::AddGameObject(well);

			//Building* tavern = new Building(XMFLOAT3(-100, 0, 100), XMFLOAT3(0, 45, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			//GameUtilities::AddGameObject(tavern);
			////AddBuilding(tavern);

			//Building* lamp = new Building(XMFLOAT3(62.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 1.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Rubble");
			//GameUtilities::AddGameObject(lamp);
			//Building* lamp2 = new Building(XMFLOAT3(-125, 0, 187.5), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 1.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Rubble");
			//GameUtilities::AddGameObject(lamp2);

			//Building* rock = new Building(XMFLOAT3(-20, 0, 220), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRock1, "Rubble");
			//GameUtilities::AddGameObject(rock);

			//Building* meeting_hall = new Building(XMFLOAT3(100, 0, 360), XMFLOAT3(0, -135, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "House4");
			//GameUtilities::AddGameObject(meeting_hall);
			////AddBuilding(meeting_hall);

			//Building* cart = new Building(XMFLOAT3(-125, 0, 310), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5, 0.25, 0.1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateCart, "Cart");
			//GameUtilities::AddGameObject(cart);

			//for (int i = 0; i < 10; ++i)
			//{
			//	Building* realtree = new Building(XMFLOAT3(RandFloat(i - 1000, i + 1000), 0, RandFloat(i - 1000, i + 1000)), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 3, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateTree, "Rubble");
			//	GameUtilities::AddGameObject(realtree);
			//}


#pragma endregion

			//villageFlock1.groupTarget = &golem->transform;

			villageFlock1.groupCohesionStrength = 0.01f;
			villageFlock1.groupSeparationStrength = 0.6f;
			villageFlock2.groupCohesionStrength = 0.01f;
			villageFlock2.groupSeparationStrength = 0.6f;
			villageFlock3.groupCohesionStrength = 0.01f;
			villageFlock3.groupSeparationStrength = 0.6f;
			villageFlock3.groupCohesionStrength = 0.01f;
			villageFlock3.groupSeparationStrength = 0.6f;
			villageFlock4.groupCohesionStrength = 0.01f;
			villageFlock4.groupSeparationStrength = 0.6f;
			villageFlock5.groupCohesionStrength = 0.01f;
			villageFlock5.groupSeparationStrength = 0.6f;

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
			GameUtilities::AddGameObject(gate1);
			minionTargets.push_back(gate1);

			Building* gate2 = new Building(XMFLOAT3(-474, 0, -595.5), XMFLOAT3(0, 90, 0), XMFLOAT3(40, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway, "Gate");
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
			GameUtilities::AddGameObject(tower1);
			AddRubble(tower1, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0));

			Building* tower2 = new Building(XMFLOAT3(271.5, 0, -826.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower2);
			AddRubble(tower2, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0));

			Building* tower3 = new Building(XMFLOAT3(-474, 0, -826.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower3);
			AddRubble(tower3, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0));

			Building* tower4 = new Building(XMFLOAT3(-474, 0, -364.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower4);
			AddRubble(tower4, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0));

			Building* tower5 = new Building(XMFLOAT3(-255, 0, -364.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower5);
			AddRubble(tower5, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0));

			Building* tower6 = new Building(XMFLOAT3(-255, 0, -145.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower6);
			AddRubble(tower6, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0));

			//========================================================Houses

			Building* tutorial_home1 = new Building(XMFLOAT3(570, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home1);
			AddRubble(tutorial_home1, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home2 = new Building(XMFLOAT3(570, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home2);
			AddRubble(tutorial_home2, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home3 = new Building(XMFLOAT3(460, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home3);
			AddRubble(tutorial_home3, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home4 = new Building(XMFLOAT3(460, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home4);
			AddRubble(tutorial_home4, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home5 = new Building(XMFLOAT3(570, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home5);
			AddRubble(tutorial_home5, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home6 = new Building(XMFLOAT3(570, 0, -850), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home6);
			AddRubble(tutorial_home6, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home7 = new Building(XMFLOAT3(460, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home7);
			AddRubble(tutorial_home7, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home8 = new Building(XMFLOAT3(460, 0, -850), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home8);
			AddRubble(tutorial_home8, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home9 = new Building(XMFLOAT3(570, 0, -100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home9);
			AddRubble(tutorial_home9, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home10 = new Building(XMFLOAT3(570, 0, -200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home10);
			AddRubble(tutorial_home10, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home11 = new Building(XMFLOAT3(460, 0, -100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home11);
			AddRubble(tutorial_home11, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* tutorial_home12 = new Building(XMFLOAT3(460, 0, -200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home12);
			AddRubble(tutorial_home12, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home1 = new Building(XMFLOAT3(575, 0, 200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home1);
			AddRubble(home1, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home2 = new Building(XMFLOAT3(525, 0, 250), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home2);
			AddRubble(home2, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home3 = new Building(XMFLOAT3(475, 0, 300), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home3);
			AddRubble(home3, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home4 = new Building(XMFLOAT3(475, 0, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home4);
			AddRubble(home4, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home5 = new Building(XMFLOAT3(475, 0, 400), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home5);
			AddRubble(home5, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home6 = new Building(XMFLOAT3(525, 0, 450), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home6);
			AddRubble(home6, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home7 = new Building(XMFLOAT3(575, 0, 500), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home7);
			AddRubble(home7, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home8 = new Building(XMFLOAT3(200, 0, 575), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home8);
			AddRubble(home8, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home9 = new Building(XMFLOAT3(250, 0, 525), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home9);
			AddRubble(home9, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home10 = new Building(XMFLOAT3(300, 0, 475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home10);
			AddRubble(home10, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home11 = new Building(XMFLOAT3(350, 0, 475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home11);
			AddRubble(home11, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home12 = new Building(XMFLOAT3(400, 0, 475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home12);
			AddRubble(home12, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home13 = new Building(XMFLOAT3(450, 0, 525), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home13);
			AddRubble(home13, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home14 = new Building(XMFLOAT3(500, 0, 575), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home14);
			AddRubble(home14, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home15 = new Building(XMFLOAT3(440, 0, 80), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home15);
			AddRubble(home15, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home16 = new Building(XMFLOAT3(400, 0, 120), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home16);
			AddRubble(home16, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home17 = new Building(XMFLOAT3(360, 0, 160), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home17);
			AddRubble(home17, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home18 = new Building(XMFLOAT3(320, 0, 200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home18);
			AddRubble(home18, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home19 = new Building(XMFLOAT3(280, 0, 240), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home19);
			AddRubble(home19, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home20 = new Building(XMFLOAT3(240, 0, 280), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home20);
			AddRubble(home20, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home21 = new Building(XMFLOAT3(180, 0, 275), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home21);
			AddRubble(home21, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home22 = new Building(XMFLOAT3(120, 0, 270), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home22);
			AddRubble(home22, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home23 = new Building(XMFLOAT3(60, 0, 265), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home23);
			AddRubble(home23, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home24 = new Building(XMFLOAT3(0, 0, 260), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home24);
			AddRubble(home24, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home25 = new Building(XMFLOAT3(-60, 0, 255), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home25);
			AddRubble(home25, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home26 = new Building(XMFLOAT3(-120, 0, 250), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home26);
			AddRubble(home26, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home27 = new Building(XMFLOAT3(180, 0, 500), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home27);
			AddRubble(home27, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home28 = new Building(XMFLOAT3(120, 0, 495), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home28);
			AddRubble(home28, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home29 = new Building(XMFLOAT3(60, 0, 490), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home29);
			AddRubble(home29, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home30 = new Building(XMFLOAT3(0, 0, 485), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home30);
			AddRubble(home30, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home31 = new Building(XMFLOAT3(-60, 0, 480), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home31);
			AddRubble(home31, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home32 = new Building(XMFLOAT3(-120, 0, 475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home32);
			AddRubble(home32, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home33 = new Building(XMFLOAT3(-180, 0, 465), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home33);
			AddRubble(home33, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home34 = new Building(XMFLOAT3(-220, 0, 435), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home34);
			AddRubble(home34, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home35 = new Building(XMFLOAT3(-250, 0, 395), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home35);
			AddRubble(home35, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home36 = new Building(XMFLOAT3(-270, 0, 345), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home36);
			AddRubble(home36, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home37 = new Building(XMFLOAT3(-270, 0, 165), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home37);
			AddRubble(home37, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home38 = new Building(XMFLOAT3(-250, 0, 115), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home38);
			AddRubble(home38, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home39 = new Building(XMFLOAT3(-220, 0, 75), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home39);
			AddRubble(home39, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home40 = new Building(XMFLOAT3(-180, 0, 45), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home40);
			AddRubble(home40, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home41 = new Building(XMFLOAT3(300, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home41);
			AddRubble(home41, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home42 = new Building(XMFLOAT3(250, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home42);
			AddRubble(home42, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home43 = new Building(XMFLOAT3(200, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home43);
			AddRubble(home43, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home44 = new Building(XMFLOAT3(150, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home44);
			AddRubble(home44, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home45 = new Building(XMFLOAT3(100, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home45);
			AddRubble(home45, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home46 = new Building(XMFLOAT3(0, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home46);
			AddRubble(home46, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home47 = new Building(XMFLOAT3(-50, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home47);
			AddRubble(home47, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home48 = new Building(XMFLOAT3(-100, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home48);
			AddRubble(home48, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home49 = new Building(XMFLOAT3(-150, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home49);
			AddRubble(home49, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home50 = new Building(XMFLOAT3(-200, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home50);
			AddRubble(home50, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home51 = new Building(XMFLOAT3(-300, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home51);
			AddRubble(home51, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home52 = new Building(XMFLOAT3(-350, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home52);
			AddRubble(home52, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home53 = new Building(XMFLOAT3(-400, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home53);
			AddRubble(home53, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home54 = new Building(XMFLOAT3(-450, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home54);
			AddRubble(home54, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home55 = new Building(XMFLOAT3(-500, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home55);
			AddRubble(home55, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home56 = new Building(XMFLOAT3(-200, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home56);
			AddRubble(home56, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home57 = new Building(XMFLOAT3(-125, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home57);
			AddRubble(home57, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home58 = new Building(XMFLOAT3(-50, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home58);
			AddRubble(home58, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home59 = new Building(XMFLOAT3(50, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home59);
			AddRubble(home59, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home60 = new Building(XMFLOAT3(125, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home60);
			AddRubble(home60, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home61 = new Building(XMFLOAT3(200, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home61);
			AddRubble(home61, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home62 = new Building(XMFLOAT3(-275, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home62);
			AddRubble(home62, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home63 = new Building(XMFLOAT3(-350, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home63);
			AddRubble(home63, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home64 = new Building(XMFLOAT3(-275, 0, -125), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home64);
			AddRubble(home64, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home65 = new Building(XMFLOAT3(-350, 0, -125), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home65);
			AddRubble(home65, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home66 = new Building(XMFLOAT3(225, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home66);
			AddRubble(home66, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home67 = new Building(XMFLOAT3(225, 0, -150), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home67);
			AddRubble(home67, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home68 = new Building(XMFLOAT3(225, 0, -225), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home68);
			AddRubble(home68, 2, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home69 = new Building(XMFLOAT3(225, 0, -300), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home69);
			AddRubble(home69, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home70 = new Building(XMFLOAT3(225, 0, -375), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home70);
			AddRubble(home70, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home71 = new Building(XMFLOAT3(225, 0, -450), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home71);
			AddRubble(home71, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home72 = new Building(XMFLOAT3(225, 0, -525), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home72);
			AddRubble(home72, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home73 = new Building(XMFLOAT3(225, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home73);
			AddRubble(home73, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home74 = new Building(XMFLOAT3(225, 0, -675), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home74);
			AddRubble(home74, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home75 = new Building(XMFLOAT3(225, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home75);
			AddRubble(home75, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home76 = new Building(XMFLOAT3(-225, 0, -150), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home76);
			AddRubble(home76, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home77 = new Building(XMFLOAT3(-187.5, 0, -187.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home77);
			AddRubble(home77, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home78 = new Building(XMFLOAT3(-150, 0, -225), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home78);
			AddRubble(home78, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home79 = new Building(XMFLOAT3(-112.5, 0, -262.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home79);
			AddRubble(home79, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home80 = new Building(XMFLOAT3(-75, 0, -300), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home80);
			AddRubble(home80, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home81 = new Building(XMFLOAT3(-225, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home81);
			AddRubble(home81, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home82 = new Building(XMFLOAT3(-187.5, 0, -387.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home82);
			AddRubble(home82, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home83 = new Building(XMFLOAT3(-150, 0, -425), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home83);
			AddRubble(home83, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home84 = new Building(XMFLOAT3(-112.5, 0, -462.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home84);
			AddRubble(home84, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home85 = new Building(XMFLOAT3(-75, 0, -500), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home85);
			AddRubble(home85, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home86 = new Building(XMFLOAT3(150, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home86);
			AddRubble(home86, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home87 = new Building(XMFLOAT3(125, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home87);
			AddRubble(home87, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home88 = new Building(XMFLOAT3(100, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home88);
			AddRubble(home88, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home89 = new Building(XMFLOAT3(75, 0, -700), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home89);
			AddRubble(home89, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home90 = new Building(XMFLOAT3(50, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home90);
			AddRubble(home90, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home91 = new Building(XMFLOAT3(-25, 0, -375), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home91);
			AddRubble(home91, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home92 = new Building(XMFLOAT3(12.5, 0, -337.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home92);
			AddRubble(home92, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home93 = new Building(XMFLOAT3(50, 0, -300), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home93);
			AddRubble(home93, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home94 = new Building(XMFLOAT3(87.5, 0, -337.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home94);
			AddRubble(home94, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home95 = new Building(XMFLOAT3(125, 0, -375), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home95);
			AddRubble(home95, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home96 = new Building(XMFLOAT3(-200, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home96);
			AddRubble(home96, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home97 = new Building(XMFLOAT3(-350, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home97);
			AddRubble(home97, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home98 = new Building(XMFLOAT3(-275, 0, -525), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home98);
			AddRubble(home98, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home99 = new Building(XMFLOAT3(-275, 0, -675), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home99);
			AddRubble(home99, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home100 = new Building(XMFLOAT3(-225, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home100);
			AddRubble(home100, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home101 = new Building(XMFLOAT3(-325, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home101);
			AddRubble(home101, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home102 = new Building(XMFLOAT3(-225, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home102);
			AddRubble(home102, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home103 = new Building(XMFLOAT3(-325, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home103);
			AddRubble(home103, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home104 = new Building(XMFLOAT3(0, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home104);
			AddRubble(home104, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home105 = new Building(XMFLOAT3(-75, 0, -675), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home105);
			AddRubble(home105, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home106 = new Building(XMFLOAT3(-150, 0, -700), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home106);
			AddRubble(home106, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home107 = new Building(XMFLOAT3(-225, 0, -725), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home107);
			AddRubble(home107, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home108 = new Building(XMFLOAT3(-300, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home108);
			AddRubble(home108, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home109 = new Building(XMFLOAT3(-375, 0, -775), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home109);
			AddRubble(home109, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home110 = new Building(XMFLOAT3(-275, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home110);
			AddRubble(home110, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home111 = new Building(XMFLOAT3(-325, 0, -425), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home111);
			AddRubble(home111, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home112 = new Building(XMFLOAT3(-375, 0, -475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home112);
			AddRubble(home112, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			Building* home113 = new Building(XMFLOAT3(-425, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home113);
			AddRubble(home113, 1, XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5));

			//========================================Large Buildings

			Building* tavern1 = new Building(XMFLOAT3(-400, 0, 250), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern1);
			AddRubble(tavern1, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0));

			Building* tavern2 = new Building(XMFLOAT3(-400, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern2);
			AddRubble(tavern2, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0));

			Building* tavern3 = new Building(XMFLOAT3(200, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern3);
			AddRubble(tavern3, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0));

			Building* tavern4 = new Building(XMFLOAT3(200, 0, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern4);
			AddRubble(tavern4, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0));

			Building* meeting_hall1 = new Building(XMFLOAT3(-100, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			GameUtilities::AddGameObject(meeting_hall1);
			AddRubble(meeting_hall1, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0));

			Building* meeting_hall2 = new Building(XMFLOAT3(50, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			GameUtilities::AddGameObject(meeting_hall2);
			AddRubble(meeting_hall2, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0));

			Building* meeting_hall3 = new Building(XMFLOAT3(-275, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			GameUtilities::AddGameObject(meeting_hall3);
			AddRubble(meeting_hall3, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0));

			Building* meeting_hall4 = new Building(XMFLOAT3(-475, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			GameUtilities::AddGameObject(meeting_hall4);
			AddRubble(meeting_hall4, 3, XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0));
			//Building* rubble1 = new Building(XMFLOAT3(580, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			//GameUtilities::AddGameObject(rubble1);
			//meeting_hall4->SetRubble(rubble1);

			//===================================================Ballista Towers

			Building* attackTower1 = new Building(XMFLOAT3(450, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower1);

			Building* ballista1 = new Building(XMFLOAT3(450, 20, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista1);
			Building* rubble1 = new Building(XMFLOAT3(450, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			rubble1->active = false;
			GameUtilities::AddGameObject(rubble1);
			attackTower1->SetRubble(rubble1);
			attackTower1->SetTurret(ballista1);
			AddBuilding(attackTower1);

			Building* attackTower2 = new Building(XMFLOAT3(580, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower2);

			Building* ballista2 = new Building(XMFLOAT3(580, 20, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista2);
			Building* rubble2 = new Building(XMFLOAT3(580, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble2);
			attackTower2->SetRubble(rubble2);
			attackTower2->SetTurret(ballista2);
			AddBuilding(attackTower2);

			Building* attackTower3 = new Building(XMFLOAT3(515, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower3);		

			Building* ballista3 = new Building(XMFLOAT3(515, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista3);	
			Building* rubble3 = new Building(XMFLOAT3(515, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble3);
			attackTower3->SetRubble(rubble3);
			attackTower3->SetTurret(ballista3);
			AddBuilding(attackTower3);

			Building* attackTower4 = new Building(XMFLOAT3(550, 0, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower4);

			Building* ballista4 = new Building(XMFLOAT3(550, 20, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista4);
			Building* rubble4 = new Building(XMFLOAT3(550, 0, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble4);
			attackTower4->SetRubble(rubble4);
			attackTower4->SetTurret(ballista4);
			AddBuilding(attackTower4);

			Building* attackTower5 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower5);

			Building* ballista5 = new Building(XMFLOAT3(350, 20, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista5);
			Building* rubble5 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble5);
			attackTower5->SetRubble(rubble5);
			attackTower5->SetTurret(ballista5);
			AddBuilding(attackTower5);

			Building* attackTower6 = new Building(XMFLOAT3(250, 0, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower6);

			Building* ballista6 = new Building(XMFLOAT3(250, 20, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista6);
			Building* rubble6 = new Building(XMFLOAT3(250, 0, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble6);
			attackTower6->SetRubble(rubble6);
			attackTower6->SetTurret(ballista6);
			AddBuilding(attackTower6);

			Building* attackTower7 = new Building(XMFLOAT3(-25, 0, 450), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower7);

			Building* ballista7 = new Building(XMFLOAT3(-25, 20, 450), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista7);
			Building* rubble7 = new Building(XMFLOAT3(-25, 0, 450), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble7);
			attackTower7->SetRubble(rubble7);
			attackTower7->SetTurret(ballista7);
			AddBuilding(attackTower7);

			Building* attackTower8 = new Building(XMFLOAT3(-25, 0, 300), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower8);

			Building* ballista8 = new Building(XMFLOAT3(-25, 20, 300), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista8);
			Building* rubble8 = new Building(XMFLOAT3(-25, 0, 300), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble8);
			attackTower8->SetRubble(rubble8);
			attackTower8->SetTurret(ballista8);
			AddBuilding(attackTower8);

			Building* attackTower9 = new Building(XMFLOAT3(-275, 0, 212.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower9);

			Building* ballista9 = new Building(XMFLOAT3(-275, 20, 212.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista9);
			Building* rubble9 = new Building(XMFLOAT3(-275, 0, 212.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble9);
			attackTower9->SetRubble(rubble9);
			attackTower9->SetTurret(ballista9);
			AddBuilding(attackTower9);

			Building* attackTower10 = new Building(XMFLOAT3(-275, 0, 287.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower10);

			Building* ballista10 = new Building(XMFLOAT3(-275, 20, 287.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista10);
			Building* rubble10 = new Building(XMFLOAT3(-275, 0, 287.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble10);
			attackTower10->SetRubble(rubble10);
			attackTower10->SetTurret(ballista10);
			AddBuilding(attackTower10);

			Building* attackTower11 = new Building(XMFLOAT3(-25, 0, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower11);

			Building* ballista11 = new Building(XMFLOAT3(-25, 20, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista11);
			Building* rubble11 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble11);
			attackTower11->SetRubble(rubble11);
			attackTower11->SetTurret(ballista11);
			AddBuilding(attackTower11);

			Building* attackTower12 = new Building(XMFLOAT3(25, 0, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower12);

			Building* ballista12 = new Building(XMFLOAT3(25, 20, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista12);
			Building* rubble12 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble12);
			attackTower12->SetRubble(rubble12);
			attackTower12->SetTurret(ballista12);
			AddBuilding(attackTower12);

			Building* attackTower13 = new Building(XMFLOAT3(-450, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower13);

			Building* ballista13 = new Building(XMFLOAT3(-450, 20, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista13);
			Building* rubble13 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble13);
			attackTower13->SetRubble(rubble13);
			attackTower13->SetTurret(ballista13);
			AddBuilding(attackTower13);

			Building* attackTower14 = new Building(XMFLOAT3(-50, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower14);

			Building* ballista14 = new Building(XMFLOAT3(-50, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista14);
			Building* rubble14 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble14);
			attackTower14->SetRubble(rubble14);
			attackTower14->SetTurret(ballista14);
			AddBuilding(attackTower14);

			Building* attackTower15 = new Building(XMFLOAT3(150, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower15);

			Building* ballista15 = new Building(XMFLOAT3(150, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista15);
			Building* rubble15 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble15);
			attackTower15->SetRubble(rubble15);
			attackTower15->SetTurret(ballista15);
			AddBuilding(attackTower15);

			Building* attackTower16 = new Building(XMFLOAT3(-449, 0, -570.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower16);

			Building* ballista16 = new Building(XMFLOAT3(-449, 20, -570.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista16);
			Building* rubble16 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble16);
			attackTower16->SetRubble(rubble16);
			attackTower16->SetTurret(ballista16);
			AddBuilding(attackTower16);

			Building* attackTower17 = new Building(XMFLOAT3(-449, 0, -620.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower17);

			Building* ballista17 = new Building(XMFLOAT3(-449, 20, -620.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista17);
			Building* rubble17 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble17);
			attackTower17->SetRubble(rubble17);
			attackTower17->SetTurret(ballista17);
			AddBuilding(attackTower17);

			Building* attackTower18 = new Building(XMFLOAT3(-212.5, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower18);

			Building* ballista18 = new Building(XMFLOAT3(-212.5, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista18);
			Building* rubble18 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble18);
			attackTower18->SetRubble(rubble18);
			attackTower18->SetTurret(ballista18);
			AddBuilding(attackTower18);

			Building* attackTower19 = new Building(XMFLOAT3(-175, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower19);

			Building* ballista19 = new Building(XMFLOAT3(-175, 20, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista19);
			Building* rubble19 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble19);
			attackTower19->SetRubble(rubble19);
			attackTower19->SetTurret(ballista19);
			AddBuilding(attackTower19);

			Building* attackTower20 = new Building(XMFLOAT3(-175, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower20);

			Building* ballista20 = new Building(XMFLOAT3(-175, 20, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista20);
			Building* rubble20 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble20);
			attackTower20->SetRubble(rubble20);
			attackTower20->SetTurret(ballista20);
			AddBuilding(attackTower20);

			Building* attackTower21 = new Building(XMFLOAT3(150, 0, -775), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower21);

			Building* ballista21 = new Building(XMFLOAT3(150, 20, -775), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista21);
			Building* rubble21 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble21);
			attackTower21->SetRubble(rubble21);
			attackTower21->SetTurret(ballista21);
			AddBuilding(attackTower21);

			Building* attackTower22 = new Building(XMFLOAT3(-350, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower22);

			Building* ballista22 = new Building(XMFLOAT3(-350, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista22);
			Building* rubble22 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble22);
			attackTower22->SetRubble(rubble22);
			attackTower22->SetTurret(ballista22);
			AddBuilding(attackTower22);

			Building* attackTower23 = new Building(XMFLOAT3(-450, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower23);

			Building* ballista23 = new Building(XMFLOAT3(-450, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista23);
			Building* rubble23 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble23);
			attackTower23->SetRubble(rubble23);
			attackTower23->SetTurret(ballista23);
			AddBuilding(attackTower23);

			Building* attackTower24 = new Building(XMFLOAT3(-50, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower24);

			Building* ballista24 = new Building(XMFLOAT3(-50, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista24);
			Building* rubble24 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble24);
			attackTower24->SetRubble(rubble24);
			attackTower24->SetTurret(ballista24);
			AddBuilding(attackTower24);

			Building* attackTower25 = new Building(XMFLOAT3(-150, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower25);

			Building* ballista25 = new Building(XMFLOAT3(-150, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista25);
			Building* rubble25 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble25);
			attackTower25->SetRubble(rubble25);
			attackTower25->SetTurret(ballista25);
			AddBuilding(attackTower25);

			Building* attackTower26 = new Building(XMFLOAT3(150, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower26);

			Building* ballista26 = new Building(XMFLOAT3(150, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista26);
			Building* rubble26 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble26);
			attackTower26->SetRubble(rubble26);
			attackTower26->SetTurret(ballista26);
			AddBuilding(attackTower26);

			Building* attackTower27 = new Building(XMFLOAT3(250, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower27);

			Building* ballista27 = new Building(XMFLOAT3(250, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista27);
			Building* rubble27 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble27);
			attackTower27->SetRubble(rubble27);
			attackTower27->SetTurret(ballista27);
			AddBuilding(attackTower27);


			Building* attackTower28 = new Building(XMFLOAT3(-450, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower28);

			Building* ballista28 = new Building(XMFLOAT3(-450, 20, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista28);
			Building* rubble28 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble28);
			attackTower28->SetRubble(rubble28);
			attackTower28->SetTurret(ballista28);
			AddBuilding(attackTower28);

			Building* attackTower29 = new Building(XMFLOAT3(-50, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower29);

			Building* ballista29 = new Building(XMFLOAT3(-50, 20, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista29);
			Building* rubble29 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble29);
			attackTower29->SetRubble(rubble29);
			attackTower29->SetTurret(ballista29);
			AddBuilding(attackTower29);

			Building* attackTower30 = new Building(XMFLOAT3(50, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower30);

			Building* ballista30 = new Building(XMFLOAT3(50, 20, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista30);
			Building* rubble30 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble30);
			attackTower30->SetRubble(rubble30);
			attackTower30->SetTurret(ballista30);
			AddBuilding(attackTower30);

#pragma endregion

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
			towers.push_back(GameUtilities::AttachTowerAI(ballista30, &villagerThreats));

			for (int i = 0; i < 10; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -245), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock1, &villagerThreats, &buildings, _engine));
			}
			for (int i = 10; i < 20; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -400), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock2, &villagerThreats, &buildings));
			}
			for (int i = 20; i < 30; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -700), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock3, &villagerThreats, &buildings));
			}
			for (int i = 30; i < 40; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -900), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock4, &villagerThreats, &buildings));
			}
			for (int i = 40; i < 50; i++)
			{
				villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3(0.f, 0.1f, (0.f+(i*-200.f))), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock5, &villagerThreats, &buildings));
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
				//cg->members.push_back(stoneMinions[i]);
				//cg->members.push_back(fireMinions[i]);
				//cg->members.push_back(waterMinions[i]);
				cg->members.push_back(stoneMinions[i]);
				sg->members.push_back(stoneMinions[i]);
				//tg->members.push_back(stoneMinions[i]);

				cg->members.push_back(waterMinions[i]);
				sg->members.push_back(waterMinions[i]);
				//tg->members.push_back(waterMinions[i]);

				cg->members.push_back(fireMinions[i]);
				sg->members.push_back(fireMinions[i]);
				//tg->members.push_back(fireMinions[i]);

				cg->members.push_back(woodMinions[i]);
				sg->members.push_back(woodMinions[i]);
				//tg->members.push_back(woodMinions[i]);
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
				//stoneMinionsAI[i]->currentTarget = golem->targetMarker;
				waterMinionsAI.push_back(GameUtilities::AttachMinionAI(waterMinions[i], golem->minionGroups[WATER], &minionTargets, WATER_MINION, pq));
				//waterMinionsAI[i]->currentTarget = golem->targetMarker;
				fireMinionsAI.push_back(GameUtilities::AttachMinionAI(fireMinions[i], golem->minionGroups[FIRE], &minionTargets, FIRE_MINION, pq));
				//fireMinionsAI[i]->currentTarget = golem->targetMarker;
				woodMinionsAI.push_back(GameUtilities::AttachMinionAI(woodMinions[i], golem->minionGroups[WOOD], &minionTargets, WOOD_MINION, pq));
				//woodMinionsAI[i]->currentTarget = golem->targetMarker;

				//stoneMinionsAI[i]->mySSM.states[1]->objectsToAvoid.push_back(villagers[i]);
				//waterMinionsAI[i]->mySSM.states[1]->objectsToAvoid.push_back(villagers[i]);
				//fireMinionsAI[i]->mySSM.states[1]->objectsToAvoid.push_back(villagers[i]);
				//woodMinionsAI[i]->mySSM.states[1]->objectsToAvoid.push_back(villagers[i]);
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

			for (unsigned int i = 0; i < sceneLights.size(); i++)
			{
				ResourceManager::AddLight(sceneLights[i]);
			}

			Plane = GameUtilities::AddSimpleAsset(PlaneArguments.Model.data(), PlaneArguments.Texture.data(), PlaneArguments.position, PlaneArguments.scale, PlaneArguments.rotation);
			ResourceManager::AddSkybox(SkyboxArguments.Model.data(), SkyboxArguments.Texture.data(), SkyboxArguments.position, SkyboxArguments.scale, SkyboxArguments.rotation);

			golem = GameUtilities::LoadGolemFromModelFile({ 500.f, 0.1f, -1150.f }, GolemArguments.scale, GolemArguments.rotation);

			for (int i = 0; i < stoneMinionCount; i++)
			{
				//stoneMinionArguments[i].position.x += i * 5;
				//waterMinionArguments[i].position.z += i * 5;
				//fireMinionArguments[i].position.x += i * 5;
				//woodMinionArguments[i].position.x += i * 5;
				stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile(stoneMinionArguments[i].Model.data(), stoneMinionArguments[i].Texture.data(), stoneMinionAnimations, { 500.f + i * 5, 0.1f, -1150.f }, stoneMinionArguments[i].scale, stoneMinionArguments[i].rotation));
				waterMinions.push_back(GameUtilities::AddDestructableFromModelFile(waterMinionArguments[i].Model.data(), waterMinionArguments[i].Texture.data(), waterMinionAnimations, { 500.f + i * 5, 0.1f, -1150.f }, waterMinionArguments[i].scale, waterMinionArguments[i].rotation));
				fireMinions.push_back(GameUtilities::AddDestructableFromModelFile(fireMinionArguments[i].Model.data(), fireMinionArguments[i].Texture.data(), fireMinionAnimations, { 500.f, 0.1f, -1150.f + i * 5 }, fireMinionArguments[i].scale, fireMinionArguments[i].rotation));
				woodMinions.push_back(GameUtilities::AddDestructableFromModelFile(woodMinionArguments[i].Model.data(), woodMinionArguments[i].Texture.data(), woodMinionAnimations, { 500.f, 0.1f, -1150.f + i * 5 }, woodMinionArguments[i].scale, woodMinionArguments[i].rotation));
			}

			//for (int i = 0; i < renderableArguments.size(); i++)
			//{
			//	renderables.push_back(GameUtilities::AddDestructableFromModelFile(renderableArguments[i].Model.data(), renderableArguments[i].Texture.data(), woodMinionAnimations, renderableArguments[i].position, renderableArguments[i].scale, renderableArguments[i].rotation));
			//	renderables[i]->physicsType = renderableArguments[i].type;
			//}

			for (int i = 0; i < stoneMinionCount; i++)
			{
				GameUtilities::AddGameObject(stoneMinions[i]);
				GameUtilities::AddGameObject(waterMinions[i]);
				GameUtilities::AddGameObject(fireMinions[i]);
				GameUtilities::AddGameObject(woodMinions[i]);
			}

			//for (int i = 0; i < renderables.size(); i++)
			//{
			//	GameUtilities::AddGameObject(renderables[i]);
			//}


			//Dan added this:
			m1 = GameUtilities::AddTriggerFromModelFile("files/models/Target.mesh", "files/textures/Target.mat", XMFLOAT3(-145, 10, -145), XMFLOAT3(3, 3, 3), XMFLOAT3(0, 0, 0));

			//m1 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(-145, 5, -145), XMFLOAT3(0.02f, 1.02f, 0.02f), XMFLOAT3(0, 0, 0));
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
				stoneMinionCount--;
				stoneMinionCountstr->clear();
				stoneMinionCountstr->append(to_string(stoneMinionCount));
				ADEvents::ADEventSystem::Instance()->SendEvent("StoneMinionCountChanged", static_cast<void*>(stoneMinionCountstr));
				break;
			}
			case ADResource::ADGameplay::OBJECT_TAG::WATER_MINION:
			{
				waterMinionCount--;
				waterMinionCountstr->clear();
				waterMinionCountstr->append(to_string(waterMinionCount));
				ADEvents::ADEventSystem::Instance()->SendEvent("WaterMinionCountChanged", static_cast<void*>(waterMinionCountstr));
				break;
			}
			case ADResource::ADGameplay::OBJECT_TAG::FIRE_MINION:
			{
				fireMinionCount--;
				fireMinionCountstr->clear();
				fireMinionCountstr->append(to_string(fireMinionCount));
				ADEvents::ADEventSystem::Instance()->SendEvent("FireMinionCountChanged", static_cast<void*>(fireMinionCountstr));
				break;
			}
			case ADResource::ADGameplay::OBJECT_TAG::WOOD_MINION:
			{
				woodMinionCount--;
				woodMinionCountstr->clear();
				woodMinionCountstr->append(to_string(woodMinionCount));
				ADEvents::ADEventSystem::Instance()->SendEvent("WoodMinionCountChanged", static_cast<void*>(woodMinionCountstr));
				break;
			}
			}
			totalMinionCount--;
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
				minionAvoids.push_back(_building->GetRubble());
			}
		}
	};
}

