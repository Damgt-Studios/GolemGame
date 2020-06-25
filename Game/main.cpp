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
#include "MinionManager.h"
#include <Scene.h>

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


// the class definition for the core "framework" of our app
ref class App sealed : public IFrameworkView
{
private:
	Engine* engine;
	TheGreatGolem* game;
	AD_AUDIO::ADAudio* audioEngine;
	ADResource::ADGameplay::Golem* golem;
	ADGameplay::Scene currentScene;
	//MinionManager* minionManager;
	AD_ULONG golem_collider = 0;
	FountainEmitter femitter;

	bool shutdown = false;

	// Timing
	XTime game_time;
	float timer = 0;
	float delta_time = 0;

	const float physics_rate = 0.2f;
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
		femitter.Initialize(engine->GetPBRRenderer()->GetRendererResources()->device.Get(), 100, XMFLOAT4(1, 1, 1, 1), L"files\\textures\\Particle_Essence.dds", 9999999);
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

		//golem = GameUtilities::LoadGolemFromModelFile("files/models/Golem_1.AnimMesh", "files/textures/Golem_1.mat", XMFLOAT3(10, 0.00001, 10), XMFLOAT3(0.1, 0.1, 0.1), XMFLOAT3(0, 0, 0));
		//minionManager->Instance()->Initialize(golem);

		// Orbit camera
		engine->SetCamera(XMFLOAT3(0, 10000.0f, -100.0f), 0, 0, 45);


		currentScene.LoadScene("files/scenes/test.scene");
		golem = currentScene.GetGolem();
		game->LoadListeners(golem, &currentScene);
		engine->GetOrbitCamera()->SetLookAt((XMFLOAT3&)(Float3ToVector((*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position)));
		engine->GetOrbitCamera()->SetRadius(20);
		engine->GetOrbitCamera()->Rotate(yaw, pitch);




		if (!engine->Initialize())
		{
			return;
		}
		game->LoadGameUserInterface(engine->GetUI(), audioEngine);

		LoadGameEmitters();
		//minionManager stuff
		//currentScene.GetMinions(&stoneMinions, &waterMinions, &fireMinions, &woodMinions);


		float mapWidth = 1000;
		float mapHeight = 1000;
		Renderable* tempPlane = currentScene.GetPlane();

		GameUtilities::AddGameObject(dynamic_cast<GameObject*>(golem));
		//GameUtilities::AddGameObject(m1);
		GameUtilities::AddGameObject(tempPlane);

		//ALl Static Models and shit before pathfinder
#pragma region Testing

		//-------------Testing Stuff
		//float mapWidth = 1000;
		//float mapLength = 1000;
		//Renderable* tempPlane = GameUtilities::AddSimpleAsset("files/models/Ground.mesh", "files/textures/Ground.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(mapWidth, 100, mapLength), XMFLOAT3(0, 0, 0));
		//GameUtilities::AddGameObject(tempPlane);


//		Renderable* cube = GameUtilities::AddSimpleAsset("files/models/Cube.mesh", "files/textures/Ground.mat", XMFLOAT3(0, 1, 10), XMFLOAT3(10, 10, 10), XMFLOAT3(0, 0, 0));
//#ifdef _DEBUG
//		Renderable* golemCollider = GameUtilities::AddRenderableCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(0.1, 0.1, 0.1), XMFLOAT3(0, 0, 0));
//		Renderable* cubeCollider = GameUtilities::AddRenderableCollider(XMFLOAT3(0, 1, 10), XMFLOAT3(10, 10, 10), XMFLOAT3(0, 0, 0));
//		cubeCollider->colliderPtr = nullptr;
//#endif
//		//GameUtilities::AddGameObject(cube);

		/*
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
		c9->physicsType = ADResource::ADGameplay::STATIC;*/


		/*GameUtilities::GenerateHouse1(XMFLOAT3(0, 0, 0), XMFLOAT3(0,45,0));

		GameUtilities::GenerateHouse2(XMFLOAT3(0, 0, -2.5), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateHouse3(XMFLOAT3(0, 0, -5), XMFLOAT3(0, -45, 0));

		GameUtilities::GenerateHouse4(XMFLOAT3(0, 0, -7.5), XMFLOAT3(0, 45, 0));

		GameUtilities::GenerateBarn1(XMFLOAT3(-10, 0, 0), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateBarn2(XMFLOAT3(-10, 0, 2.5), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateGateway(XMFLOAT3(10, 0, 0), XMFLOAT3(0, -90, 0));

		GameUtilities::GenerateScaffolding(XMFLOAT3(10, 0, 1), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateWall(XMFLOAT3(10, 0, 5), XMFLOAT3(0, 45, 0));

		GameUtilities::GenerateWell(XMFLOAT3(-5, 0, 0), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateTavern(XMFLOAT3(-5, 0, 5), XMFLOAT3(0, 45, 0));

		GameUtilities::GenerateTree(XMFLOAT3(-2.5, 0, 2.5), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateLadder(XMFLOAT3(-2.5, 0, 0), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRock1(XMFLOAT3(-2, 0, 2), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateLamp(XMFLOAT3(2.5, 0, 2.5), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateMeetingHall(XMFLOAT3(5, 0, 5), XMFLOAT3(0, 225, 0));

		GameUtilities::GenerateTower(XMFLOAT3(2.5, 0, 0), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateCart(XMFLOAT3(-2.5, 0, 1), XMFLOAT3(0,0,0));

		GameUtilities::GenerateStraw(XMFLOAT3(-2.5, 0, 2), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateDebris1(XMFLOAT3(-2.5, 0, 5), XMFLOAT3(0, 0, 0));

		GameUtilities::GenerateRubble1(XMFLOAT3(0, 0, 5), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRubble2(XMFLOAT3(-2, 0, 5), XMFLOAT3(0, 0, 0));
		GameUtilities::GenerateRubble3(XMFLOAT3(2, 0, 5), XMFLOAT3(0, 0, 0));*/

#ifdef _DEBUG
#ifdef ShowColliders
		GameUtilities::AddGameObject(golemCollider);
		GameUtilities::AddGameObject(cubeCollider);

#endif
#endif
		//-Testing End---------------------------------
#pragma endregion

		float minionWidth = 10;
		ADAI::ADPathfinding pathfinder;
		//pathfinder.Initialize(&planeModel->vertices, XMFLOAT2(mapWidth, mapLength), minionWidth, 20.f);

#pragma region PhysicsSetup

		//Physics Setup--------------------------------
		SimpleModel** tempPlaneModel = ResourceManager::GetSimpleModelPtrFromMeshId(tempPlane->GetMeshId());
		std::vector<ADPhysics::Triangle> ground;
		std::vector<ADQuadTreePoint<ADPhysics::Triangle>> treePoints;
		XMMATRIX groundWorld = XMMatrixIdentity();
		SimpleStaticModel* planeModel = static_cast<SimpleStaticModel*>(*tempPlaneModel);
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

		//-------------------------------------


		// Construct physics stuff
		//test_colider1 = ADPhysics::AABB(XMFLOAT3(0, 5, 15), XMFLOAT3(2, 2, 2));
		//test_plane = ADPhysics::Plane(XMMatrixTranslation(0, -5, 0), XMFLOAT3(10, 0, 10));
		
		//Needed to add this to the colliders for the collision queue

		/*c2->colliderPtr = &test_colider1;
		c2->type = OBJECT_TYPE::STATIC;*/

#pragma endregion
		femitter.Activate(999999999, { 250,1,250,0 });
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
				//pathfinder.clearDebug();
				//UINT row;
				//UINT column;
				//pathfinder.tileMap.GetColumnRowFromPosition(XMFLOAT2(golem->GetPosition().x, golem->GetPosition().z), column, row);
				//pathfinder.enter(0, 0, column, row);
			}
			if (!pathfinder.isDone())
			{
				pathfinder.update(0.00001f);
			}
			pathfinder.UpdatePlayerNode(golem->GetPosition().x, golem->GetPosition().z, mapWidth, mapHeight);
		

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
			femitter.UpdateParticles(delta_time, viewPass, persPass, cpos);

//#ifdef _DEBUG
//			golemCollider->transform = golem->GetColliderInfo();
//			XMMATRIX colliderLocation = cube->transform;
//
//			cube->colliderPtr = &AABB(VectorToFloat3(colliderLocation.r[3]), XMFLOAT3(1, 1, 1));
//			cube->physicsType = COLLIDABLE;
//			cubeCollider->transform = colliderLocation;
//#endif

			//Did this to represent layers, Triggers won't collider with other triggers so there is no need to test them
			//This is just tmporary code for a simple collision layer loop, this will be slow but multithreading should help

		//	Works the exact same as the commented code above
			int OBJ_COUNT = ResourceManager::GetGameObjectCount();
			ADResource::ADGameplay::GameObject** OBJS = ResourceManager::GetGameObjectPtr();

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

			//Resolve all collisions that occurred this frame
			ADResource::ADGameplay::ResolveCollisions();

			physics_timer += delta_time;
			if (physics_timer > physics_rate)
			{
				physics_timer = 0;
				for (int i = 0; i < 10; i++)
				{
					//GroundClamping(stoneMinions[i], tree, delta_time);
					//GroundClamping(waterMinions[i], tree, delta_time);
					//GroundClamping(fireMinions[i], tree, delta_time);
					//GroundClamping(woodMinions[i], tree, delta_time);
				}
			}

			GroundClamping(golem, tree, delta_time);
			//GroundClamping(cube, tree, delta_time);
			//cube->transform.r[3].m128_f32[1] += 5;

			// Poll input
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// D3d11 shit
			if (!engine->Update()) break;
			currentScene.Update(engine->GetEngineDeltaTime()); 
			if (!engine->Render()) break;
			femitter.RenderParticles(engine->GetPBRRenderer()->GetRendererResources()->context.Get());

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
	}
	virtual void Uninitialize() {}

	// an "event" that is called when the application window is ready to be activated
	void OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
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