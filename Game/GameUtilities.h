#pragma once

#include <string>

#include "Spyro.h"
#include "ResourceManager.h"

class GameUtilities
{
public:
	static Spyro* LoadSpyroFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	static ColliderBox* AddColliderBox(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);

public:
	// Game object shit
	static void AddGameObject(GameObject* obj, bool has_mesh = true);
};