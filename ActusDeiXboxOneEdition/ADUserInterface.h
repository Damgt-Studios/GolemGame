#pragma once
#include "pch.h"
#include "Types.h"
#include <vector>
#include <SpriteFont.h>

#include "Utils.h"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
#include <string>
#include <deque>
#include "Input.h"


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
    private:
        std::unique_ptr<SpriteBatch> spriteBatch;
        std::unique_ptr<SpriteFont> spriteFont;
    public:
        void Initialize(ID3D11Device1* _device, ID3D11DeviceContext1* _context);
        void Render(ADResource::AD_UI::TextLabel label);
    };

    class Image2D : public ADResource::AD_UI::UIComponent
    {
    public:
        int textureID;
        UINT frameCount;
        UINT currentFrame;
        float updateThreshold;
        float updateTimer;
        ADResource::AD_UI::QuadData* quad;

        Image2D();
        Image2D(ADResource::AD_UI::QuadData _quad, int _textureID = 0);
        Image2D(ADResource::AD_UI::QuadData _quad, ADResource::AD_UI::AnimData2d _animation, int _textureID = 0);
        ~Image2D();

        virtual void Enable();
        virtual void Disable();
        virtual void Update(float delta_time);
        virtual ADResource::AD_UI::QuadData* GetQuad() override;
    };

    class UILog : public ADResource::AD_UI::UIComponent
    {
    public:
        Image2D blackBox;
        ADResource::AD_UI::TextLabel consoleLabel;
        std::deque<std::string> messageQueue;

        void InitializeLog(float screenWidth, float screenHeight, float width = 200.f, float height = 400.f);
        virtual int ProcessInput();
        void WriteToLog(std::string message);
        virtual ADResource::AD_UI::QuadData* GetQuad();
        ADResource::AD_UI::TextLabel* GetText();
    };

    class Button2D : public ADResource::AD_UI::UIComponent
    {
    private:
        Image2D* image;
        Image2D* imageOnFocus;

    public:
        UINT actionValue;
        Button2D(Image2D* _image, Image2D* _imageOnFocus, bool _visible = true, bool _active = true, bool _controlFocus = false);
        ~Button2D();
        virtual int ProcessInput();
        virtual void Update(float delta_time);
        virtual ADResource::AD_UI::QuadData* GetQuad();
    };

    class ButtonList : public ADResource::AD_UI::UIComponent
    {
    public:
        float x, y, spacing;
        UINT columns;
        UINT maxRows;
        UINT selectedButtonIndex;
        ADResource::AD_UI::QuadData** myQuads;
        std::vector<Button2D*> buttons;

        ButtonList(float _x = 0.f, float _y = 0.f, float _spacing = 30.f, UINT _columns = 1, UINT _maxRows = INT_MAX, bool _active = false, bool _visible = false, bool _controlFocus = false);
        ~ButtonList();

        void AddButton(Button2D* _button);
        void RecalculatePositions();
        virtual void Initialize();
        virtual void Update(float delta_time);
        virtual int ProcessInput();
        virtual ADResource::AD_UI::QuadData** GetQuads();
    };

    class UISetup
    {
    public:
        std::vector<ADResource::AD_UI::TextLabel> textLabels;
        std::vector<ADResource::AD_UI::Overlay2D> overlays;
        std::vector<ADResource::AD_UI::UIComponent*> uiComponents;
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
        ~OverlayController();
        virtual bool ProcessInput(float delta_time);
        virtual bool ProcessResponse(UINT responseID);
        virtual void AddComponent(UINT _compID);
        UINT GetComponentCount();
        UINT* GetComponentIDS();
    };



    class ADUI
    {
    private:
        Text2D text;
        UISetup* setup;
        ID3D11DeviceContext1* context;
        std::vector<OverlayController*> uiControllers;
        ADResource::AD_UI::UIRendererResources uiResources;
        float responseTime;
        float inputTimer;

        inline void CreateQuad(ADResource::AD_UI::QuadData& _quad, std::vector<ADResource::AD_UI::UIVertex>& vertices, std::vector<int>& indices)
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
        int AddNewOverlay(bool _visible = false, bool _active = false, bool _dynamic = true);
        AD_ULONG AddUIComponent(ADResource::AD_UI::UIComponent* component);
        void AddUIController(OverlayController* _controller);
        UINT GetUIState();
        XMFLOAT2 GetPosition(float _percentageX, float _percentageY, float _screenWidth, float _screeHeight);
        UILog uiLog;
        UINT uiState = 1; 
        AD_UI::ADUI::UILog* GetLogComponent();
        void SetSetup(UISetup* _setup);
        void RefreshOverlay(ComPtr<ID3D11DeviceContext1> _context);
        void Initialize(ID3D11Device1* device, ID3D11DeviceContext1* _context);
        void Update(float delta_time);
        void Render(ComPtr<ID3D11DeviceContext1> _context, ComPtr<ID3D11RenderTargetView> _rtv);
        void ShutDown();
    };
}