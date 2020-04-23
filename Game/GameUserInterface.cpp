#include "pchgame.h"
#include "GameUserInterface.h"


namespace SpyroUISetup
{
    bool GameplayUIControllerSPYROGAME::ProcessResponse(UINT responseID)
    {
        bool buttonPressed = false;
        if (uiState == AD_UI::UISTATE::MENUSTATE)
        {
            switch (responseID)
            {
            case 1:
                if (setup.overlays[1].active)
                {
                    setup.overlays[1].Disable();
                    setup.overlays[2].visible = false;
                    setup.textLabels[0].visible = false;
                    setup.textLabels[1].visible = false;
                    uiState = AD_UI::UISTATE::GAMEPLAY;
                }
                else
                {
                    setup.overlays[1].Enable();
                    setup.textLabels[0].visible = true;
                    setup.textLabels[1].visible = true;
                    uiState = AD_UI::UISTATE::MENUSTATE;
                }
                buttonPressed = true;
                break;

            case 2:
                uiState = AD_UI::UISTATE::QUIT;
                buttonPressed = true;
                break;
            default:
                break;
            }
        }

        return buttonPressed;
    }

    bool GameplayUIControllerSPYROGAME::ProcessInput(float delta_time)
    {
        bool buttonPressed = false;
        if (Input::QueryButtonDown(GamepadButtons::Menu))
        {
            if (setup.overlays[1].active)
            {
                setup.overlays[1].Disable();
                setup.overlays[2].visible = false;
                setup.textLabels[0].visible = false;
                setup.textLabels[1].visible = false;
                uiState = AD_UI::UISTATE::GAMEPLAY;
            }
            else
            {
                setup.uiControllersEnabled[1] = true;
                setup.textLabels[0].visible = true;
                setup.textLabels[1].visible = true;
                setup.overlays[2].visible = true;
                setup.overlays[1].Enable();
                uiState = AD_UI::UISTATE::MENUSTATE;
            }
            buttonPressed = true;
        }
        if (Input::QueryButtonDown(GamepadButtons::Y))
        {
            if (setup.overlays[2].visible)
            {
                if (!setup.overlays[1].active)
                {
                    setup.overlays[2].visible = false;
                    setup.textLabels[0].visible = false;
                    setup.textLabels[1].visible = false;
                }
                setup.overlays[3].visible = !setup.overlays[3].visible;
                //setup.logVisible = !setup.logVisible;
            }
            else
            {
                setup.overlays[2].visible = true;
                setup.textLabels[0].visible = true;
                setup.textLabels[1].visible = true;
                setup.overlays[3].visible = true;
                //setup.logVisible = true;
            }
            buttonPressed = true;
        }
        if (Input::QueryButtonDown(GamepadButtons::X))
        {
            timesPressed += 1;
            setup.textLabels[0].output.clear();
            setup.textLabels[0].output = std::to_string(timesPressed);
            buttonPressed = true;
        }
        return buttonPressed;
    }

    bool StartMenuUIControllerSPYROGAME::ProcessInput(float delta_time)
    {
        bool buttonPressed = false;
        if (Input::QueryButtonDown(GamepadButtons::Y))
        {
            //setup.logVisible = !setup.logVisible;
            setup.overlays[3].visible = !setup.overlays[3].visible;
            buttonPressed = true;
        }
        return buttonPressed;
    }

    bool StartMenuUIControllerSPYROGAME::ProcessResponse(UINT responseID)
    {
        bool buttonPressed = false;
        switch (responseID)
        {
        case 1:
            setup.overlays[0].Disable();
            setup.overlays[2].active = true;
            setup.overlays[2].visible = false;
            setup.uiControllersEnabled[1] = true;
            setup.uiControllersEnabled[0] = false;
            uiState = AD_UI::UISTATE::GAMEPLAY;
            buttonPressed = true;
            break;

        case 2:
            uiState = AD_UI::UISTATE::QUIT;
            buttonPressed = true;
            break;
        default:
            break;
        }
        return buttonPressed;
    }
}