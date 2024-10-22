#pragma once
#include "ADUserInterface.h"
#include "Types.h"
#include "GameObjectClasses.h"
#include "ADAudio.h"
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
        std::vector<ADAI::PathingNode*>* planeNodes = nullptr;
        std::unordered_map<ADAI::PathingNode*, ADUI::Image2D*> node_image_map;

        DebugController(UINT* _uiState) : uiState(*_uiState) {};
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class HUDController : public ADUI::OverlayController
    {
        ADResource::ADGameplay::Golem* player = nullptr;
        UINT& uiState;
        std::vector<std::string> tutorialMessages = {
            "Unless directed to follow or given a destination",
            "minions will attack their nearest target.",
            "This will cause them to follow the path of",
            "the buildings around them.",

            "Your minions will die if they are attacked while",
            "focusing on homes.  Destroy towers or guide",
            "them away from harm to keep them alive. Their",
            "destructive power will be helpful to your progress.",

            "When low on health consume your minions to recover.             ", 
            " ",
            "",
            " ", 

            "Villagers will hide in buildings. When they die    ",
            "blood will splatter into the air.                  ", 
            "Remember your goal is to kill all humans, not structures.                ",
            "  ",

            "Up and down on the control pad will let you change which groups",
            "will respond to your command.  Once minions have selected a    ", 
            "target it may be hard to call them back.  Hold the left bumper ",
            "to force them to return.                                       ",

            "Villagers and structuers will drop essence. Use the        ",
            "right control pad to summon new minions. The minions born   ", 
            "will match the form that raised them. Fire, Earth and Water ",
            "minions cost 50 Essence.  Wood minions cost 30 Essence.",

            "Stone Minions have increased Health.        ",
            "Water Minions have increased Movement Speed.                   ", 
            "Fire Minions have increased Attack Power.       ",
            " ",
        };

    public:
        std::vector<ADUI::Label2D*> text;
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
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class StartMenuUIController : public ADUI::OverlayController
    {
        UINT& uiState;
        float splashTimer = 0;

    public:
        StartMenuUIController(UINT* _uiState) : uiState(*_uiState) {};
        virtual bool ProcessResponse(ADUI::UIMessage* _message, float& quick) override;
        virtual bool ProcessInput(float delta_time, float& quick) override;
    };

    class TutorialController : public ADUI::OverlayController
    {
        UINT& uiState;
        std::vector<std::string> tutorialMessages = {
            " ",
            " ",
            " ",
            " ",

            "Welcome to The Great Golem.",
            "Your objective is to eliminate of the human population.",
            "The will hide in structures.  Use A, B and X to attack.",
            "Use Y for your special attack.",

            "You can change element by using the right and left shoulder  ",
            "buttons.  Each form has a different special ability.      ",
            "Use them sparingly as you only have three casts.        ",
            "Destroying town halls will recover an ability cast.        ",

            "Towers with ballista's in them will attack you on sight.         ",
            "You minions will also attack anything they are close to.                     ",
            "Call them to you using the left bumper.  Send them toward a                ",
            "target with your right bumper.     ",

            "Up and down on the control pad will let you change which groups",
            "will respond to your command.  Once minions have selected a    ",
            "target it may be hard to call them back.  Hold the left bumper ",
            "to force them to return.                                       ",

            "Fire form's fireball will destroy everything in a line.",
            "Water form's wave will destroy everything in front of you.  ",
            "Stone form's screem will grant you temporary invulnerability. ",
            "Wood form's special will revive your minions.",

            "Stone Minions have increased Armor.        ",
            "Water Minions have increased Movement Speed.                   ",
            "Fire Minions have increased Attack Power.       ",
            " ",
        };
    public:
        UINT currentPage = 0;
        std::vector<ADUI::Label2D*> text;
        std::vector<ADUI::Image2D*> pagePicture;
        TutorialController(UINT* _uiState) : uiState(*_uiState) {};
        virtual bool ProcessInput(float delta_time, float& quick);
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
        UINT audioIndex = 0;
        AD_AUDIO::ADAudio* audioSystem = nullptr;
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
        UINT SetupTutorialScreen(ADUI::ADUI* myUI, TutorialController* _tutorialController);
        UINT SetupEngGameScreen(ADUI::ADUI* myUI);
        UINT SetupDefeatGameScreen(ADUI::ADUI* myUI);
        UINT SetupHUD(ADUI::ADUI* myUI, HUDController* _hUDController);
        UINT SetupDebugMenu(ADUI::ADUI* myUI);
        UINT SetupPathingMap(ADUI::ADUI* myUI, DebugController* _debugController); // std::vector<ADAI::PathingNode*>* planeNodes, int columnCount, float mapWidth, float mapHeight);
        UINT SetupLog(ADUI::ADUI* myUI);
        UINT SetupMessageBox(ADUI::ADUI* myUI, HUDController* _hUDController);
    public:
        void SetupUI(ADUI::ADUI* myUI, AD_AUDIO::ADAudio* _audioSystem);
    };

}