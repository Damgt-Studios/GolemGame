#pragma once
#include "GameUtilities.h"
#include "GameObjectClasses.h"

#include "ResourceManager.h"
#include "MeshLoader.h"
#include "ADCombat.h"
#include "ADEventSystem.h"
#include "ADUserInterface.h"

#include <vector>
#include <iostream>
#include <fstream>



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
		UINT stoneMinionCount = 1;
		UINT waterMinionCount = 10;
		UINT fireMinionCount = 10;
		UINT woodMinionCount = 10;
		UINT totalMinionCount = 40;
		UINT villagerCount = 10;

		//String stuff for the events.
		std::string* stoneMinionCountstr;
		std::string* waterMinionCountstr;
		std::string* fireMinionCountstr;
		std::string* woodMinionCountstr;
		std::string* allMinionCountstr;

		std::string* villagerCountstr;
		ADAI::VillagerGroup villageFlock1;

		void InitializeScene()
		{
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

			//villageFlock1.groupTarget = &golem->transform;
			villageFlock1.groupCohesionStrength = 0.01f;
			villageFlock1.groupSeparationStrength = 0.6f;

			std::vector<std::string> bucketheadAnimations;
			bucketheadAnimations.push_back("files/models/Bucket_Idle.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Run.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Fear.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Death.animfile");

			villagerThreats.push_back(golem);

			for (int i = 0; i < stoneMinionCount; i++)
			{
				villagerThreats.push_back(stoneMinions[i]);
				villagerThreats.push_back(waterMinions[i]);
				villagerThreats.push_back(fireMinions[i]);
				villagerThreats.push_back(woodMinions[i]);
			}


#pragma region Level

			//=========================================================Tutorial Stage
			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(435.5 + (i * 40.5), 0, -900), XMFLOAT3(0, 0, 0), XMFLOAT3(35, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(435.5 + (i * 40.5), 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(35, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(435.5 + (i * 40.5), 0, -300), XMFLOAT3(0, 0, 0), XMFLOAT3(35, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(435.5 + (i * 40.5), 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(35, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			//===================================================Castle

			Building* gate1 = new Building(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway, "Gate");
			GameUtilities::AddGameObject(gate1);

			Building* gate2 = new Building(XMFLOAT3(-474, 0, -595.5), XMFLOAT3(0, 90, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway, "Gate");
			GameUtilities::AddGameObject(gate2);

			Building* wall1 = new Building(XMFLOAT3(40.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall1);

			Building* wall2 = new Building(XMFLOAT3(81, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall2);

			Building* wall3 = new Building(XMFLOAT3(121.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall3);

			Building* wall4 = new Building(XMFLOAT3(162, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall4);

			Building* wall5 = new Building(XMFLOAT3(202.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall5);

			Building* wall6 = new Building(XMFLOAT3(243, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall6);

			for (unsigned int i = 0; i < 20; i++)
			{
				Building* wall = new Building(XMFLOAT3(271.5, 0, -28.5f - (i * 40.5f)), XMFLOAT3(0, 90, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 18; i++)
			{
				Building* wall = new Building(XMFLOAT3(243 - (i * 40.5), 0, -826.5), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(-474, 0, -798 + (i * 40.5)), XMFLOAT3(0, 90, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(-474, 0, -555 + (i * 40.5)), XMFLOAT3(0, 90, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(-445.5 + (i * 40.5), 0, -364.5), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 5; i++)
			{
				Building* wall = new Building(XMFLOAT3(-255, 0, -336 + (i * 40.5)), XMFLOAT3(0, 90, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 15; i++)
			{
				Building* wall = new Building(XMFLOAT3(-40.5 - (i * 40.5f), 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}

			for (unsigned int i = 0; i < 9; i++)
			{
				Building* wall = new Building(XMFLOAT3(-283.5 - (i * 40.5), 0, -145.5), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateWall, "Wall");
				GameUtilities::AddGameObject(wall);
			}


			Building* tower1 = new Building(XMFLOAT3(271.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower1);

			Building* tower2 = new Building(XMFLOAT3(271.5, 0, -826.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower2);

			Building* tower3 = new Building(XMFLOAT3(-474, 0, -826.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower3);

			Building* tower4 = new Building(XMFLOAT3(-474, 0, -364.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower4);

			Building* tower5 = new Building(XMFLOAT3(-255, 0, -364.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower5);

			Building* tower6 = new Building(XMFLOAT3(-255, 0, -145.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.5, 0), GameUtilities::GenerateTower, "Tower");
			GameUtilities::AddGameObject(tower6);

			//========================================================Houses

			Building* tutorial_home1 = new Building(XMFLOAT3(570, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home1);

			Building* tutorial_home2 = new Building(XMFLOAT3(570, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home2);

			Building* tutorial_home3 = new Building(XMFLOAT3(460, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home3);

			Building* tutorial_home4 = new Building(XMFLOAT3(460, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home4);

			Building* tutorial_home5 = new Building(XMFLOAT3(570, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home5);

			Building* tutorial_home6 = new Building(XMFLOAT3(570, 0, -850), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home6);

			Building* tutorial_home7 = new Building(XMFLOAT3(460, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home7);

			Building* tutorial_home8 = new Building(XMFLOAT3(460, 0, -850), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home8);

			Building* tutorial_home9 = new Building(XMFLOAT3(570, 0, -100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home9);

			Building* tutorial_home10 = new Building(XMFLOAT3(570, 0, -200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home10);

			Building* tutorial_home11 = new Building(XMFLOAT3(460, 0, -100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home11);

			Building* tutorial_home12 = new Building(XMFLOAT3(460, 0, -200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(tutorial_home12);

			Building* home1 = new Building(XMFLOAT3(575, 0, 200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home1);

			Building* home2 = new Building(XMFLOAT3(525, 0, 250), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home2);

			Building* home3 = new Building(XMFLOAT3(475, 0, 300), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home3);

			Building* home4 = new Building(XMFLOAT3(475, 0, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home4);

			Building* home5 = new Building(XMFLOAT3(475, 0, 400), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home5);

			Building* home6 = new Building(XMFLOAT3(525, 0, 450), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home6);

			Building* home7 = new Building(XMFLOAT3(575, 0, 500), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home7);

			Building* home8 = new Building(XMFLOAT3(200, 0, 575), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home8);

			Building* home9 = new Building(XMFLOAT3(250, 0, 525), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home9);

			Building* home10 = new Building(XMFLOAT3(300, 0, 475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home10);

			Building* home11 = new Building(XMFLOAT3(350, 0, 475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home11);

			Building* home12 = new Building(XMFLOAT3(400, 0, 475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home12);

			Building* home13 = new Building(XMFLOAT3(450, 0, 525), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home13);

			Building* home14 = new Building(XMFLOAT3(500, 0, 575), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home14);

			Building* home15 = new Building(XMFLOAT3(440, 0, 80), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home15);

			Building* home16 = new Building(XMFLOAT3(400, 0, 120), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home16);

			Building* home17 = new Building(XMFLOAT3(360, 0, 160), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home17);

			Building* home18 = new Building(XMFLOAT3(320, 0, 200), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home18);

			Building* home19 = new Building(XMFLOAT3(280, 0, 240), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home19);

			Building* home20 = new Building(XMFLOAT3(240, 0, 280), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home20);

			Building* home21 = new Building(XMFLOAT3(180, 0, 275), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home21);

			Building* home22 = new Building(XMFLOAT3(120, 0, 270), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home22);

			Building* home23 = new Building(XMFLOAT3(60, 0, 265), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home23);

			Building* home24 = new Building(XMFLOAT3(0, 0, 260), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home24);

			Building* home25 = new Building(XMFLOAT3(-60, 0, 255), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home25);

			Building* home26 = new Building(XMFLOAT3(-120, 0, 250), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home26);

			Building* home27 = new Building(XMFLOAT3(180, 0, 500), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home27);

			Building* home28 = new Building(XMFLOAT3(120, 0, 495), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home28);

			Building* home29 = new Building(XMFLOAT3(60, 0, 490), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home29);

			Building* home30 = new Building(XMFLOAT3(0, 0, 485), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home30);

			Building* home31 = new Building(XMFLOAT3(-60, 0, 480), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home31);

			Building* home32 = new Building(XMFLOAT3(-120, 0, 475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home32);

			Building* home33 = new Building(XMFLOAT3(-180, 0, 465), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home33);

			Building* home34 = new Building(XMFLOAT3(-220, 0, 435), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home34);

			Building* home35 = new Building(XMFLOAT3(-250, 0, 395), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home35);

			Building* home36 = new Building(XMFLOAT3(-270, 0, 345), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home36);

			Building* home37 = new Building(XMFLOAT3(-270, 0, 165), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home37);

			Building* home38 = new Building(XMFLOAT3(-250, 0, 115), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home38);

			Building* home39 = new Building(XMFLOAT3(-220, 0, 75), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home39);

			Building* home40 = new Building(XMFLOAT3(-180, 0, 45), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home40);

			Building* home41 = new Building(XMFLOAT3(300, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home41);

			Building* home42 = new Building(XMFLOAT3(250, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home42);

			Building* home43 = new Building(XMFLOAT3(200, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home43);

			Building* home44 = new Building(XMFLOAT3(150, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home44);

			Building* home45 = new Building(XMFLOAT3(100, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home45);

			Building* home46 = new Building(XMFLOAT3(0, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home46);

			Building* home47 = new Building(XMFLOAT3(-50, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home47);

			Building* home48 = new Building(XMFLOAT3(-100, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home48);

			Building* home49 = new Building(XMFLOAT3(-150, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home49);

			Building* home50 = new Building(XMFLOAT3(-200, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home50);

			Building* home51 = new Building(XMFLOAT3(-300, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home51);

			Building* home52 = new Building(XMFLOAT3(-350, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home52);

			Building* home53 = new Building(XMFLOAT3(-400, 0, -1100), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home53);

			Building* home54 = new Building(XMFLOAT3(-450, 0, -1050), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home54);

			Building* home55 = new Building(XMFLOAT3(-500, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home55);

			Building* home56 = new Building(XMFLOAT3(-200, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home56);

			Building* home57 = new Building(XMFLOAT3(-125, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home57);

			Building* home58 = new Building(XMFLOAT3(-50, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home58);

			Building* home59 = new Building(XMFLOAT3(50, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home59);

			Building* home60 = new Building(XMFLOAT3(125, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home60);

			Building* home61 = new Building(XMFLOAT3(200, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home61);

			Building* home62 = new Building(XMFLOAT3(-275, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home62);

			Building* home63 = new Building(XMFLOAT3(-350, 0, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home63);

			Building* home64 = new Building(XMFLOAT3(-275, 0, -125), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home64);

			Building* home65 = new Building(XMFLOAT3(-350, 0, -125), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home65);

			Building* home66 = new Building(XMFLOAT3(225, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home66);

			Building* home67 = new Building(XMFLOAT3(225, 0, -150), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home67);

			Building* home68 = new Building(XMFLOAT3(225, 0, -225), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home68);

			Building* home69 = new Building(XMFLOAT3(225, 0, -300), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home69);

			Building* home70 = new Building(XMFLOAT3(225, 0, -375), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home70);

			Building* home71 = new Building(XMFLOAT3(225, 0, -450), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home71);

			Building* home72 = new Building(XMFLOAT3(225, 0, -525), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home72);

			Building* home73 = new Building(XMFLOAT3(225, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home73);

			Building* home74 = new Building(XMFLOAT3(225, 0, -675), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home74);

			Building* home75 = new Building(XMFLOAT3(225, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home75);

			Building* home76 = new Building(XMFLOAT3(-225, 0, -150), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home76);

			Building* home77 = new Building(XMFLOAT3(-187.5, 0, -187.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home77);

			Building* home78 = new Building(XMFLOAT3(-150, 0, -225), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home78);

			Building* home79 = new Building(XMFLOAT3(-112.5, 0, -262.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home79);

			Building* home80 = new Building(XMFLOAT3(-75, 0, -300), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home80);

			Building* home81 = new Building(XMFLOAT3(-225, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home81);

			Building* home82 = new Building(XMFLOAT3(-187.5, 0, -387.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home82);

			Building* home83 = new Building(XMFLOAT3(-150, 0, -425), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home83);

			Building* home84 = new Building(XMFLOAT3(-112.5, 0, -462.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home84);

			Building* home85 = new Building(XMFLOAT3(-75, 0, -500), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home85);

			Building* home86 = new Building(XMFLOAT3(150, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home86);

			Building* home87 = new Building(XMFLOAT3(125, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home87);

			Building* home88 = new Building(XMFLOAT3(100, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home88);

			Building* home89 = new Building(XMFLOAT3(75, 0, -700), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home89);

			Building* home90 = new Building(XMFLOAT3(50, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home90);

			Building* home91 = new Building(XMFLOAT3(-25, 0, -375), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home91);

			Building* home92 = new Building(XMFLOAT3(12.5, 0, -337.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home92);

			Building* home93 = new Building(XMFLOAT3(50, 0, -300), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home93);

			Building* home94 = new Building(XMFLOAT3(87.5, 0, -337.5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home94);

			Building* home95 = new Building(XMFLOAT3(125, 0, -375), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home95);

			Building* home96 = new Building(XMFLOAT3(-200, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home96);

			Building* home97 = new Building(XMFLOAT3(-350, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home97);

			Building* home98 = new Building(XMFLOAT3(-275, 0, -525), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home98);

			Building* home99 = new Building(XMFLOAT3(-275, 0, -675), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home99);

			Building* home100 = new Building(XMFLOAT3(-225, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home100);

			Building* home101 = new Building(XMFLOAT3(-325, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home101);

			Building* home102 = new Building(XMFLOAT3(-225, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home102);

			Building* home103 = new Building(XMFLOAT3(-325, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home103);

			Building* home104 = new Building(XMFLOAT3(0, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home104);

			Building* home105 = new Building(XMFLOAT3(-75, 0, -675), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home105);

			Building* home106 = new Building(XMFLOAT3(-150, 0, -700), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home106);

			Building* home107 = new Building(XMFLOAT3(-225, 0, -725), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home107);

			Building* home108 = new Building(XMFLOAT3(-300, 0, -750), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home108);

			Building* home109 = new Building(XMFLOAT3(-375, 0, -775), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home109);

			Building* home110 = new Building(XMFLOAT3(-275, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home110);

			Building* home111 = new Building(XMFLOAT3(-325, 0, -425), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home111);

			Building* home112 = new Building(XMFLOAT3(-375, 0, -475), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home112);

			Building* home113 = new Building(XMFLOAT3(-425, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 35, 25), XMFLOAT3(0, 0.5, 5), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(home113);

			//========================================Large Buildings

			Building* tavern1 = new Building(XMFLOAT3(-400, 0, 250), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern1);

			Building* tavern2 = new Building(XMFLOAT3(-400, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern2);

			Building* tavern3 = new Building(XMFLOAT3(200, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern3);

			Building* tavern4 = new Building(XMFLOAT3(200, 0, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern4);

			Building* meeting_hall1 = new Building(XMFLOAT3(-100, 0, -1000), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			GameUtilities::AddGameObject(meeting_hall1);

			Building* meeting_hall2 = new Building(XMFLOAT3(50, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			GameUtilities::AddGameObject(meeting_hall2);

			Building* meeting_hall3 = new Building(XMFLOAT3(-275, 0, -600), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			GameUtilities::AddGameObject(meeting_hall3);

			Building* meeting_hall4 = new Building(XMFLOAT3(-475, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "Hall");
			GameUtilities::AddGameObject(meeting_hall4);

			//===================================================Ballista Towers

			Building* attackTower1 = new Building(XMFLOAT3(450, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower1);

			Building* ballista1 = new Building(XMFLOAT3(450, 20, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista1);

			Building* attackTower2 = new Building(XMFLOAT3(580, 0, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower2);

			Building* ballista2 = new Building(XMFLOAT3(580, 20, -350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista2);

			Building* attackTower3 = new Building(XMFLOAT3(515, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower3);

			Building* ballista3 = new Building(XMFLOAT3(515, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista3);

			Building* attackTower4 = new Building(XMFLOAT3(550, 0, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower4);

			Building* ballista4 = new Building(XMFLOAT3(550, 20, 350), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista4);

			Building* attackTower5 = new Building(XMFLOAT3(350, 0, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower5);

			Building* ballista5 = new Building(XMFLOAT3(350, 20, 550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista5);

			Building* attackTower6 = new Building(XMFLOAT3(250, 0, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower6);

			Building* ballista6 = new Building(XMFLOAT3(250, 20, 375), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista6);

			Building* attackTower7 = new Building(XMFLOAT3(-25, 0, 450), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower7);

			Building* ballista7 = new Building(XMFLOAT3(-25, 20, 450), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista7);

			Building* attackTower8 = new Building(XMFLOAT3(-25, 0, 300), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower8);

			Building* ballista8 = new Building(XMFLOAT3(-25, 20, 300), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista8);

			Building* attackTower9 = new Building(XMFLOAT3(-275, 0, 212.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower9);

			Building* ballista9 = new Building(XMFLOAT3(-275, 20, 212.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista9);

			Building* attackTower10 = new Building(XMFLOAT3(-275, 0, 287.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower10);

			Building* ballista10 = new Building(XMFLOAT3(-275, 20, 287.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista10);

			Building* attackTower11 = new Building(XMFLOAT3(-25, 0, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower11);

			Building* ballista11 = new Building(XMFLOAT3(-25, 20, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista11);

			Building* attackTower12 = new Building(XMFLOAT3(25, 0, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower12);

			Building* ballista12 = new Building(XMFLOAT3(25, 20, 25), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista12);

			Building* attackTower13 = new Building(XMFLOAT3(-450, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower13);

			Building* ballista13 = new Building(XMFLOAT3(-450, 20, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista13);

			Building* attackTower14 = new Building(XMFLOAT3(-50, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower14);

			Building* ballista14 = new Building(XMFLOAT3(-50, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista14);

			Building* attackTower15 = new Building(XMFLOAT3(150, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower15);

			Building* ballista15 = new Building(XMFLOAT3(150, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista15);

			Building* attackTower16 = new Building(XMFLOAT3(-449, 0, -570.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower16);

			Building* ballista16 = new Building(XMFLOAT3(-449, 20, -570.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista16);

			Building* attackTower17 = new Building(XMFLOAT3(-449, 0, -620.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower17);

			Building* ballista17 = new Building(XMFLOAT3(-449, 20, -620.5), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista17);

			Building* attackTower18 = new Building(XMFLOAT3(-212.5, 0, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower18);

			Building* ballista18 = new Building(XMFLOAT3(-212.5, 20, -400), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista18);

			Building* attackTower19 = new Building(XMFLOAT3(-175, 0, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower19);

			Building* ballista19 = new Building(XMFLOAT3(-175, 20, -550), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista19);

			Building* attackTower20 = new Building(XMFLOAT3(-175, 0, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower20);

			Building* ballista20 = new Building(XMFLOAT3(-175, 20, -650), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista20);

			Building* attackTower21 = new Building(XMFLOAT3(150, 0, -775), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower21);

			Building* ballista21 = new Building(XMFLOAT3(150, 20, -775), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista21);

			Building* attackTower22 = new Building(XMFLOAT3(-350, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower22);

			Building* ballista22 = new Building(XMFLOAT3(-350, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista22);

			Building* attackTower23 = new Building(XMFLOAT3(-450, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower23);

			Building* ballista23 = new Building(XMFLOAT3(-450, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista23);

			Building* attackTower24 = new Building(XMFLOAT3(-50, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower24);

			Building* ballista24 = new Building(XMFLOAT3(-50, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista24);

			Building* attackTower25 = new Building(XMFLOAT3(-150, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower25);

			Building* ballista25 = new Building(XMFLOAT3(-150, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista25);

			Building* attackTower26 = new Building(XMFLOAT3(150, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower26);

			Building* ballista26 = new Building(XMFLOAT3(150, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista26);

			Building* attackTower27 = new Building(XMFLOAT3(250, 0, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower27);

			Building* ballista27 = new Building(XMFLOAT3(250, 20, -925), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista27);

			Building* attackTower28 = new Building(XMFLOAT3(-450, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower28);

			Building* ballista28 = new Building(XMFLOAT3(-450, 20, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista28);

			Building* attackTower29 = new Building(XMFLOAT3(-50, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower29);

			Building* ballista29 = new Building(XMFLOAT3(-50, 20, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista29);

			Building* attackTower30 = new Building(XMFLOAT3(50, 0, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, 5, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower30);

			Building* ballista30 = new Building(XMFLOAT3(50, 20, -75), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 25, 12.5), XMFLOAT3(0, -5, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista30);

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
				villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock1, &villagerThreats, &buildings));
			}
			for (int i = 0; i < 10; i++)
			{
				minionTargets.push_back(villagers[i]);
				GameUtilities::AddGameObject(villagers[i]);
			}

			for (auto& building : buildings)
			{
				minionTargets.push_back(building);
			}

			for (int i = 0; i < stoneMinionCount; i++)
			{
				stoneMinionsAI.push_back(GameUtilities::AttachMinionAI(stoneMinions[i], golem->minionGroups[STONE], &minionTargets, STONE_MINION));
				//stoneMinionsAI[i]->currentTarget = golem->targetMarker;
				waterMinionsAI.push_back(GameUtilities::AttachMinionAI(waterMinions[i], golem->minionGroups[WATER], &minionTargets, WATER_MINION));
				//waterMinionsAI[i]->currentTarget = golem->targetMarker;
				fireMinionsAI.push_back(GameUtilities::AttachMinionAI(fireMinions[i], golem->minionGroups[FIRE], &minionTargets, FIRE_MINION));
				//fireMinionsAI[i]->currentTarget = golem->targetMarker;
				woodMinionsAI.push_back(GameUtilities::AttachMinionAI(woodMinions[i], golem->minionGroups[WOOD], &minionTargets, WOOD_MINION));
				//woodMinionsAI[i]->currentTarget = golem->targetMarker;

				//stoneMinionsAI[i]->mySSM.states[1]->objectsToAvoid.push_back(villagers[i]);
				//waterMinionsAI[i]->mySSM.states[1]->objectsToAvoid.push_back(villagers[i]);
				//fireMinionsAI[i]->mySSM.states[1]->objectsToAvoid.push_back(villagers[i]);
				//woodMinionsAI[i]->mySSM.states[1]->objectsToAvoid.push_back(villagers[i]);
			}
		}



	public:
		void LoadScene(const char* filename)
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

			golem = GameUtilities::LoadGolemFromModelFile(GolemArguments.position, GolemArguments.scale, GolemArguments.rotation);

			for (int i = 0; i < stoneMinionCount; i++)
			{
				stoneMinionArguments[i].position.x += i * 5;
				waterMinionArguments[i].position.z += i * 5;
				fireMinionArguments[i].position.x += i * 5;
				woodMinionArguments[i].position.x += i * 5;
				stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile(stoneMinionArguments[i].Model.data(), stoneMinionArguments[i].Texture.data(), stoneMinionAnimations, stoneMinionArguments[i].position, stoneMinionArguments[i].scale, stoneMinionArguments[i].rotation));
				waterMinions.push_back(GameUtilities::AddDestructableFromModelFile(waterMinionArguments[i].Model.data(), waterMinionArguments[i].Texture.data(), waterMinionAnimations, waterMinionArguments[i].position, waterMinionArguments[i].scale, waterMinionArguments[i].rotation));
				fireMinions.push_back(GameUtilities::AddDestructableFromModelFile(fireMinionArguments[i].Model.data(), fireMinionArguments[i].Texture.data(), fireMinionAnimations, fireMinionArguments[i].position, fireMinionArguments[i].scale, fireMinionArguments[i].rotation));
				woodMinions.push_back(GameUtilities::AddDestructableFromModelFile(woodMinionArguments[i].Model.data(), woodMinionArguments[i].Texture.data(), woodMinionAnimations, woodMinionArguments[i].position, woodMinionArguments[i].scale, woodMinionArguments[i].rotation));
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
			m1 = GameUtilities::AddTriggerFromModelFile("files/models/Target.mesh", "files/textures/Target.mat", XMFLOAT3(-145, 5, -145), XMFLOAT3(3, 3, 3), XMFLOAT3(0, 0, 0));

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

			InitializeScene();
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
		}
	};
}

