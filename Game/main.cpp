// Include the precompiled headers
#include "pchgame.h"
#include <vector>
#include <windows.gaming.input.h>
#include <collection.h>
#include "Engine.h"
#include "ADPhysics.h"
#include "ADQuadTree.h"
#include "TheGreatGolem.h"

#include "GameUserInterface.h"
#include "GameUtilities.h"
#include "GameObjectClasses.h"
#include "MeshLoader.h"
#include "ADAI.h"
#include "ADPathfinding.h"
#include "AnimationStateMachine.h"
#include "Listeners.h"
//#include "MinionManager.h"
#include <Scene.h>
#include <JobManager.h>
#include <future>


#define ShowColliders

// Use some common namespaces to simplify the code
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Media::Playback;
using namespace Windows::Media::Core;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::UI::ViewManagement;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Platform;
using namespace Platform::Collections;

// Settings
bool FULLSCREEN = false;
// Settings


struct PhysicsShiz
{
	int OBJ_COUNT;
	GameObject** OBJS;
	QuadTree<int>* collisionTree;
	promise<void>* physicspromise;
};

struct AiArgs
{
	ADGameplay::Scene* scene;
	float delta;
	promise<void>* scenePromise;
};

struct ClampingArgs
{
	GameObject* golem;
	QuadTree<ADPhysics::Triangle>* tree;
	vector<Destructable*>* stone;
	vector<Destructable*>* water;
	vector<Destructable*>* fire;
	vector<Destructable*>* wood;
	float time;
	promise<void>* clamppromise;
};

void ClampingWrapper(void* args, int index)
{
	ClampingArgs* temp = static_cast<ClampingArgs*>(args);
	for (int i = 0; i < 10; i++)
	{
		GroundClamping(temp->stone->at(i), temp->tree, temp->time);
		GroundClamping(temp->water->at(i), temp->tree, temp->time);
		GroundClamping(temp->fire->at(i), temp->tree, temp->time);
		GroundClamping(temp->wood->at(i), temp->tree, temp->time);
	}

	GroundClamping(temp->golem, temp->tree, temp->time);
	temp->clamppromise->set_value();
}

void AiUpdateWrapper(void* args, int index)
{
	AiArgs* temp = static_cast<AiArgs*>(args);
	temp->scene->Update(temp->delta);
	temp->scenePromise->set_value();
}

void MainPhysics(void* args, int index)
{
	PhysicsShiz* temp = static_cast<PhysicsShiz*>(args);
	for (int i = 0; i < temp->OBJ_COUNT; i++)
	{

		if (temp->OBJS[i]->colliderPtr)
		{
			int* index = new int(i);
			if (!temp->collisionTree->Insert(ADQuadTreePoint<int>(temp->OBJS[i]->colliderPtr->Pos.x, temp->OBJS[i]->colliderPtr->Pos.z, *index)))
			{
				int somethingswrong = 0;
				somethingswrong++;
			}
		}
	}

	for (unsigned int i = 0; i < temp->OBJ_COUNT; i++)
	{
		if (temp->OBJS[i]->colliderPtr)
		{
			XMFLOAT3 obj_pos = VectorToFloat3(temp->OBJS[i]->transform.r[3]);
			std::vector<ADQuadTreePoint<int>> collisionVector = temp->collisionTree->Query(ADQuad(obj_pos.x, obj_pos.z, 25, 25));

			for (unsigned int j = 0; j < collisionVector.size(); j++)
			{
				if (temp->OBJS[*collisionVector[j].data]->colliderPtr)
					temp->OBJS[i]->CheckCollision(temp->OBJS[*collisionVector[j].data]);
			}
		}
	}

	ADResource::ADGameplay::ResolveCollisions();
	temp->physicspromise->set_value();
}


// the class definition for the core "framework" of our app
ref class App sealed : public IFrameworkView
{
private:
	Jobs::JobManager* jobManagerTest;
	Engine* engine;
	Engine::AllArgs* engineArgs;
	PhysicsShiz* PhysicsArguments;
	TheGreatGolem* game;
	AD_AUDIO::ADAudio* audioEngine;
	ADResource::ADGameplay::Golem* golem;
	ADGameplay::Scene currentScene;
	ClampingArgs* clampArgs;
	AiArgs* aiArgs;
	//MinionManager* minionManager;
	AD_ULONG golem_collider = 0;
	//FountainEmitter femitter;

	bool shutdown = false;
	bool thread = false;

	// Timing
	XTime game_time;
	float timer = 0;
	float delta_time = 0;

	const float physics_rate = 0.05f;
	float physics_timer = 0;

	// Rotation
	float rot = 0;

	float yaw = 180.0f;
	float pitch = 30.0f;

	float default_yaw = 180.0f;
	float default_pitch = 30.0f;

	// Physics
	ADPhysics::AABB test_colider;
	ADPhysics::AABB test_colider1;



public:
	// some functions called by Windows
	virtual void Initialize(CoreApplicationView^ AppView)
	{

		if (FULLSCREEN)
		{
			Windows::UI::ViewManagement::ApplicationView::PreferredLaunchWindowingMode =
				Windows::UI::ViewManagement::ApplicationViewWindowingMode::FullScreen;
		}
		else
		{
			Windows::UI::ViewManagement::ApplicationView::PreferredLaunchWindowingMode =
				Windows::UI::ViewManagement::ApplicationViewWindowingMode::CompactOverlay;
		}

		// set the OnActivated function to handle to Activated "event"
		AppView->Activated += ref new TypedEventHandler
			<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

		Gamepad::GamepadAdded += ref new EventHandler<Gamepad^>(&Input::OnGamepadAdded);
		Gamepad::GamepadRemoved += ref new EventHandler<Gamepad^>(&Input::OnGamepadRemoved);
	}

	virtual void SetWindow(CoreWindow^ Window)
	{
		Window->KeyDown += ref new TypedEventHandler
			<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyDown);
	}

	virtual void Load(String^ EntryPoint) {}

	void LoadGameEmitters()
	{
		//femitter.Initialize(engine->GetPBRRenderer()->GetRendererResources()->device.Get(), 100, XMFLOAT4(1, 1, 1, 1), L"files\\textures\\Particle_Essence.dds", 9999999);
	}

	virtual void Run()
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		//---------------------Create The Engines and the Game Object
		engine = new Engine;
		audioEngine = new AD_AUDIO::ADAudio;
		game = new TheGreatGolem();
		clampArgs = new ClampingArgs();

		jobManagerTest = Jobs::JobManager::GetInstance();
		//clampArgs = new ClampingArgs();
		PhysicsArguments = new PhysicsShiz();
		aiArgs = new AiArgs();
		//Initialize.  Order Matters.
		audioEngine->Init();
		game->LoadGameAudio(audioEngine);
		game->Initialize();

		BigCloudEmitterListener bigGolemDustParticles(engine->bigCloud);
		bigGolemDustParticles.lifespan = 0.5f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("BigGolemParticles", &bigGolemDustParticles);

		RecoveryEmitterListener golemRecoveryParticles(engine->recoveryEmitter);
		golemRecoveryParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("GolemRecoveryParticles", &golemRecoveryParticles);

		SmallCloudEmitterListener smallGolemDustParticles(engine->smallCloud);
		smallGolemDustParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("SmallGolemParticles", &smallGolemDustParticles);

		WaterWaveEmitterListener waveParticles(engine->waterWave);
		waveParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("GolemWaveParticles", &waveParticles);

		IronSkinEmitterListener ironSkinParticles(engine->ironSkin);
		ironSkinParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("GolemIronSkinParticles", &ironSkinParticles);

		FireballEmitterListener fireballParticles(engine->fireball);
		fireballParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("GolemFireballParticles", &fireballParticles);

		CylinderEmitterListener woodCylinderParticles(engine->woodCylinder);
		woodCylinderParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("GolemWoodCylinderParticles", &woodCylinderParticles);

		CylinderEmitterListener fireCylinderParticles(engine->fireCylinder);
		fireCylinderParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("GolemFireCylinderParticles", &fireCylinderParticles);

		CylinderEmitterListener waterCylinderParticles(engine->waterCylinder);
		waterCylinderParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("GolemWaterCylinderParticles", &waterCylinderParticles);

		CylinderEmitterListener stoneCylinderParticles(engine->stoneCylinder);
		stoneCylinderParticles.lifespan = 1.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("GolemStoneCylinderParticles", &stoneCylinderParticles);

		// Initialize the engine
		engine->SetCamera(XMFLOAT3(0, 10000.0f, -100.0f), 0, 0, 45);
		engine->GetCamera()->SetClippingPlanes(0.1, 5000);
		currentScene.LoadScene("files/scenes/TestScene.scenery", engine);
		golem = currentScene.GetGolem();
		game->LoadListeners(golem, &currentScene);
		engine->GetOrbitCamera()->SetLookAt((XMFLOAT3&)(Float3ToVector((*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position)));

		engine->GetOrbitCamera()->Rotate(yaw, pitch);

		GameUtilities::AddGameObject(engine->GetOrbitCamera());
		golem->bigPuffs[STONE] = &engine->bigStonePuff;
		golem->bigPuffs[WATER] = &engine->bigWaterPuff;
		golem->bigPuffs[FIRE] = &engine->bigFirePuff;
		golem->bigPuffs[WOOD] = &engine->bigWoodPuff;

#ifdef _DEBUG
		Renderable* minionCollider = GameUtilities::AddRenderableCollider();

		Renderable* golemCollider = GameUtilities::AddRenderableCollider();
		Renderable* house1Collider = GameUtilities::AddRenderableCollider();
		Renderable* house2Collider = GameUtilities::AddRenderableCollider();
		Renderable* house3Collider = GameUtilities::AddRenderableCollider();
		Renderable* house4Collider = GameUtilities::AddRenderableCollider();
		Renderable* barn1Collider = GameUtilities::AddRenderableCollider();
		Renderable* barn2Collider = GameUtilities::AddRenderableCollider();
		Renderable* gatewayCollider = GameUtilities::AddRenderableCollider();
		Renderable* towerCollider = GameUtilities::AddRenderableCollider();
		Renderable* scaffoldingCollider = GameUtilities::AddRenderableCollider();
		Renderable* wallCollider = GameUtilities::AddRenderableCollider();
		Renderable* wellCollider = GameUtilities::AddRenderableCollider();
		Renderable* tavernCollider = GameUtilities::AddRenderableCollider();
		Renderable* rockCollider = GameUtilities::AddRenderableCollider();
		Renderable* meeting_hallCollider = GameUtilities::AddRenderableCollider();
		Renderable* cartCollider = GameUtilities::AddRenderableCollider();

		Renderable* rubbleCollider1 = GameUtilities::AddRenderableCollider();
		Renderable* rubbleCollider2 = GameUtilities::AddRenderableCollider();
		Renderable* rubbleCollider3 = GameUtilities::AddRenderableCollider();

		Renderable* rock_wallCollider = GameUtilities::AddRenderableCollider();

#ifdef ShowColliders
		//GameUtilities::AddGameObject(minionCollider);

		GameUtilities::AddGameObject(golemCollider);
		//GameUtilities::AddGameObject(house1Collider);
		//GameUtilities::AddGameObject(house2Collider);
		//GameUtilities::AddGameObject(house3Collider);
		//GameUtilities::AddGameObject(house4Collider);
		//GameUtilities::AddGameObject(barn1Collider);
		//GameUtilities::AddGameObject(barn2Collider);
		//GameUtilities::AddGameObject(gatewayCollider);
		//GameUtilities::AddGameObject(towerCollider);
		//GameUtilities::AddGameObject(scaffoldingCollider);
		//GameUtilities::AddGameObject(wallCollider);
		//GameUtilities::AddGameObject(wellCollider);
		//GameUtilities::AddGameObject(tavernCollider);
		//GameUtilities::AddGameObject(rockCollider);
		//GameUtilities::AddGameObject(meeting_hallCollider);
		//GameUtilities::AddGameObject(cartCollider);

		//GameUtilities::AddGameObject(rubbleCollider1);
		//GameUtilities::AddGameObject(rubbleCollider2);
		//GameUtilities::AddGameObject(rubbleCollider3);

		//GameUtilities::AddGameObject(rock_wallCollider1);
		//GameUtilities::AddGameObject(rock_wallCollider2);
		//GameUtilities::AddGameObject(rock_wallCollider3);
		//GameUtilities::AddGameObject(rock_wallCollider4);

#endif
#endif


		std::vector<Destructable*> stoneMinions;
		std::vector<Destructable*> waterMinions;
		std::vector<Destructable*> fireMinions;
		std::vector<Destructable*> woodMinions;

		currentScene.GetMinions(&stoneMinions, &waterMinions, &fireMinions, &woodMinions);


		engine->GetOrbitCamera()->SetClippingPlanes(0.1f, 10000.0f);

		//Renderable* tempPlane = currentScene.GetPlane();
		Renderable* physicsPlane = GameUtilities::AddSimpleAsset("files/models/LevelPhysics.mesh", "files/textures/Grass.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), true);
		GameUtilities::AddGameObject(physicsPlane);

		Renderable* mountainRange = GameUtilities::AddSimpleAsset("files/models/Mountain.mesh", "files/textures/Mountain.mat", XMFLOAT3(-2400, 1750, 3600), XMFLOAT3(200, 200, 200), XMFLOAT3(0, 90, 0), false);
		mountainRange->colliderPtr = nullptr;
		GameUtilities::AddGameObject(mountainRange);

#pragma region PhysicsSetup

		//Physics Setup--------------------------------
		SimpleModel** tempPlaneModel = ResourceManager::GetSimpleModelPtrFromMeshId(physicsPlane->GetMeshId());
		SimpleStaticModel* planeModel = static_cast<SimpleStaticModel*>(*tempPlaneModel);
		std::vector<ADPhysics::Triangle> ground;
		std::vector<ADQuadTreePoint<ADPhysics::Triangle>> treePoints;
		XMMATRIX groundWorld = XMMatrixIdentity();
		physicsPlane->GetWorldMatrix(groundWorld);
		for (unsigned int i = 0; i < (*planeModel).indices.size(); i += 3)
		{
			XMFLOAT3 A = planeModel->vertices[(*planeModel).indices[i]].Position;
			XMFLOAT3 B = planeModel->vertices[(*planeModel).indices[i + 1]].Position;
			XMFLOAT3 C = planeModel->vertices[(*planeModel).indices[i + 2]].Position;

			A = (XMFLOAT3&)(XMVector3Transform(Float3ToVector(A), groundWorld));
			B = (XMFLOAT3&)(XMVector3Transform(Float3ToVector(B), groundWorld));
			C = (XMFLOAT3&)(XMVector3Transform(Float3ToVector(C), groundWorld));

			ADPhysics::Triangle* tri = new Triangle(A, B, C);

			ground.push_back(*tri);

			XMFLOAT3 centroid = (XMFLOAT3&)((Float3ToVector(tri->a) + Float3ToVector(tri->b) + Float3ToVector(tri->c)) / 3);
			ADQuadTreePoint<ADPhysics::Triangle> point = ADQuadTreePoint<ADPhysics::Triangle>(centroid.x, centroid.z, *tri);
			treePoints.push_back(point);
		}

		ADQuad boundary = ADQuad((*planeModel).position.x, (*planeModel).position.z, 1000, 1000);
		QuadTree<ADPhysics::Triangle>* tree = new QuadTree<ADPhysics::Triangle>(boundary);

		for (unsigned int i = 0; i < treePoints.size(); i++)
		{
			tree->Insert(treePoints[i]);
		}



#pragma endregion

		//DON'T DELETE THIS PLEASE FOR THE LOVE OF GOD
#pragma region Level Boundary

		GameObject* object1 = new GameObject();
		object1->physicsType = OBJECT_PHYSICS_TYPE::STATIC;
		object1->team = 4;
		object1->colliderPtr = new AABB(XMFLOAT3(630, 0, -270), XMFLOAT3(30, 100, 2000));

		GameObject* object2 = new GameObject();
		object2->physicsType = OBJECT_PHYSICS_TYPE::STATIC;
		object2->team = 4;
		object2->colliderPtr = new AABB(XMFLOAT3(-630, 0, -270), XMFLOAT3(30, 100, 2000));

		GameObject* object3 = new GameObject();
		object3->physicsType = OBJECT_PHYSICS_TYPE::STATIC;
		object3->team = 4;
		object3->colliderPtr = new AABB(XMFLOAT3(0, 0, 630), XMFLOAT3(2000, 100, 30));

		GameObject* object4 = new GameObject();
		object4->physicsType = OBJECT_PHYSICS_TYPE::STATIC;
		object4->team = 4;
		object4->colliderPtr = new AABB(XMFLOAT3(0, 0, -1250), XMFLOAT3(2000, 100, 30));

		GameObject* object5 = new GameObject();
		object5->physicsType = OBJECT_PHYSICS_TYPE::STATIC;
		object5->team = 4;
		object5->colliderPtr = new AABB(XMFLOAT3(400, 0, -625), XMFLOAT3(22.5, 100, 1400));

		//std::vector<GameObject*> forcedCollisions = { object1, object2, object3, object4 };
		std::vector<GameObject*> forcedCollisions = { object1, object2, object3, object4, object5 };

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, 10), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, 50), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, 100), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, 150), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, 200), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, 240), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, 290), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, 340), XMFLOAT3(0, 180, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, 390), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, 430), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, 480), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, 530), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, 580), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, 620), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall3(XMFLOAT3(580, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(530, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall1(XMFLOAT3(480, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(440, 0, 630), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(390, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(340, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall1(XMFLOAT3(290, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(250, 0, 630), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(200, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(150, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall1(XMFLOAT3(100, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(60, 0, 630), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(10, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-40, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall1(XMFLOAT3(-90, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-130, 0, 630), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-180, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-230, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall1(XMFLOAT3(-280, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-320, 0, 630), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-370, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-420, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall1(XMFLOAT3(-470, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-510, 0, 630), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-560, 0, 630), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-610, 0, 630), XMFLOAT3(0, 90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, 620), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, 580), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, 530), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, 480), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, 430), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, 390), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, 340), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, 290), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, 240), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, 200), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, 150), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, 100), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, 50), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, 10), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, -40), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, -90), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, -140), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, -180), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, -230), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, -280), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, -330), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, -370), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, -420), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, -470), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, -520), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, -560), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, -610), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, -660), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, -710), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, -750), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, -800), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, -850), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, -900), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, -940), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, -990), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, -1040), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-630, 0, -1090), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-630, 0, -1130), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-630, 0, -1180), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-630, 0, -1230), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-610, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-570, 0, -1250), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-520, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-470, 0, -1250), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-420, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-380, 0, -1250), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-330, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-280, 0, -1250), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-230, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(-190, 0, -1250), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(-140, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(-90, 0, -1250), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(-40, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(0, 0, -1250), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(50, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(100, 0, -1250), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(150, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(190, 0, -1250), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(240, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(290, 0, -1250), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(150, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(190, 0, -1250), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(240, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(290, 0, -1250), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(340, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(380, 0, -1250), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(430, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(480, 0, -1250), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(530, 0, -1250), XMFLOAT3(0, -90, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(570, 0, -1250), XMFLOAT3(0, 90, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(620, 0, -1250), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, -1250), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, -1210), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, -1160), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, -1110), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, -1060), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, -1020), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, -970), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, -920), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, -870), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, -830), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, -780), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, -730), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, -680), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, -640), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, -590), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, -540), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, -490), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, -450), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, -400), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, -350), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, -300), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, -260), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, -210), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(630, 0, -160), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(630, 0, -110), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(630, 0, -70), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(630, 0, -20), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(400, 0, -1250), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(400, 0, -1210), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(400, 0, -1160), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(400, 0, -1110), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(400, 0, -1060), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(400, 0, -1020), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(400, 0, -970), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(400, 0, -920), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(400, 0, -870), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(400, 0, -830), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(400, 0, -780), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(400, 0, -730), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(400, 0, -680), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(400, 0, -640), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(400, 0, -590), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(400, 0, -540), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(400, 0, -490), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(400, 0, -450), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(400, 0, -400), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(400, 0, -350), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(400, 0, -300), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(400, 0, -260), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(400, 0, -210), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(400, 0, -160), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRockWall1(XMFLOAT3(400, 0, -110), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall2(XMFLOAT3(400, 0, -70), XMFLOAT3(0, 180, 0));
		GameUtilities::GenerateRockWall3(XMFLOAT3(400, 0, -20), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRockWall4(XMFLOAT3(400, 0, 30), XMFLOAT3(0, 0, 0));

		Building* rock_wall1 = new Building(XMFLOAT3(380, 0, 80), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateRockWall1, "Rubble");
		Building* rock_wall2 = new Building(XMFLOAT3(360, 0, 100), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateRockWall2, "Rubble");
		Building* rock_wall3 = new Building(XMFLOAT3(340, 0, 120), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateRockWall3, "Rubble");
		Building* rock_wall4 = new Building(XMFLOAT3(320, 0, 140), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 25, 35), XMFLOAT3(0, 1, 0), GameUtilities::GenerateRockWall1, "Rubble");

		GameUtilities::AddGameObject(rock_wall1);
		GameUtilities::AddGameObject(rock_wall2);
		GameUtilities::AddGameObject(rock_wall3);
		GameUtilities::AddGameObject(rock_wall4);

		Building* rock_wall5 = new Building(XMFLOAT3(300, 0, 160), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateRockWall1, "Rubble");
		Building* rock_wall6 = new Building(XMFLOAT3(280, 0, 180), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateRockWall2, "Rubble");
		Building* rock_wall7 = new Building(XMFLOAT3(260, 0, 200), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateRockWall3, "Rubble");
		Building* rock_wall8 = new Building(XMFLOAT3(240, 0, 220), XMFLOAT3(0, -30, 0), XMFLOAT3(25, 25, 35), XMFLOAT3(0, 1, 0), GameUtilities::GenerateRockWall1, "Rubble");

		GameUtilities::AddGameObject(rock_wall5);
		GameUtilities::AddGameObject(rock_wall6);
		GameUtilities::AddGameObject(rock_wall7);
		GameUtilities::AddGameObject(rock_wall8);

		
#pragma endregion







		ADUI::UIMessage eventUIMessage;
		eventUIMessage.targetID = 0;
		eventUIMessage.externalMsg = true;
		eventUIMessage.commandID = 1;
		MessageTrigger* tb = GameUtilities::AddMessageTrigger(eventUIMessage, XMFLOAT3(500, 0, -900), XMFLOAT3(700, 50, 800), XMFLOAT3(0, 0, 0));
		tb->active = true;
		GameUtilities::AddGameObject(tb, false);
		eventUIMessage.targetID = 1;
		MessageTrigger* tb2 = GameUtilities::AddMessageTrigger(eventUIMessage, XMFLOAT3(500, 0, -600), XMFLOAT3(700, 50, 800), XMFLOAT3(0, 0, 0));
		tb2->active = true;
		GameUtilities::AddGameObject(tb2, false);
		eventUIMessage.targetID = 2;
		MessageTrigger* tb3 = GameUtilities::AddMessageTrigger(eventUIMessage, XMFLOAT3(500, 0, -200), XMFLOAT3(700, 50, 800), XMFLOAT3(0, 0, 0));
		tb3->active = true;
		GameUtilities::AddGameObject(tb3, false);
		//eventUIMessage.targetID = 3;
		//MessageTrigger* tb4 = GameUtilities::AddMessageTrigger(eventUIMessage, XMFLOAT3(500, 0, -600), XMFLOAT3(10, 10, 10), XMFLOAT3(0, 0, 0));
		//tb4->active = true;
		//GameUtilities::AddGameObject(tb4, false);
		//eventUIMessage.targetID = 4;
		//MessageTrigger* tb5 = GameUtilities::AddMessageTrigger(eventUIMessage, XMFLOAT3(500, 0, -500), XMFLOAT3(10, 10, 10), XMFLOAT3(0, 0, 0));
		//tb5->active = true;
		//GameUtilities::AddGameObject(tb5, false);
		//eventUIMessage.targetID = 5;
		//MessageTrigger* tb6 = GameUtilities::AddMessageTrigger(eventUIMessage, XMFLOAT3(500, 0, -400), XMFLOAT3(10, 10, 10), XMFLOAT3(0, 0, 0));
		//tb6->active = true;		
		//GameUtilities::AddGameObject(tb6, false);

		ResourceManager::FinalizedStatics();



		XMFLOAT2 mapDimensions = { 1200 , 2400 };
		float minionWidth = 12;

		ADAI::ADPathfinding::Instance()->Initialize(&planeModel->vertices, mapDimensions, minionWidth, 0.f, &forcedCollisions);

		if (!engine->Initialize())
		{
			return;
		}
		game->LoadGameUserInterface(engine->GetUI(), audioEngine);

		//---Run		
		// String shit
		std::string fr; std::wstring tfw; const wchar_t* wchar;
		game_time.Restart();
		ADEvents::ADEventSystem::Instance()->SendEvent("PlayTitle", (void*)0);
		clampArgs->tree = tree;
		clampArgs->golem = golem;
		clampArgs->time = delta_time;
		clampArgs->stone = &stoneMinions;
		clampArgs->water = &waterMinions;
		clampArgs->fire = &fireMinions;
		clampArgs->wood = &woodMinions;
		aiArgs->scene = &currentScene;
		while (!shutdown)
		{
			promise<void> physicspromise;
			promise<void> scenepromise;
			promise<void> clamppromise;
			future<void> physicsfuture = physicspromise.get_future();
			future<void> scenefuture = scenepromise.get_future();
			future<void> clampfuture = clamppromise.get_future();
			aiArgs->scenePromise = &scenepromise;
			clampArgs->clamppromise = &clamppromise;
			game_time.Signal();
			delta_time = static_cast<float>(game_time.SmoothDelta());
			timer += delta_time;

			ProcessInput();
			ADEvents::ADEventSystem::Instance()->ProcessEvents();

			ADQuad collisionBoundary(0, 0, mapDimensions.x, mapDimensions.y);
			QuadTree<int>* collisionTree = new QuadTree<int>(collisionBoundary);
			PhysicsArguments->collisionTree = collisionTree;
			PhysicsArguments->physicspromise = &physicspromise;

			int OBJ_COUNT = ResourceManager::GetGameObjectCount();
			PhysicsArguments->OBJ_COUNT = OBJ_COUNT;
			ADResource::ADGameplay::GameObject** OBJS = ResourceManager::GetGameObjectPtr();
			PhysicsArguments->OBJS = OBJS;


			physics_timer += delta_time;
			if (physics_timer > physics_rate)
			{
				physics_timer = 0;
				//--------------------------------------------------------------------------------------------------------
				/*for (int i = 0; i < OBJ_COUNT; i++)
				{
					for (unsigned int j = 0; j < OBJ_COUNT; j++)
					{
						if (i != j)
						{
							if (OBJS[i]->colliderPtr != nullptr && OBJS[j]->colliderPtr != nullptr)
							{
								if (!OBJS[i]->colliderPtr->trigger || !OBJS[j]->colliderPtr->trigger)
								{
									if (OBJS[i]->colliderPtr->type != ColliderType::Plane || OBJS[j]->colliderPtr->type != ColliderType::Plane)
									{
										if (OBJS[i]->colliderPtr->type != ColliderType::Triangle || OBJS[j]->colliderPtr->type != ColliderType::Triangle)
										{
											OBJS[i]->CheckCollision(OBJS[j]);
										}
									}
								}
							}
						}
					}
				}*/

				//----------------------------------New Physics System-------------------------------
				thread = true;
				jobManagerTest->AddJob(&MainPhysics, (void*)PhysicsArguments, 0);

				/*for (int i = 0; i < 10; i++)
				{
					GroundClamping(stoneMinions[i], tree, delta_time);
					GroundClamping(waterMinions[i], tree, delta_time);
					GroundClamping(fireMinions[i], tree, delta_time);
					GroundClamping(woodMinions[i], tree, delta_time);
				}*/

			}

			//GroundClamping(golem, tree, delta_time);
			jobManagerTest->AddJob(&ClampingWrapper, (void*)clampArgs, 1);

			//ADAI::ADPathfinding::Instance()->UpdatePlayerNode(golem->GetPosition().x, golem->GetPosition().z, 3000, 3000);
			//golem->flockingGroups[commandTargetGroup

			// Debug draw
			//ResourceManager::GetModelPtrFromMeshId(golem_collider)->position = (*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position;

			//engine->GetOrbitCamera()->SetRadius(200);

			engine->GetOrbitCamera()->SetLookAtAndRotate((XMFLOAT3&)(Float3ToVector(golem->GetPosition()) + XMVectorSet(0, 15, 0, 1)), yaw, pitch, delta_time);
			XMMATRIX view;
			engine->GetOrbitCamera()->GetViewMatrix(view);


			golem->GetView(view);

			aiArgs->delta = engine->GetEngineDeltaTime();


			XMFLOAT3 CamPosition = engine->GetOrbitCamera()->GetPosition();
			audioEngine->Set3dListenerAndOrientation({ CamPosition.x, CamPosition.y, CamPosition.z });
			audioEngine->Update();

			XMFLOAT4X4 viewPass;
			XMStoreFloat4x4(&viewPass, view);
			XMFLOAT4 cpos = XMFLOAT4(CamPosition.x, CamPosition.y, CamPosition.z, 0);


			XMMATRIX pers = XMMatrixPerspectiveFovLH(engine->GetOrbitCamera()->GetFOV(), (Window->Bounds.Width / Window->Bounds.Height), engine->GetOrbitCamera()->GetNear(), engine->GetOrbitCamera()->GetFar());

			XMFLOAT4X4 persPass;
			XMStoreFloat4x4(&persPass, pers);


#ifdef _DEBUG

			//golemCollider->transform = golem->GetColliderInfo();

			///*house1Collider->transform = house1->GetColliderInfo();
			//house2Collider->transform = house2->GetColliderInfo();
			//house3Collider->transform = house3->GetColliderInfo();
			//house4Collider->transform = house4->GetColliderInfo();

			//barn1Collider->transform = barn1->GetColliderInfo();

			//barn2Collider->transform = barn2->GetColliderInfo();

			//gatewayCollider->transform = gateway->GetColliderInfo();

			//towerCollider->transform = tower->GetColliderInfo();*/

			//scaffoldingCollider->transform = scaffolding->GetColliderInfo();

			//wallCollider->transform = wall->GetColliderInfo();

			//wellCollider->transform = well->GetColliderInfo();

			//tavernCollider->transform = tavern->GetColliderInfo();

			//rockCollider->transform = rock->GetColliderInfo();

			//meeting_hallCollider->transform = meeting_hall->GetColliderInfo();

			//cartCollider->transform = cart->GetColliderInfo();

			//rubbleCollider1->transform = rubble1->GetColliderInfo();
			//rubbleCollider2->transform = rubble2->GetColliderInfo();
			//rubbleCollider3->transform = rubble3->GetColliderInfo();
#endif


			//Did this to represent layers, Triggers won't collider with other triggers so there is no need to test them
			//This is just tmporary code for a simple collision layer loop, this will be slow but multithreading should help

			//	Works the exact same as the commented code above
			
			//jobManagerTest->AddJob(&AiUpdateWrapper, (void*)aiArgs, 0);
			//GroundClamping(cube, tree, delta_time);
			//cube->transform.r[3].m128_f32[1] += 5;

			// Poll input
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// D3d11 shit
			clampfuture.wait();
			if (thread)
			{
				physicsfuture.wait();
				thread = false;
			}
			if (!engine->Update(delta_time)) break;
			currentScene.Update(engine->GetEngineDeltaTime());
			if (!engine->Render()) break;

			//scenefuture.wait();
			collisionTree->Shutdown();

			// Update framerate
			if (timer > 1)
			{
				timer = 0;

				fr = std::to_string(1.0 / delta_time) + " FPS";
				tfw = std::wstring(fr.begin(), fr.end());
				wchar = tfw.c_str();
				ApplicationView::GetForCurrentView()->Title = ref new String(wchar);
			}
		}
		audioEngine->Shutdown();
		engine->ShutDown();
		tree->Shutdown();
	}
	virtual void Uninitialize() {}

	// an "event" that is called when the application window is ready to be activated
	void OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
		SystemNavigationManager::GetForCurrentView()->BackRequested += ref new EventHandler<BackRequestedEventArgs^>(this, &App::OnBackRequested);
	}

	void OnBackRequested(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ e)
	{
		e->Handled = true;
	}

	void ProcessInput()
	{
		static float camera_rotation_thresh = 250;
		float dt = delta_time;

		if (Input::ControllerPresent())
		{
			// Camera rotation
			if (Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::RIGHT_THUMBSTICK) == (int)Input::DIRECTION::RIGHT)
			{
				yaw += camera_rotation_thresh * dt;
			}
			else if (Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::RIGHT_THUMBSTICK) == (int)Input::DIRECTION::LEFT)
			{
				yaw += -camera_rotation_thresh * dt;
			}
			if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::RIGHT_THUMBSTICK) == (int)Input::DIRECTION::UP)
			{
				pitch += -camera_rotation_thresh * dt;
			}
			else if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::RIGHT_THUMBSTICK) == (int)Input::DIRECTION::DOWN)
			{
				pitch += camera_rotation_thresh * dt;
			}
			pitch = clamp(pitch, 0, 45);
			if (Input::QueryButtonDown(GamepadButtons::LeftThumbstick) && Input::QueryButtonDown(GamepadButtons::RightThumbstick))
			{
				shutdown = true;
			}
		}
	}

	void OnKeyDown(CoreWindow^ Window, KeyEventArgs^ args)
	{
		bool enabled = false;

		if (enabled)
		{
			static float camera_movement_thresh = 5;
			static float camera_rotation_thresh = 500;

			float dt = delta_time;

			if (args->VirtualKey == VirtualKey::Escape)
			{
				shutdown = true;
			}
			if (args->VirtualKey == VirtualKey::W || args->VirtualKey == VirtualKey::GamepadLeftThumbstickUp)
			{
			}
			else if (args->VirtualKey == VirtualKey::S || args->VirtualKey == VirtualKey::GamepadLeftThumbstickDown)
			{
			}
			else if (args->VirtualKey == VirtualKey::A || args->VirtualKey == VirtualKey::GamepadLeftThumbstickLeft)
			{
			}
			else if (args->VirtualKey == VirtualKey::D || args->VirtualKey == VirtualKey::GamepadLeftThumbstickRight)
			{
			}

			if (args->VirtualKey == VirtualKey::Left || args->VirtualKey == VirtualKey::GamepadRightThumbstickRight)
			{
				yaw += camera_rotation_thresh * dt;
			}
			else if (args->VirtualKey == VirtualKey::Right || args->VirtualKey == VirtualKey::GamepadRightThumbstickLeft)
			{
				yaw += -camera_rotation_thresh * dt;
			}

			if (args->VirtualKey == VirtualKey::Up || args->VirtualKey == VirtualKey::GamepadRightThumbstickDown)
			{
				pitch += camera_rotation_thresh * dt;
			}
			else if (args->VirtualKey == VirtualKey::Down || args->VirtualKey == VirtualKey::GamepadRightThumbstickUp)
			{
				pitch += -camera_rotation_thresh * dt;
			}
		}
	}
};

// the class definition that creates an instance of our core framework class
ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new App();    // create an App class and return it
	}
};
[MTAThread]    // define main() as a multi-threaded-apartment function

// the starting point of all programs
int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());    // create and run a new AppSource class
	return 0;
}