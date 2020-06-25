#pragma once
#include "ADUserInterface.h"
#include "Types.h"
#include "GameObjectClasses.h"
#include "ADAudio.h"
#include "ADPathfinding.h"
#include "Golem.h"
#include <unordered_map>
#include "Listeners.h"

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

    class DebugController : public ADUI::OverlayController
    {
        UINT& uiState; 
    public:
        std::vector<ADAI::PathingNode*>* planeNodes;
        std::unordered_map<ADAI::PathingNode*, ADUI::Image2D*> node_image_map;

        DebugController(UINT* _uiState) : uiState(*_uiState) {};
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class HUDController : public ADUI::OverlayController
    {
        ADResource::ADGameplay::Golem* player;
        UINT& uiState;

    public:
        SetUITilingListener healthTileListener;
        SetUITextListener essenceCountListener;
        SetUIFocusListener stoneFormListener;
        SetUIFocusListener waterFormListener;
        SetUIFocusListener fireFormListener;
        SetUIFocusListener woodFormListener;
        SetUISetFrameListener tokenColorListener1;
        SetUISetFrameListener tokenColorListener2;
        SetUISetFrameListener tokenColorListener3;
        SetUIVisibilityListener tokenListener1;
        SetUIVisibilityListener tokenListener2;
        SetUIVisibilityListener tokenListener3;
        SetUITextListener stoneMinionCountListener;
        SetUITextListener waterMinionCountListener;
        SetUITextListener fireMinionCountListener;
        SetUITextListener woodMinionCountListener;
        SetUITextListener allMinionCountListener;
        SetUITextListener villagerCountListener;
        SetUISelectionListener controlGroupListener;

        HUDController(UINT* _uiState) : uiState(*_uiState) {};
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
    };

    class StartMenuUIController : public ADUI::OverlayController
    {
        UINT& uiState;
    public:
        StartMenuUIController(UINT* _uiState) : uiState(*_uiState) {};
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class PauseMenuController : public ADUI::OverlayController
    {
        UINT& uiState;
    public:
        PauseMenuController(UINT* _uiState) : uiState(*_uiState) {};
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick);
    };

    class OptionsMenuUIController : public ADUI::OverlayController
    {
        UINT& uiState;
        UINT audioIndex;
        AD_AUDIO::ADAudio* audioSystem;
    public:
        OptionsMenuUIController(UINT* _uiState) : uiState(*_uiState) {};
        void SetAudio(AD_AUDIO::ADAudio* _audioSystem);
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick);
    };

    class GameUserInterface
    {     
        UINT SetupTitleScreen(ADUI::ADUI* myUI, StartMenuUIController* _titleScreenController);
        UINT SetupPauseScreen(ADUI::ADUI* myUI, PauseMenuController* _pauseMenuController);
        UINT SetupOptionsScreen(ADUI::ADUI* myUI, OptionsMenuUIController* _optionsMenuUIController);
        UINT SetupEngGameScreen(ADUI::ADUI* myUI);
        UINT SetupHUD(ADUI::ADUI* myUI, HUDController* _hUDController);
        UINT SetupDebugMenu(ADUI::ADUI* myUI);
        UINT SetupPathingMap(ADUI::ADUI* myUI, DebugController* _debugController, std::vector<ADAI::PathingNode*>* planeNodes, int columnCount, float mapWidth, float mapHeight);
        UINT SetupLog(ADUI::ADUI* myUI);
    public:
        void SetupUI(ADUI::ADUI* myUI, AD_AUDIO::ADAudio* _audioSystem);
    };

}