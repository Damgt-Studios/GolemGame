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
#include "AudioManager.h"
#include "GameUtilities.h"
#include "GameObjectClasses.h"
#include "MeshLoader.h"
#include "../ActusDeiXboxOneEdition/Scene.h"

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
	ADResource::ADGameplay::Spyro* spyro;
	AD_ULONG spyro_collider = 0;

	AudioManager* audio_manager;

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

	//Scene
	ADGameplay::Scene currentScene;


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
		currentScene.LoadScene("files/scenes/test.scene");
	}

	virtual void SetWindow(CoreWindow^ Window)
	{
		Window->KeyDown += ref new TypedEventHandler
			<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyDown);
	}

	virtual void Load(String^ EntryPoint) {}

	virtual void Run()
	{
		// Bruh
		std::vector<std::string> sfx;
		sfx.push_back("files\\audio\\SFX_Gem_Collect.wav");
		sfx.push_back("files\\audio\\SFK_Destructable_Break.wav");
		sfx.push_back("files\\audio\\SFK_Enemy_Death.wav");
		sfx.push_back("files\\audio\\SFK_Player_Charging.wav");
		sfx.push_back("files\\audio\\SFK_Player_Death.wav");
		sfx.push_back("files\\audio\\SFK_Player_FireBreath.wav");
		sfx.push_back("files\\audio\\SFK_Player_Glide.wav");
		sfx.push_back("files\\audio\\SFK_Player_Hurt.wav");
		sfx.push_back("files\\audio\\SFK_Player_Jump.wav");
		sfx.push_back("files\\audio\\SFK_Player_Land.wav");
		sfx.push_back("files\\audio\\SFK_Player_Object_Hit.wav");
		sfx.push_back("files\\audio\\SFK_Player_Running_Jump.wav");
		sfx.push_back("files\\audio\\SFK_Player_Walking.wav");
		sfx.push_back("files\\audio\\SFK_Player_Water_Splash.wav");
		audio_manager = new AudioManager;
		audio_manager->Initialize("files\\audio\\main_theme.wav", sfx);
		

		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		// Create the engine
		engine = new Engine;

		// Initialize the engine
		engine->SetCamera(XMFLOAT3(0, 20.0f, -100.0f), 0, 0, 45);

		currentScene.Render();

		ResourceManager::AddSkybox("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 0), XMFLOAT3(-10, -10, -10), XMFLOAT3(0, 0, 0));
		spyro = GameUtilities::LoadSpyroFromModelFile("files/models/Test_Spyro.wobj", XMFLOAT3(0, 0.00001, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		spyro_collider = ResourceManager::AddPBRModel("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0.00001, 0), XMFLOAT3(.6, .6, .6), XMFLOAT3(0, 0, 0), true);
		spyro->SetAudio(audio_manager);


		Trigger* myHitBox = GameUtilities::AddHitbox("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -30), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		spyro->testAttack.active = false;
		spyro->testAttack.hitboxCount = 1;
		spyro->testAttack.cooldownDuration = 0.5;
		spyro->testAttack.hitbox = myHitBox;
		//Destructable* e2 = GameUtilities::AddEnemyFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -10), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));
		//Destructable* e3 = GameUtilities::AddEnemyFromModelFile("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, -20), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));


		ADPhysics::AABB a3c = ADPhysics::AABB(XMFLOAT3(10, 0, 0), XMFLOAT3(1, 1, 1));


		// Colliders
		Renderable* testPlane = GameUtilities::AddPBRStaticAsset("files/models/plane.wobj", XMFLOAT3(0, -0.25f, 0), XMFLOAT3(20, 10, 20), XMFLOAT3(0, 0, 0));

		// Add gameobjects
		// Comment this out - will run at 1fps
		/*int COUNT = 2500;
		for (int i = 0; i < COUNT; i++)
		{
			GameUtilities::AddGameObject(dynamic_cast<GameObject*>(spyro));
		}*/
		// Comment this out - will run at 1fps
		GameUtilities::AddGameObject(dynamic_cast<GameObject*>(spyro));

		GameUtilities::AddGameObject(testPlane);

		//testPlane->colliderPtr = nullptr;

		
		Model* models = ResourceManager::GetPBRPtr();
		int models_count = ResourceManager::GetPBRModelCount();
		Model planeModel = models[testPlane->GetMeshId() - 2];
		std::vector<ADPhysics::Triangle> ground;
		std::vector<ADQuadTreePoint> treePoints;
		XMMATRIX groundWorld = XMMatrixIdentity();
		testPlane->GetWorldMatrix(groundWorld);
		for(unsigned int i = 0; i < planeModel.indices.size(); i+=3)
		{
			ADPhysics::Triangle* tri = new ADPhysics::Triangle(planeModel.vertices[planeModel.indices[i]].Pos, planeModel.vertices[planeModel.indices[i + 1]].Pos, planeModel.vertices[planeModel.indices[i + 2]].Pos);
			tri->a = (XMFLOAT3&)(XMVector3Transform(Float3ToVector(tri->a), groundWorld));
			tri->b = (XMFLOAT3&)(XMVector3Transform(Float3ToVector(tri->b), groundWorld));
			tri->c = (XMFLOAT3&)(XMVector3Transform(Float3ToVector(tri->c), groundWorld));

			ground.push_back(*tri);

			XMFLOAT3 centroid = (XMFLOAT3&)((Float3ToVector(tri->a) + Float3ToVector(tri->b) + Float3ToVector(tri->c)) / 3);
			ADQuadTreePoint point = ADQuadTreePoint(centroid.x, centroid.z, *tri);
			treePoints.push_back(point);
		}

		ADQuad boundary = ADQuad(planeModel.position.x, planeModel.position.z, 50, 50);
		QuadTree* tree = new QuadTree(boundary);

		for (unsigned int i = 0; i < treePoints.size(); i++)
		{
			tree->Insert(treePoints[i]);
		}

		SimpleMesh mesh;
		Load_Mesh("files/models/ForestGolem_1.mesh", mesh);

		//Add Game Objects to their collision groupings
		//GameObject* passables[1];
		//passables[0] = a3;

		// Orbit camera
		engine->GetOrbitCamera()->SetLookAt(ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position);
		engine->GetOrbitCamera()->SetRadius(20);
		engine->GetOrbitCamera()->Rotate(yaw, pitch);

		SpyroUISetup::GameUserInterface gameUI;
		engine->SetupUserInterface(gameUI.SpyroGameUISetup());

		if (!engine->Initialize())
		{
			return;
		}

		// Timing
		game_time.Restart();

		// String shit
		std::string fr; std::wstring tfw; const wchar_t* wchar;

		while (!shutdown)
		{
			game_time.Signal();
			delta_time = static_cast<float>(game_time.SmoothDelta());
			timer += delta_time;
			main_music_loop_timer -= delta_time;

			ProcessInput();

			// Audio -Music doesn't play in debug mode -  annoying AF
			if (main_music_loop_timer <= 0 && !music_triggered)
			{
#ifdef NDEBUG
				music_triggered = true;
				audio_manager->PlayBackgroundMusic();
#endif
			}

			// Test
			//spyro->Update(delta_time);
			// Debug draw
			ResourceManager::GetModelPtrFromMeshId(spyro_collider)->position = ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position;

			engine->GetOrbitCamera()->SetLookAtAndRotate(spyro->GetPosition(), yaw, pitch, delta_time);
			XMMATRIX view;
			engine->GetOrbitCamera()->GetViewMatrix(view);
			spyro->GetView(view);
		
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
								if (OBJS[i]->colliderPtr->type != ColliderType::Plane)
								{
									OBJS[i]->CheckCollision(OBJS[j]);
								}
							}
						}
					}
				}
			}

			std::vector<ADQuadTreePoint> optimizedPoints = tree->Query(ADQuad(spyro->GetPosition().x, spyro->GetPosition().z, 10, 10));
			std::vector<Triangle> trisInRange;
			for (unsigned int i = 0; i < optimizedPoints.size(); i++)
			{
				trisInRange.push_back(*optimizedPoints[i].tri);
			}

			if (GroundClamping(spyro, trisInRange, delta_time))
				spyro->jumping = false;


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
		currentScene.destroy();
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
		if (Input::QueryButtonDown(GamepadButtons::DPadLeft))
		{
			if (effect_triggered) audio_manager->ResumeEffect(7, effect_id);
			else {
				effect_triggered = true;
				effect_id = audio_manager->PlayEffect(7);
			}
		}
		if (Input::QueryButtonDown(GamepadButtons::DPadRight))
		{
			audio_manager->PauseEffect(0, effect_id);
		}

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