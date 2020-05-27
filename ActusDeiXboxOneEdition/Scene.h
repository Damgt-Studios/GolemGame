#pragma once

#include <vector>
#include "Player.h"
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
			std::array<char, 260> filePath;
			XMFLOAT3 position, scale, rotation;
		};
		struct PhysicsArgs 
		{
			XMFLOAT3 A, B;
		};

		std::vector<Light>			sceneLights;
		std::vector<PBRArguments>	PBRModels;
		std::vector<PBRArguments>	PBRStaticAssets;
		std::vector<PBRArguments>	tinyEssence;
		std::vector<PBRArguments>	triggers;
		std::vector<PBRArguments>	enemyModels;
		std::vector<PBRArguments>	colliderBoxes;
		std::vector<PhysicsArgs>	PhysicsArguments;
		std::vector<ADPhysics::AABB>PhysicsColliders;
		

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
			
			sceneLights.push_back(light1);

			//PBR models
			PBRModels.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, -1.3, 0), XMFLOAT3(100, .1, 100), XMFLOAT3(0, 0, 0) });

			//Static Assets
			PBRStaticAssets.push_back({ "files/models/text.wobj", XMFLOAT3(1, 0, 0), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0) });
			PBRStaticAssets.push_back({ "files/models/plane.wobj", XMFLOAT3(0, -0.25f, 0), XMFLOAT3(20, 10, 20), XMFLOAT3(0, 0, 0) });

			//Collectables
			for (int i = 0; i < 10; ++i)
			{
				tinyEssence.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(i * 5 + -10, 1, -10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });
			}

			//Triggers
			triggers.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });

			//Enemies
			enemyModels.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });

			//Colliders
			colliderBoxes.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });
			colliderBoxes.push_back({ "files/models/mapped_skybox.wobj", XMFLOAT3(0, 5, 15), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0) });

			//physics colliders
			PhysicsColliders.push_back(ADPhysics::AABB(XMFLOAT3(0, 0, 10), XMFLOAT3(2, 2, 2)));
			PhysicsColliders.push_back(ADPhysics::AABB(XMFLOAT3(0, 5, 15), XMFLOAT3(2, 2, 2)));
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

			uint32_t NumPBRMode;
			file.read((char*)&NumPBRMode, sizeof(uint32_t));
			PBRModels.resize(NumPBRMode);
			file.read((char*)PBRModels.data(), sizeof(PBRArguments) * NumPBRMode);

			uint32_t NumPBRStaticAssets;
			file.read((char*)&NumPBRStaticAssets, sizeof(uint32_t));
			PBRStaticAssets.resize(NumPBRStaticAssets);
			file.read((char*)PBRStaticAssets.data(), sizeof(PBRArguments) * NumPBRStaticAssets);

			uint32_t NumTinyEssence;
			file.read((char*)&NumTinyEssence, sizeof(uint32_t));
			tinyEssence.resize(NumTinyEssence);
			file.read((char*)tinyEssence.data(), sizeof(PBRArguments) * NumTinyEssence);

			uint32_t NumTriggers;
			file.read((char*)&NumTriggers, sizeof(uint32_t));
			triggers.resize(NumTriggers);
			file.read((char*)triggers.data(), sizeof(PBRArguments) * NumTriggers);

			uint32_t NumEnemies;
			file.read((char*)&NumEnemies, sizeof(uint32_t));
			enemyModels.resize(NumEnemies);
			file.read((char*)enemyModels.data(), sizeof(PBRArguments) * NumEnemies);

			uint32_t NumColliders;
			file.read((char*)&NumColliders, sizeof(uint32_t));
			colliderBoxes.resize(NumColliders);
			file.read((char*)colliderBoxes.data(), sizeof(PBRArguments) * NumColliders);

			uint32_t NumPhysics;
			file.read((char*)&NumPhysics, sizeof(uint32_t));
			PhysicsArguments.resize(NumPhysics);
			file.read((char*)PhysicsArguments.data(), sizeof(PhysicsArgs) * NumPhysics);

			for (int i = 0; i < NumPhysics; i++)
			{
				PhysicsColliders.push_back((ADPhysics::AABB(PhysicsArguments[i].A, PhysicsArguments[i].B)));
			}

			file.close();
		}

		void SaveSceneState(const char* fileName)
		{
			std::ofstream file(fileName, std::ios::trunc | std::ios::binary | std::ios::out);

			assert(file.is_open());

			uint32_t NumLights = static_cast<uint32_t>(sceneLights.size());
			uint32_t NumPBRModels = static_cast<uint32_t>(PBRModels.size());
			uint32_t NumPBRStaticAssets = static_cast<uint32_t>(PBRStaticAssets.size());
			uint32_t NumTinyEssence = static_cast<uint32_t>(tinyEssence.size());
			uint32_t NumTriggers = static_cast<uint32_t>(triggers.size());
			uint32_t NumEnemies = static_cast<uint32_t>(enemyModels.size());
			uint32_t NumColliders = static_cast<uint32_t>(colliderBoxes.size());
			uint32_t NumPhysics = static_cast<uint32_t>(PhysicsColliders.size());

			file.write((const char*)&NumLights, sizeof(uint32_t));
			file.write((const char*)sceneLights.data(), sizeof(Light) * sceneLights.size());
			file.write((const char*)&NumPBRModels, sizeof(uint32_t));
			file.write((const char*)PBRModels.data(), sizeof(PBRArguments) * PBRModels.size());
			file.write((const char*)&NumPBRStaticAssets, sizeof(uint32_t));
			file.write((const char*)PBRStaticAssets.data(), sizeof(PBRArguments) * PBRStaticAssets.size());
			file.write((const char*)&NumTinyEssence, sizeof(uint32_t));
			file.write((const char*)tinyEssence.data(), sizeof(PBRArguments) * tinyEssence.size());
			file.write((const char*)&NumTriggers, sizeof(uint32_t));
			file.write((const char*)triggers.data(), sizeof(PBRArguments) * triggers.size());
			file.write((const char*)&NumEnemies, sizeof(uint32_t));
			file.write((const char*)enemyModels.data(), sizeof(PBRArguments) * enemyModels.size());
			file.write((const char*)&NumColliders, sizeof(uint32_t));
			file.write((const char*)colliderBoxes.data(), sizeof(PBRArguments) * colliderBoxes.size());
			file.write((const char*)&NumPhysics, sizeof(uint32_t));
			file.write((const char*)PhysicsColliders.data(), sizeof(ADPhysics::AABB) * PhysicsColliders.size());


			file.close();
		}

		void destroy()
		{
			sceneLights.clear();
			PBRModels.clear();
			PBRStaticAssets.clear();
			tinyEssence.clear();
			triggers.clear();
			enemyModels.clear();
			colliderBoxes.clear();
			PhysicsColliders.clear();
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
				ResourceManager::AddPBRModel((string)PBRModels[i].filePath.data(), PBRModels[i].position, PBRModels[i].scale, PBRModels[i].rotation);
			}

			for (int i = 0; i < PBRStaticAssets.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddPBRStaticAsset((string)PBRStaticAssets[i].filePath.data(), PBRStaticAssets[i].position, PBRStaticAssets[i].scale, PBRStaticAssets[i].rotation));
			}

			for (int i = 0; i < tinyEssence.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddTinyEssenceFromModelFile((string)tinyEssence[i].filePath.data(), tinyEssence[i].position, tinyEssence[i].scale, tinyEssence[i].rotation));
			}

			for (int i = 0; i < triggers.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddEndGameTriggerFromModelFile((string)triggers[i].filePath.data(), triggers[i].position, triggers[i].scale, triggers[i].rotation));
			}

			for (int i = 0; i < enemyModels.size(); i++)
			{
				GameUtilities::AddGameObject(GameUtilities::AddEnemyFromModelFile((string)enemyModels[i].filePath.data(), enemyModels[i].position, enemyModels[i].scale, enemyModels[i].rotation));
			}

			for (int i = 0; i < colliderBoxes.size(); i++)
			{
				Renderable* tempC = GameUtilities::AddColliderBox((string)colliderBoxes[i].filePath.data(), colliderBoxes[i].position, colliderBoxes[i].scale, colliderBoxes[i].rotation);
				if (i < PhysicsColliders.size())
				{
					tempC->colliderPtr = &PhysicsColliders[i];
					tempC->physicsType = OBJECT_PHYSICS_TYPE::STATIC;
				}
				GameUtilities::AddGameObject(tempC);
			}
		}
	};
}

