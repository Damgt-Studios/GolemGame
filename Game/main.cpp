// Include the precompiled headers
#include "pchgame.h"
#include <windows.gaming.input.h>
#include <collection.h>
#include "Engine.h"
#include "XTime.h"

// Use some common namespaces to simplify the code
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::UI::ViewManagement;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::Gaming::Input;
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

	bool shutdown = false;

	// Timing
	XTime game_time;
	double timer = 0;
	double delta_time = 0;

	// Rotation
	float rot = 0;

	float yaw = 180;
	float pitch = 30;

	XMFLOAT3 spryo_movement_deltas = XMFLOAT3(0, 0, 0);

	// Gameplay
	float jump_time_up = .4;
	float jump_time_length = 1;
	float jump_count_down = 0;
	float jump_height = 15;
	float og_y_pos = 0;
	float gravity = 5;
	bool jumping = false;

	// Movement bools
	bool forward = false;
	bool backward = false;
	bool left = false;
	bool right = false;

	// Turning
	float spyro_turn_speed = 5;
	float spyro_move_speed = 5;

	float z_plane_cooldown = .2;
	float z_plane_timer = 0;

	float x_plane_cooldown = .2;
	float x_plane_timer = 0;

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

		// get the first game pad
		auto myGamepads = ref new Vector<Gamepad^>();

		for (auto gamepad : Gamepad::Gamepads)
		{
			// Check if the gamepad is already in myGamepads; if it isn't, add it.
			auto it = std::find(begin(myGamepads), end(myGamepads), gamepad);

			if (it == end(myGamepads))
			{
				// This code assumes that you're interested in all gamepads.
				myGamepads->Append(gamepad);
			}
		}
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

		// Create the engine
		engine = new Engine;

		// Initialize the engine
		engine->SetCamera(XMFLOAT3(0, 20, -100), 0, 0, 45);

		Light light;
		ZeroMemory(&light, sizeof(Light));
		light.lightType = (int)LIGHTTYPE::DIRECTIONAL;
		light.ambientUp = XMFLOAT4(1, 1, 1, 1);
		light.ambientDown = XMFLOAT4(1, 1, 1, 1);
		light.ambientIntensityDown = .2;
		light.ambientIntensityUp = .4;
		light.diffuse = XMFLOAT4(1, 1, 1, 1);
		light.lightDirection = XMFLOAT4(2, -1, 3, 1);
		light.diffuseIntensity = 1;
		light.specular = XMFLOAT4(1, 1, 1, 1);
		light.specularIntensity = .2;
		engine->GetResourceManager()->AddLight(light);


		engine->GetResourceManager()->AddSkybox("files/models/mapped_skybox.wobj", XMFLOAT3(0, 0, 0), XMFLOAT3(-10, -10, -10), XMFLOAT3(0, 0, 0));
		engine->GetResourceManager()->AddPBRModel("files/models/mapped_skybox.wobj", XMFLOAT3(0, -1.3, 0), XMFLOAT3(100, .1, 100), XMFLOAT3(0, 0, 0));
		spyro = engine->GetResourceManager()->LoadSpyroFromModelFile("files/models/Spyro_LowRes.wobj", XMFLOAT3(0, 0.00001, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0));

		 engine->GetResourceManager()->AddPBRModel("files/models/oildrum.wobj", XMFLOAT3(3, 0, -1), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0));
		 engine->GetResourceManager()->AddPBRModel("files/models/text.wobj", XMFLOAT3(1, 0, 0), XMFLOAT3(.03, .03, .03), XMFLOAT3(0, 0, 0));

		// Orbit camera
		engine->GetOrbitCamera()->SetLookAt(ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position);
		engine->GetOrbitCamera()->SetRadius(20);
		engine->GetOrbitCamera()->Rotate(yaw, pitch);

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

			// Reset movement deltas
			spryo_movement_deltas = XMFLOAT3(0, 0, 0);
			if (z_plane_timer > z_plane_cooldown)
			{
				forward = backward = false;
				z_plane_timer = 0;
			}
			if (x_plane_timer > x_plane_cooldown)
			{
				left = right = false;
				x_plane_timer = 0;
			}
			z_plane_timer += delta_time;
			x_plane_timer += delta_time;
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

			// Rotation
			rot += .1;
			if (rot >= 360) rot = 0;

			// GAMEPLAY
			// Jumping

			if (forward)
			{
				spryo_movement_deltas.z += spyro_move_speed * delta_time;
			}
			if (backward)
			{
				spryo_movement_deltas.z -= spyro_move_speed * delta_time;
			}
			if (left)
			{
				spryo_movement_deltas.x -= spyro_move_speed * delta_time;
			}
			if (right)
			{
				spryo_movement_deltas.x += spyro_move_speed * delta_time;
			}

			engine->GetOrbitCamera()->Rotate(yaw, pitch);
			ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position.z += spryo_movement_deltas.z;
			ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position.x += spryo_movement_deltas.x;

			engine->GetOrbitCamera()->SetLookAtAndRotate(ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position, yaw, pitch, delta_time);

			if (jumping)
			{
				jump_count_down += delta_time;
				
				if (jump_count_down < jump_time_length)
				{
					if (jump_count_down <= jump_time_up)
					{
						// Jump completion ratio
						float jump_up_ratio = jump_count_down / jump_time_up;

						ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position.y = 
							lerp(ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position.y, jump_height, jump_up_ratio);
					}
					else
					{
						// Jump down ratio
						float jump_dn_ratio = ((jump_count_down - jump_time_up) / (jump_time_length - jump_time_up));

						ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position.y = 
							lerp(ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position.y, og_y_pos, jump_dn_ratio);
					}
				}

				if (jump_count_down > jump_time_length)
				{
					jump_count_down = 0;
					jumping = false;
				}
			}
		}
	}
	virtual void Uninitialize() {}

	// an "event" that is called when the application window is ready to be activated
	void OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
	}

	void OnPointerMoved(CoreWindow^ Window, PointerEventArgs^ args)
	{

	}

	void OnKeyDown(CoreWindow^ Window, KeyEventArgs^ args)
	{
		static float camera_movement_thresh = 5;
		static float camera_rotation_thresh = 500;

		float dt = delta_time;

		if (args->VirtualKey == VirtualKey::Escape)
		{
			shutdown = true;
		}
		if(args->VirtualKey == VirtualKey::W || args->VirtualKey == VirtualKey::GamepadLeftThumbstickUp)
		{
			/*XMFLOAT3 pos = engine->GetCamera()->GetLook();
			pos.z += camera_movement_thresh * dt;
			engine->MoveCamera(pos);*/

			//spryo_movement_deltas.z += camera_movement_thresh * delta_time;
			forward = true;
			backward = false;
			z_plane_timer = 0;

		} else if(args->VirtualKey == VirtualKey::S || args->VirtualKey == VirtualKey::GamepadLeftThumbstickDown)
		{
			/*XMFLOAT3 pos = engine->GetCamera()->GetLook();
			pos.x = -pos.x; pos.y = -pos.y; pos.z = -pos.z;
			pos.z += -camera_movement_thresh * dt;
			engine->MoveCamera(pos);*/

			//spryo_movement_deltas.z += -camera_movement_thresh * delta_time;
			backward = true;
			forward = false;
			z_plane_timer = 0;
		} else if(args->VirtualKey == VirtualKey::A || args->VirtualKey == VirtualKey::GamepadLeftThumbstickLeft)
		{
			/*XMFLOAT3 pos = engine->GetCamera()->GetRight();
			pos.x += -camera_movement_thresh * dt;
			engine->MoveCamera(pos);*/

			//ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->rotation.y += -spyro_turn_speed * dt;
			//spryo_movement_deltas.x += -1.;
			left = true;
			right = false;
			x_plane_timer = 0;
		} else if(args->VirtualKey == VirtualKey::D || args->VirtualKey == VirtualKey::GamepadLeftThumbstickRight)
		{
			/*XMFLOAT3 pos = engine->GetCamera()->GetRight();
			pos.x = -pos.x; pos.y = -pos.y; pos.z = -pos.z;
			pos.x += camera_movement_thresh * dt;
			engine->MoveCamera(pos);*/

			//ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->rotation.y += spyro_turn_speed * dt;
			//spryo_movement_deltas.x += 1.;
			right = true;
			left = false;
			x_plane_timer = 0;
		}
		
		if (args->VirtualKey == VirtualKey::Left || args->VirtualKey == VirtualKey::GamepadRightThumbstickRight)
		{
			//engine->RotateCamera(camera_rotation_thresh * dt, 0);

			yaw += camera_rotation_thresh * dt;
			//engine->GetOrbitCamera()->Rotate(yaw, pitch);
		} else if (args->VirtualKey == VirtualKey::Right || args->VirtualKey == VirtualKey::GamepadRightThumbstickLeft)
		{
			//engine->RotateCamera(-camera_rotation_thresh * dt, 0);

			yaw += -camera_rotation_thresh * dt;
			//engine->GetOrbitCamera()->Rotate(yaw, pitch);
		}

		if (args->VirtualKey == VirtualKey::Up || args->VirtualKey == VirtualKey::GamepadRightThumbstickDown)
		{
			//engine->RotateCamera(0, camera_rotation_thresh * dt);

			pitch += camera_rotation_thresh *dt;
			//engine->GetOrbitCamera()->Rotate(yaw, pitch);
		} else if (args->VirtualKey == VirtualKey::Down || args->VirtualKey == VirtualKey::GamepadRightThumbstickUp)
		{
			//engine->RotateCamera(0, -camera_rotation_thresh * dt);

			pitch += -camera_rotation_thresh * dt;
			//engine->GetOrbitCamera()->Rotate(yaw, pitch);
		}

		if (args->VirtualKey == VirtualKey::Space || args->VirtualKey == VirtualKey::GamepadA && !jumping)
		{
			jumping = true;
			og_y_pos = ResourceManager::GetModelPtrFromMeshId(spyro->GetMeshId())->position.y;
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