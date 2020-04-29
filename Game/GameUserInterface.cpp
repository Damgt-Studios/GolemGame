#include "pchgame.h"
#include "GameUserInterface.h"


namespace SpyroUISetup
{
    bool GameplayUIControllerSPYROGAME::ProcessResponse(ADResource::AD_UI::UIMessage* _message)
    {
        bool buttonPressed = false;
        if (!setup.overlays[setup.overlaysNameToID["GuideScreen"]].visible)
        {
            if (_message)
            {
                buttonPressed = true;
                if (uiState == ADUI::UISTATE::MENUSTATE)
                {

                    if (_message->messageType == ADUI::UIMessageTypes::FromButton)
                    {
                        switch (_message->number)
                        {
                        case 0:
                        {
                            UINT pauseOverlayID = setup.overlaysNameToID["PauseScreen"];
                            if (setup.overlays[pauseOverlayID].active)
                            {
                                setup.overlays[pauseOverlayID].Disable();
                                setup.overlays[setup.overlaysNameToID["HUD"]].visible = false;
                                uiState = ADUI::UISTATE::GAMEPLAY;
                            }
                            else
                            {
                                setup.overlays[pauseOverlayID].Enable();
                                uiState = ADUI::UISTATE::MENUSTATE;
                            }
                            buttonPressed = true;
                            break;
                        }
                        case 1: //guidebook/credits
                            buttonPressed = true;
                            setup.overlays[setup.overlaysNameToID["GuideScreen"]].Enable();
                            setup.overlays[setup.overlaysNameToID["PauseScreen"]].Disable();
                            break;
                        case 2: //options
                            buttonPressed = true;
                            break;
                        case 3: //Exit Level
                            buttonPressed = true;
                            break;
                        case 4:
                            uiState = ADUI::UISTATE::QUIT;
                            buttonPressed = true;
                            break;
                        default:
                            break;
                        }
                    }

                }
            }
        }
        else
        {
            if (Input::QueryButtonDown(GamepadButtons::B))
            {
                setup.overlays[setup.overlaysNameToID["GuideScreen"]].Disable();
                setup.overlays[setup.overlaysNameToID["PauseScreen"]].Enable();
                buttonPressed = true;
            }
        }

        if (_message)
        {
            if (_message->messageType == ADUI::UIMessageTypes::ExternalMsg)
            {

                switch (_message->number)
                {
                case 1:
                {
                    timesPressed += 1;
                    UINT gemCountID = setup.componentsNameToID["GemCount"];
                    Get<ADUI::Label2D*>(gemCountID)->GetText()->output.clear();
                    Get<ADUI::Label2D*>(gemCountID)->GetText()->output = std::to_string(timesPressed);
                    buttonPressed = true;
                    break;
                }
                case 2:
                {
                    health -= 1;
                    UINT gemCountID = setup.componentsNameToID["SpyroCount"];
                    Get<ADUI::Label2D*>(gemCountID)->GetText()->output.clear();
                    Get<ADUI::Label2D*>(gemCountID)->GetText()->output = std::to_string(health);
                    if (health <= 0)
                    {
                        setup.overlays[setup.overlaysNameToID["CreditsScreen"]].Enable();
                        setup.uiControllersEnabled[1] = false;
                        setup.uiControllersEnabled[2] = true;
                    }
                    buttonPressed = true;
                    break;
                }
                case 3:
                {
                    setup.overlays[setup.overlaysNameToID["SuccessScreen"]].Enable();
                    setup.uiControllersEnabled[1] = false;
                    setup.uiControllersEnabled[2] = true;
                    buttonPressed = true;
                    break;
                }
                case 4:
                {
                    health  = 3;
                    UINT gemCountID = setup.componentsNameToID["SpyroCount"];
                    Get<ADUI::Label2D*>(gemCountID)->GetText()->output.clear();
                    Get<ADUI::Label2D*>(gemCountID)->GetText()->output = std::to_string(health);

                    timesPressed = 0;
                    gemCountID = setup.componentsNameToID["GemCount"];
                    Get<ADUI::Label2D*>(gemCountID)->GetText()->output.clear();
                    Get<ADUI::Label2D*>(gemCountID)->GetText()->output = std::to_string(timesPressed);
                    buttonPressed = true;

                    break;
                }
                default:
                    break;
                }
            }
        }

        return buttonPressed;
    }

    bool GameplayUIControllerSPYROGAME::ProcessInput(float delta_time)
    {
        bool buttonPressed = false;
        if (Input::QueryButtonDown(GamepadButtons::Menu))
        {
            UINT pauseOverlayID = setup.overlaysNameToID["PauseScreen"];
            UINT hudOverlayID = setup.overlaysNameToID["HUD"];
            if (setup.overlays[pauseOverlayID].active)
            {
                setup.overlays[pauseOverlayID].Disable();
                setup.overlays[hudOverlayID].visible = false;
                uiState = ADUI::UISTATE::GAMEPLAY;
            }
            else
            {
                setup.uiControllersEnabled[pauseOverlayID] = true;
                setup.overlays[hudOverlayID].visible = true;
                setup.overlays[pauseOverlayID].Enable();
                uiState = ADUI::UISTATE::MENUSTATE;
            }
            buttonPressed = true;
        }
        if (Input::QueryButtonDown(GamepadButtons::Y))
        {
            UINT hudOverlayID = setup.overlaysNameToID["HUD"];
            UINT logOverlayID = setup.overlaysNameToID["Log"];
            if (setup.overlays[hudOverlayID].visible)
            {
                if (!setup.overlays[setup.overlaysNameToID["PauseScreen"]].active)
                {
                    setup.overlays[hudOverlayID].visible = false;
                }
                setup.overlays[logOverlayID].visible = !setup.overlays[logOverlayID].visible;
            }
            else
            {
                setup.overlays[hudOverlayID].visible = true;
                setup.overlays[logOverlayID].visible = true;
            }
            buttonPressed = true;
        }
        if (Input::QueryButtonDown(GamepadButtons::X))
        {
            timesPressed += 1;
            UINT gemCountID = setup.componentsNameToID["GemCount"];
            Get<ADUI::Label2D*>(gemCountID)->GetText()->output.clear();
            Get<ADUI::Label2D*>(gemCountID)->GetText()->output = std::to_string(timesPressed);
            buttonPressed = true;
        }
        return buttonPressed;
    }

    bool StartMenuUIControllerSPYROGAME::ProcessInput(float delta_time)
    {
        bool buttonPressed = false;
        if (setup.overlays[setup.overlaysNameToID["CreditsScreen"]].visible)
        {
            if (Input::QueryButtonDown(GamepadButtons::B))
            {
                setup.overlays[setup.overlaysNameToID["CreditsScreen"]].Disable();
                setup.overlays[setup.overlaysNameToID["TitleScreen"]].Enable();
                buttonPressed = true;
            }
        }
        if (Input::QueryButtonDown(GamepadButtons::Y))
        {
            setup.overlays[setup.overlaysNameToID["Log"]].visible = !setup.overlays[setup.overlaysNameToID["Log"]].visible;
            buttonPressed = true;
        }
        return buttonPressed;
    }

    bool StartMenuUIControllerSPYROGAME::ProcessResponse(ADResource::AD_UI::UIMessage* _message)
    {
        bool buttonPressed = false;
        if (!setup.overlays[setup.overlaysNameToID["CreditsScreen"]].visible)
        {
            if (_message)
            {
                buttonPressed = true;
                if (_message->messageType == ADUI::UIMessageTypes::FromButton)
                {
                    switch (_message->number)
                    {
                    case 0:
                    {
                        setup.overlays[setup.overlaysNameToID["TitleScreen"]].Disable();
                        UINT hudID = setup.overlaysNameToID["HUD"];
                        setup.overlays[hudID].active = true;
                        setup.overlays[hudID].visible = false;
                        setup.uiControllersEnabled[setup.controllersNameToID["HUD"]] = true;
                        setup.uiControllersEnabled[setup.controllersNameToID["TitleScreen"]] = false;
                        uiState = ADUI::UISTATE::GAMEPLAY;
                        buttonPressed = true;
                        break;
                    }
                    case 1: //saves
                        buttonPressed = true;
                        break;
                    case 2: //guidebook/credits
                        setup.overlays[setup.overlaysNameToID["CreditsScreen"]].Enable();
                        setup.overlays[setup.overlaysNameToID["TitleScreen"]].Disable();
                        uiState = ADUI::UISTATE::GAMEPLAY;
                        buttonPressed = true;
                        break;
                    case 3:
                        uiState = ADUI::UISTATE::QUIT;
                        buttonPressed = true;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        return buttonPressed;
    };

    bool EndMenuUIControllerSPYROGAME::ProcessInput(float delta_time)
    {
        bool buttonPressed = false;
        if (Input::QueryButtonDown(GamepadButtons::B))
        {
            setup.overlays[setup.overlaysNameToID["CreditsScreen"]].Disable();
            setup.overlays[setup.overlaysNameToID["SuccessScreen"]].Disable();
            setup.overlays[setup.overlaysNameToID["HUD"]].Disable();
            setup.overlays[setup.overlaysNameToID["TitleScreen"]].Enable();

            ADResource::AD_UI::UIMessage message;
            message.controllerID = 1;
            message.messageType = 2;
            message.number = 4;

            ADUI::MessageReceiver::SendMessage(&message);
            setup.uiControllersEnabled[0] = true;
            setup.uiControllersEnabled[2] = false;
            
            buttonPressed = true;
        }
        if (Input::QueryButtonDown(GamepadButtons::Y))
        {
            setup.overlays[setup.overlaysNameToID["Log"]].visible = !setup.overlays[setup.overlaysNameToID["Log"]].visible;
            buttonPressed = true;
        }
        return buttonPressed;
    }

    inline float pup(float _number)
    {
        return (_number / 3840.f);
    }

    inline float pvp(float _number)
    {
        return (_number / 5280.f);
    }

    //May be best to use width for both functions and go to largest set square size so not squished stretched on wierd resolutions.
    inline float pxp(float _number, float _screenWidth)
    {
        return (_number / 3840.f) * _screenWidth;
    }

    inline float pyp(float _number, float _screenHeight)
    {
        return (_number / 2160.f) * _screenHeight;
    }

    ADUI::ADUI* GameUserInterface::SpyroGameUISetup()
    {
        Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
        float screenWidth = Window->Bounds.Width;
        float screenHeight = Window->Bounds.Height;
        ADUI::ADUI* myUI = new ADUI::ADUI();
        ADUI::UISetup* setup = new ADUI::UISetup();
        myUI->SetSetup(setup);
        ADResource::AD_UI::UIHeader* header = new ADResource::AD_UI::UIHeader[2];
        header[0] = { "spyroatlas.dds" };
        header[1] = { "spyroatlas2.dds" };
        myUI->SetAltasHeader(header);

        // 0.35f * screenWidth, -0.3f * screenHeight
        myUI->uiLog.InitializeLog(pxp(-565.f, screenWidth), pyp(440.f, screenHeight), screenWidth, screenHeight, { 0.3f * screenWidth, -0.25f * screenHeight, pxp(1150.f, screenWidth), pyp(900.f, screenHeight), 0.f, pup(2300.f), pvp(4320.f), pvp(5280.f), 1 });
        myUI->uiLog.WriteToLog("UI Log Created.");
        myUI->uiLog.WriteToLog("1.");
        myUI->uiLog.WriteToLog("2.");
        myUI->uiLog.WriteToLog("3.");
        myUI->uiLog.WriteToLog("4.");
        myUI->uiLog.WriteToLog("5.");
        myUI->uiLog.WriteToLog("6.");

        //8192

        //TitleScreen
        UINT id = myUI->AddNewOverlay("TitleScreen", true, true, true);
        ADUI::Image2D* titleImage = new ADUI::Image2D({ 0, 0, screenWidth, screenHeight, 0, pup(3840.f), 0,  pvp(2160.f), 0 });
        UINT compId = myUI->AddUIComponent("TitleBG", titleImage);
        setup->overlays[id].AddComponent(compId);

        ////PauseScreen
        id = myUI->AddNewOverlay("PauseScreen");
        ADUI::Image2D* pauseImage = new ADUI::Image2D({ 0, 0, screenWidth, screenHeight, 0, pup(3840.f), 0,  pvp(2160.f), 1 });
        compId = myUI->AddUIComponent("PauseBG", pauseImage);
        setup->overlays[id].AddComponent(compId);

        ////OptionsScreen
        id = myUI->AddNewOverlay("OptionsScreen");
        setup->overlays[id].AddComponent(compId);

        ////MusicScreen
        id = myUI->AddNewOverlay("MusicScreen");
        ADUI::Image2D* musicImage = new ADUI::Image2D({ 0, 0, pxp(1150.f, screenWidth), pyp(900.f, screenHeight), 0.f, pup(2300.f), pvp(4320.f), pvp(5280.f), 0 });
        compId = myUI->AddUIComponent("PauseBG", musicImage);
        setup->overlays[id].AddComponent(compId);

        ////SuccessScreen
        id = myUI->AddNewOverlay("SuccessScreen");
        ADUI::Image2D* successImage = new ADUI::Image2D({ 0, 0, screenWidth, screenHeight, 0, pup(3840.f), pvp(2160.f),  pvp(4320.f), 0 });
        compId = myUI->AddUIComponent("SuccessBG", successImage);
        setup->overlays[id].AddComponent(compId);

        ////CreditsScreen
        id = myUI->AddNewOverlay("CreditsScreen");
        ADUI::Image2D* creditsImage = new ADUI::Image2D({ 0, 0, screenWidth, screenHeight, 0, pup(3840.f), pvp(2160.f),  pvp(4320.f), 1 });
        compId = myUI->AddUIComponent("CreditsBG", creditsImage);
        //ADResource::AD_UI::TextLabel gemLabel = { false, GetPosition(0.1f, 0.1f, screenWidth, screenHeight), {"0"} };
        //AD_UI::Label2D* label1 = new AD_UI::Label2D();
        //label1->SetText(gemLabel);
        //compId = myUI->AddUIComponent("GemCount", label1);
        setup->overlays[id].AddComponent(compId);

        ////GuideBookScreen
        id = myUI->AddNewOverlay("GuideScreen");
        //ADResource::AD_UI::TextLabel gemLabel = { false, GetPosition(0.1f, 0.1f, screenWidth, screenHeight), {"0"} };
        //AD_UI::Label2D* label1 = new AD_UI::Label2D();
        //label1->SetText(gemLabel);
        //compId = myUI->AddUIComponent("GemCount", label1);
        setup->overlays[id].AddComponent(compId);



        //HUD
        id = myUI->AddNewOverlay("HUD");
        ADUI::Image2D* image1 = new ADUI::Image2D({ (-0.45f * screenWidth), (0.4f * screenHeight), pxp(225.f, screenWidth), pyp(205.f, screenHeight), pup(2300.f), pup(2525.f), pvp(4320.f), pvp(4525.f), 1 });
        ADUI::Image2D* image2 = new ADUI::Image2D({ (0.40f * screenWidth), (0.4f * screenHeight), pxp(252.f, screenWidth), pyp(252.f, screenHeight), pup(2525.f), pup(2782.f), pvp(4320.f), pvp(4572.f), 1 });
        compId = myUI->AddUIComponent("GemIcon", image1);
        setup->overlays[id].AddComponent(compId);
        compId = myUI->AddUIComponent("SpyroIcon", image2);
        setup->overlays[id].AddComponent(compId);

        ADResource::AD_UI::TextLabel gemLabel = { false, GetPosition(0.1f, 0.1f, screenWidth, screenHeight), {"0"} };
        ADResource::AD_UI::TextLabel healthLabel = { false, GetPosition(0.95f, 0.1f, screenWidth, screenHeight), {"3"} };
        ADUI::Label2D* label1 = new ADUI::Label2D();
        label1->SetText(gemLabel);
        compId = myUI->AddUIComponent("GemCount", label1);
        setup->overlays[id].AddComponent(compId);
        ADUI::Label2D* label2 = new ADUI::Label2D();
        label2->SetText(healthLabel);
        compId = myUI->AddUIComponent("SpyroCount", label2);
        setup->overlays[id].AddComponent(compId);


        //Button List for Start and Pause Menus
        ADUI::Button2D* ContinueBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        ContinueBtn->SetText("Continue", pxp(-90.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);
        //button->actionValue = 1;

        ADUI::Button2D* SaveBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        SaveBtn->SetText("Save Slots", pxp(-115.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);
        //button2->actionValue = 2;

        ADUI::Button2D* CreditsBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        CreditsBtn->SetText("Credits", pxp(-90.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);

        ADUI::Button2D* QuitBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        QuitBtn->SetText("Quit", pxp(-60.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);

        ADUI::Button2D* ExitBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        ExitBtn->SetText("Exit Level", pxp(-110.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);

        ADUI::Button2D* OptionsBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        OptionsBtn->SetText("Options", pxp(-85.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);

        ADUI::Button2D* GuidebookBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        GuidebookBtn->SetText("Guidebook", pxp(-115.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);

        ADUI::Button2D* ControlsBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        ControlsBtn->SetText("Controls", pxp(-50.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);

        ADUI::Button2D* SoundBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        SoundBtn->SetText("Sound", pxp(-50.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);

        ADUI::Button2D* CameraBtn = new ADUI::Button2D(
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight), pup(2300.f), pup(3200.f), pvp(4320.f), pvp(4520.f), 0 }),
            new ADUI::Image2D({ pxp(0, screenWidth), pyp(-100, screenHeight), pxp(600.f, screenWidth), pyp(150.f, screenHeight),  pup(2300.f), pup(3200.f), pvp(4520.f), pvp(4720.f), 0 }, { 2, 0, 1, 6 })
            );
        CameraBtn->SetText("Camera", pxp(-50.f, screenWidth), pyp(25.f, screenHeight), screenWidth, screenHeight);


        ADUI::ButtonList* buttonList = new ADUI::ButtonList(screenWidth, screenHeight);
        buttonList->y = pyp(-160, screenHeight);
        buttonList->spacing = 20;
        buttonList->active = true;
        buttonList->controlFocus = true;
        buttonList->buttons.push_back(ContinueBtn);
        buttonList->buttons.push_back(SaveBtn);
        buttonList->buttons.push_back(CreditsBtn);
        buttonList->buttons.push_back(QuitBtn);
        buttonList->Initialize();
        compId = myUI->AddUIComponent("TitleMenu", buttonList);

        setup->overlays[0].AddComponent(compId);


        ADUI::ButtonList* buttonList2 = new ADUI::ButtonList(screenWidth, screenHeight);
        buttonList2->y = pyp(80, screenHeight);
        buttonList2->spacing = 20;
        buttonList2->active = true;
        buttonList2->controlFocus = true;
        buttonList2->buttons.push_back(ContinueBtn);
        buttonList2->buttons.push_back(GuidebookBtn);
        buttonList2->buttons.push_back(OptionsBtn);
        buttonList2->buttons.push_back(ExitBtn);
        buttonList2->buttons.push_back(QuitBtn);
        buttonList2->Initialize();
        UINT compId2 = myUI->AddUIComponent("PauseMenu", buttonList2);
        setup->overlays[1].AddComponent(compId2);


        ADUI::ButtonList* buttonList3 = new ADUI::ButtonList(screenWidth, screenHeight);
        buttonList3->y = +80;
        buttonList3->spacing = 20;
        buttonList3->active = true;
        buttonList3->controlFocus = true;
        buttonList3->buttons.push_back(ControlsBtn);
        buttonList3->buttons.push_back(SoundBtn);
        buttonList3->buttons.push_back(CameraBtn);
        buttonList3->Initialize();
        UINT compId3 = myUI->AddUIComponent("OptionsMenu", buttonList3);
        setup->overlays[2].AddComponent(compId3);

        buttonList->RepositionText();

        //UI Log
        id = myUI->AddNewOverlay("Log", true, true, true);
        UINT compId4 = myUI->AddUIComponent("Log", myUI->GetLogComponent());
        setup->overlays[id].AddComponent(compId4);
        ADUI::MessageReceiver::SetUI(myUI);

        //TitleScreen Controller
        StartMenuUIControllerSPYROGAME* titleScreenController = new StartMenuUIControllerSPYROGAME(myUI->uiState, *setup);
        setup->uiControllersEnabled.push_back(true);
        titleScreenController->AddComponent(compId, buttonList);
        titleScreenController->AddComponent(compId3, myUI->GetLogComponent());
        myUI->AddUIController("TitleScreen", titleScreenController);

        //HUD Controller
        GameplayUIControllerSPYROGAME* gameplayUIController = new GameplayUIControllerSPYROGAME(myUI->uiState, *setup);
        setup->uiControllersEnabled.push_back(false);
        gameplayUIController->AddComponent(compId2, buttonList2);// , buttonList);
        gameplayUIController->AddComponent(compId3, myUI->GetLogComponent());
        gameplayUIController->AddComponent(setup->componentsNameToID["GemCount"], label1);
        gameplayUIController->AddComponent(setup->componentsNameToID["SpyroCount"], label2);
        myUI->AddUIController("HUD", gameplayUIController);

        EndMenuUIControllerSPYROGAME* endUIController = new EndMenuUIControllerSPYROGAME(myUI->uiState, *setup);
        setup->uiControllersEnabled.push_back(false);
        endUIController->AddComponent(compId3, myUI->GetLogComponent());
        endUIController->AddComponent(setup->componentsNameToID["GemCount"], label1);
        endUIController->AddComponent(setup->componentsNameToID["SpyroCount"], label2);
        myUI->AddUIController("END", endUIController);

        return myUI;
    }
}