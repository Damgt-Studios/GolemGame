#pragma once
#include "ADUserInterface.h"
#include "Types.h"
#include "GameObjectClasses.h"
#include "ADAudio.h"
#include "Golem.h"

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
        ADResource::ADGameplay::Golem* player;
        ADUI::Image2D* golemIcon;
        ADUI::Image2D* healthIcon;
        ADUI::ComponentGrid* targetMinionGroup;
        std::vector<ADUI::Image2D*> tokenIcons;
        UINT& uiState;
        ADUI::Label2D* allCount;
        ADUI::Label2D* stoneCount;
        ADUI::Label2D* waterCount;
        ADUI::Label2D* fireCount;
        ADUI::Label2D* woodCount;
    public:
        HUDController(UINT* _uiState) : uiState(*_uiState) {};
        void SetPlayer(ADResource::ADGameplay::Golem* _player, ADUI::Image2D* _golemIcon, ADUI::Image2D* _healthIcon, ADUI::Image2D* _tk1, ADUI::Image2D* _tk2, ADUI::Image2D* _tk3, ADUI::ComponentGrid* _minionTargetingGroup, ADUI::Label2D * _allCount, ADUI::Label2D* _stoneCount, ADUI::Label2D* _waterCount, ADUI::Label2D* _fireCount, ADUI::Label2D* _woodCount);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class StartMenuUIController : public ADUI::OverlayController
    {
        UINT& uiState;
        AD_AUDIO::AudioSource buttonClick;
        AD_AUDIO::AudioSource buttonMove;
        AD_AUDIO::AudioSource menuBack;
    public:
        StartMenuUIController(UINT* _uiState) : uiState(*_uiState) {};
        void SetAudio(AD_AUDIO::ADAudio* _audioSystem);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class PauseMenuController : public ADUI::OverlayController
    {
        UINT& uiState;
        AD_AUDIO::AudioSource buttonClick;
        AD_AUDIO::AudioSource buttonMove;
        AD_AUDIO::AudioSource menuBack;
    public:
        PauseMenuController(UINT* _uiState) : uiState(*_uiState) {};
        void SetAudio(AD_AUDIO::ADAudio* _audioSystem);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick);
    };

    class OptionsMenuUIController : public ADUI::OverlayController
    {
        UINT& uiState;
        UINT audioIndex;
        AD_AUDIO::ADAudio* audioSystem;
        AD_AUDIO::AudioSource sliderClick;
        AD_AUDIO::AudioSource buttonClick;
        AD_AUDIO::AudioSource buttonMove;
        AD_AUDIO::AudioSource menuBack;
    public:
        OptionsMenuUIController(UINT* _uiState) : uiState(*_uiState) {};
        void SetAudio(AD_AUDIO::ADAudio* _audioSystem);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick);
    };

    class GameUserInterface
    {
    public:
        UINT SetupTitleScreen(ADUI::ADUI* myUI, StartMenuUIController* _titleScreenController);
        UINT SetupPauseScreen(ADUI::ADUI* myUI, PauseMenuController* _pauseMenuController);
        UINT SetupOptionsScreen(ADUI::ADUI* myUI, OptionsMenuUIController* _optionsMenuUIController);
        UINT SetupHUD(ADUI::ADUI* myUI, HUDController* _hUDController, ADResource::ADGameplay::Golem* _player);
        UINT SetupLog(ADUI::ADUI* myUI);
        void SetupUI(ADUI::ADUI* myUI, ADResource::ADGameplay::Golem* _player, AD_AUDIO::ADAudio* _audioSystem);
    };

}