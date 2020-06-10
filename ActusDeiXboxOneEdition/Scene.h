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
			XMFLOAT3 A, B;
		};

		std::vector<Light>			sceneLights;
		ADResource::ADGameplay::Golem* golem;
		Renderable* tempPlane;

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
			ResourceManager::AddLight(light);

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
			ResourceManager::AddLight(light1);

			sceneLights.push_back(light);
			sceneLights.push_back(light1);

			float mapWidth = 1000;
			float mapHeight = 1000;
			tempPlane = GameUtilities::AddSimpleAsset("files/models/Ground.mesh", "files/textures/Ground.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(mapWidth, 100, mapHeight), XMFLOAT3(0, 0, 0));

			
			animations.push_back("files/models/Golem_1_Idle.animfile");
			animations.push_back("files/models/Golem_1_Born.animfile");
			animations.push_back("files/models/Golem_1_Run.animfile");
			animations.push_back("files/models/Golem_1_Death.animfile");
			animations.push_back("files/models/Golem_1_Kick.animfile");
			
			stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");
			waterMinionAnimations.push_back("files/models/Minion_4_Idle.animfile");
			fireMinionAnimations.push_back("files/models/Minion_2_Idle.animfile");
			woodMinionAnimations.push_back("files/models/Minion_1_Idle.animfile");

			ResourceManager::AddSkybox("files/models/Skybox.mesh", "files/textures/Skybox.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(-10, -10, -10), XMFLOAT3(0, 0, 0));
			golem = GameUtilities::LoadGolemFromModelFile("files/models/Golem_1.AnimMesh", "files/textures/Golem_1.mat", animations, XMFLOAT3(10, 0.00001, 10), XMFLOAT3(0.1, 0.1, 0.1), XMFLOAT3(0, 0, 0));

			for (int i = 0; i < 10; i++)
			{
				stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, XMFLOAT3(-130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
				stoneMinionsAI.push_back(GameUtilities::AttachMinionAI(stoneMinions[i], golem->flockingGroups[STONE]));
				waterMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_4.AnimMesh", "files/textures/Minion_4.mat", waterMinionAnimations, XMFLOAT3(-130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
				waterMinionsAI.push_back(GameUtilities::AttachMinionAI(waterMinions[i], golem->flockingGroups[WATER]));
				fireMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_2.AnimMesh", "files/textures/Minion_2.mat", fireMinionAnimations, XMFLOAT3(130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
				fireMinionsAI.push_back(GameUtilities::AttachMinionAI(fireMinions[i], golem->flockingGroups[FIRE]));
				woodMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
				woodMinionsAI.push_back(GameUtilities::AttachMinionAI(woodMinions[i], golem->flockingGroups[WOOD]));
			}

			Renderable* c1 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(300, 0, 100), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
			Renderable* c2 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(200, 0, 100), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
			Renderable* c3 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(400, 0, 200), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
			Renderable* c7 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(350, 0, 150), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
			Renderable* c8 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(250, 0, 150), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
			Renderable* c9 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(450, 0, 250), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
			Renderable* c4 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(-200, 0, 300), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
			Renderable* c5 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(-300, 0, 400), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
			Renderable* c6 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(-400, 0, 100), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));

			c1->physicsType = ADResource::ADGameplay::STATIC;
			c2->physicsType = ADResource::ADGameplay::STATIC;
			c3->physicsType = ADResource::ADGameplay::STATIC;
			c4->physicsType = ADResource::ADGameplay::STATIC;
			c5->physicsType = ADResource::ADGameplay::STATIC;
			c6->physicsType = ADResource::ADGameplay::STATIC;
			c7->physicsType = ADResource::ADGameplay::STATIC;
			c8->physicsType = ADResource::ADGameplay::STATIC;
			c9->physicsType = ADResource::ADGameplay::STATIC;
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

