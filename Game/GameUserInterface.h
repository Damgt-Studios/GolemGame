#pragma once
#include "ADUserInterface.h"
#include "Types.h"

namespace SpyroUISetup
{
    enum SpyroGameUIComponentTypes
    {
        Image,
        Label,
        Button,
        ButtonList,
    };


    inline XMFLOAT2 GetPosition(float _percentageX, float _percentageY, float _screenWidth, float _screeHeight)
    {
        return { (_percentageX * _screenWidth), (_percentageY * _screeHeight) };
    }

    class GameplayUIControllerSPYROGAME : public ADUI::OverlayController
    {
        int timesPressed = 0;
        int health = 3;
    public:
        GameplayUIControllerSPYROGAME(UINT& _uiState, ADUI::UISetup& _setup) : ADUI::OverlayController(_uiState, _setup) {};
        virtual bool ProcessResponse(ADResource::AD_UI::UIMessage* _message);
        virtual bool ProcessInput(float delta_time);
    };

    class StartMenuUIControllerSPYROGAME : public ADUI::OverlayController
    {
    public:
        StartMenuUIControllerSPYROGAME(UINT& _uiState, ADUI::UISetup& _setup) : ADUI::OverlayController(_uiState, _setup) {};
        virtual bool ProcessInput(float delta_time);
        virtual bool ProcessResponse(ADResource::AD_UI::UIMessage* _message);
    };

    class EndMenuUIControllerSPYROGAME : public ADUI::OverlayController
    {
    public:
        EndMenuUIControllerSPYROGAME(UINT& _uiState, ADUI::UISetup& _setup) : ADUI::OverlayController(_uiState, _setup) {};
        virtual bool ProcessInput(float delta_time);
    };

    class GameUserInterface
    {
    public:
        ADUI::ADUI* SpyroGameUISetup();
    };

};

