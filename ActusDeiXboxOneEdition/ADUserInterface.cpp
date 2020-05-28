#include "pch.h"
#include "ADUserInterface.h"

bool ADUI::UIComponent::ProcessResponse(UIMessage* _message)
{
    if (_message->targetID == UIMessageTargets::Component)
    {
        switch (_message->commandID)
        {
        case ComponentOptions::SetPosition:
            SetPosition({ _message->fvalue.x, _message->fvalue.y });
            break;
        case ComponentOptions::SetCorners:
            SetCorners({ _message->fvalue.x, _message->fvalue.y, _message->fvalue.z, _message->fvalue.w });
            break;
        case ComponentOptions::SetWidth:
            SetWidth(_message->fvalue.x);
            break;
        case ComponentOptions::SetHeight:
            SetHeight(_message->fvalue.x);
            break;
        case ComponentOptions::SetStretched:
            SetStretched(_message->bvalue);
            break;
        case ComponentOptions::SetScale:
            SetScale({ _message->fvalue.x, _message->fvalue.y });
            break;
        default:
            break;
        }
        return true;
    }
    return false;
}

void ADUI::UIComponent::SetPosition(XMFLOAT2 _position)
{
    position.x = _position.x * Settings::posScalingWidth;
    position.y = _position.y * Settings::posScalingHeight;

    corners.left = position.x;
    corners.top = position.y;
    corners.right = position.x + size.x;
    corners.bottom = position.y + size.y;

    //center.x = size.x / 2;
    //center.y = size.y / 2;

    //posOffsetByAnchor.x = position.x - (center.x);
    //posOffsetByAnchor.y = position.y - (center.y);
}

void ADUI::UIComponent::SetCorners(XMFLOAT4 _position)
{
    size.x = (_position.z - _position.x) * Settings::resScalingWidth;
    size.y = (_position.w - _position.y) * Settings::resScalingHeight;

    SetPosition({ _position.x, _position.y });
}

void ADUI::UIComponent::SetWidth(float _width)
{
    size.x = _width;
    corners.right = position.x + size.x;
    //center.x = position.x + (size.x * 0.5f * Settings::resScalingWidth);
}

void ADUI::UIComponent::SetHeight(float _height)
{
    size.y = _height;
    corners.bottom = position.y + size.y;
    //center.y = position.y + (size.y * 0.5f * Settings::resScalingHeight);
}

void ADUI::UIComponent::SetStretched(bool _stretched)
{
    stretched = _stretched;
}

void ADUI::UIComponent::SetScale(XMFLOAT2 _scale)
{
    scale = _scale;
}

float ADUI::UIComponent::GetWidth()
{
    return size.x;
}

float ADUI::UIComponent::GetHeight()
{
    return size.y;
}

void ADUI::Text2D::Initialize(std::string _fileName, ID3D11Device1* _device, SpriteBatch* _spriteBatch)
{
    spriteBatch = _spriteBatch;
    spriteFont = std::make_unique<SpriteFont>(_device, ADUtils::GetTexturePath(_fileName.c_str()).c_str());
}

void ADUI::Text2D::Render(std::string& output, XMFLOAT2 position, XMVECTOR tint, float rotation, XMFLOAT2 origin)
{
    spriteFont->DrawString(spriteBatch, output.c_str(), position, tint, rotation, origin, { Settings::resScalingWidth, Settings::resScalingHeight });
}
//
//ADUI::Image2D::Image2D()
//{
//    visible = false;
//    active = false;
//    animationData = { 1,0,1,0 };
//    currentFrame = 0;
//    updateThreshold = D3D10_FLOAT32_MAX;
//    updateTimer = 0.f;
//    scale = { 1,1 };
//}


ADUI::Image2D::Image2D(SpriteBatch* _spriteBatch, ID3D11ShaderResourceView* _texture, XMFLOAT4 _posRect)
{
    spriteBatch = _spriteBatch;
    texture = _texture;
    visible = true;
    active = true;
    currentFrame = 0;
    //updateThreshold = D3D10_FLOAT32_MAX;
    updateTimer = 0.f;
    scale = { 1,1 };
    SetCorners(_posRect);
}

ADUI::Image2D::~Image2D()
{
    if (animations)
    {
        delete[] animations;
    }
}

void ADUI::Image2D::BuildAnimation(RECT _uvRect, UINT _uvColumnCount, UINT _animationCount, AnimationData* _animations)
{
    uvCount = 0;
    for (int i = 0; i < _animationCount; ++i)
    {
        uvCount += _animations[i].frameCount;
        _animations[i].updateThreshold = (1.0f / _animations[i].fps);
    }

    uvRect = new RECT[uvCount];
    uvRect[0] = _uvRect;

    animations = _animations;

    updateTimer = 0.f;

    float currentColumn = 0;
    float currentRow = 0;

    for (int i = 0; i < uvCount; ++i)
    {
        currentRow = i / _uvColumnCount;
        currentColumn = i - (currentRow * _uvColumnCount);
        uvRect[i].left = uvRect[0].left + (uvRect[0].right - uvRect[0].left) * currentColumn;
        uvRect[i].right = uvRect[0].right + (uvRect[0].right - uvRect[0].left) * currentColumn;
        uvRect[i].top = uvRect[0].top + (uvRect[0].bottom - uvRect[0].top) * currentRow;
        uvRect[i].bottom = uvRect[0].bottom + (uvRect[0].bottom - uvRect[0].top) * currentRow;
    }
}


void ADUI::Image2D::SetTint(XMVECTOR _tint)
{
    tint = _tint;
}

void ADUI::Image2D::SetRotation(float _rotation, bool _continuous)
{
    rotation = _rotation;
}

void ADUI::Image2D::SetEffects(SpriteEffects _effects)
{
    effects = _effects;
}

void ADUI::Image2D::SetCurrentFrame(UINT _frameNumber)
{
    currentFrame = _frameNumber;
}

void ADUI::Image2D::SetDepth(float _depth)
{
    depth = _depth;
}

//
//void ADUI::Image2D::SetPosition(RECT _posRect)
//{
//    posFloats.x = _posRect.left * Settings::resScalingWidth;
//    posFloats.y = _posRect.top * Settings::resScalingHeight;
//    posRect = _posRect;
//}
//
//void ADUI::Image2D::SetPosition(XMFLOAT2 _position)
//{
//    posFloats = _position;
//}
//
//void ADUI::Image2D::SetScale(XMFLOAT2 _scale)
//{
//    scale = _scale;
//}
//
//void ADUI::Image2D::SetStretched(bool _isStretched)
//{
//    stretched = _isStretched;
//}

RECT ADUI::Image2D::GetPos()
{
    return corners;
}

void ADUI::Image2D::Enable()
{
    visible = true;
    active = true;
}

void ADUI::Image2D::Disable()
{
    visible = false;
    active = false;
}

void ADUI::Image2D::Update(float delta_time)
{
    if (active)
    {
        if (controlFocus)
        {
            currentAnimation = controlFocusAnimation;
        }
        else
        {
            currentAnimation = 0;
            currentFrame = 0;
        }
        if (updateTimer < animations[currentAnimation].updateThreshold)
        {
            updateTimer += delta_time;
        }
        else if (animations[currentAnimation].frameCount >= 1)
        {
            updateTimer = 0.f;
            ++currentFrame;
            if (currentFrame >= animations[currentAnimation].frameCount + animations[currentAnimation].startFrame)
            {
                currentFrame = animations[currentAnimation].startFrame;
            }
        }
    }
}

void ADUI::Image2D::Render()
{
    if (stretched)
    {
        RECT scaledRect = { corners.left * (scale.x), // * Settings::posScalingWidth),
            corners.top * (scale.y),// Settings::posScalingHeight * ,
            corners.right * (scale.x),//Settings::resScalingWidth *),
            corners.bottom * (scale.y) };//Settings::resScalingHeight *) };
        //spriteBatch->Draw(texture, scaledRect, &uvRect[currentFrame], tint, rotation, anchor, effects, depth);
        spriteBatch->Draw(texture, scaledRect, &uvRect[currentFrame], tint, rotation, center, effects, depth);
    }
    else
        for (int i = 0; i < tiled; ++i)
        {
            float x = position.x + (i * GetWidth());
            spriteBatch->Draw(texture, { x, position.y }, &uvRect[currentFrame], tint, rotation, center, { Settings::resScalingWidth * scale.x, Settings::resScalingHeight * scale.y }, effects, depth);
        }
}


bool ADUI::Image2D::ProcessResponse(UIMessage* _message)
{
    if (_message->targetID == UIMessageTargets::Component)
    {
        if (_message->commandID < ComponentOptions::ComponentLast)
        {
            return UIComponent::ProcessResponse(_message);
        }
        switch (_message->commandID)
        {
        case ImageResponses::SetTint:
        {
            XMVECTOR temp = DirectX::XMVectorSet(_message->fvalue.x, _message->fvalue.y, _message->fvalue.z, _message->fvalue.w);
            SetTint(temp);
            break;
        }
        case ImageResponses::SetRotation:
        {
            SetRotation(_message->fvalue.x);
            break;
        }
        case ImageResponses::ChangeRotation:
        {
            SetRotation(rotation + _message->fvalue.x);
            break;
        }
        case ImageResponses::SetEffects:
        {
            if (_message->ivalue > -1)
                SetEffects(SpriteEffects(_message->ivalue));
            break;
        }
        default:
            break;
        }
        return true;
    }
    return false;
}

void ADUI::Image2D::SetTiled(UINT _tiledCount)
{
    tiled = _tiledCount;
}

const RECT ADUI::Image2D::GetPosRect()
{
    return corners;
}

const XMFLOAT2 ADUI::Image2D::GetPosFloats()
{
    return position;
}

RECT* ADUI::Image2D::GetuvRect()
{
    return uvRect;
}

const float ADUI::Image2D::GetUpdateThreshold()
{
    return animations[currentAnimation].updateThreshold;
}

const float ADUI::Image2D::GetUpdateTimer()
{
    return updateTimer;
}

const XMVECTOR ADUI::Image2D::GetTint()
{
    return tint;
}
//
//const XMFLOAT2 ADUI::Image2D::GetOrigin()
//{
//    return anchor;
//}

const XMFLOAT2 ADUI::Image2D::GetScale()
{
    return scale;
}

const float ADUI::Image2D::GetRotation()
{
    return rotation;
}

const SpriteEffects ADUI::Image2D::GetEffects()
{
    return effects;
}

const float ADUI::Image2D::GetDepth()
{
    return depth;
}

const bool ADUI::Image2D::GetStretched()
{
    return stretched;
}

bool ADUI::OverlayController::ProcessInput(float delta_time, float& quick)
{
    return false;
}

bool ADUI::OverlayController::ProcessResponse(UIMessage* responseID, float& quick)
{
    return false;
}

UINT ADUI::OverlayController::AddOverlay(Overlay2D* _overlay)
{
    unsigned int index = overlays.size();
    overlaysNameToID[_overlay->name] = index;
    overlays.push_back(_overlay);
    return index;
}

UINT ADUI::OverlayController::AddController(OverlayController* _controller)
{
    unsigned int index = controllers.size();
    controllersNameToID[_controller->name] = index;
    controllers.push_back(_controller);
    return index;
}

UINT ADUI::OverlayController::AddComponent(UIComponent* _comp)
{
    unsigned int index = componentTypeMap.size();
    componentsNameToID[_comp->name] = index;
    componentTypeMap.emplace(index, _comp);
    return index;
}

void ADUI::OverlayController::Enable()
{
    active = true;
}

void ADUI::OverlayController::Disable()
{
    active = false;
}


UINT ADUI::OverlayController::GetComponentCount()
{
    return componentTypeMap.size();
}

ADUI::UIComponent* ADUI::OverlayController::GetComponent(int id)
{
    return componentTypeMap[id];
}

ADUI::Label2D::Label2D()
{
}
//
//ADUI::TextLabel* ADUI::Label2D::GetLabel()
//{
//    return &label;
//}

SpriteFont* ADUI::Label2D::GetFont()
{
    return text->spriteFont.get();
}

ADUI::Text2D* ADUI::Label2D::GetText2D()
{
    return text;
}

void ADUI::Label2D::SetText(std::string _message)
{
    output = _message;
    XMVECTOR size = text->spriteFont.get()->MeasureString(output.c_str());
    XMFLOAT4 tsize;
    XMStoreFloat4(&tsize, size);
    center.x = (tsize.x / 2.f);
    center.y = (tsize.y / 2.f);
}

void ADUI::Label2D::SetText(std::string _message, XMFLOAT2 _position, float _scale, XMVECTOR _tint, float rotation, XMFLOAT2 origin, UINT effects, float depth)
{
    output = _message;
    //position.x = _position.x * Settings::resScalingWidth;
    //position.y = _position.y * Settings::resScalingHeight;
    //center.x = (corners.right - corners.left) / 2.f;
    //center.y = (corners.bottom - corners.top) / 2.f;
    SetPosition(_position);
}

void ADUI::Label2D::SetPosition(XMFLOAT2 _position)
{
    XMVECTOR size = text->spriteFont.get()->MeasureString(output.c_str());
    XMFLOAT4 tsize;
    XMStoreFloat4(&tsize, size);


    position.x = _position.x * Settings::posScalingWidth;
    position.y = _position.y * Settings::posScalingHeight;

    //position.x = _position.x;
    //position.y = _position.y;

    center.x = (tsize.x / 2.f);
    center.y = (tsize.y / 2.f);
}

void ADUI::Label2D::ResetPosition(XMFLOAT2 _position)
{
    XMVECTOR size = text->spriteFont.get()->MeasureString(output.c_str());
    XMFLOAT4 tsize;
    XMStoreFloat4(&tsize, size);


    position.x = _position.x;// *Settings::posScalingWidth;
    position.y = _position.y;// *Settings::posScalingHeight;

    //position.x = _position.x;
    //position.y = _position.y;

    center.x = (tsize.x / 2.f);
    center.y = (tsize.y / 2.f);
}

void ADUI::Label2D::SetFont(Text2D* _spriteFont)
{
    text = _spriteFont;
}

void ADUI::Label2D::Render()
{
    if (visible)
        text->Render(output, position, tint, rotation, center);
}

bool ADUI::Label2D::ProcessResponse(UIMessage* _message)
{
    if (_message->targetID == UIMessageTargets::Component)
    {
        if (_message->commandID < ComponentOptions::ComponentLast)
        {
            return UIComponent::ProcessResponse(_message);
        }
        switch (_message->commandID)
        {
        case ImageResponses::SetTint:
        {
            XMVECTOR temp = DirectX::XMVectorSet(_message->fvalue.x, _message->fvalue.y, _message->fvalue.z, _message->fvalue.w);
            SetTint(temp);
            break;
        }
        case ImageResponses::SetRotation:
        {
            SetRotation(_message->fvalue.x);
            break;
        }
        case ImageResponses::ChangeRotation:
        {
            SetRotation(rotation + _message->fvalue.x);
            break;
        }
        case ImageResponses::SetEffects:
        {
            if (_message->ivalue > -1)
                SetEffects(SpriteEffects(_message->ivalue));
            break;
        }
        default:
            break;
        }
        return true;
    }
    return false;
}

void ADUI::Label2D::SetTint(XMVECTOR _tint)
{
    tint = _tint;
}

void ADUI::Label2D::SetRotation(float _rotation, bool _continuous)
{
    rotation = _rotation;
}

void ADUI::Label2D::SetEffects(SpriteEffects _effects)
{
    effects = _effects;
}

void ADUI::Label2D::SetDepth(float _depth)
{
    depth = _depth;
}

ADUI::UILog::UILog()
{
}


void ADUI::UILog::InitializeLog(SpriteBatch* _spriteBatch, ID3D11ShaderResourceView* _texture) //float textXOffset, float textYOffset, float screenWidth, float screenHeight, QuadData _quad)
{
    blackBox = new Image2D(_spriteBatch, _texture, { 0,0,0,0 }); //0.35f * screenWidth, -0.3f * screenHeight, 400, 250, 0.1046f, 0.2083f, 0.8645f, 0.9876f};
    blackBox->visible = true;
    blackBox->SetStretched(true);
}

void ADUI::UILog::Setup(Image2D* _background, Label2D* _label)
{
    blackBox = _background;
    position.x = _background->GetPosFloats().x;
    position.y = _background->GetPosFloats().y;
    center.x = _background->GetWidth() / 2.f;
    center.y = _background->GetHeight() / 2.f;
    visible = false;
    active = true;
    controlFocus = false;
    consoleLabel = _label;
    consoleLabel->visible = true;
    consoleLabel->ResetPosition({ (position.x + 5), (position.y + 5) });

}

void ADUI::UILog::SetFont(Text2D* _fontSmall)
{
    consoleLabel->SetFont(_fontSmall);
}

void ADUI::UILog::SetRects(XMFLOAT4 _posRect, RECT _uvRect)
{
    AnimationData* bbd = new AnimationData[1];
    bbd[0] = { 0, 1, 1 };
    blackBox->SetCorners(_posRect);
    blackBox->BuildAnimation(_uvRect, 1, 1, bbd);
    consoleLabel->SetText("0", XMFLOAT2(_posRect.x + 5, _posRect.y + 5));
    consoleLabel->visible = true;
}

ADUI::UIMessage* ADUI::UILog::ProcessInput()
{
    return nullptr;
}

void ADUI::UILog::WriteToLog(std::string message)
{
    messageQueue.push_back(message);

    consoleLabel->SetText("");
    for (int i = 0; i < 15; ++i)
    {
        int index = messageQueue.size() - 15 + i;
        if (index >= 0)
        {
            consoleLabel->output.append(messageQueue[index]);
            consoleLabel->output.append("\n");
        }
    }
}

ADUI::ADUI::ADUI()
{
}

ADUI::ADUI::~ADUI()
{
}

void ADUI::ADUI::Initialize(ComPtr<ID3D11Device1> _device, ComPtr<ID3D11DeviceContext1> _context, ComPtr<ID3D11RenderTargetView> _rtv, D3D11_VIEWPORT* _vp)
{
    device = _device;
    context = _context;
    rtv = _rtv;
    viewport = _vp;
    spriteBatch = std::make_unique<SpriteBatch>(context.Get());
    spriteBatch->SetViewport(*_vp);

    responseTime = 0.2f;
    inputTimer = 0;

    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsDesc.DepthEnable = false;
    device->CreateDepthStencilState(&dsDesc, &uiResources.depthStencilState);

    m_states = std::make_unique<CommonStates>(device.Get());

}

void ADUI::ADUI::AddTextureArray(char** _fileNameArray, UINT _count)
{
    uiResources.textureCount = _count;
    uiResources.uiTextures = new ID3D11ShaderResourceView * [_count];
    for (int i = 0; i < _count; ++i)
    {
        ADUtils::LoadUITextures(_fileNameArray[i], &uiResources.uiTextures[i], device);
    }
}

void ADUI::ADUI::AddFontArray(char** _fileNameArray, UINT _count)
{
    fontCount = _count;
    spriteFonts = new Text2D[_count];
    for (int i = 0; i < _count; ++i)
    {
        spriteFonts[i].Initialize(_fileNameArray[i], device.Get(), spriteBatch.get());
    }
}

UINT ADUI::ADUI::AddUIComponent(std::string _name, UIComponent* _component)
{
    unsigned int index = uiComponents.size();
    _component->name = _name;
    uiComponents.push_back(_component);
    return index;
}

UINT ADUI::ADUI::AddNewOverlay(std::string _name, bool _visible, bool _active)
{
    unsigned int index = overlays.size();
    Overlay2D* temp = new Overlay2D(_visible, _active);
    temp->name = _name;
    overlays.push_back(temp);
    return index;
}

UINT ADUI::ADUI::AddUIController(std::string _name, OverlayController* _controller)
{
    unsigned int index = uiControllers.size();
    _controller->name = _name;
    uiControllers.push_back(_controller);
    return index;
}

void ADUI::ADUI::Update(float delta_time)
{
    if (inputTimer < responseTime)
    {
        inputTimer += delta_time;
    }
    else
    {
        for (int control = 0; control < uiControllers.size(); ++control)
        {
            if (uiControllers[control]->active)
            {
                float quick = 0;
                for (int cComps = 0; cComps < uiControllers[control]->GetComponentCount(); ++cComps)
                {
                    if (uiControllers[control]->ProcessResponse(uiControllers[control]->GetComponent(cComps)->ProcessInput(), quick))
                    {
                        inputTimer = quick;
                        return;
                    }
                }
                if (uiControllers[control]->ProcessInput(delta_time, quick))
                {
                    inputTimer = quick;
                    return;
                }
            }
        }
    }
    for (int overly = 0; overly < overlays.size(); ++overly)
    {
        if (overlays[overly]->visible)
        {
            for (int control = 0; control < overlays[overly]->components.size(); ++control)
            {
                overlays[overly]->components[control]->Update(delta_time);
            }
        }
    }
}

void ADUI::ADUI::Render()
{
    spriteBatch->SetViewport(*viewport);
    spriteBatch->Begin(SpriteSortMode_Deferred, m_states.get()->NonPremultiplied(), nullptr, uiResources.depthStencilState.Get());


    for (int overly = 0; overly < overlays.size(); ++overly)
    {
        if (overlays[overly]->visible)
        {
            for (int i = 0; i < overlays[overly]->components.size(); ++i)
            {
                overlays[overly]->components[i]->Render();
            }
        }
    }

    spriteBatch->End();
    context->OMSetDepthStencilState(nullptr, 1);
    context->OMSetBlendState(nullptr, Colors::Black, 0xFFFFFFFF);
}

void ADUI::ADUI::ShutDown()
{

    delete[] spriteFonts;
    for (int i = 0; i < uiComponents.size(); ++i)
    {
        if (uiComponents[i])
            delete uiComponents[i];
    }
    for (int i = 0; i < uiResources.textureCount; ++i)
    {
        if (uiResources.uiTextures[i])
            uiResources.uiTextures[i]->Release();
    }
}

void ADUI::UILog::Render()
{
    blackBox->Render();
    consoleLabel->Render();
}

void ADUI::ADUI::RecieveMessage(UIMessage* _message)
{
    _message->externalMsg = true;
    float respondFaster;
    uiControllers[_message->commandID]->ProcessResponse(_message, respondFaster);
}


UINT* ADUI::ADUI::GetUIState()
{
    return &uiState;
}

void ADUI::ADUI::GetUIState(UINT _newState)
{
    if (_newState < UISTATE::COUNT)
        uiState = _newState;
}

ADUI::UILog* ADUI::ADUI::GetLog()
{
    return &uiLog;
}

ADUI::Text2D* ADUI::ADUI::GetFont(UINT _id)
{
    if (_id >= fontCount)
        _id = fontCount - 1;
    return &spriteFonts[_id];
}

ADUI::ADUI* ADUI::MessageReceiver::userInterface = nullptr;

void ADUI::MessageReceiver::SetUI(ADUI* _userInterface)
{
    userInterface = _userInterface;
}

void ADUI::MessageReceiver::Log(std::string _message)
{
    if (userInterface)
        userInterface->GetLog()->WriteToLog(_message);
}

void ADUI::MessageReceiver::SendMessage(UIMessage* _message)
{
    if (userInterface)
    {
        userInterface->RecieveMessage(_message);
    }
}

ADUI::UIComponentSelector::UIComponentSelector(UINT _componentCount, UIComponent** _components, bool _visible, bool _active, bool _controlFocus)
{
    componentCount = _componentCount;
    components = _components;
    visible = _visible;
    active = _active;
    controlFocus = _controlFocus;
}


ADUI::UIMessage* ADUI::UIComponentSelector::ProcessInput()
{
    if (active)
    {
        UIMessage* message = nullptr;
        for (int i = 0; i < reactionCount; ++i)
        {
            message = reactions[i]->ProcessInput();
            if (message != nullptr)
            {
                if (!ProcessResponse(message))
                {
                    return message;
                }
            }
        }
        message = components[currentComponent]->ProcessInput();
        if (message != nullptr)
        {
            if (!ProcessResponse(message))
            {
                return message;
            }
        }
    }

    return nullptr;
}

bool ADUI::UIComponentSelector::ProcessResponse(UIMessage* _message)
{
    if (_message)
    {
        if (_message->targetID == UIMessageTargets::FirstSelector)
        {
            switch (_message->commandID)
            {
            case SelectorResponses::Zero:
            {
                currentComponent = 0;
                break;
            }
            case SelectorResponses::IncreaseByInt:
            {
                if (currentComponent + _message->ivalue < componentCount)
                    currentComponent += _message->ivalue;
                break;
            }
            case SelectorResponses::DecreaseByInt:
            {
                if (currentComponent - _message->ivalue >= 0)
                    currentComponent -= _message->ivalue;
                break;
            }
            case SelectorResponses::Max:
            {
                currentComponent = componentCount - 1;
                break;
            }
            default:
                break;
            }
            return true;
        }
    }
    return false;
}

void ADUI::UIComponentSelector::Update(float delta_time)
{
    components[currentComponent]->Update(delta_time);
}

void ADUI::UIComponentSelector::Render()
{
    components[currentComponent]->Render();
}

void ADUI::UIComponentSelector::SetText(std::string _text, float _textXOffset, float _textYOffset, bool _visible)
{
}

void ADUI::UIComponentSelector::SetText(TextLabel _label, float _textXOffset, float _textYOffset, bool _visible)
{
}

void ADUI::UIComponentSelector::SetFont(Text2D* _spriteFont)
{
}

SpriteFont* ADUI::UIComponentSelector::GetFont()
{
    return nullptr;
}

ADUI::Text2D* ADUI::UIComponentSelector::GetText2D()
{
    return buttonLabel.GetText2D();
}

void ADUI::UIComponentSelector::SetCorners(XMFLOAT4 _pos)
{
    for (int i = 0; i < componentCount; ++i)
    {
        components[i]->SetCorners(_pos);
    }

    XMVECTOR result = buttonLabel.GetFont()->MeasureString(buttonLabel.output.c_str());
    //buttonLabel.SetPosition(CenterOfImage(result, image));
}

RECT ADUI::UIComponentSelector::GetPosRect()
{
    return RECT();
}

float ADUI::UIComponentSelector::GetWidth()
{
    return 0.0f;
}

float ADUI::UIComponentSelector::GetHeight()
{
    return 0.0f;
}

UINT ADUI::UIComponentSelector::GetIndex()
{
    return currentComponent;
}

ADUI::UIComponent* ADUI::UIComponentSelector::GetComponent(UINT _index)
{
    if (_index < componentCount)
        return components[_index];
    return nullptr;
}

ADUI::ComponentGrid::ComponentGrid(float _x, float _y, float _spacing, UINT _columns, UINT _maxRows, bool _active, bool _visible, bool _controlFocus)
{
    topLeft.x = _x;
    topLeft.y = _y;
    spacing = _spacing;
    columns = _columns;
    maxRows = _maxRows;
    active = _active;
    visible = _visible;
    controlFocus = _controlFocus;
    selectedButtonIndex = 0;
}

void ADUI::ComponentGrid::AddComponent(UIComponent* _component)
{
    components.push_back(_component->clone());
}

void ADUI::ComponentGrid::RecalculatePositions()
{
    int XSpacing = 0;
    int YSpacing = 0;
    float width = 0;
    float height = 0;

    switch (spacingStyle)
    {
    case ComponentGridStyle::ExactSpacing:
        width = positions[0].right - positions[0].left;
        height = positions[0].bottom - positions[0].top;

        break;
    case ComponentGridStyle::FirstComponent:
        width = components[0].get()->GetWidth();
        height = components[0].get()->GetHeight();

        break;
        //case ComponentGridStyle::LargestInRowColumn:
        //I'd have to check each row and then store the size of the position for that row.
        //    break;
    default:
        break;
    }

    for (int i = 0; i < components.size(); ++i)
    {
        XSpacing = (width + spacing) * (i % columns);
        YSpacing = (height + spacing) * (i / columns);
        components[i]->SetCorners({ float(positions[0].left + XSpacing), float(positions[0].top + YSpacing),  float(positions[0].left + XSpacing + width), float(positions[0].top + YSpacing + height) });

    }
}
//
//void ADUI::ComponentGrid::Generate(SpriteBatch* _spriteBatch, ID3D11ShaderResourceView* _texture, RECT _position, XMFLOAT2 _spacing, UINT _btnCount, UINT _columns, Image2D* _button, std::string* _textArray, bool _active, bool _isFocus)
//{
//    x = _position.left;
//    y = _position.top;
//    spacing = _spacing.x;
//    columns = _columns;
//    active = _active;
//    controlFocus = _isFocus;
//
//    Image2D* button;
//    Image2D* image;
//    Image2D* imageS;
//
//    for (int i = 0; i < _btnCount; ++i)
//    {
//        /* image = new Image2D(_spriteBatch, _texture);
//         image->SetRects(_position, *button->GetuvRect(), (button->GetOrigin()));
//         imageS = new Image2D(_spriteBatch, _texture, button->GetuvRect(), button->animationData);
//         Image2D* button = new Image2D(spriteBatch, _texture);
//         TextLabel tempLabel = { true, XMFLOAT2(0, 0), _textArray[i] };
//         button->SetText(tempLabel, 0, 0);
//         button->SetFont(_button->GetText2D());
//         buttons.push_back(button);*/
//    }
//
//    //Initialize();
//}

void ADUI::ComponentGrid::Generate(UIComponent* _comp, XMFLOAT4 _position, XMFLOAT2 _spacing, UINT _count, UINT _columns, ComponentGridStyle _spacingStyle, bool _active, bool _isFocus)
{
    positions = new RECT[_count];
    positions[0] = { long(_position.x), long(_position.y), long(_position.z), long(_position.w) };
    spacing = _spacing.x;
    columns = _columns;
    active = _active;
    controlFocus = _isFocus;
    for (int i = 0; i < _count; ++i)
    {
        components.push_back(_comp->clone());
    }
    Initialize();
}

void ADUI::ComponentGrid::Initialize()
{
    components[selectedButtonIndex]->Focus();
    RecalculatePositions();
}

ADUI::UIMessage* ADUI::ComponentGrid::ProcessInput()
{
    if (active)
    {
        UIMessage* resp = nullptr;
        if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) > 0)
        {
            if ((selectedButtonIndex - columns) >= 0)
            {
                components[selectedButtonIndex]->Unfocus();
                selectedButtonIndex -= columns;
                components[selectedButtonIndex]->Focus();
                resp = new UIMessage();
                resp->targetID = UIMessageTargets::Controller;
                resp->commandID = 0;
                resp->componentIndex = selectedButtonIndex;
            }
        }
        else if (Input::QueryThumbStickUpDownY(Input::THUMBSTICKS::LEFT_THUMBSTICK) < 0)
        {
            if ((selectedButtonIndex + columns) < components.size())
            {
                components[selectedButtonIndex]->Unfocus();
                selectedButtonIndex += columns;
                components[selectedButtonIndex]->Focus();
                resp = new UIMessage();
                resp->targetID = UIMessageTargets::Controller;
                resp->commandID = 0;
                resp->componentIndex = selectedButtonIndex;
            }
        }
        if (Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) > 0)
        {
            if (selectedButtonIndex < components.size() - 1 && (selectedButtonIndex + 1) % columns != 0)
            {
                components[selectedButtonIndex]->Unfocus();
                ++selectedButtonIndex;
                components[selectedButtonIndex]->Focus();
                resp = new UIMessage();
                resp->targetID = UIMessageTargets::Controller;
                resp->commandID = 0;
                resp->componentIndex = selectedButtonIndex;
            }
        }
        else if (Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) < 0)
        {
            if (selectedButtonIndex > 0 && (selectedButtonIndex % columns) != 0)
            {
                components[selectedButtonIndex]->Unfocus();
                --selectedButtonIndex;
                components[selectedButtonIndex]->Focus();
                resp = new UIMessage();
                resp->targetID = UIMessageTargets::Controller;
                resp->commandID = 0;
                resp->componentIndex = selectedButtonIndex;
            }
        }
        if (resp)
        {
            return resp;
        }
        UIMessage* btnvalue = components[selectedButtonIndex]->ProcessInput();
        if (btnvalue)
        {
            if (btnvalue->commandID == 1)
            {
                btnvalue->componentIndex = selectedButtonIndex;
            }
        }
        return btnvalue;
    }
    return nullptr;
}

void ADUI::ComponentGrid::Update(float delta_time)
{
    for (int i = 0; i < components.size(); ++i)
    {
        components[i]->Update(delta_time);
    }
}

void ADUI::ComponentGrid::Render()
{
    if (visible)
    {
        for (int i = 0; i < components.size(); ++i)
        {
            components[i]->Render();
        }
    }
}

void ADUI::ComponentGrid::Enable()
{
    visible = true;
    active = true;
    components[selectedButtonIndex]->Focus();
}

void ADUI::ComponentGrid::Disable()
{
    visible = false;
    active = false;
    components[selectedButtonIndex]->Unfocus();
}

UINT ADUI::ComponentGrid::GetIndex()
{
    return selectedButtonIndex;
}


float ADUI::Settings::screenWidth;
float ADUI::Settings::screenHeight;
float ADUI::Settings::resolutionWidth;
float ADUI::Settings::resolutionHeight;
float ADUI::Settings::planningWidth;
float ADUI::Settings::planningHeight;
float ADUI::Settings::posScalingWidth;
float ADUI::Settings::posScalingHeight;
float ADUI::Settings::resScalingWidth;
float ADUI::Settings::resScalingHeight;


ADUI::SliderBar::SliderBar(Image2D* _slide, Slider* _slider, Image2D* _leftCap, Image2D* _rightCap, Label2D* _label, float _minValue, float _maxValue, float _currentValue, float _incriment)
{
    slider = _slider;
    slide = _slide;
    leftCap = _leftCap;
    rightCap = _rightCap;
    label = _label;
    minValue = _minValue;
    maxValue = _maxValue;
    currentRatio = _currentValue;
    incriment = _incriment;

    //align it all
    //XMFLOAT2 pos = slide->GetPosFloats();

}


ADUI::UIMessage* ADUI::SliderBar::ProcessInput()
{
    if (active)
    {
        UIMessage* resp = nullptr;
        if (Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) > 0)
        {
            if (currentRatio < maxValue)
            {
                currentRatio += incriment;
                slider->SetValue(currentRatio);
                leftCap->Focus();
                rightCap->Focus();
            }
            resp = new UIMessage();
            resp->targetID = UIMessageTargets::Controller;
            resp->commandID = 1;
            resp->fvalue.x = currentRatio;
            return resp;
        }
        else if (Input::QueryThumbStickLeftRightX(Input::THUMBSTICKS::LEFT_THUMBSTICK) < 0)
        {
            if (currentRatio > 0)
            {
                currentRatio -= incriment;
                slider->SetValue(currentRatio);
                leftCap->Focus();
                rightCap->Unfocus();
            }
            resp = new UIMessage();
            resp->targetID = UIMessageTargets::Controller;
            resp->commandID = 1;
            resp->fvalue.x = currentRatio;
            return resp;
        }
    }
    leftCap->Unfocus();
    rightCap->Unfocus();
    return nullptr;
}

void ADUI::SliderBar::Update(float delta_time)
{
    slider->SetValue(currentRatio);
    slider->Update(delta_time);
    leftCap->Update(delta_time);
    rightCap->Update(delta_time);
    slide->Update(delta_time);
    label->Update(delta_time);
}

void ADUI::SliderBar::Render()
{
    slide->Render();
    leftCap->Render();
    rightCap->Render();
    slider->Render();
    label->Render();
}

ADUI::Slider::Slider(Image2D* _sliderImage, XMFLOAT2 _position, XMFLOAT2 _maxPosition, XMFLOAT2 _minPosition)
{
    slider = _sliderImage;
    position = _position;
    maxPosition = _maxPosition;
    minPosition = _minPosition;
}

ADUI::Slider::~Slider()
{
}

void ADUI::Slider::SetValue(float _ratio)
{
    position.x = minPosition.x + (_ratio * (maxPosition.x - minPosition.x));
    position.y = minPosition.y + (_ratio * (maxPosition.y - minPosition.y));
    XMFLOAT2 scaledPosition;
    scaledPosition.x = position.x;// *Settings::posScalingWidth;
    scaledPosition.y = position.y;// *Settings::posScalingHeight;
    slider->SetPosition(scaledPosition);
}

void ADUI::Slider::Update(float delta_time)
{
    slider->Update(delta_time);
}

void ADUI::Slider::Render()
{
    slider->Render();
}

ADUI::UIComponent::~UIComponent() = default;

ADUI::UIMessage* ADUI::UIComponent::ProcessInput()
{
    if (active)
    {
        for (int i = 0; i < reactionCount; ++i)
        {
            UIMessage* message = nullptr;
            message = reactions[i]->ProcessInput();
            if (message != nullptr)
            {
                if (!ProcessResponse(message))
                {
                    return message;
                }
            }
        }
    }
    return nullptr;
}


ADUI::Button2D::Button2D(XMFLOAT4 _position, Image2D * _image, Label2D * _buttonLabel, bool _visible, bool _active, bool _controlFocus)
{
    image = _image;
    position.x = image->GetPosFloats().x;
    position.y = image->GetPosFloats().y;
    center.x = image->GetWidth() / 2.f;
    center.y = image->GetHeight() / 2.f;
    visible = _visible;
    active = _active;
    controlFocus = _controlFocus;
    buttonLabel = _buttonLabel;
    buttonLabel->visible = visible;
    buttonLabel->ResetPosition({ (position.x + center.x), (position.y + center.y) });
}

ADUI::Button2D::~Button2D()
{
    delete image;
}

ADUI::Button2D::Button2D(const Button2D & p2)
{
    {
        image = new Image2D(*p2.image);
        buttonLabel = new Label2D(*p2.buttonLabel);
    }
}

ADUI::UIMessage* ADUI::Button2D::ProcessInput()
{
    if (Input::QueryButtonDown(GamepadButtons::A))
    {
        UIMessage* resp = new UIMessage();
        resp->targetID = Controller;
        resp->commandID = 1;
        return resp;
    }
    return nullptr;
}

void ADUI::Button2D::Update(float delta_time)
{
    image->Update(delta_time);
    buttonLabel->Update(delta_time);
}

void ADUI::Button2D::Render()
{
    image->Render();
    buttonLabel->Render();
}

void ADUI::Button2D::SetCorners(XMFLOAT4 _pos)
{
    image->SetCorners(_pos);
    position.x = image->GetPosFloats().x;
    position.y = image->GetPosFloats().y;
    center.x = image->GetWidth() / 2.f;
    center.y = image->GetHeight() / 2.f;
    buttonLabel->ResetPosition({ position.x + center.x, position.y + center.y });

}

void ADUI::Button2D::SetText(std::string _message)
{
    buttonLabel->SetText(_message);
}


ADUI::Label2D* ADUI::Button2D::GetText()
{
    return buttonLabel;
}

void ADUI::Button2D::Focus()
{
    controlFocus = true;
    image->Focus();
    buttonLabel->Focus();
}

void ADUI::Button2D::Unfocus()
{
    controlFocus = false;
    image->Unfocus();
    buttonLabel->Unfocus();
}


