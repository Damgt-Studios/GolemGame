// Include the precompiled headers
#include "pchgame.h"
#include <vector>
#include <windows.gaming.input.h>
#include <collection.h>
#include "Engine.h"
#include "ADPhysics.h"
#include "ADQuadTree.h"

#include "ADUserInterface.h"
#include "GameUserInterface.h"
//#include "AudioManager.h"
#include "ADAudio.h"
#include "GameUtilities.h"
#include "GameObjectClasses.h"
#include "MeshLoader.h"
#include "ADAI.h"

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
	ADResource::ADGameplay::Golem* golem;
	AD_ULONG golem_collider = 0;

	//AudioManager* audio_manager;
	AD_ADUIO::ADAudio* audio;

	bool shutdown = false;

	// Temp music
	int effect_id;
	bool effect_triggered = false;

	// Timing
	XTime game_time;
	float timer = 0;
	float delta_time = 0;

	// Audio
	float main_music_loop_timer = 3;
	bool music_triggered = false;

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
		} else
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

	virtual void Run()
	{
		AD_ADUIO::ADAudio audioEngine;
		audioEngine.Init();

		AD_ADUIO::AudioSource titleMusic;
		titleMusic.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::MUSIC;
		titleMusic.engine = &audioEngine;
		titleMusic.personalVolume = 0.02f;
		titleMusic.soundName = "files\\audio\\Opening.mp3";
		titleMusic.LoadSound(false, true, true);


		audioEngine.LoadSound("files\\audio\\SFX_Gem_Collect.wav", true);
		//audioEngine.LoadSound("", );

		//std::vector<std::string> sfx;
		//sfx.push_back("files\\audio\\SFX_Gem_Collect.wav");
		//sfx.push_back("files\\audio\\SFX_Destructable_Break.wav");
		//sfx.push_back("files\\audio\\SFX_Enemy_Death.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Charging.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Death.wav");
		//sfx.push_back("files\\audio\\SFX_Player_FireBreath.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Glide.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Hurt.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Jump.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Land.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Object_Hit.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Running_Jump.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Walking.wav");
		//sfx.push_back("files\\audio\\SFX_Player_Water_Splash.wav");
		//audio_manager = new AudioManager;
		//audio_manager->Initialize("files\\audio\\main_theme.wav", sfx);
		

		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		// Create the engine
		engine = new Engine;

		// Initialize the engine
		engine->SetCamera(XMFLOAT3(0, 10000.0f, -100.0f), 0, 0, 45);

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

		std::vector<std::string> animations;
		animations.push_back("files/models/Golem_1_Idle.animfile");

		ResourceManager::AddSkybox("files/models/Skybox.mesh", "files/textures/Skybox.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(-10, -10, -10), XMFLOAT3(0, 0, 0));
		golem = GameUtilities::LoadGolemFromModelFile("files/models/Golem_1.AnimMesh", "files/textures/Golem_1.mat", animations, XMFLOAT3(0, 0.00001, 0), XMFLOAT3(0.1, 0.1, 0.1), XMFLOAT3(0, 0, 0));
		//golem->SetAudio(audio_manager);

		////////////////////////////////////
    //THis is the stuff for you.
	/*	ADAI::FlockingGroup commandFlock;
		ADAI::FlockingGroup idleFlock;

		Destructable* e2 = GameUtilities::AddDestructableFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(-30, 5, 30), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0, 0, 0));
		Destructable* e3 = GameUtilities::AddDestructableFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(-15, 5, -40), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0, 0, 0));
		Destructable* e4 = GameUtilities::AddDestructableFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(-5, 5, -40), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0, 0, 0));
		Destructable* e5 = GameUtilities::AddDestructableFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(5, 5, -40), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0, 0, 0));
		Destructable* e6 = GameUtilities::AddDestructableFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(15, 5, -40), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0, 0, 0));
		Destructable* e7 = GameUtilities::AddDestructableFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(30, 5, -40), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0, 0, 0));

		ADAI::AIUnit* ai1 = GameUtilities::AttachMinionAI(e2, &commandFlock, &idleFlock);
		ADAI::AIUnit* ai2 = GameUtilities::AttachMinionAI(e3, &commandFlock, &idleFlock);
		ADAI::AIUnit* ai3 = GameUtilities::AttachMinionAI(e4, &commandFlock, &idleFlock);
		ADAI::AIUnit* ai4 = GameUtilities::AttachMinionAI(e5, &commandFlock, &idleFlock);
		ADAI::AIUnit* ai5 = GameUtilities::AttachMinionAI(e6, &commandFlock, &idleFlock);
		ADAI::AIUnit* ai6 = GameUtilities::AttachMinionAI(e7, &commandFlock, &idleFlock);

		golem->commandGroup = &idleFlock;
		idleFlock.groupTarget = &golem->transform;*/

    //No more.
     ///////////////////////////////////////////////

		//Destructable* e2 = GameUtilities::AddEnemyFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//Destructable* e3 = GameUtilities::AddEnemyFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -20), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//Trigger* t1 = GameUtilities::AddEndGameTriggerFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
//=======
		//ResourceManager::AddPBRModel("files/models/mapped_skybox.wobj", XMFLOAT3(0, -1.3, 0), XMFLOAT3(100, .1, 100), XMFLOAT3(0, 0, 0));

		//Renderable* a1 = GameUtilities::AddPBRStaticAsset("files/models/oildrum.wobj", XMFLOAT3(3, 0, -1), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0));
		//Renderable* a2 = GameUtilities::AddPBRStaticAsset("files/models/text.wobj", XMFLOAT3(1, 0, 0), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0));
		//Collectable** collectables = new Collectable*[10];
		//for (int i = 0; i < 10; ++i)
		//{
		//	collectables[i] = GameUtilities::AddCollectableFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3((i&10) *-5, 0, 5*(i%2)), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//	GameUtilities::AddGameObject(collectables[i]);
		//}
		//Enemy* e1 = GameUtilities::AddEnemyFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -5), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//Enemy* e2 = GameUtilities::AddEnemyFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//Enemy* e3 = GameUtilities::AddEnemyFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -20), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//Trigger* t1 = GameUtilities::AddTriggerFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));


		//ADPhysics::AABB a3c = ADPhysics::AABB(XMFLOAT3(10, 0, 0), XMFLOAT3(1, 1, 1));

		//Trigger* myHitBox = GameUtilities::AddHitbox("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//spyro->testAttack.active = false;
		//spyro->testAttack.hitboxCount = 1;
		//spyro->testAttack.cooldownDuration = 0.5;
		//spyro->testAttack.hitbox = myHitBox;

		// Colliders
		//Renderable* c1 = GameUtilities::AddColliderBox("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//Renderable* c2 = GameUtilities::AddColliderBox("files/models/mapped_skybox.wobj", XMFLOAT3(0, 5, 15), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));

		//Renderable* testPlane = GameUtilities::AddPBRStaticAsset("files/models/plane.wobj", XMFLOAT3(0, -0.25f, 0), XMFLOAT3(20, 10, 20), XMFLOAT3(0, 0, 0));

		std::vector<std::string> animationFiles;
		animationFiles.push_back("files/models/Golem_2_Idle.animfile");

		animationFiles[0] = "files/models/BattleMage.animfile";
		//animationFiles[0] = "files/models/Trebuchet_Attack.animfile";

		//Renderable* AnimationTester = GameUtilities::AddSimpleAnimAsset("files/models/BattleMage.AnimMesh", "files/textures/BattleMage.mat", animationFiles, XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));

		//Renderable* House = GameUtilities::AddSimpleAsset("files/models/House_01.mesh", "", XMFLOAT3(5, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));

		Renderable* tempPlane = GameUtilities::AddSimpleAsset("files/models/Ground.mesh", "files/textures/Ground.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(1000, 100, 1000), XMFLOAT3(0, 0, 0));

		// Add gameobjects
		// Comment this out - will run at 1fps
		/*int COUNT = 2500;
		for (int i = 0; i < COUNT; i++)
		{
			GameUtilities::AddGameObject(dynamic_cast<GameObject*>(spyro));
		}*/
		// Comment this out - will run at 1fps
		GameUtilities::AddGameObject(dynamic_cast<GameObject*>(golem));
		//GameUtilities::AddGameObject(c1);
		//GameUtilities::AddGameObject(c2);
		////GameUtilities::AddGameObject(a1);
		//GameUtilities::AddGameObject(a2);
		//GameUtilities::AddGameObject(e1);
		//GameUtilities::AddGameObject(e2);
		//GameUtilities::AddGameObject(e3);
		//GameUtilities::AddGameObject(t1);
		//GameUtilities::AddGameObject(testPlane);
		//GameUtilities::AddGameObject(AnimationTester);
		GameUtilities::AddGameObject(tempPlane);

		//testPlane->colliderPtr = nullptr;

		
		SimpleModel** tempPlaneModel = ResourceManager::GetSimpleModelPtrFromMeshId(tempPlane->GetMeshId());
		std::vector<ADPhysics::Triangle> ground;
		std::vector<ADQuadTreePoint> treePoints;
		XMMATRIX groundWorld = XMMatrixIdentity();
		SimpleStaticModel* planeModel = static_cast<SimpleStaticModel*>(*tempPlaneModel);
		tempPlane->GetWorldMatrix(groundWorld);
		for(unsigned int i = 0; i < (*planeModel).indices.size(); i+=3)
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
			ADQuadTreePoint point = ADQuadTreePoint(centroid.x, centroid.z, *tri);
			treePoints.push_back(point);
		}

		ADQuad boundary = ADQuad((*planeModel).position.x, (*planeModel).position.z, 1000, 1000);
		QuadTree* tree = new QuadTree(boundary);

		for (unsigned int i = 0; i < treePoints.size(); i++)
		{
			tree->Insert(treePoints[i]);
		}

		//Add Game Objects to their collision groupings
		//GameObject* passables[1];
		//passables[0] = a3;

		// Orbit camera
		engine->GetOrbitCamera()->SetLookAt((XMFLOAT3&)(Float3ToVector((*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position)));
		engine->GetOrbitCamera()->SetRadius(20);
		engine->GetOrbitCamera()->Rotate(yaw, pitch);
		engine->GetOrbitCamera()->SetPosition(XMFLOAT3(100, 200, -200));

		GolemGameUISetup::GameUserInterface gameUI;

		if (!engine->Initialize())
		{
			return;
		}

		gameUI.SetupUI(engine->GetUI(), golem, &audioEngine);

		// Timing
		game_time.Restart();

		// String shit
		std::string fr; std::wstring tfw; const wchar_t* wchar;

		// Construct physics stuff
		test_colider = ADPhysics::AABB(XMFLOAT3(0, 0, 10), XMFLOAT3(2, 2, 2));
		test_colider1 = ADPhysics::AABB(XMFLOAT3(0, 5, 15), XMFLOAT3(2, 2, 2));
		//test_plane = ADPhysics::Plane(XMMatrixTranslation(0, -5, 0), XMFLOAT3(10, 0, 10));
		
		//Needed to add this to the colliders for the collision queue
		/*c1->colliderPtr = &test_colider;
		c1->type = OBJECT_TYPE::STATIC;

		c2->colliderPtr = &test_colider1;
		c2->type = OBJECT_TYPE::STATIC;*/

		while (!shutdown)
		{
			game_time.Signal();
			delta_time = static_cast<float>(game_time.SmoothDelta());
			timer += delta_time;
			main_music_loop_timer -= delta_time;

			ProcessInput();




			//Update
			//idleFlock.Update(delta_time);
			//commandFlock.Update(delta_time);

			// Test
			//spyro->Update(delta_time);
			// Debug draw
			//ResourceManager::GetModelPtrFromMeshId(golem_collider)->position = (*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position;

			//engine->GetOrbitCamera()->SetRadius(200);
			engine->GetOrbitCamera()->SetRadius(50);
			engine->GetOrbitCamera()->SetLookAtAndRotate((XMFLOAT3&)(Float3ToVector(golem->GetPosition()) + XMVectorSet(0,15,0,1)), yaw, pitch, delta_time);
			XMMATRIX view;
			engine->GetOrbitCamera()->GetViewMatrix(view);
			golem->GetView(view);
		

			XMFLOAT3 CamPosition = engine->GetOrbitCamera()->GetPosition();
			audioEngine.Set3dListenerAndOrientation({ CamPosition.x, CamPosition.y, CamPosition.z });
			audioEngine.Update();

			// Physics test
		
			/*spyro->CheckCollision(c1);
			spyro->CheckCollision(c2);
			spyro->CheckCollision(p1);
			a3->CheckCollision(spyro);
			e1->CheckCollision(spyro);
			e2->CheckCollision(spyro);
			e3->CheckCollision(spyro);
			t1->CheckCollision(spyro);*/


			//Did this to represent layers, Triggers won't collider with other triggers so there is no need to test them

			//This is just tmporary code for a simple collision layer loop, this will be slow but multithreading should help

			//Works the exact same as the commented code above
		/*	int OBJ_COUNT = ResourceManager::GetGameObjectCount();
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
								if (OBJS[i]->colliderPtr->type != ColliderType::Plane)
								{
									OBJS[i]->CheckCollision(OBJS[j]);
								}
							}
						}
					}
				}
			}
			XMFLOAT3 SpyrosPosition = VectorToFloat3(golem->transform.r[3]);
			std::vector<ADQuadTreePoint> optimizedPoints = tree->Query(ADQuad(golem->transform.r[3].m128_f32[0], golem->transform.r[3].m128_f32[2], 100, 100));
			std::vector<Triangle> trisInRange;
			for (unsigned int i = 0; i < optimizedPoints.size(); i++)
			{
				std::vector<ADQuadTreePoint> optimizedPoints = tree->Query(ADQuad(OBJS[i]->GetPosition().x, OBJS[i]->GetPosition().z, 10, 10));
				std::vector<Triangle> trisInRange;
				for (unsigned int i = 0; i < optimizedPoints.size(); i++)
				{
					trisInRange.push_back(*optimizedPoints[i].tri);
				}

			GroundClamping(golem, trisInRange, delta_time);

			}*/

			//Resolve all collisions that occurred this frame
			ADResource::ADGameplay::ResolveCollisions();

			// Test

			// Poll input
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// D3d11 shit
			if (!engine->Update()) break;
			if (!engine->Render()) break;



			// Update framerate
			if (timer > 1)
			{
				timer = 0;

				fr = std::to_string(1.0 /delta_time) + " FPS";
				tfw = std::wstring(fr.begin(), fr.end());
				wchar = tfw.c_str();
				ApplicationView::GetForCurrentView()->Title = ref new String(wchar);
			}
		}
		audioEngine.Shutdown();
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
		//if (Input::QueryButtonDown(GamepadButtons::DPadLeft))
		//{
		//	if (effect_triggered) audio_manager->ResumeEffect(7, effect_id);
		//	else {
		//		effect_triggered = true;
		//		effect_id = audio_manager->PlayEffect(7);
		//	}
		//}
		//if (Input::QueryButtonDown(GamepadButtons::DPadRight))
		//{
		//	audio_manager->PauseEffect(0, effect_id);
		//}

		static float camera_rotation_thresh = 250;
		float dt = delta_time;

		if (Input::ControllerPresent())
		{
			// Camera rotation
			if (Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::RIGHT_THUMBSTICK) == (int)Input::DIRECTION::RIGHT)
			{
				yaw += camera_rotation_thresh * dt;
			} else if (Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::RIGHT_THUMBSTICK) == (int)Input::DIRECTION::LEFT)
			{
				yaw += -camera_rotation_thresh * dt;
			}
			if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::RIGHT_THUMBSTICK) == (int)Input::DIRECTION::UP)
			{
				pitch += -camera_rotation_thresh * dt;
			} else if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::RIGHT_THUMBSTICK) == (int)Input::DIRECTION::DOWN)
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