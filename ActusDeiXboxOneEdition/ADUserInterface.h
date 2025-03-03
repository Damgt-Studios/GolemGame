#pragma once
#include "Types.h"
#include <vector>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "Utils.h"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
#include <string>
#include <deque>
#include "Input.h"
#include <map>
#include <CommonStates.h>


namespace ADUI
{
    enum UIMessageTargets
    {
        Ignore = 0,
        Component = 1,
        Controller,
        FirstSelector,
        FirstGrid
    };

    enum UISTATE
    {
        GAMEPLAY = 0,
        MENUSTATE,
        QUIT,
        COUNT
    };

    class UIMessage
    {
    public:
        UIMessage() {};
        ~UIMessage() {};

        bool externalMsg = false;
        UINT targetID = 0;
        UINT commandID = 0;
        UINT componentIndex = 0;
        union
        {
            int ivalue = 0;
            XMFLOAT4 fvalue;
            bool bvalue;
        };
    };

    struct AnimationData
    {
        UINT startFrame = 0;
        UINT frameCount = 0;
        UINT fps = 0;
        float updateThreshold = 0;
    };

    struct TextLabel
    {
        bool visible = false;
        XMFLOAT2 position = { 0,0 };
        std::string output = "";

        float scale = 0;
        XMVECTOR tint = Colors::White;
        float rotation = 0.f;
        XMFLOAT2 origin = { 0,0 };
        UINT effects = 0;
        float depth = 0;
    };

    class Reaction
    {
    public:
        GamepadButtons reactionCode;
        UIMessage message;

        Reaction(GamepadButtons _reactionCode, UIMessage _message)
        {
            reactionCode = _reactionCode;
            message = _message;
        }

        UIMessage* ProcessInput()
        {
            if (Input::QueryButtonDown(reactionCode))
            {
                return &message;
            }
        }
    };

    enum ComponentOptions
    {
        SetPosition = 0,
        SetCorners,
        SetWidth,
        SetHeight,
        SetStretched,
        SetScale,
        ComponentLast
    };

    enum ImageResponses
    {
        SetTint = ComponentOptions::ComponentLast + 1,
        SetRotation,
        ChangeRotation,
        SetEffects,
    };

    class UIComponent
    {
    protected:
        Reaction** reactions = nullptr;
        UINT reactionCount = 0;
        RECT corners = { 0,0,0,0 };
        XMFLOAT2 position = { 0,0 };
        XMFLOAT2 posOffsetByAnchor = { 0,0 };
        XMFLOAT2 size = { 0,0 };
        XMFLOAT2 center = { 0,0 };
        XMFLOAT2 scale = { 0,0 };
        bool controlFocus = false;

    public:
        std::string name = "";
        bool active = false;
        bool visible = false;
        bool stretched = false;


        virtual ~UIComponent();
        std::unique_ptr<UIComponent> clone() const
        {
            return std::unique_ptr<UIComponent>(this->clone_impl());
        }
        virtual void Update(float delta_time) {};
        virtual void Render() {};
        virtual UINT GetIndex() { return 0; };
        virtual void AddReactions(UINT _reactionCount, Reaction** _reactionArray)
        {
            reactionCount = _reactionCount;
            reactions = _reactionArray;
        };
        virtual UIMessage* ProcessInput();
        virtual bool ProcessResponse(UIMessage* _message);

        virtual void Enable() { visible = true; active = true; };
        virtual void Disable() { visible = false; active = false; };
        virtual void Focus(bool _focus = true) { controlFocus = _focus; };
        virtual void Unfocus() { controlFocus = false; };
        virtual bool IsFocus() { return controlFocus; };

        virtual void SetPosition(XMFLOAT2 _position);
        virtual void SetCorners(XMFLOAT4 _pos);
        virtual void SetWidth(float _width);
        virtual void SetHeight(float _height);
        virtual void SetStretched(bool _stretched);
        virtual void SetScale(XMFLOAT2 _scale);

        virtual void SetText(std::string _message) {};

        virtual float GetWidth();
        virtual float GetHeight();

    protected:
        virtual UIComponent* clone_impl() const { return nullptr; };
    };

    struct UIRendererResources
    {
        UINT textureCount;
        ID3D11ShaderResourceView** uiTextures = nullptr;
        ComPtr<ID3D11DepthStencilState> depthStencilState;
    };

    class Overlay2D
    {
    public:
        std::string name = "";
        bool active = true;
        bool visible = true;
        std::vector < UIComponent* > components;

        Overlay2D(bool _visible = false, bool _active = false)
        {
            visible = _visible;
            active = _active;
        };

        ~Overlay2D()
        {
            components.clear();
        };

        void AddComponent(UIComponent* _compID)
        {
            components.push_back(_compID);
        };

        void Enable()
        {
            visible = true;
            active = true;
        }

        void Disable()
        {

            visible = false;
            active = false;
        }
    };

    class OverlayController
    {
    protected:
        std::vector<Overlay2D*> overlays;
        std::vector<OverlayController*> controllers;

    public:
        std::map<std::string, UINT> overlaysNameToID;
        std::map<std::string, UINT> controllersNameToID;
        std::map<std::string, UINT> componentsNameToID;
        std::map<UINT, UIComponent*> componentTypeMap;

        bool active;
        float delayInputTimer = 0;
        std::string name;
        ~OverlayController()
        {
            overlays.clear();
            controllers.clear();
            componentTypeMap.clear();
        };
        virtual bool ProcessInput(float delta_time, float& quick);
        virtual bool ProcessResponse(UIMessage* responseID, float& quick);
        virtual UINT AddOverlay(Overlay2D* _overlay);
        virtual UINT AddController(OverlayController* _controller);
        virtual UINT AddComponent(UIComponent* _comp);
        virtual void Enable();
        virtual void Disable();
        UINT GetComponentCount();
        UIComponent* GetComponent(int id);

        template<typename T>
        T GetComponentSpecfic(int id)
        {
            std::map<UINT, UIComponent*>::iterator i = componentTypeMap.find(id);

            if (i != componentTypeMap.end())
                return dynamic_cast<T>(i->second);
            else return nullptr;
        }
    };

    class Text2D
    {
    private:
        SpriteBatch* spriteBatch = nullptr;

    public:
        std::unique_ptr<SpriteFont> spriteFont;
        void Initialize(std::string _fileName, ID3D11Device1* _device, SpriteBatch* _spriteBatch);
        void Render(std::string& output, XMFLOAT2 position, XMVECTOR tint, float rotation, XMFLOAT2 origin);
    };

    class Image2D : public UIComponent
    {
        ID3D11ShaderResourceView* texture = nullptr;
        SpriteBatch* spriteBatch = nullptr;
        RECT* uvRect = nullptr;
        AnimationData* animations = nullptr;
        UINT animationCount = 0;
        UINT uvCount = 0;
        UINT uvColumnCount = 0;
        UINT currentFrame = 0;
        UINT currentAnimation = 0;
        float updateTimer = 0.f;

        XMVECTOR tint = Colors::White;
        float rotation = 0.f;
        SpriteEffects effects;
        float depth = 0.f;

    public:
        UINT tiled = 1;
        UINT controlFocusAnimation = 0;

        Image2D(SpriteBatch* _spriteBatch, ID3D11ShaderResourceView* _texture, XMFLOAT4 _posRect);
        ~Image2D();
        void BuildAnimation(RECT _uvRect, UINT _uvColumnCount, UINT _animationCount, AnimationData* _animations);
        void SetTint(XMVECTOR _tint);
        void SetRotation(float _rotation, bool _continuous = false);
        void SetEffects(SpriteEffects _effects);
        void SetCurrentFrame(UINT _frameNumber);
        void SetDepth(float _depth);
        void Enable() override;
        void Disable() override;
        void Update(float delta_time) override;
        void Render() override;
        bool ProcessResponse(UIMessage* _message) override;
        void SetTiled(UINT _tiledCount);

        RECT GetPos();
        RECT* GetuvRect();
        const RECT GetPosRect();
        const XMFLOAT2 GetPosFloats();
        const float GetUpdateThreshold();
        const float GetUpdateTimer();
        const XMVECTOR GetTint();
        const XMFLOAT2 GetScale();
        const float GetRotation();
        const SpriteEffects GetEffects();
        const float GetDepth();
        const bool GetStretched();


        std::unique_ptr<Image2D> clone() const
        {
            return std::unique_ptr<Image2D>(this->clone_impl());
        }
    private:
        virtual Image2D* clone_impl() const override
        {
            return new Image2D(*this);
        }
    };

    class Label2D : public UIComponent
    {
        Text2D* text = nullptr;

        float scale = 1;
        XMVECTOR tint = Colors::White;
        float rotation = 0.f;
        UINT effects = 0;
        float depth = 0;

    public:
        std::string output;
        Label2D();
        ~Label2D();
        void SetText(std::string _message);
        void SetText(std::string _message, XMFLOAT2 _position, float _scale = 1, XMVECTOR _tint = Colors::White, float rotation = 0.f, XMFLOAT2 origin = { 0,0 }, UINT effects = 0, float depth = 0);
        void SetPosition(XMFLOAT2 _position);
        void ResetPosition(XMFLOAT2 _position);
        void SetFont(Text2D* _spriteFont);
        SpriteFont* GetFont();
        Text2D* GetText2D();
        virtual void Render() override;
        bool ProcessResponse(UIMessage* _message) override;
        void SetTint(XMVECTOR _tint);
        void SetRotation(float _rotation, bool _continuous = false);
        void SetEffects(SpriteEffects _effects);
        void SetDepth(float _depth);

        std::unique_ptr<Label2D> clone() const
        {
            return std::unique_ptr<Label2D>(this->clone_impl());
        }
    private:
        virtual Label2D* clone_impl() const override
        {
            return new Label2D(*this);
        }
    };


    class Button2D : public UIComponent
    {
    private:
        Image2D* image = nullptr;
        Label2D* buttonLabel = nullptr;

    public:
        UINT actionValue = 0;
        Button2D(XMFLOAT4 _position, Image2D* _image, Label2D* _buttonLabel, bool _visible = true, bool _active = true, bool _controlFocus = false);
        ~Button2D();
        Button2D(const Button2D& p2);
        virtual UIMessage* ProcessInput();
        virtual void Update(float delta_time);
        virtual void Render();
        virtual void SetCorners(XMFLOAT4 _pos) override;
        void SetText(std::string _message);
        virtual Label2D* GetText();
        virtual void Focus(bool _focus = true) override;
        virtual void Unfocus() override;


        std::unique_ptr<Button2D> clone() const
        {
            return std::unique_ptr<Button2D>(this->clone_impl());
        }
    private:
        virtual Button2D* clone_impl() const override
        {
            return new Button2D(*this);
        }
    };

    enum SelectorResponses
    {
        Zero = 0,
        IncreaseByInt,
        DecreaseByInt,
        Max
    };

    class UIComponentSelector : public UIComponent
    {
    private:
        UIComponent** components = nullptr;
        UINT componentCount = 0;
        UINT currentComponent = 0;
        Label2D buttonLabel;
        float textXOffset= 0;
        float textYOffset= 0;
        float height = 0;
        float width = 0;


    public:
        UIComponentSelector(UINT componentCount, UIComponent** components, bool _visible = true, bool _active = true, bool _controlFocus = false);
        ~UIComponentSelector();
        virtual UIMessage* ProcessInput() override;
        virtual bool ProcessResponse(UIMessage* _message) override;
        virtual void Update(float delta_time) override;
        virtual void Render() override;
        virtual void SetText(std::string _text, float _textXOffset, float _textYOffset, bool _visible = true);
        virtual void SetText(TextLabel _label, float _textXOffset, float _textYOffset, bool _visible = true);
        void SetFont(Text2D* _spriteFont);
        SpriteFont* GetFont();
        Text2D* GetText2D();
        void SetCorners(XMFLOAT4 _pos) override;
        RECT GetPosRect();
        float GetWidth() override;
        float GetHeight() override;
        UINT GetIndex() override;

        UIComponent* GetComponent(UINT _index);


        std::unique_ptr<UIComponentSelector> clone() const
        {
            return std::unique_ptr<UIComponentSelector>(this->clone_impl());
        }
    private:
        virtual UIComponentSelector* clone_impl() const override
        {
            return new UIComponentSelector(*this);
        }
    };

    enum ComponentGridStyle
    {
        ExactSpacing = 0,
        FirstComponent,
        LargestInRowColumn
    };

    class ComponentGrid : public UIComponent
    {
    public:
        RECT* positions = nullptr;
        XMFLOAT2 topLeft = { 0,0 };
        float spacing = 0;
        int columns = 0;
        int maxRows = 0;
        int selectedButtonIndex = 0;
        ComponentGridStyle spacingStyle;
        std::vector<std::unique_ptr<UIComponent>> components;


        ComponentGrid(float _x = 0.f, float _y = 0.f, float _spacing = 30.f, UINT _columns = 1, UINT _maxRows = INT_MAX, bool _active = false, bool _visible = false, bool _controlFocus = false);
        ~ComponentGrid();

        void AddComponent(UIComponent* _component);
        void RecalculatePositions();
        virtual void Generate(UIComponent* _comp, XMFLOAT4 _position, XMFLOAT2 _spacing, UINT _count, UINT _columns, ComponentGridStyle _spacingStyle, bool _active, bool _isFocus);
        virtual void Initialize();
        virtual UIMessage* ProcessInput() override;
        virtual void Update(float delta_time) override;
        virtual void Render() override;
        virtual void Enable() override;
        virtual void Disable() override;
        void SetSelected(int _selection);
        UINT GetIndex() override;

    public:

        Input::THUMBSTICKS thumbstickBinding = Input::THUMBSTICKS::LEFT_THUMBSTICK;

        GamepadButtons leftButton1 = GamepadButtons::DPadLeft;
        GamepadButtons rightButton1 = GamepadButtons::DPadRight;
        GamepadButtons upButton1 = GamepadButtons::DPadUp;
        GamepadButtons downButton1 = GamepadButtons::DPadDown;
    };

    class Slider
    {
    public:
        Image2D* slider = nullptr;
        XMFLOAT2 position = { 0,0 };
        XMFLOAT2 maxPosition= { 0,0 };
        XMFLOAT2 minPosition= { 0,0 };

        Slider(Image2D* _sliderImage, XMFLOAT2 _position, XMFLOAT2 _maxPosition, XMFLOAT2 _minPosition);
        ~Slider();
        void SetValue(float _ratio);
        void Update(float delta_time);
        void Render();

    };

    class SliderBar : public UIComponent
    {
        Image2D* slide = nullptr;
        Slider* slider = nullptr;
        Image2D* leftCap = nullptr;
        Image2D* rightCap = nullptr;
        Label2D* label = nullptr;
        float incriment = 0;
        float maxValue = 0;
        float minValue = 0;
        float currentRatio = 0;

    public:
        SliderBar(Image2D* _slide, Slider* _slider, Image2D* _leftCap, Image2D* _rightCap, Label2D* _label, float _minValue = 0, float _maxValue = 1.f, float _currentValue = 0.5f, float _incriment = 0.01f);
        ~SliderBar();
        UIMessage* ProcessInput() override;
        void Update(float delta_time) override;
        void Render() override;

        Input::THUMBSTICKS thumbstickBinding = Input::THUMBSTICKS::LEFT_THUMBSTICK;

        GamepadButtons leftButton1 = GamepadButtons::DPadLeft;
        GamepadButtons rightButton1 = GamepadButtons::DPadRight;

        std::unique_ptr<SliderBar> clone() const
        {
            return std::unique_ptr<SliderBar>(this->clone_impl());
        }
    private:
        virtual SliderBar* clone_impl() const override
        {
            return new SliderBar(*this);
        }
    };

    class UILog : public UIComponent
    {
    public:
        UILog();
        Image2D* blackBox = nullptr;
        Label2D* consoleLabel = nullptr;
        std::deque<std::string> messageQueue;

        ~UILog();
        void InitializeLog(SpriteBatch* _spriteBatch, ID3D11ShaderResourceView* _texture); 
        void Setup(Image2D* _background, Label2D* _label);
        void SetFont(Text2D* _fontSmall);
        void SetRects(XMFLOAT4 _posRect, RECT _uvRect);
        virtual UIMessage* ProcessInput();
        void WriteToLog(std::string message);
        void Render();

        std::unique_ptr<UILog> clone() const
        {
            return std::unique_ptr<UILog>(this->clone_impl());
        }
    private:
        virtual UILog* clone_impl() const override
        {
            return new UILog(*this);
        }
    };

    class ADUI
    {
        std::unique_ptr<DirectX::CommonStates> m_states;
        float responseTime = 0;
        float inputTimer = 0;
        ComPtr<ID3D11Device1> device;
        ComPtr<ID3D11DeviceContext1> context;
        ComPtr<ID3D11RenderTargetView> rtv;
        UINT fontCount = 0;
        Text2D* spriteFonts = nullptr;
        UINT uiState = 1;
        UILog* uiLog = nullptr;

    public:
        std::vector<UIComponent*> uiComponents;
        std::vector<Overlay2D*> overlays;
        std::vector<OverlayController*> uiControllers;
        std::vector<AnimationData*> animations;

        std::unique_ptr<SpriteBatch> spriteBatch;
        UIRendererResources uiResources;
        D3D11_VIEWPORT* viewport = nullptr;

        ADUI();
        ~ADUI();
        void Initialize(ComPtr<ID3D11Device1> _device, ComPtr<ID3D11DeviceContext1> _context, ComPtr<ID3D11RenderTargetView> _rtv, D3D11_VIEWPORT* _vp);
        void AddTextureArray(char** _fileNameArray, UINT _count);
        void AddFontArray(char** _fileNameArray, UINT _count);
        UINT AddUIComponent(std::string _name, UIComponent* _component);
        UINT AddNewOverlay(std::string _name, bool _visible = false, bool _active = false);
        UINT AddUIController(std::string _name, OverlayController* _controller);

        void Update(float delta_time);
        void Render();
        void Shutdown();

        void RecieveMessage(UIMessage* _message);
        UINT* GetUIState();
        void GetUIState(UINT _newState);
        UILog* GetLog();
        Text2D* GetFont(UINT _id);

    };

    class MessageReceiver
    {
        static ADUI* userInterface;
    public:
        static void SetUI(ADUI* _userInterface);
        static void Log(std::string _message);
        static void SendMessage(UIMessage* _message);
    };

    class Settings
    {
    public:
        static float screenWidth;
        static float screenHeight;
        static float planningWidth;
        static float planningHeight;
        static float resolutionWidth;
        static float resolutionHeight;
        static float resScalingWidth;
        static float resScalingHeight;
        static float posScalingWidth;
        static float posScalingHeight;

        static void RefreshResScaling()
        {
            resScalingWidth = screenWidth / resolutionWidth;
            resScalingHeight = screenHeight / resolutionHeight;
            posScalingWidth = (screenWidth / planningWidth) * (resolutionWidth / planningWidth);
            posScalingHeight = (screenHeight / planningHeight) * (resolutionHeight / planningHeight);
        }
    };

    static inline XMFLOAT2 CenterOfImage(XMVECTOR& stringSize, Image2D* _image)
    {
        XMFLOAT2 temp;
        temp.x = _image->GetPos().left + (_image->GetPos().right - _image->GetPos().left) / 2.f;
        temp.y = _image->GetPos().top + (_image->GetPos().bottom - _image->GetPos().top) / 2.f;
        temp.x = temp.x + (XMVectorGetX(stringSize) / -2.f);
        temp.y = temp.y + (XMVectorGetY(stringSize) / -2.f);
        return temp;
    }
}