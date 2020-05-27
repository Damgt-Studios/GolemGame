#pragma once
#include "ADUserInterface.h"
#include "Types.h"
#include "GameObjectClasses.h"
#include "ADAudio.h"

namespace GolemGameUISetup
{
    class GameplayUIPrimaryController : public ADUI::OverlayController
    {
        UINT& uiState;
    public:
        GameplayUIPrimaryController(UINT* _uiState) : uiState(*_uiState) {};
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class HUDController : public ADUI::OverlayController
    {
        ADResource::ADGameplay::Destructable* player;
        ADUI::Image2D* golemIcon;
        ADUI::Image2D* healthIcon;
        std::vector<ADUI::Image2D*> tokenIcons;
        UINT& uiState;
    public:
        HUDController(UINT* _uiState) : uiState(*_uiState) {};
        void SetPlayer(ADResource::ADGameplay::Destructable* _player, ADUI::Image2D* _golemIcon, ADUI::Image2D* _healthIcon, ADUI::Image2D* _tk1, ADUI::Image2D* _tk2, ADUI::Image2D* _tk3);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class StartMenuUIController : public ADUI::OverlayController
    {
        UINT& uiState;
        AD_ADUIO::AudioSource buttonClick;
        AD_ADUIO::AudioSource buttonMove;
        AD_ADUIO::AudioSource menuBack;
    public:
        StartMenuUIController(UINT* _uiState) : uiState(*_uiState) {};
        void SetAudio(AD_ADUIO::ADAudio* _audioSystem);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class PauseMenuController : public ADUI::OverlayController
    {
        UINT& uiState;
        AD_ADUIO::AudioSource buttonClick;
        AD_ADUIO::AudioSource buttonMove;
        AD_ADUIO::AudioSource menuBack;
    public:
        PauseMenuController(UINT* _uiState) : uiState(*_uiState) {};
        void SetAudio(AD_ADUIO::ADAudio* _audioSystem);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick);
    };

    class OptionsMenuUIController : public ADUI::OverlayController
    {
        UINT& uiState;
        UINT audioIndex;
        AD_ADUIO::ADAudio* audioSystem;
        AD_ADUIO::AudioSource sliderClick;
        AD_ADUIO::AudioSource buttonClick;
        AD_ADUIO::AudioSource buttonMove;
        AD_ADUIO::AudioSource menuBack;
    public:
        OptionsMenuUIController(UINT* _uiState) : uiState(*_uiState) {};
        void SetAudio(AD_ADUIO::ADAudio* _audioSystem);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick);
    };

    class GameUserInterface
    {
    public:
        UINT SetupTitleScreen(ADUI::ADUI* myUI, StartMenuUIController* _titleScreenController);
        UINT SetupPauseScreen(ADUI::ADUI* myUI, PauseMenuController* _pauseMenuController);
        UINT SetupOptionsScreen(ADUI::ADUI* myUI, OptionsMenuUIController* _optionsMenuUIController);
        UINT SetupHUD(ADUI::ADUI* myUI, HUDController* _hUDController, ADResource::ADGameplay::Destructable* _player);
        UINT SetupLog(ADUI::ADUI* myUI);
        void SetupUI(ADUI::ADUI* myUI, ADResource::ADGameplay::Destructable* _player, AD_ADUIO::ADAudio* _audioSystem);
    };

}