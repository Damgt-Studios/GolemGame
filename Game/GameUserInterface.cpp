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
		if (_message)
		{
			if (_message->externalMsg)
			{
				switch (_message->commandID)
				{
				case 1:
				{
					
					//timesPressed += 1;
					//UINT gemCountID = componentsNameToID["GemCount"];
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output.clear();
					//GetComponentSpecfic<ADUI::Label2D*>(gemCountID)->output = std::to_string(timesPressed);
					//buttonPressed = true;
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
					//overlays[overlaysNameToID["SuccessScreen"]]->Enable();
					//Disable();

					//controllers[controllersNameToID["EndScreenController"]]->Enable();

					//componentTypeMap[componentsNameToID["TitleMenu"]]->Disable();
					buttonPressed = true;
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
			overlays[overlaysNameToID["Log"]]->visible = !overlays[overlaysNameToID["Log"]]->visible;
			buttonPressed = true;
		}
		return buttonPressed;
	}

	bool HUDController::ProcessResponse(ADUI::UIMessage* _message, float& quick)
	{
		return false;
	}

	bool HUDController::ProcessInput(float delta_time, float& quick)
	{
		bool buttonPressed = false;
		//if (Input::QueryButtonDown(GamepadButtons::Y))
		//{
		//	UINT hudOverlayID = overlaysNameToID["HUD"];
		//	UINT logOverlayID = overlaysNameToID["Log"];
		//	if (overlays[hudOverlayID]->visible)
		//	{
		//		if (!overlays[overlaysNameToID["PauseScreen"]]->active)
		//		{
		//			overlays[hudOverlayID]->visible = false;
		//		}
		//		overlays[logOverlayID]->visible = !overlays[logOverlayID]->visible;
		//	}
		//	else
		//	{
		//		overlays[hudOverlayID]->visible = true;
		//		overlays[logOverlayID]->visible = true;
		//	}
		//	buttonPressed = true;
		//}
		return buttonPressed;
	}

	void StartMenuUIController::SetAudio(AD_ADUIO::ADAudio* _audioSystem)
	{
		buttonClick.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		buttonClick.engine = _audioSystem;
		buttonClick.personalVolume = 1.00f;
		buttonClick.soundName = "files\\audio\\UI_SFX_GridConfirm.wav";
		buttonClick.LoadSound(false, false, false);


		buttonMove.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		buttonMove.engine = _audioSystem;
		buttonMove.personalVolume = 1.00f;
		buttonMove.soundName = "files\\audio\\UI_SFX_GridMovement.wav";
		buttonMove.LoadSound(false, false, false);

		menuBack.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		menuBack.engine = _audioSystem;
		menuBack.personalVolume = 0.30f;
		menuBack.soundName = "files\\audio\\UI_SFX_MenuReturn.wav";
		menuBack.LoadSound(false, false, false);
	}

	bool StartMenuUIController::ProcessResponse(ADUI::UIMessage* _message, float& quick)
	{
		bool buttonPressed = false;
		if (_message)
		{
			buttonPressed = true;
			if (_message->targetID == ADUI::UIMessageTargets::Controller )
			{
				if (_message->commandID == 1)
				{
					buttonClick.Play();
					switch (_message->componentIndex)
					{
					case 0:
					{
						overlays[overlaysNameToID["TitleScreen"]]->Disable();
						Disable();
						controllers[controllersNameToID["HudController"]]->Enable();

						controllers[controllersNameToID["PauseScreenController"]]->Enable();
						overlays[overlaysNameToID["HUD"]]->Enable();
						uiState = ADUI::UISTATE::GAMEPLAY;
						buttonPressed = true;
						break;
					}
					case 1: //saves
						buttonPressed = true;
						break;
					case 2: //credits
						componentTypeMap[componentsNameToID["TitleMenu"]]->Disable();
						overlays[overlaysNameToID["CreditsScreen"]]->Enable();
						overlays[overlaysNameToID["TitleScreen"]]->Disable();
						uiState = ADUI::UISTATE::MENUSTATE;
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
				else
				{
					buttonMove.Play();
				}
				
			}
		}
		return buttonPressed;
	}

	bool StartMenuUIController::ProcessInput(float delta_time, float& quick)
	{
		bool buttonPressed = false;
		if (overlays[overlaysNameToID["CreditsScreen"]]->active)
		{
			if (Input::QueryButtonDown(GamepadButtons::B))
			{
				menuBack.Play();
				overlays[overlaysNameToID["CreditsScreen"]]->Disable();
				overlays[overlaysNameToID["TitleScreen"]]->Enable();
				componentTypeMap[componentsNameToID["TitleMenu"]]->Enable();
				buttonPressed = true;
			}
		}
		else if (overlays[overlaysNameToID["SuccessScreen"]]->active)
		{
			if (Input::QueryButtonDown(GamepadButtons::B) || Input::QueryButtonDown(GamepadButtons::A))
			{
				buttonClick.Play();
				overlays[overlaysNameToID["CreditsScreen"]]->Disable();
				overlays[overlaysNameToID["SuccessScreen"]]->Disable();
				overlays[overlaysNameToID["HUD"]]->Disable();

				ADUI::UIMessage message;
				message.targetID = 1;
				message.commandID = 2;
				message.ivalue = 4;

				ADUI::MessageReceiver::SendMessage(&message);
				Disable();

				overlays[overlaysNameToID["TitleScreen"]]->Enable();
				componentTypeMap[componentsNameToID["TitleMenu"]]->Enable();
				controllers[controllersNameToID["TitleScreenController"]]->Enable();

				buttonPressed = true;
			}
		}
		return buttonPressed;
	}



	void PauseMenuController::SetAudio(AD_ADUIO::ADAudio* _audioSystem)
	{
		buttonClick.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		buttonClick.engine = _audioSystem;
		buttonClick.personalVolume = 1.00f;
		buttonClick.soundName = "files\\audio\\UI_SFX_GridConfirm.wav";
		buttonClick.LoadSound(false, false, false);


		buttonMove.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		buttonMove.engine = _audioSystem;
		buttonMove.personalVolume = 1.00f;
		buttonMove.soundName = "files\\audio\\UI_SFX_GridMovement.wav";
		buttonMove.LoadSound(false, false, false);

		menuBack.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		menuBack.engine = _audioSystem;
		menuBack.personalVolume = 0.30f;
		menuBack.soundName = "files\\audio\\UI_SFX_MenuReturn.wav";
		menuBack.LoadSound(false, false, false);
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
						buttonClick.Play();
						if (uiState == ADUI::UISTATE::MENUSTATE)
						{
							UINT pauseOverlayID = overlaysNameToID["PauseScreen"];
							if (overlays[pauseOverlayID]->active)
							{
								switch (_message->componentIndex)
								{
								case 0:
								{
									overlays[pauseOverlayID]->Disable();
									//overlays[overlaysNameToID["HUD"]]->visible = false;
									uiState = ADUI::UISTATE::GAMEPLAY;
									break;
								}
								case 1: //guidebook/credits
									overlays[pauseOverlayID]->Disable();
									componentTypeMap[componentsNameToID["PauseMenu"]]->Disable();

									overlays[guideOverlayID]->Enable();
									break;
								case 2: //options
									overlays[pauseOverlayID]->Disable();
									//componentTypeMap[componentsNameToID["PauseMenu"]]->Disable();
									Disable();

									overlays[overlaysNameToID["OptionsScreen"]]->Enable();
									//componentTypeMap[componentsNameToID["OptionsMenu"]]->Enable();
									controllers[controllersNameToID["OptionScreenController"]]->Enable();
									break;
								case 3: //Exit Level
									overlays[pauseOverlayID]->Disable();
									componentTypeMap[componentsNameToID["PauseMenu"]]->Disable();
									Disable();

									overlays[overlaysNameToID["TitleScreen"]]->Enable();
									componentTypeMap[componentsNameToID["TitleMenu"]]->Enable();
									controllers[controllersNameToID["TitleScreenController"]]->Enable();
									break;
								case 4:
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
						buttonMove.Play();
					}
					
				}
			}
		}
		else if (Input::QueryButtonDown(GamepadButtons::B))
		{
			if (overlays[guideOverlayID]->active)
			{
				overlays[guideOverlayID]->Disable();
				componentTypeMap[componentsNameToID["PauseMenu"]]->Enable();
				overlays[overlaysNameToID["PauseScreen"]]->Enable();
				buttonPressed = true;
				menuBack.Play();
			}
		}
		return buttonPressed;
	}

	bool PauseMenuController::ProcessInput(float delta_time, float& quick)
	{

		bool buttonPressed = false;
		if (Input::QueryButtonDown(GamepadButtons::Menu))
		{
			UINT pauseOverlayID = overlaysNameToID["PauseScreen"];
			UINT hudOverlayID = overlaysNameToID["HUD"];
			if (overlays[pauseOverlayID]->active)
			{
				overlays[pauseOverlayID]->Disable();
				overlays[hudOverlayID]->visible = false;
				uiState = ADUI::UISTATE::GAMEPLAY;
				componentTypeMap[componentsNameToID["PauseMenu"]]->Disable();
				menuBack.Play();
			}
			else
			{
				overlays[hudOverlayID]->visible = true;
				overlays[pauseOverlayID]->Enable();
				uiState = ADUI::UISTATE::MENUSTATE;
				componentTypeMap[componentsNameToID["PauseMenu"]]->Enable();
			}
			buttonPressed = true;
		}
		return buttonPressed;
	}


	void OptionsMenuUIController::SetAudio(AD_ADUIO::ADAudio* _audioSystem)
	{
		audioSystem = _audioSystem;

		sliderClick.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		sliderClick.engine = audioSystem;
		sliderClick.personalVolume = 1.00f;
		sliderClick.soundName = "files\\audio\\UI_SFX_Sliderclick.wav";
		sliderClick.LoadSound(false, false, false);

		buttonClick.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		buttonClick.engine = audioSystem;
		buttonClick.personalVolume = 1.00f;
		buttonClick.soundName = "files\\audio\\UI_SFX_GridConfirm.wav";
		buttonClick.LoadSound(false, false, false);


		buttonMove.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		buttonMove.engine = audioSystem;
		buttonMove.personalVolume = 1.00f;
		buttonMove.soundName = "files\\audio\\UI_SFX_GridMovement.wav";
		buttonMove.LoadSound(false, false, false);

		menuBack.audioSourceType = AD_ADUIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		menuBack.engine = audioSystem;
		menuBack.personalVolume = 0.30f;
		menuBack.soundName = "files\\audio\\UI_SFX_MenuReturn.wav";
		menuBack.LoadSound(false, false, false);
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
						if (componentTypeMap[componentsNameToID["AudioMenu"]]->active )
						{
							sliderClick.Play();
							switch (_message->componentIndex)
							{
							case AD_ADUIO::MUSIC:
								audioSystem->masterMusicVolume = _message->fvalue.x; 
								quick = 0.1f;
								audioSystem->RefreshMusicVolumes();
								break;
							case AD_ADUIO::SOUND_FX:
								audioSystem->masterSoundFXVolume = _message->fvalue.x;
								quick = 0.1f;
								break;
							case AD_ADUIO::UI_SOUND_FX:
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
								buttonClick.Play();
								break;
							case 1: //Video
								componentTypeMap[componentsNameToID["OptionsMenu"]]->Disable();
								componentTypeMap[componentsNameToID["ControlsMenu"]]->Enable();
								buttonClick.Play();
								break;
							case 2: //Controls
								componentTypeMap[componentsNameToID["OptionsMenu"]]->Disable();
								componentTypeMap[componentsNameToID["VideoMenu"]]->Enable();
								buttonClick.Play();
								break;
							default:
								break;
							}
						}
					}
					else
					{
						buttonMove.Play();
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
			menuBack.Play();
			if (componentTypeMap[componentsNameToID["AudioMenu"]]->active)// || componentTypeMap[componentsNameToID["ControlsMenu"]]->active || componentTypeMap[componentsNameToID["VideoMenu"]]->active)
			{
				componentTypeMap[componentsNameToID["AudioMenu"]]->Disable();
				//componentTypeMap[componentsNameToID["ControlsMenu"]]->Disable();
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
		UINT titleid = myUI->AddNewOverlay("TitleScreen", true, true);
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

		//SuccessScreen
		UINT successID = myUI->AddNewOverlay("SuccessScreen", false, false);
		myUI->overlays[successID]->AddComponent(creditsImage);


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
		buttonList->Generate(btn, position, spacing, 4, 1, ADUI::ComponentGridStyle::ExactSpacing, true, true);
		buttonList->visible = true;
		buttonList->components[1]->SetText("Load");
		buttonList->components[2]->SetText("Credits");
		buttonList->components[3]->SetText("Quit");
		myUI->AddUIComponent("TitleMenu", buttonList);
		myUI->overlays[titleid]->AddComponent(buttonList);

		_titleScreenController->AddComponent(buttonList);
		_titleScreenController->AddOverlay(myUI->overlays[titleid]);
		_titleScreenController->AddOverlay(myUI->overlays[creditsID]);
		_titleScreenController->AddOverlay(myUI->overlays[successID]);

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
		//ADUI::Image2D* pauseImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { myUI->viewport->TopLeftX,  myUI->viewport->TopLeftY,  (myUI->viewport->TopLeftX + myUI->viewport->Width),   (myUI->viewport->TopLeftY + myUI->viewport->Height) });
		//pauseImage->BuildAnimation({ 0, 0, 3840, 2160 }, 1, 1, emptyAnimation);
		//myUI->AddUIComponent("PauseBG", pauseImage);
		//myUI->overlays[pauseID]->AddComponent(pauseImage);

		//Guidebook Screen
		UINT guideID = myUI->AddNewOverlay("GuideScreen", false, false);
		ADUI::Image2D* guideImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { myUI->viewport->TopLeftX,  myUI->viewport->TopLeftY,  (myUI->viewport->TopLeftX + myUI->viewport->Width),   (myUI->viewport->TopLeftY + myUI->viewport->Height) });
		guideImage->BuildAnimation({ 0, 2160, 3840, 4320 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("GuideBG", guideImage);
		myUI->overlays[guideID]->AddComponent(guideImage);

		ADUI::Label2D* guideLabel = new ADUI::Label2D();
		guideLabel->SetFont(myUI->GetFont(4));
		guideLabel->SetText("The Great Golem Guide \n",
			XMFLOAT2(800, 400));
		guideLabel->Enable();
		guideLabel->SetTint(Colors::YellowGreen);
		myUI->AddUIComponent("GuideLabel", guideLabel);
		myUI->overlays[guideID]->AddComponent(guideLabel);

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
		buttonList2->Generate(btn2, position, spacing, 5, 1, ADUI::ComponentGridStyle::ExactSpacing, true, true);
		buttonList2->visible = true;
		buttonList2->components[0]->SetText("Resume");
		buttonList2->components[1]->SetText("GuideBook");
		buttonList2->components[2]->SetText("Options");
		buttonList2->components[3]->SetText("Main Menu");
		buttonList2->components[4]->SetText("Quit");
		myUI->AddUIComponent("PauseMenu", buttonList2);
		myUI->overlays[pauseID]->AddComponent(buttonList2);

		_pauseMenuController->AddComponent(buttonList2);
		_pauseMenuController->AddOverlay(myUI->overlays[pauseID]);
		_pauseMenuController->AddOverlay(myUI->overlays[guideID]);

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
		/*ADUI::Image2D* optionsImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0], { 1550, 400, 3070, 1560 });
		optionsImage->BuildAnimation({ 0, 4320, 2299, 5280 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("OptionsBox", optionsImage);
		myUI->overlays[optionsID]->AddComponent(optionsImage);*/

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
		buttonList3->Generate(btn3, position, spacing, 3, 1, ADUI::ComponentGridStyle::ExactSpacing, true, true);
		buttonList3->Enable();
		buttonList3->components[0]->SetText("Audio");
		buttonList3->components[1]->SetText("Controls");
		buttonList3->components[2]->SetText("Gameplay");
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
		XMFLOAT2 sliderPosition = {1800, 635};
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
		ADUI::SliderBar* sliderBar2= new ADUI::SliderBar(slideImage2, slider2, leftCapImage2, rightCapImage2, sliderLabel2);
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
		sliderList->positions = new RECT[2];
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

		//ADUI::Image2D* sliderImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//sliderImage->SetRects({ 100,100, 200, 200 }, { 3200, 5050, 3215, 5090 }, { 0, 0 });
		//XMFLOAT2 sliderPosition = {1220, 700};
		//XMFLOAT2 sliderMaxPosition = { 2120, 625 };
		//XMFLOAT2 sliderMinPosition = { 1620, 625 };
		//ADUI::Slider* slider = new ADUI::Slider(sliderImage, sliderPosition, sliderMaxPosition, sliderMinPosition);
		//ADUI::Image2D* leftCapImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//leftCapImage->SetRects({ 1519, 599, 1620, 700 }, { 3201, 4921, 3301, 5020 }, { 0, 0 });
		//ADUI::Image2D* leftCapImage2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//leftCapImage2->SetRects({ 1519, 599, 1620, 700 }, { 3301, 4921, 3402, 5020 }, { 0, 0 });
		//ADUI::Image2D* rightCapImage = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//rightCapImage->SetRects({ 2150, 599, 2251, 700 }, { 3400, 4921, 3501, 5020 }, { 0, 0 });
		//ADUI::Image2D* rightCapImage2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//rightCapImage2->SetRects({ 2150, 599, 2251, 700 }, { 3500, 4921, 3601, 5020 }, { 0, 0 });
		//ADUI::Button2D* leftCap = new ADUI::Button2D(leftCapImage, leftCapImage2, true, true);
		//ADUI::Button2D* rightCap = new ADUI::Button2D(rightCapImage, rightCapImage2, true, true);
		//ADUI::Label2D sliderLabel;
		//sliderLabel.SetFont(myUI->GetFont(3));
		//sliderLabel.SetText({ true, {1750, 500}, "Music" });
		//ADUI::SliderBar* sliderBar = new ADUI::SliderBar(slideImage, slider, leftCap, rightCap, sliderLabel);
		//sliderBar->Enable();


		//ADUI::Image2D* slideImage2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//slideImage2->SetRects({ 1616, 1035, 2150, 1066 }, { 3600, 4953, 3700, 4985 }, { 0, 0 });
		//slideImage2->SetStretched(true);
		//ADUI::Image2D* sliderImage2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//sliderImage2->SetRects({ 100,100, 200, 200 }, { 3200, 5050, 3215, 5090 }, { 0, 0 });
		//XMFLOAT2 sliderPosition2 = { 1220, 1100 };
		//XMFLOAT2 sliderMaxPosition2 = { 2120, 1025 };
		//XMFLOAT2 sliderMinPosition2 = { 1620, 1025 };
		//ADUI::Slider* slider2 = new ADUI::Slider(sliderImage2, sliderPosition2, sliderMaxPosition2, sliderMinPosition2);
		//ADUI::Image2D* leftCapImage3 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//leftCapImage3->SetRects({ 1519, 999, 1620, 1100 }, { 3201, 4921, 3301, 5020 }, { 0, 0 });
		//ADUI::Image2D* leftCapImage4 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//leftCapImage4->SetRects({ 1519, 999, 1620, 1100 }, { 3301, 4921, 3402, 5020 }, { 0, 0 });
		//ADUI::Image2D* rightCapImage3 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//rightCapImage3->SetRects({ 2150, 999, 2251, 1100 }, { 3400, 4921, 3501, 5020 }, { 0, 0 });
		//ADUI::Image2D* rightCapImage4 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[0]);
		//rightCapImage4->SetRects({ 2150, 999, 2251, 1100 }, { 3500, 4921, 3601, 5020 }, { 0, 0 });
		//ADUI::Button2D* leftCap2 = new ADUI::Button2D(leftCapImage3, leftCapImage4, true, true);
		//ADUI::Button2D* rightCap2 = new ADUI::Button2D(rightCapImage3, rightCapImage4, true, true);
		//ADUI::Label2D sliderLabel2;
		//sliderLabel2.SetFont(myUI->GetFont(3));
		//sliderLabel2.SetText({ true, {1450, 900}, "Sound Effects" });
		//ADUI::SliderBar* sliderBar2 = new ADUI::SliderBar(slideImage2, slider2, leftCap2, rightCap2, sliderLabel2);
		//sliderBar2->Enable();
		
		//ADUI::ButtonList* sliderList = new ADUI::ButtonList();
		//sliderList->x = 1819; 
		//sliderList->y = 499;
		//sliderList->spacing = 20.f;
		//sliderList->columns = 1;
		//sliderList->active = false;
		//sliderList->controlFocus = false;

		//sliderList->AddButton(sliderBar);
		//sliderList->AddButton(sliderBar2);

		//sliderList->Initialize();
		//sliderList->Disable();






		////ADUI::ButtonList* controlsList = new ADUI::ButtonList();
		////controlsList->x = 0;
		////controlsList->y = 0;
		////controlsList->spacing = 20.f;
		////controlsList->columns = 1;
		////controlsList->active = false;
		////controlsList->controlFocus = false;
		////controlsList->AddButton(sliderBar);
		////controlsList->Initialize();
		////controlsList->Disable();

		//myUI->AddUIComponent("ControlsMenu", slideImage);
		//myUI->overlays[optionsID]->AddComponent(slideImage);
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
		_optionsMenuUIController->AddOverlay(myUI->overlays[optionsID]);

		return optionsID;
	}

	UINT GameUserInterface::SetupHUD(ADUI::ADUI* myUI, HUDController* _hUDController)
	{
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };

		//HUD
		UINT hudID = myUI->AddNewOverlay("HUD", false, true);
		ADUI::Image2D* image1 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 30, 30, 250, 211 });
		image1->BuildAnimation({ 2299, 4320, 3840, 4920 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("GemIcon", image1);
		myUI->overlays[hudID]->AddComponent(image1);

		ADUI::Image2D* image2 = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 640, 175, 1640, 375 });
		image2->BuildAnimation({ 0, 2161, 1000, 2361 }, 1, 1, emptyAnimation);
		myUI->AddUIComponent("SpyroIcon", image2);
		myUI->overlays[hudID]->AddComponent(image2);


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

	UINT GameUserInterface::SetupLog(ADUI::ADUI* myUI)
	{
		ADUI::AnimationData* emptyAnimation = new ADUI::AnimationData[1];
		emptyAnimation[0] = { 0, 1, 1 };

		//UI Log
		UINT logid = myUI->AddNewOverlay("Log", false, true);
		ADUI::UILog* uiLog = myUI->GetLog();
		uiLog->InitializeLog(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1]);

		ADUI::Image2D* consoleBox = new ADUI::Image2D(myUI->spriteBatch.get(), myUI->uiResources.uiTextures[1], { 1600, 1016, 3840, 2016 });
		consoleBox->BuildAnimation({ 0, 0, 2299, 960 }, 1, 2, emptyAnimation);
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

	void GameUserInterface::SetupUI(ADUI::ADUI* myUI, ADResource::ADGameplay::Destructable* _player, AD_ADUIO::ADAudio* _audioSystem)
	{
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
		textures[1] = "spyroatlas2.dds";
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

		//// Hud Controller
		HUDController* hudController = new HUDController(myUI->GetUIState());
		myUI->AddUIController("HudController", hudController);
		hudController->Disable();
		UINT hudid = SetupHUD(myUI, hudController);

		//// TitleScreen Controller
		StartMenuUIController* titleScreenController = new StartMenuUIController(myUI->GetUIState());
		myUI->AddUIController("TitleScreenController", titleScreenController);
		titleScreenController->Enable();
		UINT titleID = SetupTitleScreen(myUI, titleScreenController);

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

		titleScreenController->AddOverlay(myUI->overlays[hudid]);
		titleScreenController->AddController(hudController);
		titleScreenController->AddController(PauseScreenController);
		titleScreenController->SetAudio(_audioSystem);

		PauseScreenController->AddController(hudController);
		PauseScreenController->AddOverlay(myUI->overlays[titleID]);
		PauseScreenController->AddController(titleScreenController);
		PauseScreenController->AddOverlay(myUI->overlays[optionsID]);
		PauseScreenController->AddController(optionScreenController);
		PauseScreenController->SetAudio(_audioSystem);

		gameplayMessageController->AddOverlay(myUI->overlays[logID]);
		gameplayMessageController->AddController(hudController);
		gameplayMessageController->AddOverlay(myUI->overlays[hudid]);
		gameplayMessageController->AddController(titleScreenController);
		gameplayMessageController->AddOverlay(myUI->overlays[titleID]);
		gameplayMessageController->AddController(PauseScreenController);
		gameplayMessageController->AddOverlay(myUI->overlays[pauseID]);
		gameplayMessageController->AddController(optionScreenController);

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



		//titleScreenController->AddOverlay(myUI->overlays[logid]);
		//gameplayScreenController->AddOverlay(myUI->overlays[logid]);
		//endScreenController->AddOverlay(myUI->overlays[logid]);

		//uiLog->WriteToLog("UI Log Created.");
		//uiLog->WriteToLog("1.");
		//uiLog->WriteToLog("2.");
		//uiLog->WriteToLog("3.");
		//uiLog->WriteToLog("4.");
		//uiLog->WriteToLog("5.");
		//uiLog->WriteToLog("6.");
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