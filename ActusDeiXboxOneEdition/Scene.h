#pragma once

#include "ResourceManager.h"

namespace ADGameplay
{
	class Scene
	{
	private:
		//std::vector<ADResource::ADGameplay::Enemy> Enemies;

		void InitializeScene()
		{
			//Spawn and initialize everything with data in resource manager.
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
			RenderEnemies();
		}
	};
}

