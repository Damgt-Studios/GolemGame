#pragma once

#include <vector>
#include "ResourceManager.h"
#include "../Game/GameUtilities.h"
#include "../Game/GameObjectClasses.h"
#include "MeshLoader.h"
#include <iostream>
#include <fstream>
#include "ADCombat.h"
#include <ADEventSystem.h>
#include <ADUserInterface.h>


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

		std::vector<Destructable*> villagers;
		std::vector<ADAI::TowerAI*> towers;
		std::vector<Building*> buildings;

		//Minions
		std::vector<Destructable*> stoneMinions;
		std::vector<Destructable*> waterMinions;
		std::vector<Destructable*> fireMinions;
		std::vector<Destructable*> woodMinions;

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
		UINT stoneMinionCount = 10;
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









			Building* house1 = new Building(XMFLOAT3(-500, 0, 100), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 30, 30), XMFLOAT3(0, 0.5f, 0.15), GameUtilities::GenerateHouse1, "House1");
			house1->name = "house1";
			GameUtilities::AddGameObject(house1);
			AddBuilding(house1);
			//house1->destructionEmitter = &engine->destructionCloud;
			//house1->destructionEmitter2 = &engine->destructionCloud2;

			Building* rubble1 = new Building(XMFLOAT3(-500, 0, 100), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 30, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble1, "Rubble");
			GameUtilities::AddGameObject(rubble1);
			house1->SetRubble(rubble1);


			for (int i = 0; i < 10; i++)
			{
				Building* housey = new Building(XMFLOAT3(-200 + (i * 33), 0, -100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateHouse1, "House1");
				housey->name = "housex";
				GameUtilities::AddGameObject(housey);
				AddBuilding(housey);
				//housey->destructionEmitter = &engine->destructionCloud;
				//housey->destructionEmitter2 = &engine->destructionCloud2;

				Building* rubble2 = new Building(XMFLOAT3(-200 + (i * 33), 0, -100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble2, "Rubble");
				GameUtilities::AddGameObject(rubble2);
				housey->SetRubble(rubble2);
			}

			Building* attackTower = new Building(XMFLOAT3(10, 0, 200), XMFLOAT3(0, 0, 0), XMFLOAT3(15, 25, 15), XMFLOAT3(0, 0, 0), GameUtilities::GenerateAttackTower, "BallistaTower");
			GameUtilities::AddGameObject(attackTower);

			Building* ballista = new Building(XMFLOAT3(10, 20, 200), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateBallista, "BallistaTower");
			GameUtilities::AddGameObject(ballista);


			Building* gateway = new Building(XMFLOAT3(330, 0, -330), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway, "Gate");
			GameUtilities::AddGameObject(gateway);

			Building* scaffolding = new Building(XMFLOAT3(410, 0, -400), XMFLOAT3(0, 90, 0), XMFLOAT3(5, 25, 25), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateScaffoldWallX, "Scaffolding");
			GameUtilities::AddGameObject(scaffolding);

			Building* wall = new Building(XMFLOAT3(700, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall, "Wall");
			GameUtilities::AddGameObject(wall);

			Building* well = new Building(XMFLOAT3(1500, 0, 1500), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 5, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell, "Rubble");
			GameUtilities::AddGameObject(well);

			Building* tavern = new Building(XMFLOAT3(-100, 0, 100), XMFLOAT3(0, 45, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
			GameUtilities::AddGameObject(tavern);
			//AddBuilding(tavern);

			Building* lamp = new Building(XMFLOAT3(62.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 1.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Rubble");
			GameUtilities::AddGameObject(lamp);
			Building* lamp2 = new Building(XMFLOAT3(-125, 0, 187.5), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 1.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Rubble");
			GameUtilities::AddGameObject(lamp2);

			Building* rock = new Building(XMFLOAT3(-20, 0, 220), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRock1, "Rubble");
			GameUtilities::AddGameObject(rock);

			Building* meeting_hall = new Building(XMFLOAT3(100, 0, 360), XMFLOAT3(0, -135, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "House4");
			GameUtilities::AddGameObject(meeting_hall);
			//AddBuilding(meeting_hall);

			Building* cart = new Building(XMFLOAT3(-125, 0, 310), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5, 0.25, 0.1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateCart, "Cart");
			GameUtilities::AddGameObject(cart);

			for (int i = 0; i < 10; ++i)
			{
				Building* realtree = new Building(XMFLOAT3(RandFloat(i - 1000, i + 1000), 0, RandFloat(i - 1000, i + 1000)), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 3, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateTree, "Rubble");
				GameUtilities::AddGameObject(realtree);
			}


			//villageFlock1.groupTarget = &golem->transform;
			villageFlock1.groupCohesionStrength = 0.01f;
			villageFlock1.groupSeparationStrength = 0.6f;


			std::vector<std::string> bucketheadAnimations;
			bucketheadAnimations.push_back("files/models/Bucket_Idle.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Run.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Fear.animfile");
			bucketheadAnimations.push_back("files/models/Bucket_Death.animfile");

			villagerThreats.push_back(golem);

			for (int i = 0; i < 10; i++)
			{
				villagerThreats.push_back(stoneMinions[i]);
				villagerThreats.push_back(waterMinions[i]);
				villagerThreats.push_back(fireMinions[i]);
				villagerThreats.push_back(woodMinions[i]);
			}
			towers.push_back(GameUtilities::AttachTowerAI(ballista, &villagerThreats));

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

			for (int i = 0; i < 10; i++)
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


			//for (int i = 0; i < 10; i++)
			//{
			//	villagers.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadAnimations, XMFLOAT3((i - 5) * 10, 0, -245), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0)));
			//	villagerAI.push_back(GameUtilities::AttachVillagerAI(villagers[i], &villageFlock1));
			//	villagerAI[i]->states[0]->objectsToAvoid.push_back(golem);
			//	villagerAI[i]->states[1]->objectsToAvoid.push_back(golem);
			//	for (int j = 0; j < 10; ++j)
			//	{
			//		villagerAI[i]->states[0]->objectsToAvoid.push_back(stoneMinions[j]);
			//		villagerAI[i]->states[0]->objectsToAvoid.push_back(waterMinions[j]);
			//		villagerAI[i]->states[0]->objectsToAvoid.push_back(fireMinions[j]);
			//		villagerAI[i]->states[0]->objectsToAvoid.push_back(woodMinions[j]);
			//		villagerAI[i]->states[1]->objectsToAvoid.push_back(stoneMinions[j]);
			//		villagerAI[i]->states[1]->objectsToAvoid.push_back(waterMinions[j]);
			//		villagerAI[i]->states[1]->objectsToAvoid.push_back(fireMinions[j]);
			//		villagerAI[i]->states[1]->objectsToAvoid.push_back(woodMinions[j]);
			//	}
			//	GameUtilities::AddGameObject(villagers[i]);
			//}


			//for (int i = 0; i < 10; i++)
			//{
			//	stoneMinionsAI[i]->states[1]->objectsToAvoid.push_back(villagers[i]);
			//	waterMinionsAI[i]->states[1]->objectsToAvoid.push_back(villagers[i]);
			//	fireMinionsAI[i]->states[1]->objectsToAvoid.push_back(villagers[i]);
			//	woodMinionsAI[i]->states[1]->objectsToAvoid.push_back(villagers[i]);
			//}
		//}



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

			for (int i = 0; i < 10; i++)
			{
				stoneMinionArguments[i].position.x += i * 5;
				waterMinionArguments[i].position.z += i * 5;
				fireMinionArguments[i].position.x += i * 5;
				woodMinionArguments[i].position.x += i * 5;
				stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile(stoneMinionArguments[i].Model.data(), stoneMinionArguments[i].Texture.data(), stoneMinionAnimations, stoneMinionArguments[i].position, stoneMinionArguments[i].scale, stoneMinionArguments[i].rotation));
				waterMinions.push_back(GameUtilities::AddDestructableFromModelFile(waterMinionArguments[i].Model.data(), waterMinionArguments[i].Texture.data(), waterMinionAnimations, waterMinionArguments[i].position, waterMinionArguments[i].scale, waterMinionArguments[i].rotation));
				fireMinions.push_back(GameUtilities::AddDestructableFromModelFile(fireMinionArguments[i].Model.data(), fireMinionArguments[i].Texture.data(), fireMinionAnimations, fireMinionArguments[i].position, fireMinionArguments[i].scale, fireMinionArguments[i].rotation));
				woodMinions.push_back(GameUtilities::AddDestructableFromModelFile(woodMinionArguments[i].Model.data(), woodMinionArguments[i].Texture.data(), woodMinionAnimations, woodMinionArguments[i].position, woodMinionArguments[i].scale, woodMinionArguments[i].rotation));
				/*
				stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile(stoneMinionArguments[i].Model.data(), stoneMinionArguments[i].Texture.data(), stoneMinionAnimations, stoneMinionArguments[i].position, stoneMinionArguments[i].scale, stoneMinionArguments[i].rotation));
				waterMinions.push_back(GameUtilities::AddDestructableFromModelFile(waterMinionArguments[i].Model.data(), waterMinionArguments[i].Texture.data(), waterMinionAnimations, waterMinionArguments[i].position, waterMinionArguments[i].scale, waterMinionArguments[i].rotation));
				fireMinions.push_back(GameUtilities::AddDestructableFromModelFile(fireMinionArguments[i].Model.data(), fireMinionArguments[i].Texture.data(), fireMinionAnimations, fireMinionArguments[i].position, fireMinionArguments[i].scale, fireMinionArguments[i].rotation));
				woodMinions.push_back(GameUtilities::AddDestructableFromModelFile(woodMinionArguments[i].Model.data(), woodMinionArguments[i].Texture.data(), woodMinionAnimations, woodMinionArguments[i].position, woodMinionArguments[i].scale, woodMinionArguments[i].rotation));*/

			}

			//for (int i = 0; i < renderableArguments.size(); i++)
			//{
			//	renderables.push_back(GameUtilities::AddDestructableFromModelFile(renderableArguments[i].Model.data(), renderableArguments[i].Texture.data(), woodMinionAnimations, renderableArguments[i].position, renderableArguments[i].scale, renderableArguments[i].rotation));
			//	renderables[i]->physicsType = renderableArguments[i].type;
			//}

			for (int i = 0; i < 10; i++)
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
			for (auto& stoneMinion: stoneMinionsAI)
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

