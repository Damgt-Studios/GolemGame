#pragma once
//#include "ADUserInterface.h"

//using namespace AD_UI;

class GameUserInterface
{
    //UISetup* GenerateUISetup(float screenWidth, float screenHeight)
    //{
    //    //UISetup* setup = new UISetup();

    //    //uiLog.InitializeLog(width, height);
    //    //uiLog.WriteToLog("UI Log Created.");
    //    //uiLog.WriteToLog("1.");
    //    //uiLog.WriteToLog("2.");
    //    //uiLog.WriteToLog("3.");
    //    //uiLog.WriteToLog("4.");
    //    //uiLog.WriteToLog("5.");
    //    //uiLog.WriteToLog("6.");


    //    ////TitleScreen
    //    //UINT id = AddNewOverlay(true, true, true);
    //    //Image2D* titleImage = new Image2D({ 0, 0, width, height, 0, 1, 0, 0.333f });
    //    //UINT compId = AddUIComponent(titleImage);
    //    //setup->overlays[id].AddComponent(compId);

    //    ////PauseScreen
    //    //id = AddNewOverlay();
    //    //Image2D* pauseImage = new Image2D({ 0, 0, width, height, 0, 1, 0.333f, 0.666f });
    //    //compId = AddUIComponent(pauseImage);
    //    //setup->overlays[id].AddComponent(compId);

    //    ////(0.05f * width), (0.1f * height)
    //    ////HUD
    //    //id = AddNewOverlay();
    //    //Image2D* image1 = new Image2D({ -(0.45f * width), (0.4f * height), 80, 80, 0, 0.06f, 0.667f, 0.698f });
    //    //Image2D* image2 = new Image2D({ (0.40f * width), (0.4f * height), 80, 80, 0.06667f, 0.128f, 0.667f, 0.7065f });
    //    //compId = AddUIComponent(image1);
    //    //setup->overlays[id].AddComponent(compId);
    //    //compId = AddUIComponent(image2);
    //    //setup->overlays[id].AddComponent(compId);

    //    //TextLabel gemLabel = { false, GetPosition(0.1f, 0.1f, width, height), {"0"} };
    //    //TextLabel healthLabel = { false, GetPosition(0.95f, 0.1f, width, height), {"0"} };
    //    //setup->textLabels.push_back(gemLabel);
    //    //setup->textLabels.push_back(healthLabel);

    //    ////Button List for Start and Pause Menus
    //    //Button2D* button = new Button2D(
    //    //    new Image2D({ 0, -100, 240, 60, 0.2994f, 0.4911f, 0.6666f, 0.6907f }, { 2, 0, 2, 6 }),
    //    //    new Image2D({ 0, -100, 240, 60, 0.6848f, 0.8875f, 0.6666f, 0.6907f })
    //    //    );
    //    //button->actionValue = 1;

    //    //Button2D* button2 = new Button2D(
    //    //    new Image2D({ 0, -170, 240, 60, 0.2994f, 0.4911f, 0.7614, 0.7858f }),
    //    //    new Image2D({ 0, -170, 240, 60, 0.4921f, 0.6859f, 0.7614, 0.7858f })
    //    //    );
    //    //button2->actionValue = 2;

    //    //ButtonList* buttonList = new ButtonList();
    //    //buttonList->y = -160;
    //    //buttonList->spacing = 20;
    //    //buttonList->active = true;
    //    //buttonList->controlFocus = true;
    //    //buttonList->buttons.push_back(button);
    //    //buttonList->buttons.push_back(button2);
    //    //buttonList->Initialize();
    //    //compId = AddUIComponent(buttonList);

    //    //setup->overlays[0].AddComponent(compId);

    //    ////TitleScreen Controller
    //    //StartMenuUIControllerSPYROGAME* titleScreenController = new StartMenuUIControllerSPYROGAME(uiState, *setup);
    //    //setup->uiControllersEnabled.push_back(true);
    //    //titleScreenController->AddComponent(compId);
    //    //uiControllers.push_back(titleScreenController);


    //    //buttonList = new ButtonList();
    //    //buttonList->y = +80;
    //    //buttonList->spacing = 20;
    //    //buttonList->active = true;
    //    //buttonList->controlFocus = true;
    //    //buttonList->buttons.push_back(button);
    //    //buttonList->buttons.push_back(button2);
    //    //buttonList->Initialize();
    //    //compId = AddUIComponent(buttonList);

    //    //setup->overlays[1].AddComponent(compId);

    //    ////HUD Controller
    //    //GameplayUIControllerSPYROGAME* gameplayUIController = new GameplayUIControllerSPYROGAME(uiState, *setup);
    //    //setup->uiControllersEnabled.push_back(false);
    //    //gameplayUIController->AddComponent(compId);
    //    //uiControllers.push_back(gameplayUIController);

    //    ////UI Log
    //    //id = AddNewOverlay(true, true, true);
    //    //compId = AddUIComponent(&uiLog);
    //    //setup->overlays[id].AddComponent(compId);

    //}
};

