#include "pch.h"
#include "Renderer.h"
#include "Utils.h"

// Shaders
#include "base_vs.csh"
#include "base_ps.csh"
// Shaders

bool ADRenderer::PBRRenderer::Initialize()
{
	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();

	// Create the device
	D3D_FEATURE_LEVEL XBOX11 = D3D_FEATURE_LEVEL_10_0;
	ComPtr<ID3D11Device> tdevice;
	ComPtr<ID3D11DeviceContext> tcontext;
	HRESULT result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		&XBOX11,
		1,
		D3D11_SDK_VERSION,
		&tdevice,
		nullptr,
		&tcontext
	);
	assert(!FAILED(result));
	// Convert the device and device context pointers to the 11.1 pointers
	result = tdevice.As(&pbr.device); assert(!FAILED(result));
	result = tcontext.As(&pbr.context); assert(!FAILED(result));

	// Grab the adapter to set up the swapchain
	ComPtr<IDXGIDevice1> dxgiDevice;
	result = pbr.device.As(&dxgiDevice); assert(!FAILED(result));

	ComPtr<IDXGIAdapter> dxgiAdapter;
	result = dxgiDevice->GetAdapter(&dxgiAdapter); assert(!FAILED(result));

	ComPtr<IDXGIFactory2> dxgiFactory;
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);
	assert(!FAILED(result));

	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	scd.SampleDesc.Count = 1;
	scd.Width = Window->Bounds.Width;
	scd.Height = Window->Bounds.Height;

	result = dxgiFactory->CreateSwapChainForCoreWindow(
		pbr.device.Get(),
		reinterpret_cast<IUnknown*>(Window),
		&scd,
		nullptr,
		&pbr.chain
	);
	assert(!FAILED(result));

	// Setup viewport
	pbr.viewport.Width = scd.Width;
	pbr.viewport.Height = scd.Height;
	pbr.viewport.TopLeftY = pbr.viewport.TopLeftX = 0;
	pbr.viewport.MinDepth = 0;
	pbr.viewport.MaxDepth = 1;

	// Create render target
	ComPtr<ID3D11Texture2D> backbuffer;
	result = pbr.chain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);
	assert(!FAILED(result));

	// Render target
	result = pbr.device->CreateRenderTargetView(backbuffer.Get(), nullptr, &pbr.render_target_view);
	assert(!FAILED(result));

	// Lights
	Light light;
	ZeroMemory(&light, sizeof(Light));
	light.lightType = (int)LIGHTTYPE::DIRECTIONAL;
	light.ambientUp = XMFLOAT4(1, 1, 1, 1);
	light.ambientDown = XMFLOAT4(1, 1, 1, 1);
	light.ambientIntensityDown = .2;
	light.ambientIntensityUp = .4;
	light.diffuse = XMFLOAT4(1, 1, 1, 1);
	light.lightDirection = XMFLOAT4(2, -1, 3, 1);
	light.diffuseIntensity = 1;
	light.specular = XMFLOAT4(1, 1, 1, 1);
	light.specularIntensity = .2;
	lights.push_back(light);
	// Lights

	// Rasterizer state
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FrontCounterClockwise = false;
	rdesc.DepthBiasClamp = 1;
	rdesc.DepthBias = rdesc.SlopeScaledDepthBias = 0;
	rdesc.DepthClipEnable = true;
	rdesc.FillMode = D3D11_FILL_SOLID;
	rdesc.CullMode = D3D11_CULL_BACK;
	rdesc.AntialiasedLineEnable = false;
	rdesc.MultisampleEnable = false;

	ComPtr<ID3D11RasterizerState> traster;
	result = pbr.device->CreateRasterizerState(&rdesc, &traster);
	assert(!FAILED(result));
	result = traster.As(&pbr.defaultRasterizerState);
	assert(!FAILED(result));

	pbr.context->RSSetState(traster.Get());
	// Rasterizer state

	// Set primitive topology
	pbr.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create constant buffer
	D3D11_BUFFER_DESC bDesc;
	D3D11_SUBRESOURCE_DATA subdata;
	ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.ByteWidth = sizeof(WVP);
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;

	result = pbr.device->CreateBuffer(&bDesc, nullptr, &pbr.constantBuffer);
	assert(!FAILED(result));

	// Create light buffer
	bDesc.ByteWidth = lights.size() * sizeof(Light);

	result = pbr.device->CreateBuffer(&bDesc, nullptr, &pbr.lightBuffer);
	assert(!FAILED(result));

	// Z buffer 
	D3D11_TEXTURE2D_DESC zDesc;
	ZeroMemory(&zDesc, sizeof(zDesc));
	zDesc.ArraySize = 1;
	zDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	zDesc.Width = scd.Width;
	zDesc.Height = scd.Height;
	zDesc.Usage = D3D11_USAGE_DEFAULT;
	zDesc.Format = DXGI_FORMAT_D32_FLOAT;
	zDesc.MipLevels = 1;
	zDesc.SampleDesc.Count = 1;

	result = pbr.device->CreateTexture2D(&zDesc, nullptr, &pbr.zBuffer);
	assert(!FAILED(result));

	D3D11_DEPTH_STENCIL_VIEW_DESC zViewDesc;
	ZeroMemory(&zViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	zViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	zViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	result = pbr.device->CreateDepthStencilView(pbr.zBuffer.Get(), nullptr, &pbr.depthStencil);
	assert(!FAILED(result));

	//LoadWobjectMesh("files/models/FireExt.wobj", m_model, device);
	ADUtils::LoadWobjectMesh("files/models/oildrum.wobj", m_model, pbr.device);

	// Setup Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA vdata;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&vdata, sizeof(D3D11_SUBRESOURCE_DATA));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(Vertex) * m_model.vertices.size();
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vdata.pSysMem = m_model.vertices.data();

	result = pbr.device->CreateBuffer(&bufferDesc, &vdata, &m_model.vertexBuffer);
	assert(!FAILED(result));

	// Index buffer
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(int) * m_model.indices.size();

	D3D11_SUBRESOURCE_DATA idata;
	ZeroMemory(&idata, sizeof(D3D11_SUBRESOURCE_DATA));
	idata.pSysMem = m_model.indices.data();
	result = pbr.device->CreateBuffer(&bufferDesc, &idata, &m_model.indexBuffer);
	assert(!FAILED(result));

	// Load shaders
	result = pbr.device->CreateVertexShader(base_vs, sizeof(base_vs), nullptr, &m_model.vertexShader);
	assert(!FAILED(result));
	result = pbr.device->CreatePixelShader(base_ps, sizeof(base_ps), nullptr, &m_model.pixelShader);
	assert(!FAILED(result));

	// Make input layout for vertex buffer
	D3D11_INPUT_ELEMENT_DESC tempInputElementDesc[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	result = pbr.device->CreateInputLayout(tempInputElementDesc, ARRAYSIZE(tempInputElementDesc), base_vs, sizeof(base_vs), &m_model.vertexBufferLayout);
	assert(!FAILED(result));

	return true;
}

bool ADRenderer::PBRRenderer::Update(FPSCamera* camera)
{
	float color[4] = { 0, 0, 0, 1 };

	ID3D11RenderTargetView* tempRTV[] = { pbr.render_target_view.Get() };
	pbr.context->OMSetRenderTargets(1, tempRTV, pbr.depthStencil.Get());
	pbr.context->ClearDepthStencilView(pbr.depthStencil.Get(), D3D11_CLEAR_DEPTH, 1, 0);

	pbr.context->ClearRenderTargetView(pbr.render_target_view.Get(), color);
	pbr.context->RSSetViewports(1, &pbr.viewport);
	// Model stuff

	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;

	// World matrix projection
	XMMATRIX temp = XMMatrixIdentity();
	temp = XMMatrixMultiply(temp, XMMatrixScaling(.1, .1, .1));
	temp = XMMatrixMultiply(temp, XMMatrixTranslation(m_model.position.x, m_model.position.y, m_model.position.z));
	XMStoreFloat4x4(&WORLD.WorldMatrix, temp);
	// View
	camera->GetViewMatrix(temp);
	XMStoreFloat4x4(&WORLD.ViewMatrix, temp);
	// Projection
	temp = XMMatrixPerspectiveFovLH(camera->GetFOV(), aspectRatio, 0.1f, 1000);
	XMStoreFloat4x4(&WORLD.ProjectionMatrix, temp);
	XMFLOAT3 campos = camera->GetPosition();
	WORLD.CameraPosition = XMFLOAT4(campos.x, campos.y, campos.z, 1);

	// Send the matrix to constant buffer
	D3D11_MAPPED_SUBRESOURCE gpuBuffer;
	HRESULT result = pbr.context->Map(pbr.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
	pbr.context->Unmap(pbr.constantBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* modelCBuffers[] = { pbr.constantBuffer.Get() };
	pbr.context->VSSetConstantBuffers(0, 1, modelCBuffers);
	// Model stuff

	// Send the lights to constant buffer
	D3D11_MAPPED_SUBRESOURCE lightSub;
	result = pbr.context->Map(pbr.lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightSub);
	assert(!FAILED(result));
	memcpy(lightSub.pData, lights.data(), sizeof(Light) * lights.size());
	pbr.context->Unmap(pbr.lightBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* lightCbuffers[] = { pbr.lightBuffer.Get() };
	pbr.context->PSSetConstantBuffers(0, 1, lightCbuffers);

	// Render stuff
	// sET THE PIPELINE
	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };
	ID3D11Buffer* moelVertexBuffers[] = { m_model.vertexBuffer.Get() };
	pbr.context->IASetVertexBuffers(0, 1, moelVertexBuffers, strides, offsets);
	pbr.context->IASetIndexBuffer(m_model.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set sampler
	pbr.context->PSSetSamplers(0, 1, m_model.sampler.GetAddressOf());

	ID3D11ShaderResourceView* resource_views[] = {
		m_model.albedo.Get(),
		m_model.normal.Get(),
		m_model.metallic.Get(),
		m_model.roughness.Get(),
		m_model.ambient_occlusion.Get(),
	};

	pbr.context->PSSetShaderResources(0, 5, resource_views);

	pbr.context->VSSetShader(m_model.vertexShader.Get(), 0, 0);
	pbr.context->PSSetShader(m_model.pixelShader.Get(), 0, 0);
	pbr.context->IASetInputLayout(m_model.vertexBufferLayout.Get());

	pbr.context->DrawIndexed(m_model.indices.size(), 0, 0);
	// Render stuff

	return true;
}

bool ADRenderer::PBRRenderer::Frame()
{
	pbr.chain->Present(1, 0);

	return true;
}

bool ADRenderer::PBRRenderer::ShutDown()
{
	return true;
}