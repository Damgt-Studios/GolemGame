#pragma once

#include <vector>
#include "Player.h"
#include "ResourceManager.h"
#include "../Game/GameUtilities.h"
#include "../Game/GameObjectClasses.h"
#include "MeshLoader.h"

namespace ADGameplay
{
	class Scene
	{
	private:
		//std::vector<ADResource::ADGameplay::Enemy> Enemies;

		struct PBRArguments
		{
			string filePath;
			XMFLOAT3 position, scale, rotation;
		};

		std::vector<Light> sceneLights;
		std::vector<PBRArguments> PBRModels;
		std::vector<PBRArguments> PBRStaticAssets;
		std::vector<PBRArguments> tinyEssence;
		std::vector<PBRArguments> triggers;
		std::vector<PBRArguments> enemyModels;
		std::vector<PBRArguments> colliderBoxes;
		

		void InitializeScene()
		{
			//Spawn and initialize everything with data in resource manager.
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
			//ResourceManager::AddLight(light);
			sceneLights.push_back(light);
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
			//ResourceManager::AddLight(light1);
			sceneLights.push_back(light1);

			//ResourceManager::AddPBRModel("files/models/mapped_skybox.wobj", XMFLOAT3(0, -1.3, 0), XMFLOAT3(100, .1, 100), XMFLOAT3(0, 0, 0));
			PBRModels.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, -1.3, 0), XMFLOAT3(100, .1, 100), XMFLOAT3(0, 0, 0) });

			//Renderable* a1 = GameUtilities::AddPBRStaticAsset("files/models/oildrum.wobj", XMFLOAT3(3, 0, -1), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0));
			PBRStaticAssets.push_back({ "files/models/oildrum.wobj", XMFLOAT3(3, 0, -1), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0) });
			//Renderable* a2 = GameUtilities::AddPBRStaticAsset("files/models/text.wobj", XMFLOAT3(1, 0, 0), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0));
			PBRStaticAssets.push_back({ "files/models/text.wobj", XMFLOAT3(1, 0, 0), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0) });
			//Renderable* testPlane = GameUtilities::AddPBRStaticAsset("files/models/plane.wobj", XMFLOAT3(0, -0.25f, 0), XMFLOAT3(20, 10, 20), XMFLOAT3(0, 0, 0));
			PBRStaticAssets.push_back({ "files/models/plane.wobj", XMFLOAT3(0, -0.25f, 0), XMFLOAT3(20, 10, 20), XMFLOAT3(0, 0, 0) });

			//Trigger** collectables = new Trigger * [10];
			for (int i = 0; i < 10; ++i)
			{
				//collectables[i] = GameUtilities::AddTinyEssenceFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(i * 5 + -10, 1, -10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
				//GameUtilities::AddGameObject(collectables[i]);
				tinyEssence.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(i * 5 + -10, 1, -10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });
			}
			//Trigger* t1 = GameUtilities::AddEndGameTriggerFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
			triggers.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });
			//Destructable* e1 = GameUtilities::AddEnemyFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
			enemyModels.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });
			// Colliders
			//Renderable* c1 = GameUtilities::AddColliderBox("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
			//Renderable* c2 = GameUtilities::AddColliderBox("files/models/mapped_skybox.wobj", XMFLOAT3(0, 5, 15), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));

			colliderBoxes.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });
			colliderBoxes.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, 5, 15), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });
			/*GameUtilities::AddGameObject(c1);
			GameUtilities::AddGameObject(c2);
			GameUtilities::AddGameObject(a2);
			GameUtilities::AddGameObject(e1);
			GameUtilities::AddGameObject(t1);
			GameUtilities::AddGameObject(testPlane);*/
		}

		void UpdateEnemies()
		{
			
		}

		void RenderEnemies()
		{

		}


	public:
		void LoadScene()
		{
			InitializeScene();
			//Load the terrain
			//Load All the gameObjects into the ResourceManager.
			//Initialize Level
		}

		void SaveSceneState()
		{
			//If we make this function write the current gameObjects we could combine it with a save function that also saves the gamestate to create a save in place.
		}

		void Update()
		{
			UpdateEnemies();			//Happens after player input and uses the player's matrix to react.
		}

		void Render()
		{
			//Render the Level Here
			RenderEnemies();
			for (int i = 0; i < sceneLights.size(); i++)
			{
				ResourceManager::AddLight(sceneLights[i]);
			}

			for (int i = 0; i < PBRModels.size(); i++)
			{
				ResourceManager::AddPBRModel(PBRModels[i].filePath, PBRModels[i].position, PBRModels[i].scale, PBRModels[i].rotation);
			}

			for (int i = 0; i < PBRStaticAssets.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddPBRStaticAsset(PBRStaticAssets[i].filePath, PBRStaticAssets[i].position, PBRStaticAssets[i].scale, PBRStaticAssets[i].rotation));
			}

			for (int i = 0; i < tinyEssence.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddTinyEssenceFromModelFile(tinyEssence[i].filePath, tinyEssence[i].position, tinyEssence[i].scale, tinyEssence[i].rotation));
			}

			for (int i = 0; i < triggers.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddEndGameTriggerFromModelFile(triggers[i].filePath, triggers[i].position, triggers[i].scale, triggers[i].rotation));
			}

			for (int i = 0; i < enemyModels.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddEnemyFromModelFile(enemyModels[i].filePath, enemyModels[i].position, enemyModels[i].scale, enemyModels[i].rotation));
			}

			for (int i = 0; i < colliderBoxes.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddColliderBox(colliderBoxes[i].filePath, colliderBoxes[i].position, colliderBoxes[i].scale, colliderBoxes[i].rotation));
			}
		}
	};
}

