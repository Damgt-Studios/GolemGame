#include "pchgame.h"
#include "GameUserInterface.h"

namespace GolemGameUISetup
{
	enum MESSAGE_TO_UI_TYPE
	{
		IGNORE_MESSAGE = 0,
		ENDGAME,
		RESTART
	};

	bool GameplayUIPrimaryController::ProcessResponse(ADUI::UIMessage* _message, float& quick)
	{
		bool buttonPressed = false;
		//if (delayInputTimer > 0)
		//{
		//	delayInputTimer -= 
		//}
		if (_message)
		{
			if (_message->externalMsg)
			{
				switch (_message->targetID)
				{
				case 0:
				{
					//overlays[overlaysNameToID["TitleScreen"]]->Enable();
					uiState = ENGINE_STATE::PAUSED;
					controllers[controllersNameToID["TitleScreenController"]]->Enable();
					overlays[overlaysNameToID["SuccessScreen"]]->Enable();
					ADEvents::ADEventSystem::Instance()->SendEvent("PlayEnd", (void*)0);
					controllers[controllersNameToID["HUD"]]->Disable();

					//controllers[controllersNameToID["SuccessScreen"]]->Enable();
					Disable();

					//controllers[controllersNameToID["EndScreenController"]]->Enable();

					//componentTypeMap[componentsNameToID["TitleMenu"]]->Disable();
					buttonPressed = true;
					break;
				}
				case 1:
				{
					//overlays[overlaysNameToID["TitleScreen"]]->Enable();
					uiState = ENGINE_STATE::PAUSED;
					controllers[controllersNameToID["TitleScreenController"]]->Enable();
					overlays[overlaysNameToID["DefeatScreen"]]->Enable();
					ADEvents::ADEventSystem::Instance()->SendEvent("PlayEnd", (void*)0);
					controllers[controllersNameToID["HUD"]]->Disable();
					//controllers[controllersNameToID["SuccessScreen"]]->Enable();
					Disable();

					//controllers[controllersNameToID["EndScreenController"]]->Enable();

					//componentTypeMap[componentsNameToID["TitleMenu"]]->Disable();
					buttonPressed = true;
					break;
				}
				case 2:
				{
					//health -= 1;
					//UINT gemCountID = componentsNameToID["Health"];
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output.clear();
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output = std::to_string(health);
					//if (health <= 0)
					//{
					//	overlays[overlaysNameToID["CreditsScreen"]]->Enable();
					//	Disable();
					//	controllers[controllersNameToID["TitleScreenController"]]->Enable();
					//}
					//buttonPressed = true;
					break;
				}
				case 3:
				{

					//timesPressed += 1;
					//UINT gemCountID = componentsNameToID["GemCount"];
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output.clear();
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output = std::to_string(timesPressed);
					//buttonPressed = true;
					break;
				}
				case 4:
				{
					//health = 3;
					//UINT gemCountID = componentsNameToID["Health"];
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output.clear();
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output = std::to_string(health);

					//timesPressed = 0;
					//gemCountID = componentsNameToID["GemCount"];
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output.clear();
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output = std::to_string(timesPressed);
					//buttonPressed = true;

					break;
				}
				default:
					break;
				}
			}
		}
		return false;
	}

	bool GameplayUIPrimaryController::ProcessInput(float delta_time, float& quick)
	{
		bool buttonPressed = false;
		if (Input::QueryButtonDown(GamepadButtons::View))
		{
			overlays[overlaysNameToID["Log"]]->visible = false;
			overlays[overlaysNameToID["PathingMap"]]->visible = !overlays[overlaysNameToID["PathingMap"]]->visible;
			buttonPressed = true;
		}
		return buttonPressed;
	}

	bool HUDController::ProcessResponse(ADUI::UIMessage* _message, float& quick)
	{
		if (uiState == ADUI::UISTATE::GAMEPLAY)
		{
			if (_message)
			{
				if (_message->externalMsg)
				{
					overlays[overlaysNameToID["MessageBox"]]->visible = true;
					text[0]->SetText(tutorialMessages[(_message->targetID*4)]);
					text[1]->SetText(tutorialMessages[(_message->targetID*4)+1]);
					text[2]->SetText(tutorialMessages[(_message->targetID*4)+2]);
					text[3]->SetText(tutorialMessages[(_message->targetID*4)+3]);
					uiState = PAUSED;
				}
			}
		}
		return false;
	}

	bool HUDController::ProcessInput(float delta_time, float& quick)
	{
		bool buttonPressed = false;
		if (overlays[overlaysNameToID["MessageBox"]]->visible)
		{
			if (Input::QueryButtonDown(GamepadButtons::B) || Input::QueryButtonDown(GamepadButtons::A))
			{
				ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Return", (void*)0);
				overlays[overlaysNameToID["MessageBox"]]->visible = false;
				uiState = GAMEPLAY;
				buttonPressed = true;
			}
		}
		return buttonPressed;
	}

	bool StartMenuUIController::ProcessResponse(ADUI::UIMessage* _message, float& quick)
	{
		if (splashTimer < 4.f)
		{
			return false;
		}

		bool buttonPressed = false;
		if (_message && !overlays[overlaysNameToID["SuccessScreen"]]->active && !overlays[overlaysNameToID["DefeatScreen"]]->active)
		{
			buttonPressed = true;
			if (_message->targetID == ADUI::UIMessageTargets::Controller)
			{
				if (_message->commandID == 1)
				{
					ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Confirm", (void*)0);
					switch (_message->componentIndex)
					{
					case 0:
					{
						overlays[overlaysNameToID["TitleScreen"]]->Disable();
						//componentTypeMap[componentsNameToID["TitleMenu"]]->Disable();
						Disable();

						controllers[controllersNameToID["TutorialController"]]->Enable();
						overlays[overlaysNameToID["Tutorial"]]->Enable();
						ADEvents::ADEventSystem::Instance()->SendEvent("PlayLevel", (void*)0);
						buttonPressed = true;
						break;
					}
					//case 1: //saves
					//	buttonPressed = true;
					//	break;
					case 1: //credits
						componentTypeMap[componentsNameToID["TitleMenu"]]->Disable();
						overlays[overlaysNameToID["CreditsScreen"]]->Enable();
						overlays[overlaysNameToID["TitleScreen"]]->Disable();
						uiState = ADUI::UISTATE::MENUSTATE;
						buttonPressed = true;
						break;
					case 2:
						uiState = ADUI::UISTATE::QUIT;
						buttonPressed = true;
						break;
					default:
						break;
					}
				}
				else
				{
					ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_MoveCursor", (void*)0);
				}

			}
		}
		return buttonPressed;
	}

	bool StartMenuUIController::ProcessInput(float delta_time, float& quick)
	{
		if (splashTimer < 4.f)
		{
			splashTimer += delta_time;
		}
		else
		{
			overlays[overlaysNameToID["SplashScreen"]]->Disable();
			overlays[overlaysNameToID["TitleScreen"]]->Enable();
		}
		bool buttonPressed = false;
		if (overlays[overlaysNameToID["CreditsScreen"]]->active)
		{
			if (Input::QueryButtonDown(GamepadButtons::B))
			{
				ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Return", (void*)0);
				overlays[overlaysNameToID["CreditsScreen"]]->Disable();
				overlays[overlaysNameToID["TitleScreen"]]->Enable();
				componentTypeMap[componentsNameToID["TitleMenu"]]->Enable();
				buttonPressed = true;
			}
		}
		else if (overlays[overlaysNameToID["SuccessScreen"]]->active || overlays[overlaysNameToID["DefeatScreen"]]->active)
		{
			if (Input::QueryButtonDown(GamepadButtons::B) || Input::QueryButtonDown(GamepadButtons::A))
			{
				ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Confirm", (void*)0);
				overlays[overlaysNameToID["CreditsScreen"]]->Disable();
				overlays[overlaysNameToID["SuccessScreen"]]->Disable();
				overlays[overlaysNameToID["DefeatScreen"]]->Disable();
				overlays[overlaysNameToID["HUD"]]->Disable();

				ADUI::UIMessage message;
				message.targetID = 1;
				message.commandID = 2;
				message.ivalue = 4;

				ADUI::MessageReceiver::SendMessage(&message);
				//Disable();

				overlays[overlaysNameToID["TitleScreen"]]->Enable();
				componentTypeMap[componentsNameToID["TitleMenu"]]->Enable();
				//controllers[controllersNameToID["TitleScreenController"]]->Enable();

				buttonPressed = true;
			}
		}
		return buttonPressed;
	}

	bool PauseMenuController::ProcessResponse(ADUI::UIMessage* _message, float& quick)
	{
		bool buttonPressed = false;
		UINT guideOverlayID = overlaysNameToID["GuideScreen"];
		if (_message)
		{
			if (!_message->externalMsg)
			{
				buttonPressed = true;
				if (_message->targetID == ADUI::UIMessageTargets::Controller)
				{
					if (_message->commandID == 1)
					{
						if (uiState == ADUI::UISTATE::MENUSTATE)
						{
							ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Confirm", (void*)0);
							UINT pauseOverlayID = overlaysNameToID["PauseScreen"];
							if (overlays[pauseOverlayID]->active)
							{
								switch (_message->componentIndex)
								{
								case 0:
								{
									overlays[overlaysNameToID["HUD"]]->visible = true;
									componentTypeMap[componentsNameToID["PauseMenu"]]->Disable();
									overlays[pauseOverlayID]->Disable();
									uiState = ADUI::UISTATE::GAMEPLAY;

									break;
								}
								case 1: //options
									overlays[pauseOverlayID]->Disable();
									//componentTypeMap[componentsNameToID["PauseMenu"]]->Disable();
									Disable();

									overlays[overlaysNameToID["OptionsScreen"]]->Enable();
									//componentTypeMap[componentsNameToID["OptionsMenu"]]->Enable();
									controllers[controllersNameToID["OptionScreenController"]]->Enable();
									break;
								case 2: //Exit Level
									overlays[pauseOverlayID]->Disable();
									componentTypeMap[componentsNameToID["PauseMenu"]]->Disable();
									Disable();

									overlays[overlaysNameToID["TitleScreen"]]->Enable();
									//componentTypeMap[componentsNameToID["TitleMenu"]]->Enable();
									controllers[controllersNameToID["TitleScreenController"]]->Enable();
									break;
								case 3:
									uiState = ADUI::UISTATE::QUIT;
									break;
								default:
									break;
								}
							}
						}
					}
					else
					{
						ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_MoveCursor", (void*)0);
					}

				}
			}
		}
		return buttonPressed;
	}

	bool PauseMenuController::ProcessInput(float delta_time, float& quick)
	{
		bool buttonPressed = false;
		if (Input::QueryButtonDown(GamepadButtons::Menu))
		{
			buttonPressed = true;
			UINT pauseOverlayID = overlaysNameToID["PauseScreen"];
			UINT hudOverlayID = overlaysNameToID["HUD"];
			if (overlays[pauseOverlayID]->active)
			{
				overlays[hudOverlayID]->visible = false;
				overlays[pauseOverlayID]->Disable();
				uiState = ADUI::UISTATE::GAMEPLAY;
				componentTypeMap[componentsNameToID["PauseMenu"]]->Disable();
				ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Return", (void*)0);
			}
			else
			{
				overlays[hudOverlayID]->visible = true;
				overlays[pauseOverlayID]->Enable();
				uiState = ADUI::UISTATE::MENUSTATE;
				componentTypeMap[componentsNameToID["PauseMenu"]]->Enable();
			}
		}
		else if (Input::QueryButtonDown(GamepadButtons::B))
		{
			buttonPressed = true;
			if (overlays[overlaysNameToID["GuideScreen"]]->active)
			{
				overlays[overlaysNameToID["GuideScreen"]]->Disable();
				componentTypeMap[componentsNameToID["PauseMenu"]]->Enable();
				overlays[overlaysNameToID["PauseScreen"]]->Enable();
				ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Return", (void*)0);
			}
		}
		return buttonPressed;
	}

	void OptionsMenuUIController::SetAudio(AD_AUDIO::ADAudio* _audioSystem)
	{
		audioSystem = _audioSystem;
	}

	bool OptionsMenuUIController::ProcessResponse(ADUI::UIMessage* _message, float& quick)
	{
		bool buttonPressed = false;
		if (_message)
		{
			if (!_message->externalMsg)
			{
				if (_message->targetID == ADUI::UIMessageTargets::Controller)
				{
					buttonPressed = true;
					if (_message->commandID == 1)
					{
						if (componentTypeMap[componentsNameToID["AudioMenu"]]->active)
						{
							ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_SliderClick", (void*)0);
							switch (_message->componentIndex)
							{
							case AD_AUDIO::MUSIC:
								audioSystem->masterMusicVolume = _message->fvalue.x;
								quick = 0.1f;
								audioSystem->RefreshMusicVolumes();
								break;
							case AD_AUDIO::SOUND_FX:
								audioSystem->masterSoundFXVolume = _message->fvalue.x;
								quick = 0.1f;
								break;
							case AD_AUDIO::UI_SOUND_FX:
								audioSystem->masterUISoundFXVolume = _message->fvalue.x;
								quick = 0.1f;
								break;
							default:
								break;
							}

						}
						else
						{
							switch (_message->componentIndex)
							{
							case 0: //Audio
								componentTypeMap[componentsNameToID["OptionsMenu"]]->Disable();
								componentTypeMap[componentsNameToID["AudioMenu"]]->Enable();
								ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Confirm", (void*)0);
								break;
							case 1: //Controls
								componentTypeMap[componentsNameToID["OptionsMenu"]]->Disable();
								componentTypeMap[componentsNameToID["ControlsBG"]]->Enable();
								componentTypeMap[componentsNameToID["ControlsMenu"]]->Enable();
								ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Confirm", (void*)0);
								break;
							default:
								break;
							}
						}
					}
					else
					{
						ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_MoveCursor", (void*)0);
					}
				}
			}
		}
		return buttonPressed;
	}

	bool OptionsMenuUIController::ProcessInput(float delta_time, float& quick)
	{
		bool buttonPressed = false;
		if (Input::QueryButtonDown(GamepadButtons::B))
		{
			ADEvents::ADEventSystem::Instance()->SendEvent("UI_Sfx_Return", (void*)0);
			if (componentTypeMap[componentsNameToID["AudioMenu"]]->active || componentTypeMap[componentsNameToID["ControlsMenu"]]->active)// || componentTypeMap[componentsNameToID["VideoMenu"]]->active)
			{
				componentTypeMap[componentsNameToID["AudioMenu"]]->Disable();
				componentTypeMap[componentsNameToID["ControlsBG"]]->Disable();
				componentTypeMap[componentsNameToID["ControlsMenu"]]->Disable();
				//componentTypeMap[componentsNameToID["VideoMenu"]]->Disable();
				componentTypeMap[componentsNameToID["OptionsMenu"]]->Enable();
				buttonPressed = true;
			}
			else
			{
				overlays[overlaysNameToID["OptionsScreen"]]->Disable();
				Disable();

				overlays[overlaysNameToID["PauseScreen"]]->Enable();
				controllers[controllersNameToID["PauseScreenController"]]->Enable();
				buttonPressed = true;
			}
		}
		return buttonPressed;
	}

	UINT GameUserInterface::SetupTitleScreen(ADUI::ADUI* myUI, StartMenuUIController* _titleScreenController)
	{
		float buttonSpacing = 20;
		UINT titleFont = 3;
		UINT CreditsFont = 4;
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };


		//TitleScreen
		UINT splashID = myUI->AddNewOverlay("SplashScreen", true, true);


		ADUI::Image2D* splashScreenbg = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { 0, 0,  3840, 2160});
		splashScreenbg->BuildAnimation({ 960,4626,961,4627 }, 1, 1, emptyAnimation);
		splashScreenbg->visible = true;
		splashScreenbg->stretched = true;
		myUI->AddUIComponent("Splash", splashScreenbg);
		myUI->overlays[splashID]->AddComponent(splashScreenbg);
		
		ADUI::Image2D* splashScreen = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { 1650, 845, 0,0 });
		splashScreen->BuildAnimation({ 3300,4812,3840,5280 }, 1, 1, emptyAnimation);
		splashScreen->visible = true;
		myUI->AddUIComponent("Splash2", splashScreen);
		myUI->overlays[splashID]->AddComponent(splashScreen);
		
		//TitleScreen
		UINT titleid = myUI->AddNewOverlay("TitleScreen", false, false);
		ADUI::Image2D* titleImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { myUI->viewport->TopLeftX,  myUI->viewport->TopLeftY,  (myUI->viewport->TopLeftX + myUI->viewport->Width),   (myUI->viewport->TopLeftY + myUI->viewport->Height) });
		titleImage->BuildAnimation({ 0,0,3840,2160 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("TitleBG", titleImage);
		myUI->overlays[titleid]->AddComponent(titleImage);


		//Credits Screen
		UINT creditsID = myUI->AddNewOverlay("CreditsScreen", false, false);
		ADUI::Image2D* creditsImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { myUI->viewport->TopLeftX,  myUI->viewport->TopLeftY,  (myUI->viewport->TopLeftX + myUI->viewport->Width),   (myUI->viewport->TopLeftY + myUI->viewport->Height) });
		creditsImage->BuildAnimation({ 0, 2160, 3840, 4320 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("CreditsBG", creditsImage);
		myUI->overlays[creditsID]->AddComponent(creditsImage);

		ADUI::Label2D* creditsLabel0 = new ADUI::Label2D();
		creditsLabel0->SetFont(myUI->GetFont(CreditsFont));
		creditsLabel0->SetText("Programmers", XMFLOAT2(800, 400)
		);
		creditsLabel0->Enable();
		creditsLabel0->SetTint(Colors::SteelBlue);
		myUI->AddUIComponent("CreditsLabel0", creditsLabel0);
		myUI->overlays[creditsID]->AddComponent(creditsLabel0);

		ADUI::Label2D* creditsLabel = new ADUI::Label2D();
		creditsLabel->SetFont(myUI->GetFont(CreditsFont - 1));
		creditsLabel->SetText(" Taylor Addington \n Moises Carmona \n Gage Dietz \n Daniel Hayward \n Ahmed Fayez", XMFLOAT2(800, 750)
		);
		creditsLabel->Enable();
		creditsLabel->SetTint(Colors::SteelBlue);
		myUI->AddUIComponent("CreditsLabel1", creditsLabel);
		myUI->overlays[creditsID]->AddComponent(creditsLabel);

		ADUI::Label2D* creditsLabel2 = new ADUI::Label2D();
		creditsLabel2->SetFont(myUI->GetFont(CreditsFont));
		creditsLabel2->SetText("Producers", XMFLOAT2(2300, 400)
		);
		creditsLabel2->Enable();
		creditsLabel2->SetTint(Colors::SteelBlue);
		myUI->AddUIComponent("CreditsLabel2", creditsLabel2);
		myUI->overlays[creditsID]->AddComponent(creditsLabel2);

		ADUI::Label2D* creditsLabel3 = new ADUI::Label2D();
		creditsLabel3->SetFont(myUI->GetFont(CreditsFont - 1));
		creditsLabel3->SetText(" Michael Hervieux \n Verdayne Graham", XMFLOAT2(2400, 600)
		);
		creditsLabel3->Enable();
		creditsLabel3->SetTint(Colors::SteelBlue);
		myUI->AddUIComponent("CreditsLabel3", creditsLabel3);
		myUI->overlays[creditsID]->AddComponent(creditsLabel3);

		ADUI::Label2D* creditsLabel4 = new ADUI::Label2D();
		creditsLabel4->SetFont(myUI->GetFont(CreditsFont));
		creditsLabel4->SetText("Audio Designer", XMFLOAT2(2450, 900)
		);
		creditsLabel4->Enable();
		creditsLabel4->SetTint(Colors::SteelBlue);
		myUI->AddUIComponent("CreditsLabel4", creditsLabel4);
		myUI->overlays[creditsID]->AddComponent(creditsLabel4);

		ADUI::Label2D* creditsLabel5 = new ADUI::Label2D();
		creditsLabel5->SetFont(myUI->GetFont(CreditsFont - 1));
		creditsLabel5->SetText(" Josh Packard", XMFLOAT2(2350, 1000)
		);
		creditsLabel5->Enable();
		creditsLabel5->SetTint(Colors::SteelBlue);
		myUI->AddUIComponent("CreditsLabel3", creditsLabel5);
		myUI->overlays[creditsID]->AddComponent(creditsLabel5);


		//Start Menu Buttons
		ADUI::AnimationData* buttonAnimation = new ADUI::AnimationData[2];
		buttonAnimation[0] = { 0, 1, 1 };
		buttonAnimation[1] = { 1, 2, 6 };
		ADUI::ComponentGrid* buttonList = new ADUI::ComponentGrid();
		XMFLOAT4 position = { 1540, 1080, 2320, 1275 };
		XMFLOAT2 spacing = { buttonSpacing, buttonSpacing };
		ADUI::Image2D* buttonIcon = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], position);
		buttonIcon->BuildAnimation({ 60, 4380, 840, 4575 }, 1, 2, buttonAnimation);
		buttonIcon->active = true;
		buttonIcon->controlFocusAnimation = 1;
		ADUI::Label2D* lbl = new ADUI::Label2D();
		lbl->SetFont(myUI->GetFont(CreditsFont));
		lbl->SetText("Start", { 1920, 1180 });// XMFLOAT2(1920, 1080));
		lbl->active = true;
		lbl->visible = true;
		ADUI::Button2D* btn = new ADUI::Button2D(position, buttonIcon, lbl, true, true, false);
		buttonList->Generate(btn, position, spacing, 3, 1, ADUI::ComponentGridStyle::ExactSpacing, true, true);
		buttonList->visible = true;
		//buttonList->components[1]->SetText("Load");
		buttonList->components[1]->SetText("Credits");
		buttonList->components[2]->SetText("Quit");
		myUI->AddUIComponent("TitleMenu", buttonList);
		myUI->overlays[titleid]->AddComponent(buttonList);

		_titleScreenController->AddComponent(buttonList);
		_titleScreenController->AddOverlay(myUI->overlays[splashID]);
		_titleScreenController->AddOverlay(myUI->overlays[titleid]);
		_titleScreenController->AddOverlay(myUI->overlays[creditsID]);

		return titleid;
	}

	UINT GameUserInterface::SetupPauseScreen(ADUI::ADUI* myUI, PauseMenuController* _pauseMenuController)
	{
		float buttonSpacing = 20;
		XMFLOAT2 spacing = { buttonSpacing, buttonSpacing };
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };
		ADUI::AnimationData* buttonAnimation = new ADUI::AnimationData[2];
		buttonAnimation[0] = { 0, 1, 1 };
		buttonAnimation[1] = { 1, 2, 6 };

		//Pause Screen
		UINT pauseID = myUI->AddNewOverlay("PauseScreen", false, false);


		//Pause Menu Buttons
		ADUI::ComponentGrid* buttonList2 = new ADUI::ComponentGrid();
		XMFLOAT4 position = { 1540, 780, 2320, 975 };
		ADUI::Image2D* buttonIcon = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], position);
		buttonIcon->BuildAnimation({ 60, 4380, 840, 4575 }, 1, 2, buttonAnimation);
		buttonIcon->active = true;
		buttonIcon->controlFocusAnimation = 1;
		ADUI::Label2D* lbl = new ADUI::Label2D();
		lbl->SetFont(myUI->GetFont(4));
		lbl->SetText("Start", { 1920, 1180 });// XMFLOAT2(1920, 1080));
		lbl->active = true;
		lbl->visible = true;
		ADUI::Button2D* btn2 = new ADUI::Button2D(position, buttonIcon, lbl, true, true, false);
		buttonList2->Generate(btn2, position, spacing, 4, 1, ADUI::ComponentGridStyle::ExactSpacing, true, true);
		buttonList2->visible = true;
		buttonList2->components[0]->SetText("Resume");
		buttonList2->components[1]->SetText("Options");
		buttonList2->components[2]->SetText("Main Menu");
		buttonList2->components[3]->SetText("Quit");
		buttonList2->Disable();
		myUI->AddUIComponent("PauseMenu", buttonList2);
		myUI->overlays[pauseID]->AddComponent(buttonList2);

		_pauseMenuController->AddComponent(buttonList2);
		_pauseMenuController->AddOverlay(myUI->overlays[pauseID]);

		return pauseID;
	}

	UINT GameUserInterface::SetupOptionsScreen(ADUI::ADUI* myUI, OptionsMenuUIController* _optionsMenuUIController)
	{
		float buttonSpacing = 20;
		XMFLOAT2 spacing = { buttonSpacing, buttonSpacing };
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };
		ADUI::AnimationData* buttonAnimation = new ADUI::AnimationData[2];
		buttonAnimation[0] = { 0, 1, 1 };
		buttonAnimation[1] = { 1, 2, 6 };

		//OptionsScreen
		UINT optionsID = myUI->AddNewOverlay("OptionsScreen", false, false);
		/////*ADUI::Image2D* optionsImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { 1550, 400, 3070, 1560 });
		////optionsImage->BuildAnimation({ 0, 4320, 2299, 5280 }, 1, 1, emptyAnimation);
		////myUI->AddUIComponent("OptionsBox", optionsImage);
		////myUI->overlays[optionsID]->AddComponent(optionsImage);

			//Option Menu Buttons
		ADUI::ComponentGrid* buttonList3 = new ADUI::ComponentGrid();
		XMFLOAT4 position = { 1540, 780, 2320, 975 };
		ADUI::Image2D* buttonIcon = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], position);
		buttonIcon->BuildAnimation({ 60, 4380, 840, 4575 }, 1, 2, buttonAnimation);
		buttonIcon->active = true;
		buttonIcon->controlFocusAnimation = 1;
		ADUI::Label2D* lbl = new ADUI::Label2D();
		lbl->SetFont(myUI->GetFont(4));
		lbl->SetText("Start", { 1920, 1180 });// XMFLOAT2(1920, 1080));
		lbl->active = true;
		lbl->visible = true;
		ADUI::Button2D* btn3 = new ADUI::Button2D(position, buttonIcon, lbl, true, true, false);
		buttonList3->Generate(btn3, position, spacing, 2, 1, ADUI::ComponentGridStyle::ExactSpacing, true, true);
		buttonList3->Enable();
		buttonList3->components[0]->SetText("Audio");
		buttonList3->components[1]->SetText("Controls");
		myUI->AddUIComponent("OptionsMenu", buttonList3);
		myUI->overlays[optionsID]->AddComponent(buttonList3);


		ADUI::Image2D* slideImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1614, 635, 1692, 715 });
		slideImage->BuildAnimation({ 2911, 975, 2989, 1055 }, 1, 1, emptyAnimation);
		slideImage->active = true;
		slideImage->controlFocusAnimation = 0;
		slideImage->tiled = 8;
		ADUI::Image2D* sliderImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1616, 635, 2150, 715 });
		sliderImage->BuildAnimation({ 3070, 975, 3150, 1055 }, 1, 1, buttonAnimation);
		sliderImage->active = true;
		sliderImage->controlFocusAnimation = 0;
		XMFLOAT2 sliderPosition = { 1800, 635 };
		XMFLOAT2 sliderMinPosition = { 1540, 635 };
		XMFLOAT2 sliderMaxPosition = { 2238, 635 };
		ADUI::Slider* slider = new ADUI::Slider(sliderImage, sliderPosition, sliderMaxPosition, sliderMinPosition);
		ADUI::Image2D* leftCapImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1536, 635, 1620, 715 });
		leftCapImage->BuildAnimation({ 2830, 975, 2908, 1055 }, 1, 1, emptyAnimation);
		leftCapImage->active = true;
		leftCapImage->controlFocusAnimation = 0;
		ADUI::Image2D* rightCapImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 2238, 635, 2251, 715 });
		rightCapImage->BuildAnimation({ 2993, 975, 3071, 1055 }, 1, 1, emptyAnimation);
		rightCapImage->active = true;
		rightCapImage->controlFocusAnimation = 0;
		ADUI::Label2D* sliderLabel = new ADUI::Label2D();
		sliderLabel->SetFont(myUI->GetFont(4));
		sliderLabel->SetText("Music", { 1920, 580 });// XMFLOAT2(1920, 1080));
		sliderLabel->active = true;
		sliderLabel->visible = true;
		ADUI::SliderBar* sliderBar = new ADUI::SliderBar(slideImage, slider, leftCapImage, rightCapImage, sliderLabel);
		sliderBar->Enable();

		ADUI::Image2D* slideImage2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1614, 835, 1692, 915 });
		slideImage2->BuildAnimation({ 2911, 975, 2989, 1055 }, 1, 1, emptyAnimation);
		slideImage2->active = true;
		slideImage2->controlFocusAnimation = 0;
		slideImage2->tiled = 8;
		ADUI::Image2D* sliderImage2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1616, 835, 2150, 915 });
		sliderImage2->BuildAnimation({ 3070, 975, 3150, 1055 }, 1, 1, buttonAnimation);
		sliderImage2->active = true;
		sliderImage2->controlFocusAnimation = 0;
		sliderPosition = { 1800, 835 };
		sliderMinPosition = { 1540, 835 };
		sliderMaxPosition = { 2238, 835 };
		ADUI::Slider* slider2 = new ADUI::Slider(sliderImage2, sliderPosition, sliderMaxPosition, sliderMinPosition);
		ADUI::Image2D* leftCapImage2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1536, 835, 1620, 915 });
		leftCapImage2->BuildAnimation({ 2830, 975, 2908, 1055 }, 1, 1, emptyAnimation);
		leftCapImage2->active = true;
		leftCapImage2->controlFocusAnimation = 0;
		ADUI::Image2D* rightCapImage2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 2238, 835, 2251, 915 });
		rightCapImage2->BuildAnimation({ 2993, 975, 3071, 1055 }, 1, 1, emptyAnimation);
		rightCapImage2->active = true;
		rightCapImage2->controlFocusAnimation = 0;
		ADUI::Label2D* sliderLabel2 = new ADUI::Label2D();
		sliderLabel2->SetFont(myUI->GetFont(4));
		sliderLabel2->SetText("Sound FX", { 1920, 780 });// XMFLOAT2(1920, 1080));
		sliderLabel2->active = true;
		sliderLabel2->visible = true;
		ADUI::SliderBar* sliderBar2 = new ADUI::SliderBar(slideImage2, slider2, leftCapImage2, rightCapImage2, sliderLabel2);
		sliderBar2->Enable();

		ADUI::Image2D* slideImage3 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1614, 1035, 1692, 1115 });
		slideImage3->BuildAnimation({ 2911, 975, 2989, 1055 }, 1, 1, emptyAnimation);
		slideImage3->active = true;
		slideImage3->controlFocusAnimation = 0;
		slideImage3->tiled = 8;
		ADUI::Image2D* sliderImage3 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1616, 1035, 2150, 1115 });
		sliderImage3->BuildAnimation({ 3070, 975, 3150, 1055 }, 1, 1, buttonAnimation);
		sliderImage3->active = true;
		sliderImage3->controlFocusAnimation = 0;
		sliderPosition = { 1800, 1235 };
		sliderMinPosition = { 1540, 1035 };
		sliderMaxPosition = { 2238, 1035 };
		ADUI::Slider* slider3 = new ADUI::Slider(sliderImage3, sliderPosition, sliderMaxPosition, sliderMinPosition);
		ADUI::Image2D* leftCapImage3 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1536, 1035, 1620, 1115 });
		leftCapImage3->BuildAnimation({ 2830, 975, 2908, 1055 }, 1, 1, emptyAnimation);
		leftCapImage3->active = true;
		leftCapImage3->controlFocusAnimation = 0;
		ADUI::Image2D* rightCapImage3 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 2238, 1035, 2251, 1115 });
		rightCapImage3->BuildAnimation({ 2993, 975, 3071, 1055 }, 1, 1, emptyAnimation);
		rightCapImage3->active = true;
		rightCapImage3->controlFocusAnimation = 0;
		ADUI::Label2D* sliderLabel3 = new ADUI::Label2D();
		sliderLabel3->SetFont(myUI->GetFont(4));
		sliderLabel3->SetText("Menu Sound FX", { 1920, 980 });// XMFLOAT2(1920, 1080));
		sliderLabel3->active = true;
		sliderLabel3->visible = true;
		ADUI::SliderBar* sliderBar3 = new ADUI::SliderBar(slideImage3, slider3, leftCapImage3, rightCapImage3, sliderLabel3);
		sliderBar3->Enable();

		ADUI::ComponentGrid* sliderList = new ADUI::ComponentGrid();
		sliderList->SetCorners({ 1614, 635, 1692, 715 });
		sliderList->positions = new RECT[3];
		sliderList->positions[0] = { 1614, 635, 1692, 715 };
		sliderList->spacing = 20.f;
		sliderList->columns = 1;
		sliderList->active = false;

		sliderList->AddComponent(sliderBar);
		sliderList->AddComponent(sliderBar2);
		sliderList->AddComponent(sliderBar3);

		sliderList->Initialize();
		sliderList->Disable();

		myUI->AddUIComponent("AudioMenu", sliderList);
		myUI->overlays[optionsID]->AddComponent(sliderList);


		ADUI::Image2D* controlScreenBG = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 125, 400, 3720, 1820 });
		controlScreenBG->BuildAnimation({ 2315, 1080, 3825, 1970 }, 1, 1, emptyAnimation);
		controlScreenBG->active = false;
		controlScreenBG->visible = false;
		controlScreenBG->stretched = true;
		myUI->AddUIComponent("ControlsBG", controlScreenBG);
		myUI->overlays[optionsID]->AddComponent(controlScreenBG);

		ADUI::Image2D* controlsScreenControls = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 175, 550, 3400, 2000 });
		controlsScreenControls->BuildAnimation({ 210, 3550, 3705, 4800 }, 1, 1, emptyAnimation);
		controlsScreenControls->active = false;
		controlsScreenControls->visible = false;
		myUI->AddUIComponent("ControlsMenu", controlsScreenControls);
		myUI->overlays[optionsID]->AddComponent(controlsScreenControls);
		//////myUI->AddUIComponent("ControlsMenu", controlsList);
		//////myUI->overlays[optionsID]->AddComponent(controlsList);

		//////ADUI::ButtonList* VideoList = new ADUI::ButtonList();
		//////VideoList->x = 0;
		//////VideoList->y = 0;
		//////VideoList->spacing = 20.f;
		//////VideoList->columns = 1;
		//////VideoList->active = false;
		//////VideoList->controlFocus = false;
		//////VideoList->AddButton(sliderBar);
		//////VideoList->Initialize();
		//////VideoList->Disable();

		//myUI->AddUIComponent("VideoMenu", slideImage);
		//myUI->overlays[optionsID]->AddComponent(slideImage);
		////myUI->AddUIComponent("VideoMenu", VideoList);
		////myUI->overlays[optionsID]->AddComponent(VideoList);

		_optionsMenuUIController->AddComponent(sliderList);
		_optionsMenuUIController->AddComponent(buttonList3);
		_optionsMenuUIController->AddComponent(controlScreenBG);
		_optionsMenuUIController->AddComponent(controlsScreenControls);
		_optionsMenuUIController->AddOverlay(myUI->overlays[optionsID]);

		return optionsID;
	}

	UINT GameUserInterface::SetupTutorialScreen(ADUI::ADUI* myUI, TutorialController* _tutorialController)
	{
		UINT tutorialID = myUI->AddNewOverlay("Tutorial", false, false);

		//SuccessScreen
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };

		ADUI::Image2D* controlScreenBG = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 125, 400, 3720, 1820 });
		controlScreenBG->BuildAnimation({ 2315, 1080, 3825, 1970 }, 1, 1, emptyAnimation);
		controlScreenBG->active = false;
		controlScreenBG->visible = true;
		controlScreenBG->stretched = true;
		myUI->AddUIComponent("ControlsBG", controlScreenBG);
		myUI->overlays[tutorialID]->AddComponent(controlScreenBG);

		ADUI::Image2D* controlsScreenControls = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 175, 550, 3400, 2000 });
		controlsScreenControls->BuildAnimation({ 210, 3550, 3705, 4800 }, 1, 1, emptyAnimation);
		controlsScreenControls->active = false;
		controlsScreenControls->visible = false;
		myUI->AddUIComponent("Page0", controlsScreenControls);
		myUI->overlays[tutorialID]->AddComponent(controlsScreenControls);
		_tutorialController->AddComponent(controlsScreenControls);

		ADUI::Image2D* hidingInHouse = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 400, 550, 3400, 2000 });
		hidingInHouse->BuildAnimation({ 2740, 1970, 3330, 2625 }, 1, 1, emptyAnimation);
		hidingInHouse->active = false;
		hidingInHouse->visible = false;
		myUI->AddUIComponent("Page1", hidingInHouse);
		myUI->overlays[tutorialID]->AddComponent(hidingInHouse);
		_tutorialController->AddComponent(hidingInHouse);
		
		ADUI::Image2D* townHall = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1800, 550, 3400, 2000 });
		townHall->BuildAnimation({ 2316, 1966, 2731, 2326 }, 1, 1, emptyAnimation);
		townHall->active = false;
		townHall->visible = false;
		myUI->AddUIComponent("Page2", townHall);
		myUI->overlays[tutorialID]->AddComponent(townHall);
		_tutorialController->AddComponent(townHall);

		ADUI::Image2D* spellIcons = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1700, 550, 3400, 2000 });
		spellIcons->BuildAnimation({ 0, 1113, 560, 1236 }, 1, 1, emptyAnimation);
		spellIcons->active = false;
		spellIcons->visible = false;
		myUI->AddUIComponent("Page3", spellIcons);
		myUI->overlays[tutorialID]->AddComponent(spellIcons);
		_tutorialController->AddComponent(spellIcons);
		

		ADUI::Image2D* faceIcons = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1800, 550, 3400, 2000 });
		faceIcons->BuildAnimation({ 220, 1337, 622, 1437 }, 1, 1, emptyAnimation);
		faceIcons->active = false;
		faceIcons->visible = false;
		myUI->AddUIComponent("Page45", faceIcons);
		myUI->overlays[tutorialID]->AddComponent(faceIcons);
		_tutorialController->AddComponent(faceIcons);

		ADUI::Label2D* messageLabel = new ADUI::Label2D();
		messageLabel->SetFont(myUI->GetFont(3));
		messageLabel->SetText(" ", { 1900, 1100 });// XMFLOAT2(1920, 1080));
		messageLabel->active = true;
		messageLabel->visible = true;
		ADUI::Label2D* messageLabel2 = new ADUI::Label2D();
		messageLabel2->SetFont(myUI->GetFont(3));
		messageLabel2->SetText(" ", { 1900, 1300 });// XMFLOAT2(1920, 1080));
		messageLabel2->active = true;
		messageLabel2->visible = true;
		ADUI::Label2D* messageLabel3 = new ADUI::Label2D();
		messageLabel3->SetFont(myUI->GetFont(3));
		messageLabel3->SetText(" ", { 1900, 1500 });// XMFLOAT2(1920, 1080));
		messageLabel3->active = true;
		messageLabel3->visible = true;
		ADUI::Label2D* messageLabel4 = new ADUI::Label2D();
		messageLabel4->SetFont(myUI->GetFont(3));
		messageLabel4->SetText(" ", { 1900, 1700 });// XMFLOAT2(1920, 1080));
		messageLabel4->active = true;
		messageLabel4->visible = true;
		myUI->AddUIComponent("messageLabel", messageLabel);
		myUI->overlays[tutorialID]->AddComponent(messageLabel);
		myUI->AddUIComponent("messageLabel2", messageLabel2);
		myUI->overlays[tutorialID]->AddComponent(messageLabel2);
		myUI->AddUIComponent("messageLabel3", messageLabel3);
		myUI->overlays[tutorialID]->AddComponent(messageLabel3);
		myUI->AddUIComponent("messageLabel4", messageLabel4);
		myUI->overlays[tutorialID]->AddComponent(messageLabel4);
		_tutorialController->AddOverlay(myUI->overlays[tutorialID]);
		_tutorialController->pagePicture.push_back(controlsScreenControls);
		_tutorialController->pagePicture.push_back(hidingInHouse);
		_tutorialController->pagePicture.push_back(townHall);
		_tutorialController->pagePicture.push_back(faceIcons);
		_tutorialController->pagePicture.push_back(faceIcons);
		_tutorialController->pagePicture.push_back(spellIcons);
		_tutorialController->text.push_back(messageLabel);
		_tutorialController->text.push_back(messageLabel2);
		_tutorialController->text.push_back(messageLabel3);
		_tutorialController->text.push_back(messageLabel4);



		return tutorialID;
	}

	UINT GameUserInterface::SetupEngGameScreen(ADUI::ADUI* myUI)
	{
		//SuccessScreen
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };
		UINT successID = myUI->AddNewOverlay("SuccessScreen", false, false);
		ADUI::Image2D* creditsImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { myUI->viewport->TopLeftX,  myUI->viewport->TopLeftY,  (myUI->viewport->TopLeftX + myUI->viewport->Width),   (myUI->viewport->TopLeftY + myUI->viewport->Height) });
		creditsImage->BuildAnimation({ 0, 2160, 3840, 4320 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("CreditsBG", creditsImage);
		myUI->overlays[successID]->AddComponent(creditsImage);


		ADUI::Label2D* gameOverText = new ADUI::Label2D();
		gameOverText->SetFont(myUI->GetFont(5));
		gameOverText->SetText("Success!", { 1900, 480 });// XMFLOAT2(1920, 1080));
		gameOverText->active = true;
		gameOverText->visible = true;
		gameOverText->SetTint({ .1f,0.5f,0.8f,1 });
		myUI->AddUIComponent("EndScreenText", gameOverText);
		myUI->overlays[successID]->AddComponent(gameOverText);



		return successID;
	}

	UINT GameUserInterface::SetupDefeatGameScreen(ADUI::ADUI* myUI)
	{
		//SuccessScreen
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };
		UINT creditsID = myUI->AddNewOverlay("DefeatScreen", false, false);
		ADUI::Image2D* creditsImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { myUI->viewport->TopLeftX,  myUI->viewport->TopLeftY,  (myUI->viewport->TopLeftX + myUI->viewport->Width),   (myUI->viewport->TopLeftY + myUI->viewport->Height) });
		creditsImage->BuildAnimation({ 0, 2160, 3840, 4320 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("DefeatBG", creditsImage);
		myUI->overlays[creditsID]->AddComponent(creditsImage);


		ADUI::Label2D* gameOverText = new ADUI::Label2D();
		gameOverText->SetFont(myUI->GetFont(5));
		gameOverText->SetText("Defeat!", { 1900, 480 });// XMFLOAT2(1920, 1080));
		gameOverText->active = true;
		gameOverText->visible = true;
		gameOverText->SetTint({ 0.8f,0.5f,.1f,1 });
		myUI->AddUIComponent("GameOverScreenText", gameOverText);
		myUI->overlays[creditsID]->AddComponent(gameOverText);


		return creditsID;
	}

	UINT GameUserInterface::SetupHUD(ADUI::ADUI* myUI, HUDController* _hUDController)
	{
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };
		ADUI::AnimationData* multiFrameStillAnimation = new ADUI::AnimationData[1];
		multiFrameStillAnimation[0] = { 0, 2, 0 };
		ADUI::AnimationData* buttonAnimation = new ADUI::AnimationData[2];
		buttonAnimation[0] = { 0, 1, 1 };
		buttonAnimation[1] = { 0, 2, 6 };
		ADUI::AnimationData* healthFlash = new ADUI::AnimationData[2];
		healthFlash[0] = { 0, 1, 1 };
		healthFlash[1] = { 0, 2, 4 };
		ADUI::AnimationData* faceButtons = new ADUI::AnimationData[2];
		faceButtons[0] = { 0, 1, 24 };
		faceButtons[1] = { 1, 1, 24 };
		ADUI::AnimationData* faceAnimation = new ADUI::AnimationData[1];
		faceAnimation[0] = { 0, 4, 0 };
		ADUI::AnimationData* essenceAnimation = new ADUI::AnimationData[3];
		essenceAnimation[0] = { 0, 12, 24 };
		essenceAnimation[1] = { 19, 22, 24 };
		essenceAnimation[2] = { 40, 48, 24 };

		//HUD
		UINT hudID = myUI->AddNewOverlay("HUD", false, true);


		ADUI::Image2D* healthBar = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 100, 30, 322, 495 });
		healthBar->BuildAnimation({ 579, 960, 1910, 1175 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("HealhBarEmpty", healthBar);
		myUI->overlays[hudID]->AddComponent(healthBar);
		healthBar->Focus();
		_hUDController->AddComponent(healthBar);

		ADUI::Image2D* healthUnits = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 141, 38, 153, 111 });
		healthUnits->BuildAnimation({ 494, 961, 580, 1034 }, 1, 2, healthFlash);
		healthUnits->controlFocusAnimation = 1;
		healthUnits->tiled = 100;
		myUI->AddUIComponent("HealhBarUnit", healthUnits);
		myUI->overlays[hudID]->AddComponent(healthUnits);
		//healthUnits->Focus();
		_hUDController->AddComponent(healthUnits);

		ADUI::Image2D* fullring = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 99, 30, 315, 247 });
		fullring->BuildAnimation({ 0, 1236, 217, 1453 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("FullRing", fullring);
		myUI->overlays[hudID]->AddComponent(fullring);
		_hUDController->AddComponent(fullring);

		ADUI::Image2D* essenceGraphic = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 78, -60, 285, 378 });
		essenceGraphic->BuildAnimation({ 0, 1500, 256, 1800 }, 8, 3, essenceAnimation);
		essenceGraphic->controlFocusAnimation = 1;
		essenceGraphic->Focus();
		myUI->AddUIComponent("EssenceGraphic", essenceGraphic);
		myUI->overlays[hudID]->AddComponent(essenceGraphic);
		_hUDController->AddComponent(essenceGraphic);

		ADUI::Label2D* essenceLabel = new ADUI::Label2D();
		essenceLabel->SetFont(myUI->GetFont(2));
		essenceLabel->SetText("200", { 200, 140 });// XMFLOAT2(1920, 1080));
		essenceLabel->active = true;
		essenceLabel->visible = true;
		myUI->AddUIComponent("EssenceLabel", essenceLabel);
		myUI->overlays[hudID]->AddComponent(essenceLabel);
		_hUDController->AddComponent(essenceLabel);


		ADUI::Image2D* fireSelect = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 131, 30, 282, 78 });
		fireSelect->BuildAnimation({ 0, 1010, 151, 1058 }, 2, 2, buttonAnimation);
		fireSelect->controlFocusAnimation = 1;
		myUI->AddUIComponent("FireSelect", fireSelect);
		myUI->overlays[hudID]->AddComponent(fireSelect);
		_hUDController->AddComponent(fireSelect);

		ADUI::Image2D* waterSelect = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 99, 66, 315, 273 });
		waterSelect->BuildAnimation({ 302, 962, 351, 1113 }, 2, 2, buttonAnimation);
		waterSelect->controlFocusAnimation = 1;
		myUI->AddUIComponent("WaterSelect", waterSelect);
		myUI->overlays[hudID]->AddComponent(waterSelect);
		_hUDController->AddComponent(waterSelect);

		ADUI::Image2D* stoneSelect = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 131, 201, 282, 248 });
		stoneSelect->BuildAnimation({ 0, 962, 151, 1010 }, 2, 2, buttonAnimation);
		stoneSelect->controlFocusAnimation = 1;
		myUI->AddUIComponent("StoneSelect", stoneSelect);
		myUI->overlays[hudID]->AddComponent(stoneSelect);
		_hUDController->AddComponent(stoneSelect);

		ADUI::Image2D* woodSelect = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 269, 66, 485, 273 });
		woodSelect->BuildAnimation({ 398, 962, 446, 1113 }, 2, 2, buttonAnimation);
		woodSelect->controlFocusAnimation = 1;
		myUI->AddUIComponent("WoodSelect", woodSelect);
		myUI->overlays[hudID]->AddComponent(woodSelect);
		_hUDController->AddComponent(woodSelect);

		ADUI::Image2D* specialIcon = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 311, 124, 451, 244 });
		specialIcon->BuildAnimation({ 0, 1114, 140, 1235 }, 4, 1, faceAnimation);
		specialIcon->Focus();
		myUI->AddUIComponent("Specials", specialIcon);
		myUI->overlays[hudID]->AddComponent(specialIcon);
		_hUDController->AddComponent(specialIcon);
		ADUI::Image2D* specialIcon2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 452, 124, 591, 244 });
		specialIcon2->BuildAnimation({ 0, 1114, 140, 1235 }, 4, 1, faceAnimation);
		specialIcon2->Focus();
		myUI->AddUIComponent("Specials2", specialIcon2);
		myUI->overlays[hudID]->AddComponent(specialIcon2);
		_hUDController->AddComponent(specialIcon2);
		ADUI::Image2D* specialIcon3 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 592, 124, 732, 244 });
		specialIcon3->BuildAnimation({ 0, 1114, 140, 1235 }, 4, 1, faceAnimation);
		specialIcon3->Focus();
		myUI->AddUIComponent("Specials3", specialIcon3);
		myUI->overlays[hudID]->AddComponent(specialIcon3);
		_hUDController->AddComponent(specialIcon3);

		ADUI::Image2D* minionRightBar = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 80, 280, 3676, 1104 });
		minionRightBar->BuildAnimation({ 2180, 960, 2313, 1556 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("RightPlate", minionRightBar);
		myUI->overlays[hudID]->AddComponent(minionRightBar);
		minionRightBar->Focus();
		_hUDController->AddComponent(minionRightBar);

		ADUI::Image2D* minionFacesRightAll = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 97, 331, 197, 431 });
		minionFacesRightAll->BuildAnimation({ 221, 1237, 321, 1337 }, 1, 2, faceButtons);
		myUI->AddUIComponent("GolemFaceAll", minionFacesRightAll);
		myUI->overlays[hudID]->AddComponent(minionFacesRightAll);
		minionFacesRightAll->Focus();
		minionFacesRightAll->controlFocusAnimation = 1;
		//_hUDController->AddComponent(minionFacesRightAll);

		ADUI::Label2D* stoneCountLabel = new ADUI::Label2D();
		stoneCountLabel->SetFont(myUI->GetFont(2));
		stoneCountLabel->SetText("10", { 250, 380 });// XMFLOAT2(1920, 1080));
		stoneCountLabel->active = true;
		stoneCountLabel->visible = true;
		myUI->AddUIComponent("StoneCountLabel", stoneCountLabel);
		myUI->overlays[hudID]->AddComponent(stoneCountLabel);
		_hUDController->AddComponent(stoneCountLabel);

		ADUI::Image2D* minionFacesRight1 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 97, 433, 197, 533 });
		minionFacesRight1->BuildAnimation({ 321, 1237, 421, 1337 }, 1, 2, faceButtons);
		myUI->AddUIComponent("GolemFaceRight1", minionFacesRight1);
		myUI->overlays[hudID]->AddComponent(minionFacesRight1);
		//minionFacesRight1->Focus();
		minionFacesRight1->controlFocusAnimation = 1;
		//_hUDController->AddComponent(minionFacesRight1);

		ADUI::Label2D* waterCountLabel = new ADUI::Label2D();
		waterCountLabel->SetFont(myUI->GetFont(2));
		waterCountLabel->SetText("10", { 250, 480 });// XMFLOAT2(1920, 1080));
		waterCountLabel->active = true;
		waterCountLabel->visible = true;
		myUI->AddUIComponent("WaterCountLabel", waterCountLabel);
		myUI->overlays[hudID]->AddComponent(waterCountLabel);
		_hUDController->AddComponent(waterCountLabel);

		ADUI::Image2D* minionFacesRight2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 97, 535, 197, 635 });
		minionFacesRight2->BuildAnimation({ 421, 1237, 521, 1337 }, 1, 2, faceButtons);
		myUI->AddUIComponent("GolemFaceRight2", minionFacesRight2);
		myUI->overlays[hudID]->AddComponent(minionFacesRight2);
		//minionFacesRight2->Focus();
		minionFacesRight2->controlFocusAnimation = 1;
		//_hUDController->AddComponent(minionFacesRight2);

		ADUI::Label2D* fireCountLabel = new ADUI::Label2D();
		fireCountLabel->SetFont(myUI->GetFont(2));
		fireCountLabel->SetText("10", { 250, 580 });// XMFLOAT2(1920, 1080));
		fireCountLabel->active = true;
		fireCountLabel->visible = true;
		myUI->AddUIComponent("FireCountLabel", fireCountLabel);
		myUI->overlays[hudID]->AddComponent(fireCountLabel);
		_hUDController->AddComponent(fireCountLabel);

		ADUI::Image2D* minionFacesRight3 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 97, 637, 197, 737 });
		minionFacesRight3->BuildAnimation({ 521, 1237, 621, 1337 }, 1, 2, faceButtons);
		myUI->AddUIComponent("GolemFaceRight3", minionFacesRight3);
		myUI->overlays[hudID]->AddComponent(minionFacesRight3);
		//minionFacesRight3->Focus();
		minionFacesRight3->controlFocusAnimation = 1;
		//_hUDController->AddComponent(minionFacesRight3);

		ADUI::Label2D* woodCountLabel = new ADUI::Label2D();
		woodCountLabel->SetFont(myUI->GetFont(2));
		woodCountLabel->SetText("10", { 250, 680 });// XMFLOAT2(1920, 1080));
		woodCountLabel->active = true;
		woodCountLabel->visible = true;
		myUI->AddUIComponent("WoodCountLabel", woodCountLabel);
		myUI->overlays[hudID]->AddComponent(woodCountLabel);
		_hUDController->AddComponent(woodCountLabel);

		ADUI::Image2D* minionFacesRight4 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 97, 742, 197, 842 });
		minionFacesRight4->BuildAnimation({ 621, 1237, 721, 1337 }, 1, 2, faceButtons);
		myUI->AddUIComponent("GolemFaceRight4", minionFacesRight4);
		myUI->overlays[hudID]->AddComponent(minionFacesRight4);
		//minionFacesRight4->Focus();
		minionFacesRight4->controlFocusAnimation = 1;
		//_hUDController->AddComponent(minionFacesRight4);

		ADUI::Label2D* allCountLabel = new ADUI::Label2D();
		allCountLabel->SetFont(myUI->GetFont(2));
		allCountLabel->SetText("40", { 250, 780 });// XMFLOAT2(1920, 1080));
		allCountLabel->active = true;
		allCountLabel->visible = true;
		myUI->AddUIComponent("AllCountLabel", allCountLabel);
		myUI->overlays[hudID]->AddComponent(allCountLabel);
		_hUDController->AddComponent(allCountLabel);

		ADUI::Image2D* villagerBG = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 3300, 50, 3800, 400 });
		villagerBG->BuildAnimation({ 710, 1190, 840, 1300 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("VillagerBG", villagerBG);
		myUI->overlays[hudID]->AddComponent(villagerBG);
		villagerBG->stretched = true;
		_hUDController->AddComponent(villagerBG);

		ADUI::Label2D* villagerCountLabel = new ADUI::Label2D();
		villagerCountLabel->SetFont(myUI->GetFont(2));
		villagerCountLabel->SetText("50", { 3550, 250 });// XMFLOAT2(1920, 1080));
		villagerCountLabel->active = true;
		villagerCountLabel->visible = true;
		myUI->AddUIComponent("VillagerCountLabel", villagerCountLabel);
		myUI->overlays[hudID]->AddComponent(villagerCountLabel);
		_hUDController->AddComponent(villagerCountLabel);

		ADUI::Label2D* villagerLabel = new ADUI::Label2D();
		villagerLabel->SetFont(myUI->GetFont(2));
		villagerLabel->SetText("Villagers Remaining", { 3550, 100 });// XMFLOAT2(1920, 1080));
		villagerLabel->active = true;
		villagerLabel->visible = true;
		myUI->AddUIComponent("villagerLabel", villagerLabel);
		myUI->overlays[hudID]->AddComponent(villagerLabel);

		ADUI::ComponentGrid* rightButtonList = new ADUI::ComponentGrid();
		rightButtonList->SetCorners({ 97, 331, 197, 431 });
		rightButtonList->positions = new RECT[4];
		rightButtonList->positions[0] = { 97, 331, 197, 431 };
		rightButtonList->spacing = { 2 };
		rightButtonList->columns = 1;

		rightButtonList->AddComponent(minionFacesRightAll);
		rightButtonList->AddComponent(minionFacesRight1);
		rightButtonList->AddComponent(minionFacesRight2);
		rightButtonList->AddComponent(minionFacesRight3);
		rightButtonList->AddComponent(minionFacesRight4);

		rightButtonList->Initialize();
		rightButtonList->active = false;
		rightButtonList->visible = true;
		rightButtonList->Focus();

		myUI->AddUIComponent("rightButtonsSelect", rightButtonList);
		myUI->overlays[hudID]->AddComponent(rightButtonList);
		_hUDController->AddComponent(rightButtonList);

		//Health Units
		_hUDController->healthTileListener.SetTarget(healthUnits, 0, 20000, true);
		ADEvents::ADEventSystem::Instance()->RegisterClient("ChangedHealth", &_hUDController->healthTileListener);

		//Essence
		_hUDController->essenceCountListener.SetTarget(essenceLabel);
		ADEvents::ADEventSystem::Instance()->RegisterClient("ChangeEssence", &_hUDController->essenceCountListener);

		//Simon Wheel
		_hUDController->stoneFormListener.isOnSwitch = true;
		_hUDController->stoneFormListener.switchFailures = true;
		_hUDController->stoneFormListener.keyIndex = 0;
		_hUDController->stoneFormListener.SetTarget(stoneSelect);
		ADEvents::ADEventSystem::Instance()->RegisterClient("SelectElementForm", &_hUDController->stoneFormListener);
		_hUDController->waterFormListener.isOnSwitch = true;
		_hUDController->waterFormListener.switchFailures = true;
		_hUDController->waterFormListener.keyIndex = 1;
		_hUDController->waterFormListener.SetTarget(waterSelect);
		ADEvents::ADEventSystem::Instance()->RegisterClient("SelectElementForm", &_hUDController->waterFormListener);
		_hUDController->fireFormListener.isOnSwitch = true;
		_hUDController->fireFormListener.switchFailures = true;
		_hUDController->fireFormListener.keyIndex = 2;
		_hUDController->fireFormListener.SetTarget(fireSelect);
		ADEvents::ADEventSystem::Instance()->RegisterClient("SelectElementForm", &_hUDController->fireFormListener);
		_hUDController->woodFormListener.isOnSwitch = true;
		_hUDController->woodFormListener.switchFailures = true;
		_hUDController->woodFormListener.keyIndex = 3;
		_hUDController->woodFormListener.SetTarget(woodSelect);
		ADEvents::ADEventSystem::Instance()->RegisterClient("SelectElementForm", &_hUDController->woodFormListener);

		//Minion Count Labels
		_hUDController->stoneMinionCountListener.SetTarget(stoneCountLabel);
		ADEvents::ADEventSystem::Instance()->RegisterClient("StoneMinionCountChanged", &_hUDController->stoneMinionCountListener);
		_hUDController->waterMinionCountListener.SetTarget(waterCountLabel);
		ADEvents::ADEventSystem::Instance()->RegisterClient("WaterMinionCountChanged", &_hUDController->waterMinionCountListener);
		_hUDController->fireMinionCountListener.SetTarget(fireCountLabel);
		ADEvents::ADEventSystem::Instance()->RegisterClient("FireMinionCountChanged", &_hUDController->fireMinionCountListener);
		_hUDController->woodMinionCountListener.SetTarget(woodCountLabel);
		ADEvents::ADEventSystem::Instance()->RegisterClient("WoodMinionCountChanged", &_hUDController->woodMinionCountListener);
		_hUDController->allMinionCountListener.SetTarget(allCountLabel);
		ADEvents::ADEventSystem::Instance()->RegisterClient("MinionCountChanged", &_hUDController->allMinionCountListener);

		_hUDController->villagerCountListener.SetTarget(villagerCountLabel);
		ADEvents::ADEventSystem::Instance()->RegisterClient("VillagerCountChanged", &_hUDController->villagerCountListener);

		//Group Selection
		_hUDController->controlGroupListener.SetTarget(rightButtonList);
		ADEvents::ADEventSystem::Instance()->RegisterClient("SelectElementalGroup", &_hUDController->controlGroupListener);

		//Token Visibility (There is an empty slot behind them)
		_hUDController->tokenListener1.isOnSwitch = true;
		_hUDController->tokenListener1.switchFailures = true;
		_hUDController->tokenListener1.keyIndex = 1;
		_hUDController->tokenListener1.isKeyThreshold = true;
		_hUDController->tokenListener1.SetTarget(specialIcon);
		ADEvents::ADEventSystem::Instance()->RegisterClient("TokensChanged", &_hUDController->tokenListener1);
		_hUDController->tokenListener2.isOnSwitch = true;
		_hUDController->tokenListener2.switchFailures = true;
		_hUDController->tokenListener2.keyIndex = 2;
		_hUDController->tokenListener2.isKeyThreshold = true;
		_hUDController->tokenListener2.SetTarget(specialIcon2);
		ADEvents::ADEventSystem::Instance()->RegisterClient("TokensChanged", &_hUDController->tokenListener2);
		_hUDController->tokenListener3.isOnSwitch = true;
		_hUDController->tokenListener3.switchFailures = true;
		_hUDController->tokenListener3.keyIndex = 3;
		_hUDController->tokenListener3.isKeyThreshold = true;
		_hUDController->tokenListener3.SetTarget(specialIcon3);
		ADEvents::ADEventSystem::Instance()->RegisterClient("TokensChanged", &_hUDController->tokenListener3);

		//Token Color by Form
		_hUDController->tokenColorListener1.SetTarget(specialIcon);
		ADEvents::ADEventSystem::Instance()->RegisterClient("SelectElementForm", &_hUDController->tokenColorListener1);
		_hUDController->tokenColorListener2.SetTarget(specialIcon2);
		ADEvents::ADEventSystem::Instance()->RegisterClient("SelectElementForm", &_hUDController->tokenColorListener2);
		_hUDController->tokenColorListener3.SetTarget(specialIcon3);
		ADEvents::ADEventSystem::Instance()->RegisterClient("SelectElementForm", &_hUDController->tokenColorListener3);

		//ADUI::Label2D* gemLabel = new ADUI::Label2D();
		//gemLabel->SetFont(myUI->GetFont(CreditsFont));
		//gemLabel->SetText("0", XMFLOAT2(350, 88));
		//myUI->AddUIComponent("GemCount", gemLabel);
		//myUI->overlays[hudID]->AddComponent(gemLabel);
		//ADUI::Label2D* healthLabel = new ADUI::Label2D();
		//healthLabel->SetFont(myUI->GetFont(CreditsFont));
		//healthLabel->SetText("3", XMFLOAT2(3600, 88));
		//myUI->AddUIComponent("Health", healthLabel);
		//myUI->overlays[hudID]->AddComponent(healthLabel);

		return hudID;
	}

	UINT GameUserInterface::SetupDebugMenu(ADUI::ADUI* myUI)
	{
		return 0;
	}

	UINT GameUserInterface::SetupPathingMap(ADUI::ADUI* myUI, DebugController* _debugController)
	{
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 5, 0 };

		//UI Log
		UINT pathingID = myUI->AddNewOverlay("PathingMap", false, true);
		ADUI::Image2D* consoleBox = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 100, 100, 3540, 1816 });
		consoleBox->BuildAnimation({ 0, 0, 2299, 960 }, 1, 1, emptyAnimation);
		consoleBox->active = true;
		consoleBox->visible = false;
		consoleBox->stretched = true;
		myUI->AddUIComponent("mapbg", consoleBox);
		myUI->overlays[pathingID]->AddComponent(consoleBox);


		//ADAI::PathingGrid* grid = &ADAI::ADPathfinding::Instance()->tileMap;
		//_debugController->planeNodes = &grid->nodeGrid;
		//for (int i = 0; i < grid->nodeGrid.size(); i++)
		//{

		//	ADUI::Image2D* tempImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 2000.f - (grid->nodeGrid[i]->position.x + 300.f) , 2100 - grid->nodeGrid[i]->position.z, 2000.f - (grid->nodeGrid[i]->position.x + 300.f),  2100 - grid->nodeGrid[i]->position.z });
		//	long heightValue = int((grid->nodeGrid)[i]->position.y * 100.f);
		//	if (heightValue < -4)
		//	{
		//		heightValue = -4;
		//	}
		//	if (heightValue > 4)
		//	{
		//		heightValue = 4;
		//	}

		//	tempImage->BuildAnimation({ 2299, 184 + (heightValue * 16), 2315, 200 + (heightValue * 16) }, 5, 1, emptyAnimation);
		//	tempImage->active = true;
		//	tempImage->visible = true;
		//	tempImage->Focus();
		//	myUI->AddUIComponent("genericImage", tempImage);
		//	myUI->overlays[pathingID]->AddComponent(tempImage);
		//	_debugController->node_image_map[(grid->nodeGrid)[i]] = tempImage;
		//}

		return pathingID;
	}


	UINT GameUserInterface::SetupMessageBox(ADUI::ADUI* myUI, HUDController* _hUDController)
	{
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };

		//UI Log
		UINT messageID = myUI->AddNewOverlay("MessageBox", false, true);

		ADUI::Image2D* messageBox = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 400, 1000, 3400, 2000 });
		messageBox->BuildAnimation({ 2315, 1080, 3825, 1970 }, 1, 1, emptyAnimation);
		messageBox->active = true;
		messageBox->visible = true;
		messageBox->stretched = true;
		myUI->AddUIComponent("MessageBoxBG", messageBox);
		myUI->overlays[messageID]->AddComponent(messageBox);

		ADUI::Label2D* messageLabel = new ADUI::Label2D();
		messageLabel->SetFont(myUI->GetFont(3));
		messageLabel->SetText("Basic Message Text", { 1900, 1100 });// XMFLOAT2(1920, 1080));
		messageLabel->active = true;
		messageLabel->visible = true;
		ADUI::Label2D* messageLabel2 = new ADUI::Label2D();
		messageLabel2->SetFont(myUI->GetFont(3));
		messageLabel2->SetText("Basic Message Text", { 1900, 1300 });// XMFLOAT2(1920, 1080));
		messageLabel2->active = true;
		messageLabel2->visible = true;
		ADUI::Label2D* messageLabel3 = new ADUI::Label2D();
		 messageLabel3->SetFont(myUI->GetFont(3));
		 messageLabel3->SetText("Basic Message Text", { 1900, 1500 });// XMFLOAT2(1920, 1080));
		 messageLabel3->active = true;
		 messageLabel3->visible = true;
		ADUI::Label2D* messageLabel4 = new ADUI::Label2D();
		messageLabel4->SetFont(myUI->GetFont(3));
		messageLabel4->SetText("Basic Message Text", { 1900, 1700 });// XMFLOAT2(1920, 1080));
		messageLabel4->active = true;
		messageLabel4->visible = true;
		myUI->AddUIComponent("messageLabel", messageLabel);
		myUI->overlays[messageID]->AddComponent(messageLabel);
		myUI->AddUIComponent("messageLabel2", messageLabel2);
		myUI->overlays[messageID]->AddComponent(messageLabel2);
		myUI->AddUIComponent("messageLabel3", messageLabel3);
		myUI->overlays[messageID]->AddComponent(messageLabel3);
		myUI->AddUIComponent("messageLabel4", messageLabel4);
		myUI->overlays[messageID]->AddComponent(messageLabel4);
		_hUDController->AddOverlay(myUI->overlays[messageID]);
		_hUDController->text.push_back(messageLabel);
		_hUDController->text.push_back(messageLabel2);
		_hUDController->text.push_back(messageLabel3);
		_hUDController->text.push_back(messageLabel4);

		return messageID;
	}


	UINT GameUserInterface::SetupLog(ADUI::ADUI* myUI)
	{
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };

		//UI Log
		UINT logid = myUI->AddNewOverlay("Log", false, true);
		ADUI::UILog* uiLog = myUI->GetLog();
		uiLog->InitializeLog(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1]);

		ADUI::Image2D* consoleBox = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1600, 1016, 3840, 2016 });
		consoleBox->BuildAnimation({ 0, 0, 2299, 960 }, 1, 1, emptyAnimation);
		consoleBox->active = true;
		consoleBox->visible = true;
		consoleBox->controlFocusAnimation = 1;
		consoleBox->stretched = true;
		ADUI::Label2D* consoleLabel = new ADUI::Label2D();
		consoleLabel->SetFont(myUI->GetFont(2));
		consoleLabel->SetText("", { 1920, 1180 });// XMFLOAT2(1920, 1080));
		consoleLabel->active = true;
		consoleLabel->visible = true;
		uiLog->Setup(consoleBox, consoleLabel);

		myUI->AddUIComponent("Log", uiLog);
		myUI->overlays[logid]->AddComponent(uiLog);
		ADUI::MessageReceiver::SetUI(myUI);

		return logid;
	}


	void GameUserInterface::SetupUI(ADUI::ADUI* myUI, AD_AUDIO::ADAudio* _audioSystem)
	{
		//ADAI::PathingGrid* grid = &ADAI::ADPathfinding::Instance()->tileMap;
		ADUI::Settings::screenWidth = myUI->viewport->Width;
		ADUI::Settings::screenHeight = myUI->viewport->Height;

		//Used to Scaling
		ADUI::Settings::planningWidth = 3840;
		ADUI::Settings::planningHeight = 2160;
		ADUI::Settings::resolutionWidth = 3840.f;
		ADUI::Settings::resolutionHeight = 2160.f;
		ADUI::Settings::RefreshResScaling();

		char** textures = new char* [2];
		textures[0] = "spyroatlas.dds";
		textures[1] = "GGAtlas2.dds";
		myUI->AddTextureArray(textures, 2);

		char** fonts = new char* [5];
		fonts[0] = "SpyroFont12.spritefont";
		fonts[1] = "SpyroFont16.spritefont";
		fonts[2] = "SpyroFont32.spritefont";
		fonts[3] = "SpyroFont64.spritefont";
		fonts[4] = "SpyroFont96.spritefont";
		myUI->AddFontArray(fonts, 5);


		UINT guideFont = 3;
		float buttonSpacing = 20;

		//// Message Controller
		GameplayUIPrimaryController* gameplayMessageController = new GameplayUIPrimaryController(myUI->GetUIState());
		myUI->AddUIController("MessageController", gameplayMessageController);
		gameplayMessageController->Enable();

		// Hud Controller
		HUDController* hudController = new HUDController(myUI->GetUIState());
		myUI->AddUIController("HudController", hudController);
		hudController->Disable();
		UINT hudid = SetupHUD(myUI, hudController);
		UINT humsgID = SetupMessageBox(myUI, hudController);

		//// TitleScreen Controller
		StartMenuUIController* titleScreenController = new StartMenuUIController(myUI->GetUIState());
		myUI->AddUIController("TitleScreenController", titleScreenController);
		titleScreenController->Enable();
		UINT titleID = SetupTitleScreen(myUI, titleScreenController);
		UINT endgameID = SetupEngGameScreen(myUI);
		UINT defeatedID = SetupDefeatGameScreen(myUI);
		titleScreenController->AddOverlay(myUI->overlays[endgameID]);
		titleScreenController->AddOverlay(myUI->overlays[defeatedID]);

		//// TutorialScreen Controller
		TutorialController* TutorialScreenController = new TutorialController(myUI->GetUIState());
		myUI->AddUIController("TutorialController", TutorialScreenController);
		TutorialScreenController->Disable();
		UINT tutorialID = SetupTutorialScreen(myUI, TutorialScreenController);

		//// PauseScreen Controller
		PauseMenuController* PauseScreenController = new PauseMenuController(myUI->GetUIState());
		myUI->AddUIController("PauseScreenController", PauseScreenController);
		PauseScreenController->Disable();
		UINT pauseID = SetupPauseScreen(myUI, PauseScreenController);


		// OptionsScreen Controller
		OptionsMenuUIController* optionScreenController = new OptionsMenuUIController(myUI->GetUIState());
		myUI->AddUIController("OptionScreenController", optionScreenController);
		optionScreenController->Disable();
		UINT optionsID = SetupOptionsScreen(myUI, optionScreenController);

		UINT logID = SetupLog(myUI);



		// DebugScreen Controller
		DebugController* debugController = new DebugController(myUI->GetUIState());
		myUI->AddUIController("DebugController", debugController);
		debugController->Enable();


		UINT pathingID = SetupPathingMap(myUI, debugController);


		titleScreenController->AddOverlay(myUI->overlays[hudid]);
		titleScreenController->AddController(hudController);
		titleScreenController->AddController(PauseScreenController);
		titleScreenController->AddOverlay(myUI->overlays[logID]);
		titleScreenController->AddController(TutorialScreenController);
		titleScreenController->AddOverlay(myUI->overlays[tutorialID]);

		TutorialScreenController->AddOverlay(myUI->overlays[hudid]);
		TutorialScreenController->AddController(hudController);
		TutorialScreenController->AddController(PauseScreenController);
		TutorialScreenController->AddOverlay(myUI->overlays[logID]);

		PauseScreenController->AddController(hudController);
		PauseScreenController->AddOverlay(myUI->overlays[titleID]);
		PauseScreenController->AddController(titleScreenController);
		PauseScreenController->AddOverlay(myUI->overlays[optionsID]);
		PauseScreenController->AddController(optionScreenController);

		gameplayMessageController->AddOverlay(myUI->overlays[logID]);
		gameplayMessageController->AddController(hudController);
		gameplayMessageController->AddOverlay(myUI->overlays[hudid]);
		gameplayMessageController->AddController(titleScreenController);
		gameplayMessageController->AddOverlay(myUI->overlays[titleID]);
		gameplayMessageController->AddController(PauseScreenController);
		gameplayMessageController->AddOverlay(myUI->overlays[pauseID]);
		gameplayMessageController->AddController(optionScreenController);

		gameplayMessageController->AddOverlay(myUI->overlays[endgameID]);
		gameplayMessageController->AddOverlay(myUI->overlays[defeatedID]);


		//gameplayMessageController

		gameplayMessageController->AddOverlay(myUI->overlays[pathingID]);

		optionScreenController->AddOverlay(myUI->overlays[pauseID]);
		optionScreenController->AddController(PauseScreenController);
		optionScreenController->SetAudio(_audioSystem);

		//////gameplayScreenController->AddComponent(sliderList);
		//gameplayScreenController->AddComponent(buttonList2);
		//gameplayScreenController->AddComponent(buttonList3);
		////gameplayScreenController->AddComponent(gemLabel);
		////gameplayScreenController->AddComponent(healthLabel);
		//gameplayScreenController->AddOverlay(myUI->overlays[titleid]);
		//gameplayScreenController->AddOverlay(myUI->overlays[pauseID]);
		//gameplayScreenController->AddOverlay(myUI->overlays[guideID]);
		//gameplayScreenController->AddOverlay(myUI->overlays[hudID]);
		//gameplayScreenController->AddOverlay(myUI->overlays[creditsID]);
		//gameplayScreenController->AddOverlay(myUI->overlays[successID]);
		//gameplayScreenController->AddOverlay(myUI->overlays[optionsID]);
		//myUI->AddUIController("GamePlayController", gameplayScreenController);


		//titleScreenController->AddController(gameplayScreenController);
		//gameplayScreenController->AddController(titleScreenController);
		//gameplayScreenController->AddController(endScreenController);
		//endScreenController->AddController(titleScreenController);




		//uiLog->WriteToLog("UI Log Created.");
		//uiLog->WriteToLog("1.");
		//uiLog->WriteToLog("2.");
		//uiLog->WriteToLog("3.");
		//uiLog->WriteToLog("4.");
		//uiLog->WriteToLog("5.");
		//uiLog->WriteToLog("6.");
		
	}



	bool DebugController::ProcessResponse(ADUI::UIMessage* _message, float& quick)
	{
		return false;
	}

	bool DebugController::ProcessInput(float delta_time, float& quick)
	{
		for (auto it = node_image_map.begin(), itEnd = node_image_map.end(); it != itEnd; ++it)
		{
			it->second->SetCurrentFrame(it->first->displayState);
		}
		return false;
	}


	bool TutorialController::ProcessInput(float delta_time, float& quick)
	{
		bool buttonPressed = false;
		if (currentPage == 0)
		{
			pagePicture[currentPage]->Enable();
		}
		if (Input::QueryButtonDown(GamepadButtons::Menu))
		{
			//Disable overlay as well
			overlays[0]->Disable();
			Disable();
			controllers[controllersNameToID["HudController"]]->Enable();
			controllers[controllersNameToID["PauseScreenController"]]->Enable();
			overlays[overlaysNameToID["HUD"]]->Enable();
			uiState = ADUI::UISTATE::GAMEPLAY;
			currentPage = 0;

			buttonPressed = true;

		}
		else if (Input::QueryButtonDown(GamepadButtons::B) || Input::QueryButtonDown(GamepadButtons::A))
		{
			buttonPressed = true;				
			if (pagePicture.size() > currentPage)
			{
				pagePicture[currentPage]->Disable();
			}
			++currentPage;

			if (currentPage == 6)
			{
				//Disable overlay as well
				overlays[0]->Disable();
				for (int i = 0; i < pagePicture.size(); ++i)
				{
					pagePicture[i]->Disable();
				}
				Disable();
				controllers[controllersNameToID["HudController"]]->Enable();
				controllers[controllersNameToID["PauseScreenController"]]->Enable();
				overlays[overlaysNameToID["HUD"]]->Enable();
				uiState = ADUI::UISTATE::GAMEPLAY;
				currentPage = 0;
				text[0]->SetText("");
				text[1]->SetText("");
				text[2]->SetText("");
				text[3]->SetText("");
			}
			else
			{
				if (pagePicture.size() > currentPage)
				{
					pagePicture[currentPage]->Enable();
				}
				text[0]->SetText(tutorialMessages[(currentPage * 4)]);
				text[1]->SetText(tutorialMessages[(currentPage * 4) + 1]);
				text[2]->SetText(tutorialMessages[(currentPage * 4) + 2]);
				text[3]->SetText(tutorialMessages[(currentPage * 4) + 3]);
				uiState = PAUSED;
			}
		}
		return buttonPressed;
	}

}



////testing area
//
//	//ADUI::UIMessage _moveTitleMsg;
//	//_moveTitleMsg.targetID = ADUI::UIMessageTargets::FirstSelector;
//	//_moveTitleMsg.commandID = ADUI::SelectorResponses::IncreaseByInt;
//	//_moveTitleMsg.ivalue = 1;
//	//ADUI::Reaction** moveTitle = new ADUI::Reaction * [1];

//	ADUI::UIMessage _testing;
//	_testing.targetID = ADUI::UIMessageTargets::Component;
//	_testing.commandID = ADUI::ComponentOptions::SetAnchor;
//	_testing.fvalue = {1920, 1080, 0, 0};

//	ADUI::UIMessage _testing2;
//	_testing2.targetID = ADUI::UIMessageTargets::Component;
//	_testing2.commandID = ADUI::ComponentOptions::SetAnchorByKeyPoint;
//	_testing2.ivalue = 0;

//	ADUI::UIMessage _testing3;
//	_testing3.targetID = ADUI::UIMessageTargets::Component;
//	_testing3.commandID = ADUI::ComponentOptions::SetAnchorByKeyPoint;
//	_testing3.ivalue = 1;

//	ADUI::UIMessage _testing4;
//	_testing4.targetID = ADUI::UIMessageTargets::Component;
//	_testing4.commandID = ADUI::ComponentOptions::SetAnchorByKeyPoint;
//	_testing4.ivalue = 2;

//	ADUI::UIMessage _testing5;
//	_testing5.targetID = ADUI::UIMessageTargets::Component;
//	_testing5.commandID = ADUI::ComponentOptions::SetAnchorByKeyPoint;
//	_testing5.ivalue = 3;

//	ADUI::UIMessage _testing6;
//	_testing6.targetID = ADUI::UIMessageTargets::Component;
//	_testing6.commandID = ADUI::ComponentOptions::SetAnchorByKeyPoint;
//	_testing6.ivalue = 4;

//	ADUI::UIMessage _testing7;
//	_testing7.targetID = ADUI::UIMessageTargets::Component;
//	_testing7.commandID = ADUI::ImageResponses::ChangeRotation;
//	_testing7.fvalue = { 5, 0, 0, 0 };

//	ADUI::UIMessage _testing8;
//	_testing8.targetID = ADUI::UIMessageTargets::Component;
//	_testing8.commandID = ADUI::ImageResponses::SetRotation;
//	_testing8.fvalue = { 0, 0, 0, 0 };

//	ADUI::Reaction** test = new ADUI::Reaction * [8];
//	test[0] = new ADUI::Reaction(GamepadButtons::A, _testing6);
//	test[1] = new ADUI::Reaction(GamepadButtons::B, _testing5);
//	test[2] = new ADUI::Reaction(GamepadButtons::X, _testing4);
//	test[3] = new ADUI::Reaction(GamepadButtons::Y, _testing3);
//	test[4] = new ADUI::Reaction(GamepadButtons::RightShoulder, _testing7);
//	test[5] = new ADUI::Reaction(GamepadButtons::LeftShoulder, _testing8);
//	test[6] = new ADUI::Reaction(GamepadButtons::LeftThumbstick, _testing7);
//	test[7] = new ADUI::Reaction(GamepadButtons::RightThumbstick, _testing8);

//	lvl->AddReactions(8, test);

//	//ADUI::UIComponent** comps = new ADUI::UIComponent*[2];
//	//comps[0] = titleImage;
//	//comps[1] = creditsImage;
//	//ADUI::UIComponentSelector* titleSwap = new ADUI::UIComponentSelector(2, comps);
//	//titleSwap->active = true;
//	//myUI->overlays[titleid]->AddComponent(titleSwap);

//	
//