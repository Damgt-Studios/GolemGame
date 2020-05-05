#pragma once
#include "pch.h"
#include "Types.h"
#include <vector>
#include <SpriteFont.h>

#include "Utils.h"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
#include <string>

using namespace ADResource::AD_UI;

namespace AD_UI
{
 /*   enum COLORS
    {
        RED_PURPLE,
        RED,
        RED_ORANGE,
        ORANGE,
        YELLOW_ORANGE,
        YELLOW,
        YEllOW_GREEN,
        GREEN,
        BLUE_GREEN,
        BLUE,
        BLUE_PURPLE,
        PURPLE,
    };*/

    enum UISTATE
    {
        GAMEPLAY = 0,
        MENUSTATE,
        QUIT
    };



    class Text2D
    {
        std::unique_ptr<SpriteBatch> spriteBatch;
        std::unique_ptr<SpriteFont> spriteFont;

    public:
        void Initialize(ID3D11Device1* _device, ID3D11DeviceContext1* _context)
        {
            spriteBatch = std::make_unique<SpriteBatch>(_context);
            spriteFont = std::make_unique<SpriteFont>(_device, ADUtils::GetTexturePath("SpyroFont.spritefont").c_str());
        };

        void Render(TextLabel label)
        {
            spriteBatch->Begin();
            spriteFont->DrawString(spriteBatch.get(), label.output.c_str(), label.position);
            spriteBatch->End();
        }
    };

    class Image2D : public UIComponent
    {
    public:
        int textureID;
        UINT frameCount;
        UINT currentFrame;
        float updateThreshold;
        float updateTimer;
        QuadData* quad;

        Image2D()
        {
            visible = true;
            active = false;
            textureID = 0;
            quadCount = 1;
            frameCount = 1;
            currentFrame = 0;
            updateThreshold = 100.f;
            updateTimer = 0.f;
        };

        Image2D(QuadData _quad, int _textureID = 0)
        { 
            visible = true;
            active = false;
            textureID = _textureID;
            quadCount = 1; 
            frameCount = 1;
            currentFrame = 0;
            updateThreshold = 100.f;
            updateTimer = 0.f;
            quad = new QuadData(_quad);
        };

        Image2D(QuadData _quad, AnimData2d _animation, int _textureID = 0)
        {
            visible = true;
            active = true;
            textureID = _textureID;
            quadCount = 1;
            updateTimer = 0.f;
            frameCount = _animation.frameCount;
            currentFrame = _animation.startFrame;
            quad = new QuadData[frameCount];
            quad[0] = _quad;
            float currentColumn = 0;
            float currentRow = 0;
            float UDelta = quad[0].maxU - quad[0].minU;
            float VDelta = quad[0].maxV - quad[0].minV;
            for (int i = 1; i < frameCount; ++i)
            {
                currentRow = i / _animation.columns;
                currentColumn = i - (currentRow * _animation.columns);
                quad[i].x = quad[0].x;
                quad[i].y = quad[0].y;
                quad[i].quadHeight = quad[0].quadHeight;
                quad[i].quadWidth = quad[0].quadWidth;
                quad[i].minU = quad[0].minU + UDelta * currentColumn;
                quad[i].maxU = quad[0].maxU + UDelta * currentColumn;
                quad[i].minV = quad[0].minV + VDelta * currentRow;
                quad[i].maxV = quad[0].maxV + VDelta * currentRow;
            }
            updateThreshold = (1.0f / _animation.fps);
        };

        ~Image2D() { CleanUp(); };
        virtual void Enable() { visible = true; };
        virtual void Disable() { visible = false; };


        virtual void Update(float delta_time) 
        {
            if (active && visible)
            {
                if (updateTimer < updateThreshold)
                {
                    updateTimer += delta_time;
                }
                else if(frameCount > 1)
                {
                    updateTimer = 0.f;
                    currentFrame++;
                    if (currentFrame >= frameCount)
                    {
                        currentFrame = 0;
                    }
                }
            }
        };

        virtual QuadData* GetQuad() override
        {
            return &quad[currentFrame];
        };

        virtual void CleanUp() 
        {
            for (int i = 0; i < frameCount; ++i)
            {
                delete &quad[i];
            }
            delete[] quad;
        };
    };

    class UILog : public UIComponent
    {
    public:
        Image2D blackBox;
        TextLabel consoleLabel;
        std::deque<std::string> messageQueue;



        void InitializeLog(float screenWidth, float screenHeight, float width = 200.f, float height = 400.f)
        {
            blackBox.quad = new QuadData{ 0.35f * screenWidth, -0.3f * screenHeight, 400, 250, 0.1046f, 0.2083f, 0.8645f, 0.9876f };
            blackBox.visible = true;
            consoleLabel = { true, XMFLOAT2(0.75f * screenWidth, 0.7f * screenHeight), {"0"} };
        }

        virtual int ProcessInput() 
        { 
            return 0; 
        };

        void WriteToLog(std::string message)
        {
            messageQueue.push_back(message);

            consoleLabel.output = "";
            for (int i = 0; i < 6; ++i)
            {
                int index = messageQueue.size() - 6 + i;
                if (index >= 0)
                {
                    consoleLabel.output.append(messageQueue[index]);
                    consoleLabel.output.append("\n");
                }

            }
        }

        virtual QuadData* GetQuad() 
        { 
            return blackBox.GetQuad(); 
        };

        TextLabel* GetText()
        {
            return &consoleLabel;
        }
    };

    class Button2D : public UIComponent
    {
    private:
        Image2D* image;
        Image2D* imageOnFocus;

    public:
        UINT actionValue;

        Button2D(Image2D* _image, Image2D* _imageOnFocus, bool _visible = true, bool _active = true, bool _controlFocus = false)
        { 
            image = _image;
            imageOnFocus = _imageOnFocus;
            visible = _visible;
            active = _active;
            controlFocus = _controlFocus;
            quadCount = 1;
        };

        ~Button2D()
        {
            delete image;
            delete imageOnFocus;
        };

        virtual int ProcessInput() 
        {
            if (Input::QueryButtonDown(GamepadButtons::A))
                return actionValue; 
            return 0;
        };

        virtual void Update(float delta_time)
        {
            if (controlFocus)
                imageOnFocus->Update(delta_time);
            else
                image->Update(delta_time);
        }

        virtual QuadData* GetQuad() 
        { 
            if (visible)
            {
                if (controlFocus)
                    return imageOnFocus->GetQuad();
                else
                    return image->GetQuad();
            }
            return nullptr;
        };
    };

    class ButtonList : public UIComponent
    {
    public:
        float x, y, spacing;
        UINT columns;
        UINT maxRows;
        UINT selectedButtonIndex;
        QuadData** myQuads;
        std::vector<Button2D*> buttons;

        ButtonList(float _x = 0.f, float _y = 0.f, float _spacing = 30.f, UINT _columns = 1, UINT _maxRows = INT_MAX, bool _active = false, bool _visible = false, bool _controlFocus = false)
        { 
            x = _x;
            y = _y;
            spacing = _spacing;
            columns = _columns;
            maxRows = _maxRows;
            active = _active;
            visible = _visible;
            controlFocus = _controlFocus;
            selectedButtonIndex = 0; 
            quadCount = 0; 
        };

        ~ButtonList() { CleanUp(); };

        void AddButton(Button2D* _button)
        {
            buttons.push_back(_button);
            quadCount++;
        }

        void RecalculatePositions()
        {
            int XSpacing = 0;
            int YSpacing = 0;

            for (int i = 0; i < quadCount; ++i)
            {
                XSpacing = (myQuads[0]->quadWidth + spacing) * (i % columns);
                YSpacing = (myQuads[0]->quadHeight + spacing) * (i / columns);
                myQuads[i]->x = x + XSpacing;
                myQuads[i]->y = y - YSpacing;
            }
        }

        virtual void Initialize() 
        {
            buttons[selectedButtonIndex]->controlFocus = true;  //This needs to move to Enable //Disable, etc..
            quadCount = buttons.size();
            myQuads = new QuadData*[quadCount]();

            for (int i = 0; i < buttons.size(); ++i)
            {
                myQuads[i] = buttons[i]->GetQuad();
            }
            RecalculatePositions();
        };

        virtual void Update(float delta_time)
        {
            for (int i = 0; i < buttons.size(); ++i)
            {
                buttons[i]->Update(delta_time);
            }
        }

        virtual int ProcessInput() 
        {
            if (Input::QueryButtonDown(GamepadButtons::DPadUp))
            {
                if (selectedButtonIndex != 0)
                {
                    buttons[selectedButtonIndex]->controlFocus = false;
                    selectedButtonIndex--;
                    buttons[selectedButtonIndex]->controlFocus = true;
                }
            }
            if (Input::QueryButtonDown(GamepadButtons::DPadDown))
            {
                if (selectedButtonIndex < buttons.size() - 1)
                {
                    buttons[selectedButtonIndex]->controlFocus = false;
                    selectedButtonIndex++;
                    buttons[selectedButtonIndex]->controlFocus = true;
                }
            }

            return buttons[selectedButtonIndex]->ProcessInput();
        }

        virtual QuadData** GetQuads() 
        {
            for (int i = 0; i < buttons.size(); ++i)
            {
                myQuads[i] = buttons[i]->GetQuad();
            }
            RecalculatePositions();
            return myQuads;
        }

        virtual void CleanUp()
        {
            for (int i = 0; i < buttons.size(); ++i)
            {
                if(buttons[i])
                    delete buttons[i];
            }
            //The quads we are pointing to are stored in the images and they will delete their own quads.
            delete[] myQuads;
        }
    };

    struct UISetup
    {
        std::vector<TextLabel> textLabels;
        std::vector<Overlay2D> overlays;
        std::vector<UIComponent*> uiComponents;
        std::vector<bool> uiControllersEnabled;
    };


    class OverlayController
    {
    protected:
        UISetup& setup;
        UINT& uiState;
        std::vector<UINT> componentIDs;

    public:
        OverlayController(UINT& _uiState, UISetup& _setup) : uiState(_uiState), setup(_setup) {};
        ~OverlayController() {};
        virtual bool ProcessInput(float delta_time) { return false; };
        virtual bool ProcessResponse(UINT responseID) { return false; };
        virtual void AddComponent(UINT _compID) { componentIDs.push_back(_compID); }
        UINT GetComponentCount() { return componentIDs.size(); };
        UINT* GetComponentIDS() { return componentIDs.data(); };
    };


    class GameplayUIControllerSPYROGAME : public OverlayController
    {
    public:
        GameplayUIControllerSPYROGAME(UINT& _uiState, UISetup& _setup) : OverlayController(_uiState, _setup) {};

        virtual bool ProcessResponse(UINT responseID)
        {
            bool buttonPressed = false;
            if (uiState == UISTATE::MENUSTATE)
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
                        uiState = UISTATE::GAMEPLAY;
                    }
                    else
                    {
                        setup.overlays[1].Enable();
                        setup.textLabels[0].visible = true;
                        setup.textLabels[1].visible = true;
                        uiState = UISTATE::MENUSTATE;
                    }
                    buttonPressed = true;
                    break;

                case 2:
                    uiState = UISTATE::QUIT;
                    buttonPressed = true;
                    break;
                default:
                    break;
                }
            }
          
            return buttonPressed;
        };

        virtual bool ProcessInput(float delta_time) 
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
                    uiState = UISTATE::GAMEPLAY;
                }
                else
                {
                    setup.uiControllersEnabled[1] = true;
                    setup.textLabels[0].visible = true;
                    setup.textLabels[1].visible = true;
                    setup.overlays[2].visible = true;
                    setup.overlays[1].Enable();
                    uiState = UISTATE::MENUSTATE;
                }
                buttonPressed = true;
            }
            if (Input::QueryButtonDown(GamepadButtons::DPadUp))
            {
                if (setup.overlays[2].visible )
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
        return buttonPressed;
        };
    };

    class StartMenuUIControllerSPYROGAME : public OverlayController
    {
    public:
        StartMenuUIControllerSPYROGAME(UINT& _uiState, UISetup& _setup) : OverlayController(_uiState, _setup) {};

        virtual bool ProcessInput(float delta_time)
        {
            bool buttonPressed = false;
            if (Input::QueryButtonDown(GamepadButtons::Y))
            {
                //setup.logVisible = !setup.logVisible;
                setup.overlays[3].visible = !setup.overlays[3].visible;
                buttonPressed = true;
            }
            return buttonPressed;
        };

        virtual bool ProcessResponse(UINT responseID)
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
                uiState = UISTATE::GAMEPLAY;
                buttonPressed = true;
                break;

            case 2:
                uiState = UISTATE::QUIT;
                buttonPressed = true;
                break;
            default:
                break;
            }
            return buttonPressed;
        };
    };

    class ADUI
    {
    private:
        UILog uiLog;
        Text2D text;
        UISetup* setup;
        std::vector<OverlayController*> uiControllers;
        UIRendererResources uiResources;
        float responseTime;
        float inputTimer;
        UINT uiState = 1;

        void CreateQuad(QuadData& _quad, std::vector<ADResource::AD_UI::UIVertex>& vertices, std::vector<int>& indices)
        {
            UINT startIndex = vertices.size();
            vertices.push_back({ {1,1,1,1},{_quad.x - _quad.quadWidth /2.f, _quad.y - _quad.quadHeight / 2.f, 1},  {_quad.minU, _quad.maxV} });
            vertices.push_back({ {1,1,1,1},{_quad.x - _quad.quadWidth /2.f, _quad.y + _quad.quadHeight / 2.f, 1},  {_quad.minU, _quad.minV} });
            vertices.push_back({ {1,1,1,1},{_quad.x + _quad.quadWidth / 2.f, _quad.y + _quad.quadHeight / 2.f, 1},  {_quad.maxU, _quad.minV} });
            vertices.push_back({ {1,1,1,1},{_quad.x + _quad.quadWidth / 2.f, _quad.y - _quad.quadHeight / 2.f, 1},  {_quad.maxU, _quad.maxV} });

            indices.push_back(startIndex);
            indices.push_back(startIndex + 1);
            indices.push_back(startIndex + 2);
            indices.push_back(startIndex);
            indices.push_back(startIndex + 2);
            indices.push_back(startIndex + 3);
        }

    public:
        bool visible;

        int AddNewOverlay(bool _visible = false, bool _active = false, bool _dynamic = true)
        {
            unsigned int index = setup->overlays.size();
            Overlay2D temp(index, _visible, _active, _dynamic);
            setup->overlays.push_back(temp);
            return index;
        }

        AD_ULONG AddUIComponent(UIComponent* component)
        {
            unsigned int index = setup->uiComponents.size();
            setup->uiComponents.push_back(component);

            return index;
        }

        UINT GetUIState()
        {
            return uiState;
        }


        XMFLOAT2 GetPosition(float _percentageX, float _percentageY, float _screenWidth, float _screeHeight)
        {
            return { (_percentageX * _screenWidth), (_percentageY * _screeHeight) };
        }

        UILog GetLogComponent()
        {
            return uiLog;
        }

        void GameSideCode(float width, float height)
        {
            setup = new UISetup();

            uiLog.InitializeLog(width, height);
            uiLog.WriteToLog("UI Log Created.");
            uiLog.WriteToLog("1.");
            uiLog.WriteToLog("2.");
            uiLog.WriteToLog("3.");
            uiLog.WriteToLog("4.");
            uiLog.WriteToLog("5.");
            uiLog.WriteToLog("6.");


            //TitleScreen
            UINT id = AddNewOverlay(true, true, true);
            Image2D* titleImage = new Image2D({ 0, 0, width, height, 0, 1, 0, 0.333f });
            UINT compId = AddUIComponent(titleImage);
            setup->overlays[id].AddComponent(compId);

            //PauseScreen
            id = AddNewOverlay();
            Image2D* pauseImage = new Image2D({ 0, 0, width, height, 0, 1, 0.333f, 0.666f });
            compId = AddUIComponent(pauseImage);
            setup->overlays[id].AddComponent(compId);

            //(0.05f * width), (0.1f * height)
            //HUD
            id = AddNewOverlay();
            Image2D* image1 = new Image2D({ -(0.45f * width), (0.4f * height), 80, 80, 0, 0.06f, 0.667f, 0.698f });
            Image2D* image2 = new Image2D({ (0.40f * width), (0.4f * height), 80, 80, 0.06667f, 0.128f, 0.667f, 0.7065f });
            compId = AddUIComponent(image1);
            setup->overlays[id].AddComponent(compId);
            compId = AddUIComponent(image2);
            setup->overlays[id].AddComponent(compId);

            TextLabel gemLabel = { false, GetPosition(0.1f, 0.1f, width, height), {"0"} };
            TextLabel healthLabel = { false, GetPosition(0.95f, 0.1f, width, height), {"0"} };
            setup->textLabels.push_back(gemLabel);
            setup->textLabels.push_back(healthLabel);

            //Button List for Start and Pause Menus
            Button2D* button = new Button2D(
                new Image2D({ 0, -100, 240, 60, 0.2994f, 0.4911f, 0.6666f, 0.6907f }, { 2, 0, 2, 6 }),
                new Image2D({ 0, -100, 240, 60, 0.6848f, 0.8875f, 0.6666f, 0.6907f })
                );
            button->actionValue = 1;

            Button2D* button2 = new Button2D(
                new Image2D({ 0, -170, 240, 60, 0.2994f, 0.4911f, 0.7614, 0.7858f }),
                new Image2D({ 0, -170, 240, 60, 0.4921f, 0.6859f, 0.7614, 0.7858f })
                );
            button2->actionValue = 2;

            ButtonList* buttonList = new ButtonList();
            buttonList->y = -160;
            buttonList->spacing = 20;
            buttonList->active = true;
            buttonList->controlFocus = true;
            buttonList->buttons.push_back(button);
            buttonList->buttons.push_back(button2);
            buttonList->Initialize();
            compId = AddUIComponent(buttonList);

            setup->overlays[0].AddComponent(compId);

            //TitleScreen Controller
            StartMenuUIControllerSPYROGAME* titleScreenController = new StartMenuUIControllerSPYROGAME(uiState, *setup);
            setup->uiControllersEnabled.push_back(true);
            titleScreenController->AddComponent(compId);
            uiControllers.push_back(titleScreenController);


            buttonList = new ButtonList();
            buttonList->y = +80;
            buttonList->spacing = 20;
            buttonList->active = true;
            buttonList->controlFocus = true;
            buttonList->buttons.push_back(button);
            buttonList->buttons.push_back(button2);
            buttonList->Initialize();
            compId = AddUIComponent(buttonList);

            setup->overlays[1].AddComponent(compId);

            //HUD Controller
            GameplayUIControllerSPYROGAME* gameplayUIController = new GameplayUIControllerSPYROGAME(uiState, *setup);
            setup->uiControllersEnabled.push_back(false);
            gameplayUIController->AddComponent(compId);
            uiControllers.push_back(gameplayUIController);

            //UI Log
            id = AddNewOverlay(true, true, true);
            compId = AddUIComponent(&uiLog);
            setup->overlays[id].AddComponent(compId);

        }

        void SetSetup(UISetup* _setup)
        {
            setup = _setup;
        }

        void RefreshOverlay(ComPtr<ID3D11DeviceContext1> _context)
        {
            for (int over = 0; over < setup->overlays.size(); ++over)
            {
                if (setup->overlays[over].IsDynamic() && setup->overlays[over].visible)
                {
                    setup->overlays[over].vertices.clear();
                    setup->overlays[over].indices.clear();
                    for (int i = 0; i < setup->overlays[over].componentIDs.size(); ++i)
                    {
                        Overlay2D ovl = setup->overlays[over];
                        UINT cid = ovl.componentIDs[i];
                        if (setup->uiComponents[cid]->GetQuadCount() > 1)
                        {
                            QuadData** qds = setup->uiComponents[cid]->GetQuads();
                            for (int addedQuads = 0; addedQuads < setup->uiComponents[cid]->GetQuadCount(); ++addedQuads)
                            {
                                QuadData* qd = qds[addedQuads];
                                CreateQuad(*qd, setup->overlays[over].vertices, setup->overlays[over].indices);
                            }
                        }
                        else
                        {
                            QuadData* qds = setup->uiComponents[cid]->GetQuad();
                            if (qds)
                                CreateQuad(*qds, setup->overlays[over].vertices, setup->overlays[over].indices);
                        }
                    }

                    //if (uiLog.visible)
                    //{
                    //    QuadData* qd = uiLog.blackBox.GetQuad();
                    //    if (qd)
                    //        CreateQuad(*qd, setup->overlays[over].vertices, setup->overlays[over].indices);
                    //}

                    //Update the Vertex buffers
                    D3D11_MAPPED_SUBRESOURCE mappedResource;
                    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
                    _context->Map(setup->overlays[over].vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                    memcpy(mappedResource.pData, setup->overlays[over].vertices.data(), sizeof(UIVertex) * setup->overlays[over].vertices.size());
                    _context->Unmap(setup->overlays[over].vertexBuffer.Get(), 0);

                    //Update the Index buffers
                    D3D11_MAPPED_SUBRESOURCE mappedResource2;
                    ZeroMemory(&mappedResource2, sizeof(D3D11_MAPPED_SUBRESOURCE));
                    _context->Map(setup->overlays[over].indexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);
                    memcpy(mappedResource2.pData, setup->overlays[over].indices.data(), sizeof(int) * setup->overlays[over].indices.size());
                    _context->Unmap(setup->overlays[over].indexBuffer.Get(), 0);
                }
            }
        }

        void Initialize(ComPtr<ID3D11Device1> device, float width, float height)
        {
            GameSideCode(width, height);

            text.Initialize(device.Get(), ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()->context.Get());

            uiState = true;

            responseTime = 0.2f;
            UIHeader header = { "spyroatlas.dds" };
            ////
            ////	std::ifstream file;
            ////
            ////	std::string path = std::string(ADUtils::READ_PATH.begin(), ADUtils::READ_PATH.end()).append("files\\UI\\");
            ////	file.open(path, std::ios::binary | std::ios::in);
            ////	assert(file.is_open());
            ////
            ////	// Read in the header
            ////	file.read((char*)&header, sizeof(UIHeader));
            ////	std::string s = header.t_albedo;
            ////
            ////	file.close();

                // Load textures

            ADUtils::LoadUITextures(header, uiResources.uiTextures, device);
            visible = true;

            for (int over = 0; over < setup->overlays.size(); ++over)
            {
                for (int i = 0; i < setup->overlays[over].componentIDs.size(); ++i)
                {
                    Overlay2D ovl = setup->overlays[over];
                    UINT cid = ovl.componentIDs[i];
                    if (setup->uiComponents[cid]->GetQuadCount() > 1)
                    {
                        QuadData** qds = setup->uiComponents[cid]->GetQuads();
                        for (int addedQuads = 0; addedQuads < setup->uiComponents[cid]->GetQuadCount(); ++addedQuads)
                        {
                            QuadData* qd = qds[addedQuads];
                            CreateQuad(*qd, setup->overlays[over].vertices, setup->overlays[over].indices);
                        }
                    }
                    else
                    {
                        QuadData* qds = setup->uiComponents[cid]->GetQuad();
                        if (qds)
                            CreateQuad(*qds, setup->overlays[over].vertices, setup->overlays[over].indices);
                    }
                    //CreateQuad( *components[overlay[currentOverlay].componentIDs[i]]->GetQuads() );
                }

                //QuadData* qd = uiLog.blackBox.GetQuad();
                //if (qd)
                //    CreateQuad(*qd, setup->overlays[over].vertices, setup->overlays[over].indices);

                //Create buffers
                // Setup Vertex Buffer
                D3D11_BUFFER_DESC bufferDesc;
                D3D11_SUBRESOURCE_DATA vdata;
                ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
                ZeroMemory(&vdata, sizeof(D3D11_SUBRESOURCE_DATA));
                bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                bufferDesc.ByteWidth = sizeof(UIVertex) * setup->overlays[over].vertices.size();
                bufferDesc.MiscFlags = 0;
                bufferDesc.StructureByteStride = 0;
                vdata.pSysMem = setup->overlays[over].vertices.data();


                if (setup->overlays[over].IsDynamic())
                {
                    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
                    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                }
                else
                {
                    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
                    bufferDesc.CPUAccessFlags = 0;
                }


                HRESULT result = device->CreateBuffer(&bufferDesc, &vdata, &setup->overlays[over].vertexBuffer);
                assert(!FAILED(result));

                //Index buffer
                bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                bufferDesc.ByteWidth = sizeof(int) * setup->overlays[over].indices.size();

                D3D11_SUBRESOURCE_DATA idata;
                ZeroMemory(&idata, sizeof(D3D11_SUBRESOURCE_DATA));
                idata.pSysMem = setup->overlays[over].indices.data();
                result = device->CreateBuffer(&bufferDesc, &idata, &setup->overlays[over].indexBuffer);
                assert(!FAILED(result));

            }
            
            ////	//// Load shaders
            ComPtr<ID3D10Blob> vertexblob;
            ComPtr<ID3D10Blob> pixelblob;

            Platform::String^ appInstallFolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
            std::string READ_PATH = std::string(appInstallFolder->Begin(), appInstallFolder->End()).append("\\");

            std::string v = std::string(READ_PATH.begin(), READ_PATH.end()).append("files\\shaders\\ui_vs.hlsl");
            std::string p = std::string(READ_PATH.begin(), READ_PATH.end()).append("files\\shaders\\ui_ps.hlsl");

            std::wstring vshadername(v.begin(), v.end());
            std::wstring pshadername(p.begin(), p.end());

            HRESULT result = D3DCompileFromFile(vshadername.c_str(), NULL, NULL, "main", "vs_4_0", D3DCOMPILE_DEBUG, 0, &vertexblob, nullptr);
            assert(!FAILED(result));
            result = D3DCompileFromFile(pshadername.c_str(), NULL, NULL, "main", "ps_4_0", D3DCOMPILE_DEBUG, 0, &pixelblob, nullptr);
            assert(!FAILED(result));

            result = device->CreateVertexShader(vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), nullptr, &uiResources.vertexShader);
            assert(!FAILED(result));
            result = device->CreatePixelShader(pixelblob->GetBufferPointer(), pixelblob->GetBufferSize(), nullptr, &uiResources.pixelShader);
            assert(!FAILED(result));

            // Make input layout for vertex buffer
            D3D11_INPUT_ELEMENT_DESC tempInputElementDesc[] =
            {
                { "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
                { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16 , D3D11_INPUT_PER_VERTEX_DATA, 0},
                { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };

            result = device->CreateInputLayout(tempInputElementDesc, ARRAYSIZE(tempInputElementDesc), vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), &uiResources.vertexBufferLayout);
            assert(!FAILED(result));

            D3D11_DEPTH_STENCIL_DESC dsDesc;
            ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
            dsDesc.DepthEnable = false;

            // Create depth stencil state
            device.Get()->CreateDepthStencilState(&dsDesc, &uiResources.depthStencilState);

            // Create constant buffer
            D3D11_BUFFER_DESC bDesc;
            D3D11_SUBRESOURCE_DATA subdata;
            ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

            bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bDesc.ByteWidth = sizeof(XMFLOAT4X4);
            bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bDesc.MiscFlags = 0;
            bDesc.StructureByteStride = 0;
            bDesc.Usage = D3D11_USAGE_DYNAMIC;

            result = device->CreateBuffer(&bDesc, nullptr, &uiResources.constantBuffer);
            assert(!FAILED(result));

            // Create an orthographic projection matrix for 2D rendering.
            Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
            XMStoreFloat4x4(&uiResources.ortoprojMatrix, XMMatrixOrthographicLH(Window->Bounds.Width, Window->Bounds.Height, 0.1f, 1000.f));
        }

        void Update(float delta_time)
        {
            if (inputTimer < responseTime)
            {
                inputTimer += delta_time;
            }
            else
            {
                for (int control = 0; control < uiControllers.size(); ++control)
                {
                    if (setup->uiControllersEnabled[control])
                    {
                        UINT* ids = uiControllers[control]->GetComponentIDS();
                        for (int cComps = 0; cComps < uiControllers[control]->GetComponentCount(); ++cComps)
                        {
                            if (uiControllers[control]->ProcessResponse(setup->uiComponents[ids[cComps]]->ProcessInput()))
                            {
                                inputTimer = 0;
                                return;
                            }
                        }
                        if (uiControllers[control]->ProcessInput(delta_time))
                        {
                            inputTimer = 0;
                            return;
                        }
                    }
                }
            }
            for (int overly = 0; overly < setup->overlays.size(); ++overly)
            {
                if (visible && setup->overlays[overly].visible)
                {
                    for (int control = 0; control < setup->overlays[overly].componentIDs.size(); ++control)
                    {
                        setup->uiComponents[setup->overlays[overly].componentIDs[control]]->Update(delta_time);
                    }
                }
            }
        }

        void Render(ComPtr<ID3D11DeviceContext1> _context, ComPtr<ID3D11RenderTargetView> _rtv)
        {
            RefreshOverlay(_context);
            ID3D11ShaderResourceView* resource_views[] = { uiResources.uiTextures.Get() };

            _context->PSSetShaderResources(0, 1, resource_views);
            _context->VSSetShader(uiResources.vertexShader.Get(), 0, 0);
            _context->PSSetShader(uiResources.pixelShader.Get(), 0, 0);
            _context->IASetInputLayout(uiResources.vertexBufferLayout.Get());

            // Send the matrix to constant buffer
            D3D11_MAPPED_SUBRESOURCE gpuBuffer;
            HRESULT result = _context->Map(uiResources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
            memcpy(gpuBuffer.pData, &uiResources.ortoprojMatrix, sizeof(XMFLOAT4X4));
            _context->Unmap(uiResources.constantBuffer.Get(), 0);
            // Connect constant buffer to the pipeline
            ID3D11Buffer* uiCBuffers[] = { uiResources.constantBuffer.Get() };
            _context->VSSetConstantBuffers(0, 1, uiCBuffers);

            ID3D11RenderTargetView* tempRTV[] = { _rtv.Get() };

            _context->OMSetDepthStencilState(uiResources.depthStencilState.Get(), 1);

            for (int overly = 0; overly < setup->overlays.size(); ++overly)
            {
                if (visible && setup->overlays[overly].visible)
                {
                    UINT strides[] = { sizeof(UIVertex) };
                    UINT offsets[] = { 0 };
                    ID3D11Buffer* uiVertexBuffers[] = { setup->overlays[overly].vertexBuffer.Get() };
                    _context->IASetVertexBuffers(0, 1, uiVertexBuffers, strides, offsets);
                    _context->IASetIndexBuffer(setup->overlays[overly].indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
                    _context->DrawIndexed(setup->overlays[overly].indices.size(), 0, 0);

                    for (int i = 0; i < setup->overlays[overly].componentIDs.size(); ++i)
                    {

                        Overlay2D ovl = setup->overlays[overly];
                        UINT cid = ovl.componentIDs[i];
                        TextLabel* label = setup->uiComponents[cid]->GetText();
                        if (label)
                        {
                            text.Render(*label);
                        }
                    }
                }
            }
            for (int label = 0; label < setup->textLabels.size(); ++label)
            {
                if(setup->textLabels[label].visible)
                    text.Render(setup->textLabels[label]);
            }

            _context->OMSetDepthStencilState(nullptr, 1);
        }

        void ShutDown()
        {
            for (int i = 0; setup->uiComponents.size(); ++i)
            {
                delete setup->uiComponents[i];
            }
            delete setup;
            for (int i = 0; uiControllers.size(); ++i)
            {
                delete uiControllers[i];
            }
        }   
    };
}

// NOTE: UI shit needs to be removed
/*

*/