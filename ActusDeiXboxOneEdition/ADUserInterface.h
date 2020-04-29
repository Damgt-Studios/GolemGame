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
#include <map>


namespace ADUI
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

    inline XMFLOAT2 componentPosToTextPos(XMFLOAT2 _original, float screenWidth, float screenHeight)
    {
        XMFLOAT2 value;
        value.x = _original.x + (screenWidth / 2.f);
        value.y = (-1 * _original.y) + (screenHeight / 2.f);
        return value;
    }

    class Text2D
    {
    private:
        SpriteBatch* spriteBatch;
        std::unique_ptr<SpriteFont> spriteFont;
    public:
        void Initialize(ID3D11Device1* _device, SpriteBatch* _spriteBatch);
        void Render(ADResource::AD_UI::TextLabel* label);
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

    class Label2D : public ADResource::AD_UI::UIComponent
    {
        ADResource::AD_UI::TextLabel consoleLabel;
    public:
        void SetText(std::string message);
        void SetText(ADResource::AD_UI::TextLabel message);
        ADResource::AD_UI::TextLabel* GetText();
    };

    class UILog : public ADResource::AD_UI::UIComponent
    {
    public:
        Image2D blackBox;
        ADResource::AD_UI::TextLabel consoleLabel;
        std::deque<std::string> messageQueue;

        void InitializeLog(float textXOffset, float textYOffset, float screenWidth, float screenHeight, ADResource::AD_UI::QuadData _quad);
        virtual ADResource::AD_UI::UIMessage* ProcessInput();
        void WriteToLog(std::string message);
        virtual ADResource::AD_UI::QuadData* GetQuad();
        ADResource::AD_UI::TextLabel* GetText();
    };

    enum UIMessageTypes
    {
        Ignore = 0,
        FromButton = 1,
        ExternalMsg = 2
    };

    enum ButtonResponses
    {
        A = 0,
        B,
        X,
        Y,
        RightTrigger,
        LeftTrigger,
        RightBumper,
        LeftBumper
    };

    class Button2D : public ADResource::AD_UI::UIComponent
    {
    private:
        Image2D* image;
        Image2D* imageOnFocus;
        ADResource::AD_UI::TextLabel buttonLabel;
        float textXOffset;
        float textYOffset;

    public:
        UINT actionValue;
        Button2D(Image2D* _image, Image2D* _imageOnFocus, bool _visible = true, bool _active = true, bool _controlFocus = false);
        ~Button2D();
        virtual ADResource::AD_UI::UIMessage* ProcessInput();
        virtual void Update(float delta_time);
        virtual ADResource::AD_UI::QuadData* GetQuad();
        virtual void SetText(char* _text, float textXOffset, float textYOffset, float screenWidth, float screenHeight, bool visible = true);
        virtual void ResetText(float _originalX, float _originalY, float screenWidth, float screenHeight, bool visible = true);
        virtual ADResource::AD_UI::TextLabel* GetText();
        virtual void Refresh();
    };

    class ButtonList : public ADResource::AD_UI::UIComponent
    {
    private:
        float screenWidth;
        float screenHeight;
    public:
        float x, y, spacing;
        UINT columns;
        UINT maxRows;
        UINT selectedButtonIndex;
        ADResource::AD_UI::QuadData** myQuads;
        std::vector<Button2D*> buttons;
        ADResource::AD_UI::TextLabel** buttonLabel;


        ButtonList(float screenWidth, float screenHeight, float _x = 0.f, float _y = 0.f, float _spacing = 30.f, UINT _columns = 1, UINT _maxRows = INT_MAX, bool _active = false, bool _visible = false, bool _controlFocus = false);
        ~ButtonList();

        void AddButton(Button2D* _button);
        void RecalculatePositions();
        void RepositionText();
        virtual void Initialize();
        virtual void Update(float delta_time);
        virtual ADResource::AD_UI::UIMessage* ProcessInput();
        virtual ADResource::AD_UI::QuadData** GetQuads();
        virtual ADResource::AD_UI::TextLabel** GetTexts();
    };

    class UISetup
    {
    public:
        std::map<std::string, UINT> componentsNameToID;
        std::map<std::string, UINT> overlaysNameToID;
        std::map<std::string, UINT> controllersNameToID;
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
        std::vector<UINT> componentNames;
        std::map<UINT, ADResource::AD_UI::UIComponent*> componentTypeMap;  //Keep to access them from editor  BOTH come in from AddCompoenent.  We won't use types.

    public:
        OverlayController(UINT& _uiState, UISetup& _setup) : uiState(_uiState), setup(_setup) {};
        ~OverlayController();
        virtual bool ProcessInput(float delta_time);
        virtual bool ProcessResponse(ADResource::AD_UI::UIMessage* responseID);
        //virtual void AddComponent(UINT _compID);
        virtual void AddComponent(UINT _typeID, ADResource::AD_UI::UIComponent* _comp);
        UINT GetComponentCount();
        UINT* GetComponentIDS();

        template<typename T>
        T Get(int id)
        {
            std::map<UINT, ADResource::AD_UI::UIComponent*>::iterator i = componentTypeMap.find(id);

            if (i != componentTypeMap.end())
                return dynamic_cast<T>(i->second);
            else return nullptr;
        }
    };


    class ADUI
    {
    private:
        ADResource::AD_UI::UIHeader header;
        SpriteBatch* spriteBatch;
        Text2D text;
        UISetup* setup;
        ID3D11DeviceContext1* context;
        std::vector<OverlayController*> uiControllers;
        ADResource::AD_UI::UIRendererResources uiResources;
        float responseTime;
        float inputTimer;

        inline void UpdateQuad(UINT _startQuad, ADResource::AD_UI::QuadData& _quad, std::vector<ADResource::AD_UI::UIVertex>& vertices)
        {
            UINT index = _startQuad * 4;
            vertices[index].Color = { 1,1,1,1 };
            vertices[index].Pos = { _quad.x - _quad.quadWidth / 2.f, _quad.y - _quad.quadHeight / 2.f, 1 };
            vertices[index].Tex = { _quad.minU, _quad.maxV };

            index = (_startQuad * 4) + 1;
            vertices[index].Color = { 1,1,1,1 };
            vertices[index].Pos = { _quad.x - _quad.quadWidth / 2.f, _quad.y + _quad.quadHeight / 2.f, 1 };
            vertices[index].Tex = { _quad.minU, _quad.minV };

            index = (_startQuad * 4) + 2;
            vertices[index].Color = { 1,1,1,1 };
            vertices[index].Pos = { _quad.x + _quad.quadWidth / 2.f, _quad.y + _quad.quadHeight / 2.f, 1 };
            vertices[index].Tex = { _quad.maxU, _quad.minV };

            index = (_startQuad * 4) + 3;
            vertices[index].Color = { 1,1,1,1 };
            vertices[index].Pos = { _quad.x + _quad.quadWidth / 2.f, _quad.y - _quad.quadHeight / 2.f, 1 };
            vertices[index].Tex = { _quad.maxU, _quad.maxV };
        }

        inline void CreateQuad(ADResource::AD_UI::QuadData& _quad, std::vector<ADResource::AD_UI::UIVertex>& vertices, std::vector<int>& indices)
        {
            UINT startIndex = vertices.size();
            vertices.push_back({ {1,1,1,1},{_quad.x - _quad.quadWidth / 2.f, _quad.y - _quad.quadHeight / 2.f, 1},  {_quad.minU, _quad.maxV} });
            vertices.push_back({ {1,1,1,1},{_quad.x - _quad.quadWidth / 2.f, _quad.y + _quad.quadHeight / 2.f, 1},  {_quad.minU, _quad.minV} });
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
        int AddNewOverlay(std::string _name, bool _visible = false, bool _active = false, bool _dynamic = true);
        AD_ULONG AddUIComponent(std::string _name, ADResource::AD_UI::UIComponent* component);
        void AddUIController(std::string _name, OverlayController* _controller);
        UINT GetUIState();
        XMFLOAT2 GetPosition(float _percentageX, float _percentageY, float _screenWidth, float _screeHeight);
        UILog uiLog;
        UINT uiState = 1;
        ADUI::ADUI::UILog* GetLogComponent();
        void SetAltasHeader(ADResource::AD_UI::UIHeader _header);
        void SetSetup(UISetup* _setup);
        void RefreshOverlay(ComPtr<ID3D11DeviceContext1> _context);
        void Initialize(ID3D11Device1* device, ID3D11DeviceContext1* _context);
        void Update(float delta_time);
        void Render(ComPtr<ID3D11DeviceContext1> _context, ComPtr<ID3D11RenderTargetView> _rtv);
        void RecieveMessage(ADResource::AD_UI::UIMessage* _message);
        void ShutDown();
    };

    class MessageReceiver
    {
        static ADUI* userInterface;
    public:
        static void SetUI(ADUI* _userInterface);
        static void Log(std::string _message);
        static void SendMessage(ADResource::AD_UI::UIMessage* _message);
    };
}