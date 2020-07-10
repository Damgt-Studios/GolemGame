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

//#define ShowColliders

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

//struct ClampingArgs
//{
//	GameObject* golem;
//	QuadTree<ADPhysics::Triangle>* tree;
//	float time;
//};

struct whateverargs
{
	int a, b;
	std::promise<int>* jobpromise;
};

void dumbassMath(void* arg, int index)
{
	whateverargs* penis = static_cast<whateverargs*>(arg);

	penis->jobpromise->set_value(penis->a + penis->b);

}

// the class definition for the core "framework" of our app
ref class App sealed : public IFrameworkView
{
private:
	Jobs::JobManager* jobManagerTest;
	Engine* engine;
	TheGreatGolem* game;
	AD_AUDIO::ADAudio* audioEngine;
	ADResource::ADGameplay::Golem* golem;
	ADGameplay::Scene currentScene;
	//MinionManager* minionManager;
	AD_ULONG golem_collider = 0;
	//FountainEmitter femitter;

	bool shutdown = false;

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
	void* testingthread;
	ClampingArgs* clampArgs;

	// Physics
	ADPhysics::AABB test_colider;
	ADPhysics::AABB test_colider1;
	ADPhysics::Plane test_plane;


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

		//Initialize.  Order Matters.
		audioEngine->Init();
		game->LoadGameAudio(audioEngine);
		game->Initialize();
		jobManagerTest = Jobs::JobManager::GetInstance();
		clampArgs = new ClampingArgs();

		/*vector<promise<int>> promises;
		vector<future<int>> futures;
		vector<whateverargs*> peni;
		for (int i = 0; i < 20; i++)
		{
			whateverargs* superpenis = new whateverargs();
			superpenis->a = i + 1;
			superpenis->b = i + 4;
			promises.push_back(std::promise<int>());
			futures.push_back(std::future<int>());
			peni.push_back(superpenis);
		}

		for (int i = 0; i < 20; i++)
		{
			futures[i] = promises[i].get_future();
			peni[i]->jobpromise = &promises[i];
			jobManagerTest->AddJob(&dumbassMath, (void*)(peni[i]), 0);
		}

		for (int i = 0; i < 20; i++)
		{
			futures[i].wait();
		}*/

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
		currentScene.LoadScene("files/scenes/TestScene.scenery");
		golem = currentScene.GetGolem();
		game->LoadListeners(golem, &currentScene);
		engine->GetOrbitCamera()->SetLookAt((XMFLOAT3&)(Float3ToVector((*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position)));
		engine->GetOrbitCamera()->SetRadius(20);
		engine->GetOrbitCamera()->Rotate(yaw, pitch);
		golem->bigPuffs[STONE] = &engine->bigStonePuff;
		golem->bigPuffs[WATER] = &engine->bigWaterPuff;
		golem->bigPuffs[FIRE] = &engine->bigFirePuff;
		golem->bigPuffs[WOOD] = &engine->bigWoodPuff;



		std::vector<std::string> bucketheadanims;
		bucketheadanims.push_back("files/models/Bucket_Fear.animfile");



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

#ifdef ShowColliders
		GameUtilities::AddGameObject(minionCollider);

		GameUtilities::AddGameObject(golemCollider);
		GameUtilities::AddGameObject(house1Collider);
		GameUtilities::AddGameObject(house2Collider);
		GameUtilities::AddGameObject(house3Collider);
		GameUtilities::AddGameObject(house4Collider);
		GameUtilities::AddGameObject(barn1Collider);
		GameUtilities::AddGameObject(barn2Collider);
		GameUtilities::AddGameObject(gatewayCollider);
		GameUtilities::AddGameObject(towerCollider);
		GameUtilities::AddGameObject(scaffoldingCollider);
		GameUtilities::AddGameObject(wallCollider);
		GameUtilities::AddGameObject(wellCollider);
		GameUtilities::AddGameObject(tavernCollider);
		GameUtilities::AddGameObject(rockCollider);
		GameUtilities::AddGameObject(meeting_hallCollider);
		GameUtilities::AddGameObject(cartCollider);

		GameUtilities::AddGameObject(rubbleCollider1);
		GameUtilities::AddGameObject(rubbleCollider2);
		GameUtilities::AddGameObject(rubbleCollider3);

#endif
#endif

		//ADAI::FlockingGroup commandFlock;
		//ADAI::FlockingGroup idleFlock;

		std::vector<Destructable*> stoneMinions;
		std::vector<Destructable*> waterMinions;
		std::vector<Destructable*> fireMinions;
		std::vector<Destructable*> woodMinions;

		currentScene.GetMinions(&stoneMinions, &waterMinions, &fireMinions, &woodMinions);

		//Renderable* m1 = GameUtilities::AddSimpleAsset("files/models/Target.mesh", "files/textures/Target.mesh", XMFLOAT3(0, 5, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));

		//Destructable* m1 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", stoneMinionAnimations, XMFLOAT3(0, 5, 0), XMFLOAT3(0.02f, 1.02f, 0.02f), XMFLOAT3(0, 0, 0));
		//Destructable* m2 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", minionAnimations, XMFLOAT3(50, 5, 30), XMFLOAT3(0.02f, 0.02f, 0.02f), XMFLOAT3(0, 0, 0));

		//golem->targetMarker = m1;
		//golemEatingEvent.SetTarget(golem);

		//m1->colliderPtr = nullptr;

		//animationFiles[0] = "files/models/BattleMage.animfile";



		Renderable* tempPlane = currentScene.GetPlane();
		GameUtilities::AddGameObject(tempPlane);

#pragma region PhysicsSetup

		//Physics Setup--------------------------------
		SimpleModel** tempPlaneModel = ResourceManager::GetSimpleModelPtrFromMeshId(tempPlane->GetMeshId());
		SimpleStaticModel* planeModel = static_cast<SimpleStaticModel*>(*tempPlaneModel);
		std::vector<ADPhysics::Triangle> ground;
		std::vector<ADQuadTreePoint<ADPhysics::Triangle>> treePoints;
		XMMATRIX groundWorld = XMMatrixIdentity();
		tempPlane->GetWorldMatrix(groundWorld);
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


		// Construct physics stuff
		//test_colider1 = ADPhysics::AABB(XMFLOAT3(0, 5, 15), XMFLOAT3(2, 2, 2));
		//test_plane = ADPhysics::Plane(XMMatrixTranslation(0, -5, 0), XMFLOAT3(10, 0, 10));

		//Needed to add this to the colliders for the collision queue

		/*c2->colliderPtr = &test_colider1;
		c2->type = OBJECT_TYPE::STATIC;*/

#pragma endregion


		Building* house1 = new Building(XMFLOAT3(-500, 0, 100), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0.5f, 0.15), GameUtilities::GenerateHouse1, "House1");
		GameUtilities::AddGameObject(house1);
		currentScene.AddBuilding(house1);
		house1->destructionEmitter = &engine->destructionCloud;
		house1->destructionEmitter2 = &engine->destructionCloud2;

		Building* rubble1 = new Building(XMFLOAT3(-500, 0, 100), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble1, "Rubble");
		GameUtilities::AddGameObject(rubble1);
		house1->SetRubble(rubble1);


		for (int i = 0; i < 10; i++)
		{
			Building* housey = new Building(XMFLOAT3(-200 + (i * 33), 0, -100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateHouse1, "House1");
			GameUtilities::AddGameObject(housey);
			currentScene.AddBuilding(housey);
			housey->destructionEmitter = &engine->destructionCloud;
			housey->destructionEmitter2 = &engine->destructionCloud2;

			Building* rubble2 = new Building(XMFLOAT3(-200 + (i * 33), 0, -100), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble2, "Rubble");
			GameUtilities::AddGameObject(rubble2);
			housey->SetRubble(rubble2);

		}


		//Building* house2 = new Building(XMFLOAT3(-500, 0, 30), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 35), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse2);
		//GameUtilities::AddGameObject(house2);
		//currentScene.AddBuilding(house2);
		//
		//Building* house3 = new Building(XMFLOAT3(-500, 0, -160), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 30), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse3);
		//GameUtilities::AddGameObject(house3);
		//currentScene.AddBuilding(house3);

		//Building* house4 = new Building(XMFLOAT3(-500, 0, -70), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse4);
		//GameUtilities::AddGameObject(house4);
		//currentScene.AddBuilding(house4);

		//Building* barn1 = new Building(XMFLOAT3(-500, 0, 250), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 37.5), XMFLOAT3(0.25f, 0.5f, 0), GameUtilities::GenerateBarn1);
		//GameUtilities::AddGameObject(barn1);
		//currentScene.AddBuilding(barn1);

		//Building* barn2 = new Building(XMFLOAT3(-500, 0, -370), XMFLOAT3(0, 0, 0), XMFLOAT3(20, 20, 20), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateBarn2);
		//GameUtilities::AddGameObject(barn2);
		//currentScene.AddBuilding(barn2);

		Building* tower = new Building(XMFLOAT3(115, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower, "Tower");
		//Building* tower = new Building(XMFLOAT3(-24.475, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower2 = new Building(XMFLOAT3(-6.75, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower3 = new Building(XMFLOAT3(-6.75, 0, -3.125), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower4 = new Building(XMFLOAT3(0.5, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower5 = new Building(XMFLOAT3(13.875, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		GameUtilities::AddGameObject(tower);
		currentScene.AddBuilding(tower);
		//GameUtilities::AddGameObject(tower2);
		//GameUtilities::AddGameObject(tower3);
		//GameUtilities::AddGameObject(tower4);
		//GameUtilities::AddGameObject(tower5);

		Building* gateway = new Building(XMFLOAT3(330, 0, -330), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway, "Gate");
		GameUtilities::AddGameObject(gateway);

		Building* scaffolding = new Building(XMFLOAT3(410, 0, -400), XMFLOAT3(0, 90, 0), XMFLOAT3(5, 25, 25), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateScaffoldWallX, "Scaffolding");
		GameUtilities::AddGameObject(scaffolding);

		//1.625 difference
		Building* wall = new Building(XMFLOAT3(700, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall, "Wall");
		//Building* wall = new Building(XMFLOAT3(-23.475, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		///*Building* wall2 = new Building(XMFLOAT3(-20.25, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall3 = new Building(XMFLOAT3(-18.625, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall4 = new Building(XMFLOAT3(-17, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall5 = new Building(XMFLOAT3(-15.375, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall6 = new Building(XMFLOAT3(-12.125, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall7 = new Building(XMFLOAT3(-10.5, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall8 = new Building(XMFLOAT3(-8.875, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall9 = new Building(XMFLOAT3(-7.25, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall10 = new Building(XMFLOAT3(-6.75, 0, -2.125), XMFLOAT3(0, 90, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall11 = new Building(XMFLOAT3(1.5, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall12 = new Building(XMFLOAT3(3.125, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall13 = new Building(XMFLOAT3(4.75, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall14 = new Building(XMFLOAT3(6.375, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall15 = new Building(XMFLOAT3(8, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall16 = new Building(XMFLOAT3(9.625, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall17 = new Building(XMFLOAT3(11.25, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall18 = new Building(XMFLOAT3(12.875, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);*/
		GameUtilities::AddGameObject(wall);
		//GameUtilities::AddGameObject(wall2);
		//GameUtilities::AddGameObject(wall3);
		//GameUtilities::AddGameObject(wall4);
		//GameUtilities::AddGameObject(wall5);
		//GameUtilities::AddGameObject(wall6);
		//GameUtilities::AddGameObject(wall7);
		//GameUtilities::AddGameObject(wall8);
		//GameUtilities::AddGameObject(wall9);
		//GameUtilities::AddGameObject(wall10);
		//GameUtilities::AddGameObject(wall11);
		//GameUtilities::AddGameObject(wall12);
		//GameUtilities::AddGameObject(wall13);
		//GameUtilities::AddGameObject(wall14);
		//GameUtilities::AddGameObject(wall15);
		//GameUtilities::AddGameObject(wall16);
		//GameUtilities::AddGameObject(wall17);
		//GameUtilities::AddGameObject(wall18);


		Building* well = new Building(XMFLOAT3(1500, 0, 1500), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 5, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell, "Rubble");
		GameUtilities::AddGameObject(well);

		Building* tavern = new Building(XMFLOAT3(-100, 0, 100), XMFLOAT3(0, 45, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern, "Tavern");
		GameUtilities::AddGameObject(tavern);
		currentScene.AddBuilding(tavern);

		Building* lamp = new Building(XMFLOAT3(-350, 0, 75), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 1.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Rubble");
		GameUtilities::AddGameObject(lamp);
		Building* lamp2 = new Building(XMFLOAT3(425, 0, 30), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 1.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp, "Rubble");
		GameUtilities::AddGameObject(lamp2);

		Building* rock = new Building(XMFLOAT3(-20, 0, 220), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRock1, "Rubble");
		GameUtilities::AddGameObject(rock);

		Building* meeting_hall = new Building(XMFLOAT3(100, 0, 360), XMFLOAT3(0, -135, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall, "House4");
		GameUtilities::AddGameObject(meeting_hall);
		currentScene.AddBuilding(meeting_hall);

		Building* cart = new Building(XMFLOAT3(-125, 0, 310), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5, 0.25, 0.1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateCart, "Cart");
		GameUtilities::AddGameObject(cart);

		for (int i = 0; i < 10; ++i)
		{
			Building* realtree = new Building(XMFLOAT3(RandFloat(i - 1000, i + 1000), 0, RandFloat(i - 1000, i + 1000)), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 3, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateTree, "Rubble");
			GameUtilities::AddGameObject(realtree);
		}



		//Building* rubble2 = new Building(XMFLOAT3(-220, 0, 330), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble2);
		//GameUtilities::AddGameObject(rubble2);

		//Building* rubble3 = new Building(XMFLOAT3(210, 0, 180), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble3);
		//GameUtilities::AddGameObject(rubble3);

		//Building* rubble4 = new Building(XMFLOAT3(130, 0, 650), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble1);
		//GameUtilities::AddGameObject(rubble4); 

		//Building* rubble5 = new Building(XMFLOAT3(-220, 0, 630), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble2);
		//GameUtilities::AddGameObject(rubble5);

		//Building* rubble6 = new Building(XMFLOAT3(210, 0, 680), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble3);
		//GameUtilities::AddGameObject(rubble6);


		ResourceManager::FinalizedStatics();



		XMFLOAT2 mapDimensions = { 3000 , 3000 };
		float minionWidth = 10;
		ADAI::ADPathfinding pathfinder;
		pathfinder.Initialize(&planeModel->vertices, mapDimensions, minionWidth, 20.f);

		if (!engine->Initialize())
		{
			return;
		}
		game->LoadGameUserInterface(engine->GetUI(), audioEngine, &pathfinder.tileMap);



		//rubble1->Remove();
		//rubble1->RemoveFromScene();

		//---Run		
		// String shit
		std::string fr; std::wstring tfw; const wchar_t* wchar;
		game_time.Restart();
		ADEvents::ADEventSystem::Instance()->SendEvent("PlayTitle", (void*)0);
		while (!shutdown)
		{
			game_time.Signal();
			delta_time = static_cast<float>(game_time.SmoothDelta());
			timer += delta_time;

			ProcessInput();
			ADEvents::ADEventSystem::Instance()->ProcessEvents();

			if (Input::QueryButtonDown(GamepadButtons::RightShoulder))
			{
				//minionManager->Instance()->BirthStoneMinion(golem->flockingGroups[0]);
				//pathfinder.ClearDebug();
				UINT srow;
				UINT scolumn;
				UINT drow;
				UINT dcolumn;
				pathfinder.tileMap.GetColumnRowFromPosition(XMFLOAT2(golem->GetPosition().x, golem->GetPosition().z), scolumn, srow);
				pathfinder.tileMap.GetColumnRowFromPosition(XMFLOAT2(golem->targetMarker->GetPosition().x, golem->targetMarker->GetPosition().z), dcolumn, drow);
				pathfinder.enter(scolumn, srow, dcolumn, drow);
			}
			if (!pathfinder.isDone())
			{
				pathfinder.update(0.001f);
			}
			pathfinder.UpdatePlayerNode(golem->GetPosition().x, golem->GetPosition().z, 3000, 3000);
			//golem->flockingGroups[commandTargetGroup

			// Debug draw
			//ResourceManager::GetModelPtrFromMeshId(golem_collider)->position = (*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position;

			//engine->GetOrbitCamera()->SetRadius(200);
			engine->GetOrbitCamera()->SetRadius(50);
			engine->GetOrbitCamera()->SetLookAtAndRotate((XMFLOAT3&)(Float3ToVector(golem->GetPosition()) + XMVectorSet(0, 15, 0, 1)), yaw, pitch, delta_time);
			XMMATRIX view;
			engine->GetOrbitCamera()->GetViewMatrix(view);
			golem->GetView(view);


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
			minionCollider->transform = XMMatrixScaling(20, 5, 20);
			minionCollider->transform.r[3] = { 20,10,20, 1 };

			//golemCollider->transform = golem->GetColliderInfo();

			/*house1Collider->transform = house1->GetColliderInfo();
			house2Collider->transform = house2->GetColliderInfo();
			house3Collider->transform = house3->GetColliderInfo();
			house4Collider->transform = house4->GetColliderInfo();

			barn1Collider->transform = barn1->GetColliderInfo();

			barn2Collider->transform = barn2->GetColliderInfo();

			gatewayCollider->transform = gateway->GetColliderInfo();

			towerCollider->transform = tower->GetColliderInfo();*/

			//scaffoldingCollider->transform = scaffolding->GetColliderInfo();

			//wallCollider->transform = wall->GetColliderInfo();

			//wellCollider->transform = well->GetColliderInfo();

			//tavernCollider->transform = tavern->GetColliderInfo();

			//rockCollider->transform = rock->GetColliderInfo();

			//meeting_hallCollider->transform = meeting_hall->GetColliderInfo();

			//cartCollider->transform = cart->GetColliderInfo();

			rubbleCollider1->transform = rubble1->GetColliderInfo();
			//rubbleCollider2->transform = rubble2->GetColliderInfo();
			//rubbleCollider3->transform = rubble3->GetColliderInfo();
#endif


			//Did this to represent layers, Triggers won't collider with other triggers so there is no need to test them
			//This is just tmporary code for a simple collision layer loop, this will be slow but multithreading should help

			//	Works the exact same as the commented code above
			ADQuad collisionBoundary(0, 0, mapDimensions.x, mapDimensions.y);
			QuadTree<int>* collisionTree = new QuadTree<int>(collisionBoundary);

			int OBJ_COUNT = ResourceManager::GetGameObjectCount();
			ADResource::ADGameplay::GameObject** OBJS = ResourceManager::GetGameObjectPtr();


			physics_timer += delta_time;
			if (physics_timer > physics_rate)
			{
				physics_timer = 0;
				/*for (int i = 0; i < OBJ_COUNT; i++)
				{

					if (OBJS[i]->colliderPtr)
					{
						int* index = new int(i);
						if (!collisionTree->Insert(ADQuadTreePoint<int>(OBJS[i]->colliderPtr->Pos.x, OBJS[i]->colliderPtr->Pos.z, *index)))
						{
							int somethingswrong = 0;
							somethingswrong++;
						}
					}
				}

				for (unsigned int i = 0; i < OBJ_COUNT; i++)
				{
					if (OBJS[i]->colliderPtr)
					{
						XMFLOAT3 obj_pos = VectorToFloat3(OBJS[i]->transform.r[3]);
						std::vector<ADQuadTreePoint<int>> collisionVector = collisionTree->Query(ADQuad(obj_pos.x, obj_pos.z, OBJS[i]->colliderPtr->GetWidth()+2, OBJS[i]->colliderPtr->GetWidth()+2));

						for (unsigned int j = 0; j < collisionVector.size(); j++)
						{
							if (OBJS[*collisionVector[j].data]->colliderPtr)
								OBJS[i]->CheckCollision(OBJS[*collisionVector[j].data]);
						}
					}
				}*/
				//----------------------------------------------------------------------------------------------------
				//for (unsigned int i = 0; i < OBJ_COUNT; i++)
				//{
				//	if (OBJS[i]->colliderPtr)
				//	{
				//		XMFLOAT3 obj_pos = VectorToFloat3(OBJS[i]->transform.r[3]);
				//		std::vector<ADQuadTreePoint<int>> collisionVector = collisionTree->Query(ADQuad(obj_pos.x, obj_pos.z, 50, 50));

				//		for (unsigned int j = 0; j < collisionVector.size(); j++)
				//		{
				//			if (OBJS[*collisionVector[j].data]->colliderPtr)
				//				OBJS[i]->CheckCollision(OBJS[*collisionVector[j].data]);
				//		}
				//	}
				//}
				//--------------------------------------------------------------------------------------------------------
				for (int i = 0; i < OBJ_COUNT; i++)
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
				}
				//----------------------------------New Physics system does not operate with properly with triggers-------------------------------
				//for (int i = 0; i < OBJ_COUNT; i++)
				//{
				//	if (OBJS[i]->colliderPtr)
				//	{
				//		int* index = new int(i);
				//		if (!collisionTree->Insert(ADQuadTreePoint<int>(OBJS[i]->colliderPtr->Pos.x, OBJS[i]->colliderPtr->Pos.z, *index))) 
				//		{
				//			int somethingswrong = 0;
				//			somethingswrong++;
				//		}
				//	}
				//}

				//for (unsigned int i = 0; i < OBJ_COUNT; i++)
				//{
				//	if (OBJS[i]->colliderPtr && !OBJS[i]->colliderPtr->trigger) 
				//	{
				//		XMFLOAT3 obj_pos = VectorToFloat3(OBJS[i]->transform.r[3]);
				//		std::vector<ADQuadTreePoint<int>> collisionVector = collisionTree->Query(ADQuad(obj_pos.x, obj_pos.z, 50, 50));

				//		for (unsigned int j = 0; j < collisionVector.size(); j++)
				//		{
				//			if (OBJS[*collisionVector[j].data]->colliderPtr)
				//				OBJS[i]->CheckCollision(OBJS[*collisionVector[j].data]);
				//		}
				//	}
				//}
				//---------------------------------------------End New Physics System------------------------------------------

				//Resolve all collisions that occurred this frame
				//ADResource::ADGameplay::ResolveCollisions();
				jobManagerTest->AddJob(&ADResource::ADGameplay::ResolveCollisionsWrapper, testingthread, 0);

				for (int i = 0; i < 10; i++)
				{
					GroundClamping(stoneMinions[i], tree, delta_time);
					GroundClamping(waterMinions[i], tree, delta_time);
					GroundClamping(fireMinions[i], tree, delta_time);
					GroundClamping(woodMinions[i], tree, delta_time);
				}

			}

			clampArgs->golem = golem;
			clampArgs->tree = tree;
			clampArgs->time = delta_time;
			

			//GroundClamping(golem, tree, delta_time);
			jobManagerTest->AddJob(&ADResource::ADGameplay::GroundClampingWrapper, (void*)clampArgs, 0);
			//GroundClamping(cube, tree, delta_time);
			//cube->transform.r[3].m128_f32[1] += 5;

			// Poll input
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// D3d11 shit
			if (!engine->Update()) break;
			currentScene.Update(engine->GetEngineDeltaTime());
			if (!engine->Render()) break;

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

			if (Input::QueryButtonDown(GamepadButtons::LeftThumbstick) && Input::QueryButtonDown(GamepadButtons::RightThumbstick))
			{
				shutdown = true;
			}
			else if (Input::QueryButtonDown(GamepadButtons::RightThumbstick))
			{
				yaw = default_yaw;
				pitch = default_pitch;
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