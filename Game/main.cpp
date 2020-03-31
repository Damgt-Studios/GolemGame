// Include the precompiled headers
#include "pchgame.h"
#include "Engine.h"

// Use some common namespaces to simplify the code
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

// Settings
bool FULLSCREEN = false;
// Settings

// the class definition for the core "framework" of our app
ref class App sealed : public IFrameworkView
{
private:
	Engine* engine;
	bool shutdown = false;
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

		// set the OnActivated function to handle to Activated "event"
		AppView->Activated += ref new TypedEventHandler
			<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);
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
		engine->SetCamera(XMFLOAT3(0, 0, -30), -12, 12, 30);

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

		engine->GetResourceManager()->AddPBRModel("files/models/text.wobj", XMFLOAT3(0, 0, 0), XMFLOAT3(.01, .01, .01), XMFLOAT3(0, 0, 0));
		engine->GetResourceManager()->AddPBRModel("files/models/oildrum.wobj", XMFLOAT3(0, 0, 0), XMFLOAT3(.1, .1, .1), XMFLOAT3(0, 0, 0));
		engine->GetResourceManager()->AddPBRModel("files/models/fireExt.wobj", XMFLOAT3(-10, 0, 0), XMFLOAT3(.01, .01, .01), XMFLOAT3(0, 0, 0));

		if (!engine->Initialize())
		{
			return;
		}

		ADResource::ADRenderer::Header header;

		while (!shutdown)
		{
			// Poll input
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// D3d11 shit
			if (!engine->Update()) break;
			if (!engine->Render()) break;
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
		static float camera_movement_thresh = .25;
		static float camera_rotation_thresh = .25;

		if (args->VirtualKey == VirtualKey::Escape)
		{
			shutdown = true;
		}
		else if(args->VirtualKey == VirtualKey::W)
		{
			XMFLOAT3 pos(0, 0, 0);
			pos.z += camera_movement_thresh;
			engine->MoveCamera(pos);
		} else if(args->VirtualKey == VirtualKey::S)
		{
			XMFLOAT3 pos(0, 0, 0);
			pos.z += -camera_movement_thresh;
			engine->MoveCamera(pos);
		} else if(args->VirtualKey == VirtualKey::A)
		{
			XMFLOAT3 pos(0, 0, 0);
			pos.x += -camera_movement_thresh;
			engine->MoveCamera(pos);
		} else if(args->VirtualKey == VirtualKey::D)
		{
			XMFLOAT3 pos(0, 0, 0);
			pos.x += camera_movement_thresh;
			engine->MoveCamera(pos);
		}
		else if (args->VirtualKey == VirtualKey::Left)
		{
			engine->RotateCamera(camera_rotation_thresh, 0);
		} else if (args->VirtualKey == VirtualKey::Right)
		{
			engine->RotateCamera(-camera_rotation_thresh, 0);
		}
		else if (args->VirtualKey == VirtualKey::Up)
		{
			engine->RotateCamera(0, camera_rotation_thresh);
		} else if (args->VirtualKey == VirtualKey::Down)
		{
			engine->RotateCamera(0, -camera_rotation_thresh);
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