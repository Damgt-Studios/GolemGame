#pragma once

#include <vector>
#include "ResourceManager.h"
#include "../Game/GameUtilities.h"
#include "../Game/GameObjectClasses.h"
#include "MeshLoader.h"
#include <iostream>
#include <fstream>


namespace ADGameplay
{
	class Scene
	{
	private:
		//std::vector<ADResource::ADGameplay::Enemy> Enemies;

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

		//Minions
		std::vector<Destructable*> stoneMinions;
		std::vector<Destructable*> waterMinions;
		std::vector<Destructable*> fireMinions;
		std::vector<Destructable*> woodMinions;

		//Minion AI
		std::vector<ADAI::AIUnit*> stoneMinionsAI;
		std::vector<ADAI::AIUnit*> waterMinionsAI;
		std::vector<ADAI::AIUnit*> fireMinionsAI;
		std::vector<ADAI::AIUnit*> woodMinionsAI;

		//Animation vecters
		std::vector<std::string> animations;
		std::vector<std::string> stoneMinionAnimations;
		std::vector<std::string> waterMinionAnimations;
		std::vector<std::string> fireMinionAnimations;
		std::vector<std::string> woodMinionAnimations;

		

		void InitializeScene()
		{
			Light light;
			ZeroMemory(&light, sizeof(Light));
			light.lightType = (int)LIGHTTYPE::DIRECTIONAL;
			light.diffuse =
				light.ambientUp =
				light.ambientDown =
				light.specular =
				XMFLOAT4(1, 1, 1, 1);
			light.ambientIntensityDown = .1;
			light.ambientIntensityUp = .1;
			light.lightDirection = XMFLOAT4(0, -1, 0, 1);
			light.diffuseIntensity = 1;
			light.specularIntensity = .2;
			light.diffuse =
				light.ambientUp =
				light.ambientDown =
				light.specular =
				XMFLOAT4(1, 1, 1, 1);
			

			// Point light
			Light light1;
			ZeroMemory(&light1, sizeof(Light));
			light1.ambientIntensityDown = .1;
			light1.ambientIntensityUp = .1;
			light1.lightDirection = XMFLOAT4(0, 0, 10, 1);
			light1.diffuseIntensity = .5;
			light1.specularIntensity = .2;
			light1.diffuse =
				light1.ambientUp =
				light1.ambientDown =
				light1.specular =
				XMFLOAT4(1, 1, 1, 1);
			light1.lightType = (int)LIGHTTYPE::POINT;
			light1.position = XMFLOAT4(10, 0, 0, 1);
			light1.lightRadius = 100;

			sceneLights.push_back(light);
			sceneLights.push_back(light1);

			float mapWidth = 1000;
			float mapHeight = 1000;

			PlaneArguments = { "files/models/Plane.mesh", "files/textures/Grass.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(mapWidth, 100, mapHeight), XMFLOAT3(0, 0, 0) };
			

			
			animations.push_back("files/models/Golem_1_Idle.animfile");
			animations.push_back("files/models/Golem_1_Born.animfile");
			animations.push_back("files/models/Golem_1_Run.animfile");
			animations.push_back("files/models/Golem_1_Death.animfile");
			animations.push_back("files/models/Golem_1_Kick.animfile");
			
			stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");
			waterMinionAnimations.push_back("files/models/Minion_4_Idle.animfile");
			fireMinionAnimations.push_back("files/models/Minion_2_Idle.animfile");
			woodMinionAnimations.push_back("files/models/Minion_1_Idle.animfile");

			SkyboxArguments = { "files/models/Skybox.mesh", "files/textures/Skybox.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(-10, -10, -10), XMFLOAT3(0, 0, 0) };

			GolemArguments = { "files/models/Golem_1.AnimMesh", "files/textures/Golem_1.mat", XMFLOAT3(10, 0.00001, 10), XMFLOAT3(0.1, 0.1, 0.1), XMFLOAT3(0, 0, 0) };
			

			for (int i = 0; i < 10; i++)
			{
				stoneMinionArguments.push_back({ "files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", XMFLOAT3(-130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0) });
				waterMinionArguments.push_back({ "files/models/Minion_4.AnimMesh", "files/textures/Minion_4.mat", XMFLOAT3(-130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0) });
				fireMinionArguments.push_back({"files/models/Minion_2.AnimMesh", "files/textures/Minion_2.mat", XMFLOAT3(130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)});
				woodMinionArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0) });
			}

			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(300, 0, 100), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(200, 0, 100), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(400, 0, 200), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(350, 0, 150), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(250, 0, 150), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(450, 0, 250), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(-200, 0, 300), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(-300, 0, 400), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			renderableArguments.push_back({ "files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", XMFLOAT3(-400, 0, 100), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0),ADResource::ADGameplay::STATIC });
			//m1 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", stoneMinionAnimations, XMFLOAT3(0, 5, 0), XMFLOAT3(0.02f, 1.02f, 0.02f), XMFLOAT3(0, 0, 0));
			//golem->targetMarker = m1;
		}

		void UpdateEnemies()
		{
			
		}

		void RenderEnemies()
		{

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

			ResourceManager::AddLight(sceneLights[0]);
			ResourceManager::AddLight(sceneLights[1]);

			Plane = GameUtilities::AddSimpleAsset(PlaneArguments.Model.data(), PlaneArguments.Texture.data(), PlaneArguments.position, PlaneArguments.scale, PlaneArguments.rotation);
			ResourceManager::AddSkybox(SkyboxArguments.Model.data(), SkyboxArguments.Texture.data(), SkyboxArguments.position, SkyboxArguments.scale, SkyboxArguments.rotation);

			golem = GameUtilities::LoadGolemFromModelFile(GolemArguments.Model.data(),GolemArguments.Texture.data(), animations, GolemArguments.position, GolemArguments.scale, GolemArguments.rotation);

			/*for (int i = 0; i < 10; i++)
			{
				stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile(stoneMinionArguments[i].Model.data(), stoneMinionArguments[i].Texture.data(), stoneMinionAnimations, stoneMinionArguments[i].position, stoneMinionArguments[i].scale, stoneMinionArguments[i].rotation));
				stoneMinionsAI.push_back(GameUtilities::AttachMinionAI(stoneMinions[i], golem->flockingGroups[STONE], STONE_MINION));
				waterMinions.push_back(GameUtilities::AddDestructableFromModelFile(waterMinionArguments[i].Model.data(), waterMinionArguments[i].Texture.data(), waterMinionAnimations, waterMinionArguments[i].position, waterMinionArguments[i].scale, waterMinionArguments[i].rotation));
				waterMinionsAI.push_back(GameUtilities::AttachMinionAI(waterMinions[i], golem->flockingGroups[WATER], WATER_MINION));
				fireMinions.push_back(GameUtilities::AddDestructableFromModelFile(fireMinionArguments[i].Model.data(), fireMinionArguments[i].Texture.data(), fireMinionAnimations, fireMinionArguments[i].position, fireMinionArguments[i].scale, fireMinionArguments[i].rotation));
				fireMinionsAI.push_back(GameUtilities::AttachMinionAI(fireMinions[i], golem->flockingGroups[FIRE], FIRE_MINION));
				woodMinions.push_back(GameUtilities::AddDestructableFromModelFile(woodMinionArguments[i].Model.data(), woodMinionArguments[i].Texture.data(), woodMinionAnimations, woodMinionArguments[i].position, woodMinionArguments[i].scale, woodMinionArguments[i].rotation));
				woodMinionsAI.push_back(GameUtilities::AttachMinionAI(woodMinions[i], golem->flockingGroups[WOOD], WOOD_MINION));
			}*/

			for (int i = 0; i < renderableArguments.size(); i++)
			{
				renderables.push_back(GameUtilities::AddDestructableFromModelFile(renderableArguments[i].Model.data(), renderableArguments[i].Texture.data(), woodMinionAnimations, renderableArguments[i].position, renderableArguments[i].scale, renderableArguments[i].rotation));
				renderables[i]->physicsType = renderableArguments[i].type;
			}

			/*for (int i = 0; i < 10; i++)
			{
				GameUtilities::AddGameObject(stoneMinions[i]);
				GameUtilities::AddGameObject(waterMinions[i]);
				GameUtilities::AddGameObject(fireMinions[i]);
				GameUtilities::AddGameObject(woodMinions[i]);
			}*/

			for (int i = 0; i < renderables.size(); i++)
			{
				GameUtilities::AddGameObject(renderables[i]);
			}
			//GameUtilities::AddGameObject(m1);
		}

		ADResource::ADGameplay::Golem* GetGolem()
		{
			return golem;
		}

		Renderable* GetPlane()
		{
			return Plane;
		}

		void GetMinions(std::vector<Destructable*> *stone, std::vector<Destructable*>* water, std::vector<Destructable*>* fire, std::vector<Destructable*>* wood)
		{
			*stone = stoneMinions;
			*water = waterMinions;
			*fire = fireMinions;
			*wood = woodMinions;
		}

		void destroy()
		{
			sceneLights.clear();
		}

		void Update()
		{
			UpdateEnemies();			//Happens after player input and uses the player's matrix to react.
		}

		void Render()
		{
		
		}
	};
}

