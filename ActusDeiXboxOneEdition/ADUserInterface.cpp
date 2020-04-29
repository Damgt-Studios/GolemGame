#include "pch.h"
#include "ADUserInterface.h"

void ADUI::Text2D::Initialize(ID3D11Device1* _device, SpriteBatch* _spriteBatch)
{
    spriteBatch = _spriteBatch;
    spriteFont = std::make_unique<SpriteFont>(_device, ADUtils::GetTexturePath("SpyroFont.spritefont").c_str());
}

void ADUI::Text2D::Render(ADResource::AD_UI::TextLabel* label)
{
    spriteFont->DrawString(spriteBatch, label->output.c_str(), label->position);
}

ADUI::Image2D::Image2D()
{
    visible = true;
    active = false;
    textureID = 0;
    quadCount = 1;
    frameCount = 1;
    currentFrame = 0;
    updateThreshold = 100.f;
    updateTimer = 0.f;
}

ADUI::Image2D::Image2D(ADResource::AD_UI::QuadData _quad, int _textureID)
{
    visible = true;
    active = false;
    textureID = _textureID;
    quadCount = 1;
    frameCount = 1;
    currentFrame = 0;
    updateThreshold = 100.f;
    updateTimer = 0.f;
    quad = new ADResource::AD_UI::QuadData(_quad);
}

ADUI::Image2D::Image2D(ADResource::AD_UI::QuadData _quad, ADResource::AD_UI::AnimData2d _animation, int _textureID)
{
    visible = true;
    active = true;
    textureID = _textureID;
    quadCount = 1;
    updateTimer = 0.f;
    frameCount = _animation.frameCount;
    currentFrame = _animation.startFrame;
    quad = new ADResource::AD_UI::QuadData[frameCount];
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
}

ADUI::Image2D::~Image2D()
{
    for (int i = 0; i < frameCount; ++i)
    {
        delete& quad[i];
    }
    delete[] quad;
}

void ADUI::Image2D::Enable()
{
    visible = true;
    requiresRefresh = true;
}

void ADUI::Image2D::Disable()
{
    visible = false;
    requiresRefresh = true;
}

void ADUI::Image2D::Update(float delta_time)
{
    if (active && visible)
    {
        if (updateTimer < updateThreshold)
        {
            updateTimer += delta_time;
        }
        else if (frameCount > 1)
        {
            updateTimer = 0.f;
            currentFrame++;
            if (currentFrame >= frameCount)
            {
                currentFrame = 0;
            }
            requiresRefresh = true;
        }
    }
}

ADResource::AD_UI::QuadData* ADUI::Image2D::GetQuad()
{
    return &quad[currentFrame];
}


void ADUI::UILog::InitializeLog(float textXOffset, float textYOffset, float screenWidth, float screenHeight, ADResource::AD_UI::QuadData _quad)
{
    blackBox.quad = new ADResource::AD_UI::QuadData{ _quad }; //0.35f * screenWidth, -0.3f * screenHeight, 400, 250, 0.1046f, 0.2083f, 0.8645f, 0.9876f};
    blackBox.visible = true;
    consoleLabel = { true, componentPosToTextPos(XMFLOAT2(_quad.x + textXOffset, _quad.y + textYOffset), screenWidth, screenHeight), {"0"} };
}

ADResource::AD_UI::UIMessage* ADUI::UILog::ProcessInput()
{
    return nullptr;
}

void ADUI::UILog::WriteToLog(std::string message)
{
    messageQueue.push_back(message);

    consoleLabel.output = "";
    for (int i = 0; i < 15; ++i)
    {
        int index = messageQueue.size() - 15 + i;
        if (index >= 0)
        {
            consoleLabel.output.append(messageQueue[index]);
            consoleLabel.output.append("\n");
        }
    }
}

ADResource::AD_UI::QuadData* ADUI::UILog::GetQuad()
{
    return blackBox.GetQuad();
}

ADResource::AD_UI::TextLabel* ADUI::UILog::GetText()
{
    return &consoleLabel;
}


ADUI::Button2D::Button2D(Image2D* _image, Image2D* _imageOnFocus, bool _visible, bool _active, bool _controlFocus)
{
    image = _image;
    imageOnFocus = _imageOnFocus;
    visible = _visible;
    active = _active;
    controlFocus = _controlFocus;
    quadCount = 1;
}

ADUI::Button2D::~Button2D()
{
    delete image;
    delete imageOnFocus;
}

ADResource::AD_UI::UIMessage* ADUI::Button2D::ProcessInput()
{
    if (Input::QueryButtonDown(GamepadButtons::A))
    {
        ADResource::AD_UI::UIMessage* resp = new ADResource::AD_UI::UIMessage();
        resp->messageType = UIMessageTypes::FromButton;
        resp->controllerID = 0;
        return resp;
    }
    return nullptr;
}

void ADUI::Button2D::Update(float delta_time)
{
    if (controlFocus)
    {
        imageOnFocus->Update(delta_time);
        requiresRefresh = imageOnFocus->requiresRefresh;
    }
    else
    {
        image->Update(delta_time);
        requiresRefresh = image->requiresRefresh;
    }
}

ADResource::AD_UI::QuadData* ADUI::Button2D::GetQuad()
{
    if (visible)
    {
        if (controlFocus)
            return imageOnFocus->GetQuad();
        else
            return image->GetQuad();
    }
    return nullptr;
}


void ADUI::Button2D::SetText(char* _text, float _textXOffset, float _textYOffset, float screenWidth, float screenHeight, bool visible)
{
    textXOffset = _textXOffset;
    textYOffset = _textYOffset;
    buttonLabel = { visible, componentPosToTextPos(XMFLOAT2(image->quad->x + textXOffset, image->quad->y + textYOffset), screenWidth, screenHeight), {_text} };
}

void ADUI::Button2D::ResetText(float _originalX, float _originalY, float screenWidth, float screenHeight, bool visible)
{
    buttonLabel = { visible, componentPosToTextPos(XMFLOAT2(_originalX + textXOffset, _originalY + textYOffset), screenWidth, screenHeight), buttonLabel.output };
}

ADResource::AD_UI::TextLabel* ADUI::Button2D::GetText()
{
    return &buttonLabel;
}

void ADUI::Button2D::Refresh()
{
    requiresRefresh = true;
    image->requiresRefresh = true;
    imageOnFocus->requiresRefresh = true;
}


ADUI::ButtonList::ButtonList(float _screenWidth, float _screenHeight, float _x, float _y, float _spacing, UINT _columns, UINT _maxRows, bool _active, bool _visible, bool _controlFocus)
{
    screenWidth = _screenWidth;
    screenHeight = _screenHeight;
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
}

ADUI::ButtonList::~ButtonList()
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        if (buttons[i])
            delete buttons[i];
    }
    //The quads we are pointing to are stored in the images and they will delete their own quads.
    delete[] myQuads;
}

void ADUI::ButtonList::AddButton(Button2D* _button)
{
    buttons.push_back(_button);
    quadCount++;
}

void ADUI::ButtonList::RecalculatePositions()
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

void ADUI::ButtonList::RepositionText()
{
    labelCount = 0;
    for (int i = 0; i < buttons.size(); ++i)
    {
        ADResource::AD_UI::TextLabel* label = buttons[i]->GetText();
        if (label)
        {
            labelCount++;
            buttons[i]->ResetText(myQuads[i]->x, myQuads[i]->y, screenWidth, screenHeight);
        }
    }

    buttonLabel = new ADResource::AD_UI::TextLabel * [labelCount];

    for (int i = 0; i < buttons.size(); ++i)
    {
        ADResource::AD_UI::TextLabel* label = buttons[i]->GetText();
        if (label)
        {
            buttonLabel[i] = buttons[i]->GetText();
        }
    }

    //Figure out how many buttons have text, can't be done here though becasue we need to return that first
    //Return an array of Text containing children text after previously returning count.
}

void ADUI::ButtonList::Initialize()
{
    buttons[selectedButtonIndex]->controlFocus = true;  //This needs to move to Enable //Disable, etc..
    quadCount = buttons.size();
    myQuads = new ADResource::AD_UI::QuadData * [quadCount]();

    for (int i = 0; i < buttons.size(); ++i)
    {
        myQuads[i] = buttons[i]->GetQuad();
    }
    RecalculatePositions();
    RepositionText();
}

void ADUI::ButtonList::Update(float delta_time)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        buttons[i]->Update(delta_time);
        if (buttons[i]->requiresRefresh)
            requiresRefresh = true;
    }
}

ADResource::AD_UI::UIMessage* ADUI::ButtonList::ProcessInput()
{
    if (Input::QueryButtonDown(GamepadButtons::DPadUp))
    {
        if (selectedButtonIndex != 0)
        {
            buttons[selectedButtonIndex]->controlFocus = false;
            buttons[selectedButtonIndex]->Refresh();
            selectedButtonIndex--;
            buttons[selectedButtonIndex]->controlFocus = true;
            buttons[selectedButtonIndex]->Refresh();
            requiresRefresh = true;
        }
        ADResource::AD_UI::UIMessage* resp = new ADResource::AD_UI::UIMessage();
        resp->messageType = UIMessageTypes::Ignore;
        resp->controllerID = 0;
        return resp;
    }
    if (Input::QueryButtonDown(GamepadButtons::DPadDown))
    {
        if (selectedButtonIndex < buttons.size() - 1)
        {
            buttons[selectedButtonIndex]->controlFocus = false;
            buttons[selectedButtonIndex]->Refresh();
            selectedButtonIndex++;
            buttons[selectedButtonIndex]->controlFocus = true;
            buttons[selectedButtonIndex]->Refresh();
            requiresRefresh = true;
        }
        ADResource::AD_UI::UIMessage* resp = new ADResource::AD_UI::UIMessage();
        resp->messageType = UIMessageTypes::Ignore;
        resp->controllerID = 0;
        return resp;
    }
    ADResource::AD_UI::UIMessage* btnvalue = buttons[selectedButtonIndex]->ProcessInput();
    if (btnvalue)
        btnvalue->number = selectedButtonIndex;
    return btnvalue;
}

ADResource::AD_UI::QuadData** ADUI::ButtonList::GetQuads()
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        myQuads[i] = buttons[i]->GetQuad();
    }
    RecalculatePositions();
    RepositionText();
    return myQuads;
}

ADResource::AD_UI::TextLabel** ADUI::ButtonList::GetTexts()
{
    //Figure out how many buttons have text, can't be done here though becasue we need to return that first
    //Return an array of Text containing children text after previously returning count.

    return buttonLabel;
}

ADUI::OverlayController::~OverlayController()
{
}

bool ADUI::OverlayController::ProcessInput(float delta_time)
{
    return false;
}

bool ADUI::OverlayController::ProcessResponse(ADResource::AD_UI::UIMessage* responseID)
{
    return false;
}

void ADUI::OverlayController::AddComponent(UINT _compID, ADResource::AD_UI::UIComponent* _comp)
{
    componentIDs.push_back(_compID);
    componentTypeMap.emplace(_compID, _comp);
}
//
//void AD_UI::OverlayController::AddComponentType(UINT _typeID, ADResource::AD_UI::UIComponent* _comp)
//{
//    componentTypeMap.emplace(_typeID, _comp);
//}

UINT ADUI::OverlayController::GetComponentCount()
{
    return componentIDs.size();
}

UINT* ADUI::OverlayController::GetComponentIDS()
{
    return componentIDs.data();
}

int ADUI::ADUI::AddNewOverlay(std::string _name, bool _visible, bool _active, bool _dynamic)
{
    unsigned int index = setup->overlays.size();
    ADResource::AD_UI::Overlay2D temp(index, _visible, _active, _dynamic);
    setup->overlaysNameToID[_name] = index;
    setup->overlays.push_back(temp);
    return index;
}

AD_ULONG ADUI::ADUI::AddUIComponent(std::string _name, ADResource::AD_UI::UIComponent* component)
{
    unsigned int index = setup->uiComponents.size();
    setup->componentsNameToID[_name] = index;
    setup->uiComponents.push_back(component);

    return index;
}

void ADUI::ADUI::AddUIController(std::string _name, OverlayController* _controller)
{
    unsigned int index = uiControllers.size();
    setup->controllersNameToID[_name] = index;
    uiControllers.push_back(_controller);
}

UINT ADUI::ADUI::GetUIState()
{
    return uiState;
}

XMFLOAT2 ADUI::ADUI::GetPosition(float _percentageX, float _percentageY, float _screenWidth, float _screeHeight)
{
    return { (_percentageX * _screenWidth), (_percentageY * _screeHeight) };
}

ADUI::UILog* ADUI::ADUI::GetLogComponent()
{
    return &uiLog;
}

void ADUI::ADUI::SetAltasHeader(ADResource::AD_UI::UIHeader* _header, UINT _headercount)
{
    header = _header;
    headerCount = _headercount;
}

void ADUI::ADUI::SetSetup(UISetup* _setup)
{
    setup = _setup;
}

void ADUI::ADUI::RefreshOverlay(ComPtr<ID3D11DeviceContext1> _context)
{
    for (int over = 0; over < setup->overlays.size(); ++over)
    {
        if (setup->overlays[over].IsDynamic() && setup->overlays[over].visible)
        {

            for (int i = 0; i < setup->overlays[over].componentIDs.size(); ++i)
            {
                UINT cid = setup->overlays[over].componentIDs[i];
                if (setup->uiComponents[cid]->requiresRefresh)
                {
                    if (setup->uiComponents[cid]->GetQuadCount() > 1)
                    {
                        ADResource::AD_UI::QuadData** qds = setup->uiComponents[cid]->GetQuads();
                        for (int addedQuads = 0; addedQuads < setup->uiComponents[cid]->GetQuadCount(); ++addedQuads)
                        {
                            ADResource::AD_UI::QuadData* qd = qds[addedQuads];
                            UpdateQuad(setup->overlays[over].startQuad[i] + addedQuads, *qd, setup->overlays[over].vertices);
                        }
                    }
                    else
                    {
                        ADResource::AD_UI::QuadData* qds = setup->uiComponents[cid]->GetQuad();
                        if (qds)
                            UpdateQuad(setup->overlays[over].startQuad[i], *qds, setup->overlays[over].vertices);
                    }
                    setup->uiComponents[cid]->requiresRefresh = false;
                }
            }

            //Update the Vertex buffers
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
            _context->Map(setup->overlays[over].vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            memcpy(mappedResource.pData, setup->overlays[over].vertices.data(), sizeof(ADResource::AD_UI::UIVertex) * setup->overlays[over].vertices.size());
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

void ADUI::ADUI::Initialize(ID3D11Device1* device, ID3D11DeviceContext1* _context)
{
    spriteBatch = new SpriteBatch(_context);
    //GameSideCode(640,480);
    context = _context;
    text.Initialize(device, spriteBatch);

    uiState = true;

    responseTime = 0.2f;
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

    ADUtils::LoadUITextures(header[0], uiResources.uiTextures, device);
    ADUtils::LoadUITextures(header[1], uiResources.uiTextures2, device);
    visible = true;

    for (int over = 0; over < setup->overlays.size(); ++over)
    {
        UINT currentQuad = 0;
        for (int i = 0; i < setup->overlays[over].componentIDs.size(); ++i)
        {
            setup->overlays[over].startQuad.push_back(currentQuad);
            UINT cid = setup->overlays[over].componentIDs[i];
            if (setup->uiComponents[cid]->GetQuadCount() > 1)
            {
                ADResource::AD_UI::QuadData** qds = setup->uiComponents[cid]->GetQuads();
                for (int addedQuads = 0; addedQuads < setup->uiComponents[cid]->GetQuadCount(); ++addedQuads)
                {
                    ADResource::AD_UI::QuadData* qd = qds[addedQuads];
                    CreateQuad(*qd, setup->overlays[over].vertices, setup->overlays[over].indices);
                    currentQuad++;
                }
            }
            else
            {
                ADResource::AD_UI::QuadData* qds = setup->uiComponents[cid]->GetQuad();
                if (qds)
                {
                    CreateQuad(*qds, setup->overlays[over].vertices, setup->overlays[over].indices);
                    currentQuad++;
                }
            }
        }

        //Create buffers
        // Setup Vertex Buffer
        D3D11_BUFFER_DESC bufferDesc;
        D3D11_SUBRESOURCE_DATA vdata;
        ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
        ZeroMemory(&vdata, sizeof(D3D11_SUBRESOURCE_DATA));
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.ByteWidth = sizeof(ADResource::AD_UI::UIVertex) * setup->overlays[over].vertices.size();
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
    device->CreateDepthStencilState(&dsDesc, &uiResources.depthStencilState);

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

void ADUI::ADUI::Render(ComPtr<ID3D11DeviceContext1> _context, ComPtr<ID3D11RenderTargetView> _rtv)
{
    RefreshOverlay(_context);
    ID3D11ShaderResourceView* resource_views[] = { uiResources.uiTextures.Get() , uiResources.uiTextures2.Get() };

    _context->PSSetShaderResources(0, 2, resource_views);
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

    spriteBatch->Begin();
    for (int overly = 0; overly < setup->overlays.size(); ++overly)
    {
        if (visible && setup->overlays[overly].visible)
        {
            UINT strides[] = { sizeof(ADResource::AD_UI::UIVertex) };
            UINT offsets[] = { 0 };
            ID3D11Buffer* uiVertexBuffers[] = { setup->overlays[overly].vertexBuffer.Get() };
            _context->IASetVertexBuffers(0, 1, uiVertexBuffers, strides, offsets);
            _context->IASetIndexBuffer(setup->overlays[overly].indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
            _context->DrawIndexed(setup->overlays[overly].indices.size(), 0, 0);

            for (int i = 0; i < setup->overlays[overly].componentIDs.size(); ++i)
            {
                UINT cid = setup->overlays[overly].componentIDs[i];
                if (setup->uiComponents[cid]->labelCount > 1)
                {
                    ADResource::AD_UI::TextLabel** label = setup->uiComponents[cid]->GetTexts();
                    for (int lbls = 0; lbls < setup->uiComponents[cid]->labelCount; ++lbls)
                    {
                        if (label[lbls])
                        {
                            text.Render(label[lbls]);
                        }
                    }
                }
                else
                {
                    ADResource::AD_UI::TextLabel* label = setup->uiComponents[cid]->GetText();
                    if (label)
                    {
                        text.Render(label);
                    }
                }

            }
        }
    }
    spriteBatch->End();
    //for (int label = 0; label < setup->textLabels.size(); ++label)
    //{
    //    if (setup->textLabels[label].visible)
    //        text.Render(&setup->textLabels[label]);
    //}

    _context->OMSetDepthStencilState(nullptr, 1);
}

void ADUI::ADUI::RecieveMessage(ADResource::AD_UI::UIMessage* _message)
{
    uiControllers[_message->controllerID]->ProcessResponse(_message);
}

void ADUI::ADUI::ShutDown()
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

ADUI::ADUI* ADUI::MessageReceiver::userInterface = nullptr;

void ADUI::MessageReceiver::SetUI(ADUI* _userInterface)
{
    userInterface = _userInterface;
}

void ADUI::MessageReceiver::Log(std::string _message)
{
    if (userInterface)
        userInterface->uiLog.WriteToLog(_message);
}

void ADUI::MessageReceiver::SendMessage(ADResource::AD_UI::UIMessage* _message)
{
    if (userInterface)
    {
        userInterface->RecieveMessage(_message);
    }
}

void ADUI::Label2D::SetText(std::string message)
{
    consoleLabel.output = message;
}

void ADUI::Label2D::SetText(ADResource::AD_UI::TextLabel message)
{
    consoleLabel = message;
}

ADResource::AD_UI::TextLabel* ADUI::Label2D::GetText()
{
    return &consoleLabel;
}
