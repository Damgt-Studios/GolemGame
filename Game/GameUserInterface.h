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

    class GameplayUIControllerSPYROGAME : public AD_UI::OverlayController
    {
        int timesPressed = 0;
    public:
        GameplayUIControllerSPYROGAME(UINT& _uiState, AD_UI::UISetup& _setup) : AD_UI::OverlayController(_uiState, _setup) {};
        virtual bool ProcessResponse(ADResource::AD_UI::UIMessage* _message);
        virtual bool ProcessInput(float delta_time);
    };

    class StartMenuUIControllerSPYROGAME : public AD_UI::OverlayController
    {
    public:
        StartMenuUIControllerSPYROGAME(UINT& _uiState, AD_UI::UISetup& _setup) : AD_UI::OverlayController(_uiState, _setup) {};
        virtual bool ProcessInput(float delta_time);
        virtual bool ProcessResponse(ADResource::AD_UI::UIMessage* _message);
    };

    class GameUserInterface
    {
    public:
        AD_UI::ADUI* SpyroGameUISetup();
    };

};

