#pragma once
#include "ADUserInterface.h"
#include "Types.h"

namespace SpyroUISetup
{
    inline XMFLOAT2 GetPosition(float _percentageX, float _percentageY, float _screenWidth, float _screeHeight)
    {
        return { (_percentageX * _screenWidth), (_percentageY * _screeHeight) };
    }

    class GameplayUIControllerSPYROGAME : public AD_UI::OverlayController
    {
        int timesPressed = 0;
    public:
        GameplayUIControllerSPYROGAME(UINT& _uiState, AD_UI::UISetup& _setup) : AD_UI::OverlayController(_uiState, _setup) {};
        virtual bool ProcessResponse(UINT responseID);
        virtual bool ProcessInput(float delta_time);
    };

    class StartMenuUIControllerSPYROGAME : public AD_UI::OverlayController
    {
    public:
        StartMenuUIControllerSPYROGAME(UINT& _uiState, AD_UI::UISetup& _setup) : AD_UI::OverlayController(_uiState, _setup) {};
        virtual bool ProcessInput(float delta_time);
        virtual bool ProcessResponse(UINT responseID);
    };

    class GameUserInterface
    {
    public:
        AD_UI::ADUI* SpyroGameUISetup()
            {
                Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
                float screenWidth = Window->Bounds.Width;
                float screenHeight = Window->Bounds.Height;
                AD_UI::ADUI* myUI = new AD_UI::ADUI();
                AD_UI::UISetup* setup = new AD_UI::UISetup();
                myUI->SetSetup(setup);

                myUI->uiLog.InitializeLog(screenWidth, screenHeight);
                myUI->uiLog.WriteToLog("UI Log Created.");
                myUI->uiLog.WriteToLog("1.");
                myUI->uiLog.WriteToLog("2.");
                myUI->uiLog.WriteToLog("3.");
                myUI->uiLog.WriteToLog("4.");
                myUI->uiLog.WriteToLog("5.");
                myUI->uiLog.WriteToLog("6.");


                //TitleScreen
                UINT id = myUI->AddNewOverlay(true, true, true);
                AD_UI::Image2D* titleImage = new AD_UI::Image2D({ 0, 0, screenWidth, screenHeight, 0, 1, 0, 0.333f });
                UINT compId = myUI->AddUIComponent(titleImage);
                setup->overlays[id].AddComponent(compId);

                ////PauseScreen
                id = myUI->AddNewOverlay();
                AD_UI::Image2D* pauseImage = new AD_UI::Image2D({ 0, 0, screenWidth, screenHeight, 0, 1, 0.333f, 0.666f });
                compId = myUI->AddUIComponent(pauseImage);
                setup->overlays[id].AddComponent(compId);

                //(0.05f * width), (0.1f * height)
                //HUD
                id = myUI->AddNewOverlay();
                AD_UI::Image2D* image1 = new AD_UI::Image2D({ -(0.45f * screenWidth), (0.4f * screenHeight), 80, 80, 0, 0.06f, 0.667f, 0.698f });
                AD_UI::Image2D* image2 = new AD_UI::Image2D({ (0.40f * screenWidth), (0.4f * screenHeight), 80, 80, 0.06667f, 0.128f, 0.667f, 0.7065f });
                compId = myUI->AddUIComponent(image1);
                setup->overlays[id].AddComponent(compId);
                compId = myUI->AddUIComponent(image2);
                setup->overlays[id].AddComponent(compId);

                ADResource::AD_UI::TextLabel gemLabel = { false, GetPosition(0.1f, 0.1f, screenWidth, screenHeight), {"0"} };
                ADResource::AD_UI::TextLabel healthLabel = { false, GetPosition(0.95f, 0.1f, screenWidth, screenHeight), {"0"} };
                setup->textLabels.push_back(gemLabel);
                setup->textLabels.push_back(healthLabel);

                //Button List for Start and Pause Menus
                AD_UI::Button2D* button = new AD_UI::Button2D(
                    new AD_UI::Image2D({ 0, -100, 240, 60, 0.2994f, 0.4911f, 0.6666f, 0.6907f }, { 2, 0, 2, 6 }),
                    new AD_UI::Image2D({ 0, -100, 240, 60, 0.6848f, 0.8875f, 0.6666f, 0.6907f })
                    );
                button->actionValue = 1;

                AD_UI::Button2D* button2 = new AD_UI::Button2D(
                    new AD_UI::Image2D({ 0, -170, 240, 60, 0.2994f, 0.4911f, 0.7614, 0.7858f }),
                    new AD_UI::Image2D({ 0, -170, 240, 60, 0.4921f, 0.6859f, 0.7614, 0.7858f })
                    );
                button2->actionValue = 2;

                AD_UI::ButtonList* buttonList = new AD_UI::ButtonList();
                buttonList->y = -160;
                buttonList->spacing = 20;
                buttonList->active = true;
                buttonList->controlFocus = true;
                buttonList->buttons.push_back(button);
                buttonList->buttons.push_back(button2);
                buttonList->Initialize();
                compId = myUI->AddUIComponent(buttonList);

                setup->overlays[0].AddComponent(compId);

                //TitleScreen Controller
                StartMenuUIControllerSPYROGAME* titleScreenController = new StartMenuUIControllerSPYROGAME(myUI->uiState, *setup);
                setup->uiControllersEnabled.push_back(true);
                titleScreenController->AddComponent(compId);
                myUI->AddUIController(titleScreenController);


                buttonList = new AD_UI::ButtonList();
                buttonList->y = +80;
                buttonList->spacing = 20;
                buttonList->active = true;
                buttonList->controlFocus = true;
                buttonList->buttons.push_back(button);
                buttonList->buttons.push_back(button2);
                buttonList->Initialize();
                compId = myUI->AddUIComponent(buttonList);

                setup->overlays[1].AddComponent(compId);

                //HUD Controller
                GameplayUIControllerSPYROGAME* gameplayUIController = new GameplayUIControllerSPYROGAME(myUI->uiState, *setup);
                setup->uiControllersEnabled.push_back(false);
                gameplayUIController->AddComponent(compId);
                myUI->AddUIController(gameplayUIController);

                //UI Log
                id = myUI->AddNewOverlay(true, true, true);
                compId = myUI->AddUIComponent(myUI->GetLogComponent());
                setup->overlays[id].AddComponent(compId);
                return myUI;
            }
    };
    
};

