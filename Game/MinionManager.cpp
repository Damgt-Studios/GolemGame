#include "pchgame.h"
#include "MinionManager.h"
//
//
//
//void MinionManager::Initialize(Golem* _golem)
//{
//	stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");
//	waterMinionAnimations.push_back("files/models/Minion_4_Idle.animfile");
//	fireMinionAnimations.push_back("files/models/Minion_2_Idle.animfile");
//	woodMinionAnimations.push_back("files/models/Minion_1_Idle.animfile");
//
//	for (int i = 0; i < MAX_MINIONS; i++)
//	{
//		available.push_back(i);
//		allUnits[i] = new ADAI::AIUnit;
//		ADResource::ADGameplay::Destructable* minion = new ADResource::ADGameplay::Destructable;
//		XMFLOAT3 scale = { 0.03f, 0.03f, 0.03f };
//		XMFLOAT3 rotation = { 0,0,0 };
//		XMFLOAT3 position = { -130, 5, -130 };
//
//		minion->SetScale(scale);
//		scale.x *= 100.0f;
//		scale.y *= 100.0f;
//		scale.z *= 100.0f;
//		minion->colScale = scale;
//		minion->SetRotation(rotation);
//		minion->SetPosition(position);
//		minion->collider = ADPhysics::AABB(position, minion->colScale);
//		minion->colliderPtr = &minion->collider;
//		minion->team = 0;
//		minion->active = false;
//		minion->gamePlayType = UNTYPED_MINION;
//		minion->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["GreatGolem"]));
//
//		allUnits[i]->owner = minion;
//		ADAI::IdleState* idling = new ADAI::IdleState();
//		ADAI::FlockingState* charging = new ADAI::FlockingState();
//		allUnits[i]->states.push_back(idling);
//		allUnits[i]->states.push_back(charging);
//
//		GameUtilities::AddGameObject(minion);
//	}
//}
//
////
////void MinionManager::Initialize(Golem* _golem)
////{
////	//Create the entire list of golems and put them aside as inactive.
////
////	//GameUtilities::InitializeMinionModels();
////
////	stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");
////	waterMinionAnimations.push_back("files/models/Minion_4_Idle.animfile");
////	fireMinionAnimations.push_back("files/models/Minion_2_Idle.animfile");
////	woodMinionAnimations.push_back("files/models/Minion_1_Idle.animfile");
////
////
////	//std::vector<Destructable*> stoneMinions;
////	//std::vector<ADAI::AIUnit*> stoneMinionsAI;
////	//std::vector<Destructable*> waterMinions;
////	//std::vector<ADAI::AIUnit*> waterMinionsAI;
////	//std::vector<Destructable*> fireMinions;
////	//std::vector<ADAI::AIUnit*> fireMinionsAI;
////	//std::vector<Destructable*> woodMinions;
////	//std::vector<ADAI::AIUnit*> woodMinionsAI;
////
////	//for (int i = 0; i < 10; i++)
////	//{
////	//	//stoneMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, XMFLOAT3(-130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
////	//	//stoneMinionsAI.push_back(GameUtilities::AttachMinionAI(stoneMinions[i], golem->flockingGroups[STONE], STONE_MINION));
////
////	//	stoneMinionsAI.push_back(GameUtilities::BirthStoneMinion(_golem->flockingGroups[STONE]));
////
////
////	//	waterMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_4.AnimMesh", "files/textures/Minion_4.mat", waterMinionAnimations, XMFLOAT3(-130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
////	//	waterMinionsAI.push_back(GameUtilities::AttachMinionAI(waterMinions[i], _golem->flockingGroups[WATER], WATER_MINION));
////	//	fireMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_2.AnimMesh", "files/textures/Minion_2.mat", fireMinionAnimations, XMFLOAT3(130, 5, -130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
////	//	fireMinionsAI.push_back(GameUtilities::AttachMinionAI(fireMinions[i], _golem->flockingGroups[FIRE], FIRE_MINION));
////	//	woodMinions.push_back(GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", woodMinionAnimations, XMFLOAT3(130, 5, 130), XMFLOAT3(0.03f, 0.03f, 0.03f), XMFLOAT3(0, 0, 0)));
////	//	woodMinionsAI.push_back(GameUtilities::AttachMinionAI(woodMinions[i], _golem->flockingGroups[WOOD], WOOD_MINION));
////	//}
////	//Destructable* m1 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_4.AnimMesh", "files/textures/Minion_4.mat", waterMinionAnimations, XMFLOAT3(0, 5, 0), XMFLOAT3(0.02f, 1.02f, 0.02f), XMFLOAT3(0, 0, 0));
////	//_golem->targetMarker = m1;
////
////	//for (int i = 0; i < 10; i++)
////	//{
////	//	GameUtilities::AddGameObject(stoneMinions[i]);
////	//	GameUtilities::AddGameObject(waterMinions[i]);
////	//	GameUtilities::AddGameObject(fireMinions[i]);
////	//	GameUtilities::AddGameObject(woodMinions[i]);
////
////	//}
////	//GameUtilities::AddGameObject(m1);
////}
////
//void MinionManager::Shutdown()
//{
//
//}
////
////
////
//////ADResource::ADGameplay::Destructable* fMinion = new ADResource::ADGameplay::Destructable;
////
//////XMFLOAT3 position = { -130, 5, -130 };
//////XMFLOAT3 scale = { 0.03f, 0.03f, 0.03f };
//////XMFLOAT3 rotation = { 0,0,0 };
////
////
//////// Transform data
//////fMinion->SetScale(scale);
//////fMinion->SetRotation(rotation);
//////fMinion->SetPosition(position);
////
////
////////fMinion->SetMeshID(DefinitionDatabase::Instance()->stoneMinionMeshid);
//////fMinion->team = 0;
////
//////scale.x *= 100.0f;
//////scale.y *= 100.0f;
//////scale.z *= 100.0f;
//////fMinion->colScale = scale;
//////fMinion->collider = ADPhysics::AABB(position, fMinion->colScale);
//////fMinion->colliderPtr = &fMinion->collider;
//////fMinion->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["GreatGolem"]));
////
//////ADAI::AIUnit* fAI = new ADAI::AIUnit;
//////fAI->owner = fMinion;
//////fMinion->gamePlayType = FIRE_MINION;
//////fMinion->deathEvent = "StoneMinionCountChange";
//////fMinion->team = 0;
//////ADAI::IdleState* idling = new ADAI::IdleState();
//////ADAI::FlockingState* charging = new ADAI::FlockingState();
//////fAI->states.push_back(idling);
//////fAI->states.push_back(charging);
////
////////DefinitionDatabase::Instance()->stoneMinions.push_back(fMinion);
//////_commandGroup->AddUnitToGroup(fMinion, charging);
////
//////return fAI;
////
////
////
////This will need to take the Spawners Location when used.
//ADAI::AIUnit* MinionManager::BirthStoneMinion(ADAI::FlockingGroup* _commandGroup, XMFLOAT3 _position)
//{
//	if (!available.empty())
//	{
//		UINT selection = available.front();
//		available.pop_front();
//		ADAI::AIUnit* currentAI = allUnits[selection];
//		//if (currentAI->owner->gamePlayType == UNTYPED_MINION)
//		//{
//		//	XMFLOAT3 scale = { 0.03f, 0.03f, 0.03f };
//		//	XMFLOAT3 rotation = { 0,0,0 };
//		//	//XMFLOAT3 position = { -130, 5, -130 };
//
//		//	// Transform data
//		//	//temp->SetScale(scale);
//		//	//temp->SetRotation(rotation);
//		//	currentAI->owner->SetPosition(_position);
//		//	//ADResource::ADGameplay::Destructable* minion = GameUtilities::AddDestructableFromModelFile("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, position, scale, rotation);
//		//	AD_ULONG id = ResourceManager::InitializeAnimatedModel("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, _position, scale, rotation, GameUtilities::shader);
//		//	currentAI->owner->SetMeshID(id);
//		//	//scale.x *= 100.0f;
//		//	//scale.y *= 100.0f;
//		//	//scale.z *= 100.0f;
//		//	//temp->colScale = scale;
//		//	//temp->collider = ADPhysics::AABB(position, temp->colScale);
//		//	//temp->colliderPtr = &temp->collider;
//		//	/*
//		//	currentAI->owner->->team = 0;*/
//		//	currentAI->owner->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["StoneMinion"]));
//		//	
//		//	//currentAI = new ADAI::AIUnit;
//		//	//currentAI->owner = minion;
//		//	//minion->gamePlayType = STONE_MINION;
//		//	//minion->deathEvent = "StoneMinionCountChange";
//		//	//ADAI::IdleState* idling = new ADAI::IdleState();
//		//	//ADAI::FlockingState* charging = new ADAI::FlockingState();
//		//	//currentAI->states.push_back(idling);
//		//	//currentAI->states.push_back(charging);
//
//		//	//_commandGroup->AddUnitToGroup(minion, charging);
//		//	//stoneMinions.push_back(currentAI);
//		//	//GameUtilities::AddGameObject(minion);
//
//		//	return currentAI;
//		//}
//		if (currentAI->owner->gamePlayType == STONE_MINION)
//		{
//			//soft reset
//		}
//		else
//		{
//			XMFLOAT3 scale = { 0.03f, 0.03f, 0.03f };
//			XMFLOAT3 rotation = { 0,0,0 };
//			currentAI->owner->SetPosition(_position);
//			AD_ULONG id = ResourceManager::InitializeAnimatedModel("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, _position, scale, rotation, GameUtilities::shader);
//			currentAI->owner->SetMeshID(id);
//			//currentAI->owner->SetStatSheet(new StatSheet(*DefinitionDatabase::Instance()->statsheetDatabase["GreatGolem"]));
//			currentAI->owner->gamePlayType = STONE_MINION;
//			currentAI->owner->deathEvent = "StoneMinionCountChange";
//			currentAI->owner->has_mesh = true;
//			currentAI->owner->active = true;
//			//hard reset
//		}
//	}
//	return nullptr;
//}
////
////void MinionManager::Death(UINT _index)
////{
////	//Remove golem from specific group vector using value it has stored for ID.
////	//Send out count change event.
////	//Deactivate
////	//Add golems TRUE_INDEX not SPECIFICGROUP_INDEX to the avaiable queue.
////}
