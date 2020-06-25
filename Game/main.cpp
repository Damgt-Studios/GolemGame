// Include the precompiled headers
#include "pchgame.h"
#include <vector>
#include <windows.gaming.input.h>
#include <collection.h>
#include "Engine.h"
#include "ADPhysics.h"
#include "ADQuadTree.h"
#include "TheGreatGolem.h"

#include "ADUserInterface.h"
#include "GameUserInterface.h"
//#include "AudioManager.h"
#include "ADAudio.h"
#include "GameUtilities.h"
#include "GameObjectClasses.h"
#include "MeshLoader.h"
#include "ADAI.h"
#include "ADPathfinding.h"
#include "AnimationStateMachine.h"
#include "Listeners.h"
#include "../ActusDeiXboxOneEdition/Scene.h"

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
	ADGameplay::Scene currentScene;
	TheGreatGolem* game;
	ADResource::ADGameplay::Golem* golem;
	AnimationStateMachine GolemAnimController;
	AD_ULONG golem_collider = 0;

	//AudioManager* audio_manager;
	AD_AUDIO::ADAudio* audio;

	bool shutdown = false;

	//// Temp music
	//int effect_id;
	//bool effect_triggered = false;

	// Timing
	XTime game_time;
	float timer = 0;
	float delta_time = 0;

	const float physics_rate = 0.2f;
	float physics_timer = 0;

	//// Audio
	//float main_music_loop_timer = 3;
	//bool music_triggered = false;

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

	virtual void Run()
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		//---------------------Create The Engines
		engine = new Engine;
		AD_AUDIO::ADAudio audioEngine;
		audioEngine.Init();

		//Implement Audio Engine
		audioEngine.LoadBank("files//audio//Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
		audioEngine.LoadBank("files//audio//Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);


		AD_AUDIO::AudioSource golemSteps;
		golemSteps.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemSteps.engine = &audioEngine;
		golemSteps.personalVolume = 0.5f;
		golemSteps.restartOnRepeat = true;
		golemSteps.LoadSound("event:/Sfx_GolemStep", true, false, true, true);

		AD_AUDIO::AudioSource titleMusic;
		titleMusic.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::MUSIC;
		titleMusic.engine = &audioEngine;
		titleMusic.personalVolume = 0.02f;
		titleMusic.LoadSound("files\\audio\\Opening.mp3", false, false, true, true);

		AudioSourceEvent playTitleEvent(titleMusic);
		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayTitle", &playTitleEvent);

		AD_AUDIO::AudioSource golemPunchSound;
		golemPunchSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemPunchSound.engine = &audioEngine;
		golemPunchSound.personalVolume = 0.5f;
		golemPunchSound.restartOnRepeat = false;
		golemPunchSound.LoadSound("event:/Sfx_MinorGrunt", true, true, false, false);
		AudioSourceEvent golemPunchEvent(golemPunchSound);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemPunch", &golemPunchEvent);

		AD_AUDIO::AudioSource golemKickSound;
		golemKickSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemKickSound.engine = &audioEngine;
		golemKickSound.personalVolume = 0.5f;
		golemKickSound.restartOnRepeat = false;
		golemKickSound.LoadSound("event:/Sfx_EarthHit2", true, true, false, false);
		AudioSourceEvent golemKickEvent(golemKickSound);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemKick", &golemKickEvent);

		AD_AUDIO::AudioSource golemSlamSound;
		golemSlamSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemSlamSound.engine = &audioEngine;
		golemSlamSound.personalVolume = 0.5f;
		golemSlamSound.restartOnRepeat = false;
		golemSlamSound.LoadSound("event:/Sfx_EarthHit", true, true, false, false);
		AudioSourceEvent golemSlamEvent(golemSlamSound);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemSlam", &golemSlamEvent);

		AD_AUDIO::AudioSource golemWaveSound;
		golemWaveSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemWaveSound.engine = &audioEngine;
		golemWaveSound.personalVolume = 0.5f;
		golemWaveSound.restartOnRepeat = false;
		golemWaveSound.LoadSound("event:/Sfx_WaterWaveSpell", true, true, false, false);
		AudioSourceEvent golemWaveEvent(golemWaveSound);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemWaterWave", &golemWaveEvent);

		AD_AUDIO::AudioSource golemFireball;
		golemFireball.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemFireball.engine = &audioEngine;
		golemFireball.personalVolume = 0.5f;
		golemFireball.restartOnRepeat = false;
		golemFireball.LoadSound("event:/Sfx_FireBallSpell", true, true, false, false);
		AudioSourceEvent golemFireballEvent(golemFireball);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemFireball", &golemFireballEvent);

		AD_AUDIO::AudioSource golemIronHide;
		golemIronHide.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemIronHide.engine = &audioEngine;
		golemIronHide.personalVolume = 0.5f;
		golemIronHide.restartOnRepeat = false;
		golemIronHide.LoadSound("event:/Sfx_IronSkinSpell", true, true, false, false);
		AudioSourceEvent golemTauntEvent(golemIronHide);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemTaunt", &golemTauntEvent);



		AD_AUDIO::AudioSource golemRootingSpell;
		golemRootingSpell.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemRootingSpell.engine = &audioEngine;
		golemRootingSpell.personalVolume = 0.5f;
		golemRootingSpell.restartOnRepeat = false;
		golemRootingSpell.LoadSound("event:/Sfx_RootCrushSpell", true, true, false, false);
		AudioSourceEvent golemRootEvent(golemRootingSpell);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemRooting", &golemRootEvent);

		AD_AUDIO::AudioSource golemEatMinion;
		golemEatMinion.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemEatMinion.engine = &audioEngine;
		golemEatMinion.personalVolume = 0.5f;
		golemEatMinion.restartOnRepeat = false;
		golemEatMinion.LoadSound("event:/Sfx_MinnionScream", true, true, false, false);
		AudioSourceEvent golemEatMinionEvent(golemEatMinion);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemEat", &golemEatMinionEvent);

		DefinitionReader df;
		df.ReadMasterFile();

		ApplyEffectEvent golemEatingEvent(*DefinitionDatabase::Instance()->effectsDatabase["GolemEat"]);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Apply_GolemEat", &golemEatingEvent);

		ParticleEmitterEvent golemPunchParticles(engine->bigCloud);
		golemPunchParticles.lifespan = 20.0f;
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemPunch", &golemPunchParticles);

		//Light light;
		//ZeroMemory(&light, sizeof(Light));
		//light.lightType = (int)LIGHTTYPE::DIRECTIONAL;
		//light.diffuse = XMFLOAT4(0.5, 0.5, 0.5, 1);
		//light.diffuseIntensity = 1;
		//light.ambient = XMFLOAT4(0.156f, 0.003f, 0.215f, 1);
		//light.ambientIntensity = 3;
		//light.lightDirection = XMFLOAT4(0, -0.5f, -1, 1);
		//light.position = XMFLOAT4(0, 0, 0, 1);

		// Point light
		/*Light light1;
		ZeroMemory(&light1, sizeof(Light));
		light1.lightType = (int)LIGHTTYPE::POINT;
		light1.diffuse = XMFLOAT4(1, 0.807, 0, 1);
		light1.diffuseIntensity = 4;
		light1.position = (XMFLOAT4(-5 * 25, 6, 7.5 * 25 + 2.25, 1));
		light1.lightRadius = 25;*/

		//ResourceManager::AddLight(light);
		//ResourceManager::AddLight(light1);

		// Initialize the engine
		engine->SetCamera(XMFLOAT3(0, 10000.0f, -100.0f), 0, 0, 45);

		currentScene.LoadScene("files/scenes/TestScene.scenery");

		golem = currentScene.GetGolem();

		/*std::vector<std::string> animations;
		animations.push_back("files/models/Golem_1_Idle.animfile");
		animations.push_back("files/models/Golem_1_Born.animfile");
		animations.push_back("files/models/Golem_1_Run.animfile");
		animations.push_back("files/models/Golem_1_Death.animfile");
		animations.push_back("files/models/Golem_1_Kick.animfile");*/
		//golem = GameUtilities::LoadGolemFromModelFile("files/models/Golem_1.AnimMesh", "files/textures/Golem_1.mat", animations, XMFLOAT3(0, 0, 0), XMFLOAT3(0.1, 0.1, 0.1), XMFLOAT3(0, 0, 0));
		
		// Orbit camera
		engine->GetOrbitCamera()->SetLookAt((XMFLOAT3&)(Float3ToVector((*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position)));
		engine->GetOrbitCamera()->SetRadius(20);
		engine->GetOrbitCamera()->Rotate(yaw, pitch);

		GolemGameUISetup::GameUserInterface gameUI;
		if (!engine->Initialize())
		{
			return;
		}


		std::vector<std::string> stoneMinionAnimations;
		stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");

		
		//ResourceManager::AddSkybox("files/models/Skybox.mesh", "files/textures/Skybox.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(-10, -10, -10), XMFLOAT3(0, 0, 0));

		GolemAnimController.Initialize(golem);
		golem->GetAnimationController(GolemAnimController);

		std::vector<std::string> bucketheadanims;
		bucketheadanims.push_back("files/models/Bucket_Idle.animfiles");

		Renderable* buckethead = GameUtilities::AddSimpleAnimAsset("files/models/Buckethead.AnimMesh", "files/textures/Buckethead.mat", bucketheadanims, XMFLOAT3(2.5, 0, 0), XMFLOAT3(0.1, 0.1, 0.1), XMFLOAT3(0, 0, 0));
		GameUtilities::AddGameObject(buckethead);

		// Orbit camera
		engine->GetOrbitCamera()->SetLookAt((XMFLOAT3&)(Float3ToVector((*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position)));
		engine->GetOrbitCamera()->SetRadius(20);
		engine->GetOrbitCamera()->Rotate(yaw, pitch);

#ifdef _DEBUG
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

		ADAI::FlockingGroup commandFlock;
		ADAI::FlockingGroup idleFlock;

		std::vector<Destructable*> stoneMinions;
		std::vector<Destructable*> waterMinions;
		std::vector<Destructable*> fireMinions;
		std::vector<Destructable*> woodMinions;

		//currentScene.GetMinions(&stoneMinions, &waterMinions, &fireMinions, &woodMinions);

		Destructable* m1 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", stoneMinionAnimations, XMFLOAT3(0, 5, 0), XMFLOAT3(0.02f, 1.02f, 0.02f), XMFLOAT3(0, 0, 0));
		//Destructable* m2 = GameUtilities::AddDestructableFromModelFile("files/models/Minion_1.AnimMesh", "files/textures/Minion_1.mat", minionAnimations, XMFLOAT3(50, 5, 30), XMFLOAT3(0.02f, 0.02f, 0.02f), XMFLOAT3(0, 0, 0));

		golem->targetMarker = m1;
		golemEatingEvent.SetTarget(golem);

		m1->colliderPtr = nullptr;

		//animationFiles[0] = "files/models/BattleMage.animfile";
		float mapWidth = 1000;
		float mapHeight = 1000;
		//Renderable* tempPlane = GameUtilities::AddSimpleAsset("files/models/Plane.mesh", "files/textures/Grass.mat", XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		Renderable* tempPlane = currentScene.GetPlane();

		GameUtilities::AddGameObject(dynamic_cast<GameObject*>(golem));
		GameUtilities::AddGameObject(m1);
		GameUtilities::AddGameObject(tempPlane);

		Building* house1 = new Building(XMFLOAT3(-5, 0, 0), XMFLOAT3(0, -45, 0), XMFLOAT3(25, 25, 30), XMFLOAT3(0, 0.5f, 0.15), GameUtilities::GenerateHouse1);
		GameUtilities::AddGameObject(house1);
		
		Building* house2 = new Building(XMFLOAT3(-5, 0, 3), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 35), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse2);
		GameUtilities::AddGameObject(house2);
		//
		Building* house3 = new Building(XMFLOAT3(-5, 0, 6), XMFLOAT3(0, 45, 0), XMFLOAT3(25, 35, 30), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse3);
		GameUtilities::AddGameObject(house3);
		
		Building* house4 = new Building(XMFLOAT3(-5, 0, 9), XMFLOAT3(0, 90, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0.5, 0.15), GameUtilities::GenerateHouse4);
		GameUtilities::AddGameObject(house4);
		
		Building* barn1 = new Building(XMFLOAT3(-5, 0, -5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 37.5), XMFLOAT3(0.25f, 0.5f, 0), GameUtilities::GenerateBarn1);
		GameUtilities::AddGameObject(barn1);
		
		Building* barn2 = new Building(XMFLOAT3(-5, 0, -7), XMFLOAT3(0, 0, 0), XMFLOAT3(20, 20, 20), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateBarn2);
		GameUtilities::AddGameObject(barn2);

		Building* tower = new Building(XMFLOAT3(1.125, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower = new Building(XMFLOAT3(-24.475, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower2 = new Building(XMFLOAT3(-6.75, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower3 = new Building(XMFLOAT3(-6.75, 0, -3.125), XMFLOAT3(0, 90, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower4 = new Building(XMFLOAT3(0.5, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		//Building* tower5 = new Building(XMFLOAT3(13.875, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(12.5, 45, 12.5), XMFLOAT3(0, 0.75, 0), GameUtilities::GenerateTower);
		GameUtilities::AddGameObject(tower);
		//GameUtilities::AddGameObject(tower2);
		//GameUtilities::AddGameObject(tower3);
		//GameUtilities::AddGameObject(tower4);
		//GameUtilities::AddGameObject(tower5);

		//Building* gateway = new Building(XMFLOAT3(-1.625, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 1, 0), GameUtilities::GenerateGateway);
		//GameUtilities::AddGameObject(gateway);
		//
		//Building* scaffolding = new Building(XMFLOAT3(-13.75, 0, -1), XMFLOAT3(0, 90, 0), XMFLOAT3(5, 25, 25), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateScaffoldWallX);
		//GameUtilities::AddGameObject(scaffolding);

		//1.625 difference
		Building* wall = new Building(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		//Building* wall = new Building(XMFLOAT3(-23.475, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		/*Building* wall2 = new Building(XMFLOAT3(-20.25, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall3 = new Building(XMFLOAT3(-18.625, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall4 = new Building(XMFLOAT3(-17, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall5 = new Building(XMFLOAT3(-15.375, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall6 = new Building(XMFLOAT3(-12.125, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall7 = new Building(XMFLOAT3(-10.5, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall8 = new Building(XMFLOAT3(-8.875, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall9 = new Building(XMFLOAT3(-7.25, 0, -1), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall10 = new Building(XMFLOAT3(-6.75, 0, -2.125), XMFLOAT3(0, 90, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall11 = new Building(XMFLOAT3(1.5, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall12 = new Building(XMFLOAT3(3.125, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall13 = new Building(XMFLOAT3(4.75, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall14 = new Building(XMFLOAT3(6.375, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall15 = new Building(XMFLOAT3(8, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall16 = new Building(XMFLOAT3(9.625, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall17 = new Building(XMFLOAT3(11.25, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);
		Building* wall18 = new Building(XMFLOAT3(12.875, 0, 29), XMFLOAT3(0, 0, 0), XMFLOAT3(32.5, 35, 12.5), XMFLOAT3(0, 0.5f, 0), GameUtilities::GenerateWall);*/
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


		Building* well = new Building(XMFLOAT3(0, 0, 1), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 5, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateWell);
		GameUtilities::AddGameObject(well);

		Building* tavern = new Building(XMFLOAT3(-10, 0, 10), XMFLOAT3(0, 45, 0), XMFLOAT3(30, 70, 70), XMFLOAT3(0, 1, 0), GameUtilities::GenerateTavern);
		GameUtilities::AddGameObject(tavern);

		Building* lamp = new Building(XMFLOAT3(-5, 0, 7.5), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 1.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp);
		GameUtilities::AddGameObject(lamp);
		Building* lamp2 = new Building(XMFLOAT3(2.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 1.5), XMFLOAT3(0, 0, 0), GameUtilities::GenerateLamp);
		GameUtilities::AddGameObject(lamp2);

		Building* rock = new Building(XMFLOAT3(-2, 0, 2), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRock1);
		GameUtilities::AddGameObject(rock);

		Building* meeting_hall = new Building(XMFLOAT3(10, 0, 10), XMFLOAT3(0, -135, 0), XMFLOAT3(40, 45, 25), XMFLOAT3(0, 1, 0), GameUtilities::GenerateMeetingHall);
		GameUtilities::AddGameObject(meeting_hall);

		Building* cart = new Building(XMFLOAT3(-2.5, 0, 1), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5, 0.25, 0.1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateCart);
		GameUtilities::AddGameObject(cart);

		Building* realtree = new Building(XMFLOAT3(-2.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 3, 1), XMFLOAT3(0, 0, 0), GameUtilities::GenerateTree);
		GameUtilities::AddGameObject(realtree);

		Building* rubble1 = new Building(XMFLOAT3(0, 0, 5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble1);
		GameUtilities::AddGameObject(rubble1);

		Building* rubble2 = new Building(XMFLOAT3(-2, 0, 5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble2);
		GameUtilities::AddGameObject(rubble2);

		Building* rubble3 = new Building(XMFLOAT3(2, 0, 5), XMFLOAT3(0, 0, 0), XMFLOAT3(25, 25, 25), XMFLOAT3(0, 0, 0), GameUtilities::GenerateRubble3);
		GameUtilities::AddGameObject(rubble3);

		for (auto it = DefinitionDatabase::Instance()->hitboxDatabase.begin(), itEnd = DefinitionDatabase::Instance()->hitboxDatabase.end(); it != itEnd; ++it)
		{
			GameUtilities::AddGameObject(it->second, false);
		}

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

		float minionWidth = 10;

		//FountainEmitter femitter;
		//femitter.Initialize(engine->GetPBRRenderer()->GetRendererResources()->device.Get(), 100, XMFLOAT4(1, 1, 1, 0), L"files\\textures\\Particle_Dust.dds", 1000);
		//ParticleEmitterEvent golemPunchParticles(femitter);
		//golemPunchParticles.lifespan = 1000.f;
		//ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemPunch", &golemPunchParticles);



		// Timing
		game_time.Restart();

		// String shit
		std::string fr; std::wstring tfw; const wchar_t* wchar;

		// Construct physics stuff
		//test_colider1 = ADPhysics::AABB(XMFLOAT3(0, 5, 15), XMFLOAT3(2, 2, 2));
		//test_plane = ADPhysics::Plane(XMMatrixTranslation(0, -5, 0), XMFLOAT3(10, 0, 10));

		//Needed to add this to the colliders for the collision queue

		/*c2->colliderPtr = &test_colider1;
		c2->type = OBJECT_TYPE::STATIC;*/

		ADAI::ADPathfinding pathfinder;
		//pathfinder.Initialize(&planeModel->vertices, XMFLOAT2(mapWidth, mapLength), minionWidth, 20.f);
		gameUI.SetupUI(engine->GetUI(), golem, &audioEngine, pathfinder.tileMap.columns, mapWidth, mapHeight);
    
		ADEvents::ADEventSystem::Instance()->SendEvent("PlayTitle", (void*)0);

		while (!shutdown)
		{
			if (Input::QueryButtonDown(GamepadButtons::RightShoulder))
			{
				//pathfinder.clearDebug();
				//UINT row;
				//UINT column;
				//pathfinder.tileMap.GetColumnRowFromPosition(XMFLOAT2(golem->GetPosition().x, golem->GetPosition().z), column, row);
				//pathfinder.enter(0, 0, column, row);
			}


			game_time.Signal();
			delta_time = static_cast<float>(game_time.SmoothDelta());
			timer += delta_time;
			//main_music_loop_timer -= delta_time;

			ProcessInput();

			if (!pathfinder.isDone())
			{
				pathfinder.update(0.00001f);
			}
			pathfinder.UpdatePlayerNode(golem->GetPosition().x, golem->GetPosition().z, mapWidth, mapHeight);
		
			ADEvents::ADEventSystem::Instance()->ProcessEvents();

			// Debug draw
			//ResourceManager::GetModelPtrFromMeshId(golem_collider)->position = (*ResourceManager::GetSimpleModelPtrFromMeshId(golem->GetMeshId()))->position;

			//engine->GetOrbitCamera()->SetRadius(200);
			engine->GetOrbitCamera()->SetRadius(50);
			engine->GetOrbitCamera()->SetLookAtAndRotate((XMFLOAT3&)(Float3ToVector(golem->GetPosition()) + XMVectorSet(0, 15, 0, 1)), yaw, pitch, delta_time);
			XMMATRIX view;
			engine->GetOrbitCamera()->GetViewMatrix(view);
			golem->GetView(view);


			XMFLOAT3 CamPosition = engine->GetOrbitCamera()->GetPosition();
			audioEngine.Set3dListenerAndOrientation({ CamPosition.x, CamPosition.y, CamPosition.z });
			audioEngine.Update();

			XMFLOAT4X4 viewPass;
			XMStoreFloat4x4(&viewPass, view);
			XMFLOAT4 cpos = XMFLOAT4(CamPosition.x, CamPosition.y, CamPosition.z, 0);


			XMMATRIX pers = XMMatrixPerspectiveFovLH(engine->GetOrbitCamera()->GetFOV(), (Window->Bounds.Width / Window->Bounds.Height), engine->GetOrbitCamera()->GetNear(), engine->GetOrbitCamera()->GetFar());
			XMFLOAT4X4 persPass;
			XMStoreFloat4x4(&persPass, pers);
			//femitter.UpdateParticles(delta_time, viewPass, persPass, cpos);

#ifdef _DEBUG
			golemCollider->transform = golem->GetColliderInfo();

			/*house1Collider->transform = house1->GetColliderInfo();
			house2Collider->transform = house2->GetColliderInfo();
			house3Collider->transform = house3->GetColliderInfo();
			house4Collider->transform = house4->GetColliderInfo();

			barn1Collider->transform = barn1->GetColliderInfo();
			
			barn2Collider->transform = barn2->GetColliderInfo();

			gatewayCollider->transform = gateway->GetColliderInfo();

			towerCollider->transform = tower->GetColliderInfo();*/

			//scaffoldingCollider->transform = scaffolding->GetColliderInfo();
			
	/*		wallCollider->transform = wall->GetColliderInfo();

			wellCollider->transform = well->GetColliderInfo();

			tavernCollider->transform = tavern->GetColliderInfo();

			rockCollider->transform = rock->GetColliderInfo();

			meeting_hallCollider->transform = meeting_hall->GetColliderInfo();

			cartCollider->transform = cart->GetColliderInfo();

			rubbleCollider1->transform = rubble1->GetColliderInfo();
			rubbleCollider2->transform = rubble2->GetColliderInfo();
			rubbleCollider3->transform = rubble3->GetColliderInfo();*/
#endif

			//Did this to represent layers, Triggers won't collider with other triggers so there is no need to test them

			//This is just tmporary code for a simple collision layer loop, this will be slow but multithreading should help

		//	Works the exact same as the commented code above
			ADQuad collisionBoundary(0, 0, mapWidth * 2, mapHeight * 2);
			QuadTree<int>* collisionTree = new QuadTree<int>(collisionBoundary);

			int OBJ_COUNT = ResourceManager::GetGameObjectCount();
			ADResource::ADGameplay::GameObject** OBJS = ResourceManager::GetGameObjectPtr();

			for (int i = 0; i < OBJ_COUNT; i++)
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
				if (OBJS[i]->colliderPtr && !OBJS[i]->colliderPtr->trigger) 
				{
					XMFLOAT3 obj_pos = VectorToFloat3(OBJS[i]->transform.r[3]);
					std::vector<ADQuadTreePoint<int>> collisionVector = collisionTree->Query(ADQuad(obj_pos.x, obj_pos.z, 50, 50));

					for (unsigned int j = 0; j < collisionVector.size(); j++)
					{
						if (OBJS[*collisionVector[j].data]->colliderPtr)
							OBJS[i]->CheckCollision(OBJS[*collisionVector[j].data]);
					}
				}
			}

			//Resolve all collisions that occurred this frame
			ADResource::ADGameplay::ResolveCollisions();

			physics_timer += delta_time;
			if (physics_timer > physics_rate)
			{
				physics_timer = 0;
				/*for (int i = 0; i < 10; i++)
				{
					GroundClamping(stoneMinions[i], tree, delta_time);
					GroundClamping(waterMinions[i], tree, delta_time);
					GroundClamping(fireMinions[i], tree, delta_time);
					GroundClamping(woodMinions[i], tree, delta_time);
				}*/


			}

			GroundClamping(golem, tree, delta_time);
			//GroundClamping(cube, tree, delta_time);
			//cube->transform.r[3].m128_f32[1] += 5;

			// Poll input
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			GolemAnimController.SetModel_To_CurrentAnimation();

			// D3d11 shit
			if (!engine->Update()) break;
			if (!engine->Render()) break;
			//femitter.RenderParticles(engine->GetPBRRenderer()->GetRendererResources()->context.Get());

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
		audioEngine.Shutdown();
		tree->Shutdown();
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