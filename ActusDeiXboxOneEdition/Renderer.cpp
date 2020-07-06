#include "pch.h"

#include "Renderer.h"
#include "Utils.h"

ADResource::ADRenderer::PBRRenderer::RendererResources ADResource::ADRenderer::PBRRenderer::renderer_resources = ADResource::ADRenderer::PBRRenderer::RendererResources();

ADResource::ADRenderer::PBRRenderer::PBRRenderer()
{
	// Create the device
	D3D_FEATURE_LEVEL XBOX11 = D3D_FEATURE_LEVEL_10_0;
	ComPtr<ID3D11Device> tdevice;
	ComPtr<ID3D11DeviceContext> tcontext;
#ifdef _DEBUG
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
#else
	HRESULT result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		NULL,
		&XBOX11,
		1,
		D3D11_SDK_VERSION,
		&tdevice,
		nullptr,
		&tcontext
		);
#endif
	assert(!FAILED(result));

	// Convert the device and device context pointers to the 11.1 pointers
	result = tdevice.As(&renderer_resources.device); assert(!FAILED(result));
	result = tcontext.As(&renderer_resources.context); assert(!FAILED(result));

	// Grab the adapter to set up the swapchain
	result = renderer_resources.device.As(&dxgiDevice); assert(!FAILED(result));
}

bool ADResource::ADRenderer::PBRRenderer::Initialize()
{
	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();

	ComPtr<IDXGIAdapter> dxgiAdapter;
	HRESULT result = dxgiDevice->GetAdapter(&dxgiAdapter); assert(!FAILED(result));

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
		renderer_resources.device.Get(),
		reinterpret_cast<IUnknown*>(Window),
		&scd,
		nullptr,
		&renderer_resources.chain
		);
	assert(!FAILED(result));

	// Setup viewport
	renderer_resources.viewport.Width = scd.Width;
	renderer_resources.viewport.Height = scd.Height;
	renderer_resources.viewport.TopLeftY = renderer_resources.viewport.TopLeftX = 0;
	renderer_resources.viewport.MinDepth = 0;
	renderer_resources.viewport.MaxDepth = 1;

	// Create render target
	ComPtr<ID3D11Texture2D> backbuffer;
	result = renderer_resources.chain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);
	assert(!FAILED(result));

	// Render target
	result = renderer_resources.device->CreateRenderTargetView(backbuffer.Get(), nullptr, &renderer_resources.render_target_view);
	assert(!FAILED(result));

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
	ComPtr<ID3D11RasterizerState> twireraster;
	result = renderer_resources.device->CreateRasterizerState(&rdesc, &traster);
	assert(!FAILED(result));
	renderer_resources.defaultRasterizerState = traster;

	// Wireframe raster
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	result = renderer_resources.device->CreateRasterizerState(&rdesc, &twireraster);
	assert(!FAILED(result));
	renderer_resources.wireframeRasterizerState = twireraster;

	renderer_resources.context->RSSetState(renderer_resources.defaultRasterizerState.Get());
	// Rasterizer state

	// Set primitive topology
	renderer_resources.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

	result = renderer_resources.device->CreateBuffer(&bDesc, nullptr, &renderer_resources.constantBuffer);
	assert(!FAILED(result));

	// Create light buffer
	bDesc.ByteWidth = ResourceManager::GetLightCount() * sizeof(Light);

	result = renderer_resources.device->CreateBuffer(&bDesc, nullptr, &renderer_resources.lightBuffer);
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

	result = renderer_resources.device->CreateTexture2D(&zDesc, nullptr, &renderer_resources.zBuffer);
	assert(!FAILED(result));

	D3D11_DEPTH_STENCIL_VIEW_DESC zViewDesc;
	ZeroMemory(&zViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	zViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	zViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	result = renderer_resources.device->CreateDepthStencilView(renderer_resources.zBuffer.Get(), nullptr, &renderer_resources.depthStencil);
	assert(!FAILED(result));

	// Create normal map sampler state
	D3D11_SAMPLER_DESC sdesc;
	ZeroMemory(&sdesc, sizeof(D3D11_SAMPLER_DESC));
	sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sdesc.MinLOD = 0;
	sdesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the sampler state
	result = renderer_resources.device->CreateSamplerState(&sdesc, &renderer_resources.normal_sampler);
	assert(!FAILED(result));

#pragma region Shadow Initialization

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.Width = scd.Width;
	depthStencilDesc.Height = scd.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));

	dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
	ZeroMemory(&sr_desc, sizeof(sr_desc));

	sr_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	sr_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sr_desc.Texture2D.MostDetailedMip = 0;
	sr_desc.Texture2D.MipLevels = -1;

	result = renderer_resources.device->CreateTexture2D(&depthStencilDesc, 0, renderer_resources.shadowTexture.GetAddressOf());
	assert(!FAILED(result));

	result = renderer_resources.device->CreateDepthStencilView(renderer_resources.shadowTexture.Get(), &dsv_desc, renderer_resources.shadowDepth.GetAddressOf());
	assert(!FAILED(result));
	
	result = renderer_resources.device->CreateShaderResourceView(renderer_resources.shadowTexture.Get(), &sr_desc, renderer_resources.shadowView.GetAddressOf());
	assert(!FAILED(result));

	D3D11_SAMPLER_DESC shadowSamp;
	ZeroMemory(&shadowSamp, sizeof(shadowSamp));
	shadowSamp.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.BorderColor[0] = 1.0f;
	shadowSamp.BorderColor[1] = 1.0f;
	shadowSamp.BorderColor[2] = 1.0f;
	shadowSamp.BorderColor[3] = 1.0f;
	
	result = renderer_resources.device->CreateSamplerState(&shadowSamp, renderer_resources.shadowSampler.GetAddressOf());
	assert(!FAILED(result));

	ADUtils::SHADER shader = { 0 };

	strcpy_s(shader.vshader, "files\\shaders\\shadows_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\shadows_ps.hlsl");

	// Load shaders // Thanks Whittington
	ComPtr<ID3D10Blob> vertexblob;
	ComPtr<ID3D10Blob> pixelblob;

	Platform::String^ appInstallFolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
	std::string READ_PATH = std::string(appInstallFolder->Begin(), appInstallFolder->End()).append("\\");

	std::string vname(shader.vshader);
	std::string pname(shader.pshader);

	std::string v = std::string(READ_PATH.begin(), READ_PATH.end()).append(vname);
	std::string p = std::string(READ_PATH.begin(), READ_PATH.end()).append(pname);

	std::wstring vshadername(v.begin(), v.end());
	std::wstring pshadername(p.begin(), p.end());

	result = D3DCompileFromFile(vshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_VS, D3DCOMPILE_DEBUG, 0, &vertexblob, nullptr);
	assert(!FAILED(result));
	result = D3DCompileFromFile(pshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_PS, D3DCOMPILE_DEBUG, 0, &pixelblob, nullptr);
	assert(!FAILED(result));

	result = renderer_resources.device->CreateVertexShader(vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), nullptr, renderer_resources.shadowVertex.GetAddressOf());
	assert(!FAILED(result));
	result = renderer_resources.device->CreatePixelShader(pixelblob->GetBufferPointer(), pixelblob->GetBufferSize(), nullptr, renderer_resources.shadowPixel.GetAddressOf());
	assert(!FAILED(result));
#pragma endregion

	return true;
}

bool ADResource::ADRenderer::PBRRenderer::Update(FPSCamera* camera, OrbitCamera* ocamera)
{
	float color[4] = { 0, 0, 0, 1 };

	ID3D11RenderTargetView* tempRTV[] = { renderer_resources.render_target_view.Get() };
	renderer_resources.context->OMSetRenderTargets(1, tempRTV, renderer_resources.depthStencil.Get());
	renderer_resources.context->ClearDepthStencilView(renderer_resources.depthStencil.Get(), D3D11_CLEAR_DEPTH, 1, 0);

	renderer_resources.context->ClearRenderTargetView(renderer_resources.render_target_view.Get(), color);
	renderer_resources.context->RSSetViewports(1, &renderer_resources.viewport);
	renderer_resources.context->RSSetState(renderer_resources.defaultRasterizerState.Get());

	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;

	// Send the lights to constant buffer
	D3D11_MAPPED_SUBRESOURCE lightSub;
	HRESULT result = renderer_resources.context->Map(renderer_resources.lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightSub);
	assert(!FAILED(result));
	memcpy(lightSub.pData, ResourceManager::GetLightDataPtr(), sizeof(Light) * ResourceManager::GetLightCount());
	renderer_resources.context->Unmap(renderer_resources.lightBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* lightCbuffers[] = { renderer_resources.lightBuffer.Get() };
	renderer_resources.context->PSSetConstantBuffers(0, 1, lightCbuffers);

	//XMFLOAT3 campos = camera->GetPosition();
	XMFLOAT3 campos = ocamera->GetPosition();
	XMFLOAT3 pos, rot, scale;
	XMMATRIX temp;

	// Skybox
	// disable depth buffer
	renderer_resources.context->OMSetRenderTargets(1, tempRTV, nullptr);

	// sET THE PIPELINE
	UINT skystrices[] = { sizeof(Vertex) };
	UINT skyoffsets[] = { 0 };
	ID3D11Buffer* skyVertexBuffers[] = { ResourceManager::GetSkybox()->vertexBuffer.Get() };
	renderer_resources.context->IASetVertexBuffers(0, 1, skyVertexBuffers, skystrices, skyoffsets);
	renderer_resources.context->IASetIndexBuffer(ResourceManager::GetSkybox()->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pos = ResourceManager::GetSkybox()->position;
	rot = ResourceManager::GetSkybox()->rotation;
	scale = ResourceManager::GetSkybox()->scale;

	temp = XMMatrixRotationX(XMConvertToRadians(180));
	temp = XMMatrixMultiply(temp, XMMatrixScaling(scale.x, scale.y, scale.z));
	temp = XMMatrixMultiply(temp, XMMatrixTranslation(campos.x, campos.y, campos.z));
	XMStoreFloat4x4(&WORLD.WorldMatrix, temp);
	// View
	ocamera->GetViewMatrix(temp);
	XMStoreFloat4x4(&WORLD.ViewMatrix, temp);
	// Projection
	//temp = XMMatrixPerspectiveFovLH(camera->GetFOV(), aspectRatio, 0.1f, 1000);
	temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, ocamera->GetNear(), ocamera->GetFar());
	XMStoreFloat4x4(&WORLD.ProjectionMatrix, temp);

	WORLD.CameraPosition = XMFLOAT4(campos.x, campos.y, campos.z, 1);

	// Send the matrix to constant buffer
	D3D11_MAPPED_SUBRESOURCE gpuBuffer;
	result = renderer_resources.context->Map(renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
	renderer_resources.context->Unmap(renderer_resources.constantBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* modelCBuffers[] = { renderer_resources.constantBuffer.Get() };
	renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);
	// Model stuff

	// Render stuff
	// Set sampler
	renderer_resources.context->PSSetSamplers(0, 1, ResourceManager::GetSkybox()->sampler.GetAddressOf());

	ID3D11ShaderResourceView* resource_views[] = {
		ResourceManager::GetSkybox()->albedo->texture.Get(),
	};

	renderer_resources.context->PSSetShaderResources(0, 1, resource_views);

	renderer_resources.context->VSSetShader(ResourceManager::GetSkybox()->vertexShader.Get(), 0, 0);
	renderer_resources.context->PSSetShader(ResourceManager::GetSkybox()->pixelShader.Get(), 0, 0);
	renderer_resources.context->IASetInputLayout(ResourceManager::GetSkybox()->inputLayout.Get());

	renderer_resources.context->DrawIndexed(ResourceManager::GetSkybox()->indices.size(), 0, 0);
	// Skybox

	// Set depth buffer
	renderer_resources.context->OMSetRenderTargets(1, tempRTV, renderer_resources.depthStencil.Get());

	// sET THE PIPELINE
	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };
	ID3D11Buffer* moelVertexBuffers[] = { ResourceManager::GetVertexBuffer().Get() };
	renderer_resources.context->IASetVertexBuffers(0, 1, moelVertexBuffers, strides, offsets);
	renderer_resources.context->IASetIndexBuffer(ResourceManager::GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);



	//for (int i = 0; i < model_count; i++)
	//{
	//	bool bruh = ResourceManager::GetPBRPtr()[i].desc.wireframe_mode;
	//	if (bruh)
	//	{
	//		pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.wireframeRasterizerState.Get());
	//	}
	//	else
	//	{
	//		pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.defaultRasterizerState.Get());
	//	}

	//	// Model stuff
	//	// World matrix projection
	//	// TODO: Translate rotation to quaternion
	//	pos = ResourceManager::GetPBRPtr()[i].position;
	//	rot = ResourceManager::GetPBRPtr()[i].rotation;
	//	scale = ResourceManager::GetPBRPtr()[i].scale;

	//	temp = XMMatrixRotationZ(ResourceManager::GetPBRPtr()[i].rotation.z);
	//	temp = XMMatrixMultiply(temp, XMMatrixRotationY(rot.y));
	//	temp = XMMatrixMultiply(temp, XMMatrixRotationX(rot.x));
	//	temp = XMMatrixMultiply(temp, XMMatrixScaling(scale.x, scale.y, scale.z));
	//	temp = XMMatrixMultiply(temp, XMMatrixTranslation(pos.x, pos.y, pos.z));
	//	XMStoreFloat4x4(&WORLD.WorldMatrix, temp);
	//	// View
	//	//camera->GetViewMatrix(temp);
	//	ocamera->GetViewMatrix(temp);
	//	XMStoreFloat4x4(&WORLD.ViewMatrix, temp);
	//	// Projection
	//	temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, 0.1f, 3000);
	//	XMStoreFloat4x4(&WORLD.ProjectionMatrix, temp);
	//	
	//	WORLD.CameraPosition = XMFLOAT4(campos.x, campos.y, campos.z, 1);

	//	// Send the matrix to constant buffer
	//	D3D11_MAPPED_SUBRESOURCE gpuBuffer;
	//	HRESULT result = pbr_renderer_resources.context->Map(pbr_renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	//	memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
	//	pbr_renderer_resources.context->Unmap(pbr_renderer_resources.constantBuffer.Get(), 0);
	//	// Connect constant buffer to the pipeline
	//	ID3D11Buffer* modelCBuffers[] = { pbr_renderer_resources.constantBuffer.Get() };
	//	pbr_renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);
	//	// Model stuff

	//	// Render stuff
	//	// Set sampler
	//	pbr_renderer_resources.context->PSSetSamplers(0, 1, ResourceManager::GetPBRPtr()[i].sampler.GetAddressOf());

	//	ID3D11ShaderResourceView* resource_views[] = {
	//		ResourceManager::GetPBRPtr()[i].albedo.Get(),
	//		ResourceManager::GetPBRPtr()[i].normal.Get(),
	//		ResourceManager::GetPBRPtr()[i].metallic.Get(),
	//		ResourceManager::GetPBRPtr()[i].roughness.Get(),
	//		ResourceManager::GetPBRPtr()[i].ambient_occlusion.Get(),
	//	};

	//	pbr_renderer_resources.context->PSSetShaderResources(0, 5, resource_views);

	//	pbr_renderer_resources.context->VSSetShader(ResourceManager::GetPBRPtr()[i].vertexShader.Get(), 0, 0);
	//	pbr_renderer_resources.context->PSSetShader(ResourceManager::GetPBRPtr()[i].pixelShader.Get(), 0, 0);
	//	pbr_renderer_resources.context->IASetInputLayout(ResourceManager::GetPBRPtr()[i].vertexBufferLayout.Get());

	//	int istart = ResourceManager::GetPBRPtr()[i].desc.index_start;
	//	int ibase = ResourceManager::GetPBRPtr()[i].desc.base_vertex_location;
	//	int icount = ResourceManager::GetPBRPtr()[i].desc.index_count;
	//	pbr_renderer_resources.context->DrawIndexed(icount, istart, ibase);
	//}

	return true;
}

bool ADResource::ADRenderer::PBRRenderer::Render(FPSCamera* camera, OrbitCamera* ocamera, float delta_time)
{
	float color[4] = { 0, 0, 0, 1 };

	ID3D11RenderTargetView* tempRTV[] = { renderer_resources.render_target_view.Get() };
	renderer_resources.context->OMSetRenderTargets(1, tempRTV, renderer_resources.depthStencil.Get());
	renderer_resources.context->ClearDepthStencilView(renderer_resources.depthStencil.Get(), D3D11_CLEAR_DEPTH, 1, 0);

	renderer_resources.context->ClearRenderTargetView(renderer_resources.render_target_view.Get(), color);
	renderer_resources.context->RSSetViewports(1, &renderer_resources.viewport);
	renderer_resources.context->RSSetState(renderer_resources.defaultRasterizerState.Get());

	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;

	// Send the lights to constant buffer
	D3D11_MAPPED_SUBRESOURCE lightSub;
	HRESULT result = renderer_resources.context->Map(renderer_resources.lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightSub);
	assert(!FAILED(result));
	memcpy(lightSub.pData, ResourceManager::GetLightDataPtr(), sizeof(Light) * ResourceManager::GetLightCount());
	renderer_resources.context->Unmap(renderer_resources.lightBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* lightCbuffers[] = { renderer_resources.lightBuffer.Get() };
	renderer_resources.context->PSSetConstantBuffers(0, 1, lightCbuffers);

	FPSCamera* shadowCamera = new FPSCamera(XMFLOAT3(0, 500, 1000), 0, 3.141592f / 2.0f);

	XMFLOAT3 campos = ocamera->GetPosition();

	WORLD.CameraPosition = XMFLOAT4(campos.x, campos.y, campos.z, 1);

	XMFLOAT3 pos, rot, scale;
	XMMATRIX temp;

	ADResource::ADGameplay::GameObject* current_obj = nullptr;
	SimpleModel** current_model = nullptr;
	SimpleStaticModel* current_static_model = nullptr;
	SimpleAnimModel* current_animated_model = nullptr;

	ComPtr<IDXGIFactory> factory;

	renderer_resources.context->OMSetRenderTargets(0, nullptr, renderer_resources.shadowDepth.Get());

	while (!ResourceManager::ShadowQueueEmpty())
	{
		current_obj = ResourceManager::PopFromShadowQueue();
		current_model = ResourceManager::GetSimpleModelPtrFromMeshId(current_obj->GetMeshId());

		if (current_model == nullptr)
			continue;

		if ((*current_model)->animated)
		{
			continue;
		}
		else
		{
			current_static_model = static_cast<SimpleStaticModel*>(*current_model);

			current_obj->GetWorldMatrix(temp);
			XMStoreFloat4x4(&WORLD.WorldMatrix, temp);

			shadowCamera->GetViewMatrix(temp);
			XMStoreFloat4x4(&WORLD.ViewMatrix, temp);

			temp = XMMatrixPerspectiveFovLH(shadowCamera->GetFOV(), aspectRatio, shadowCamera->GetNear(), shadowCamera->GetFar());
			XMStoreFloat4x4(&WORLD.ProjectionMatrix, temp);

			XMFLOAT3 shadowCameraPosition = shadowCamera->GetPosition();
			XMFLOAT4 shadowTemp = XMFLOAT4(shadowCameraPosition.x, shadowCameraPosition.y, shadowCameraPosition.z, 1);
			XMStoreFloat4(&WORLD.CameraPosition, Float4ToVector(shadowTemp));

			D3D11_MAPPED_SUBRESOURCE gpuBuffer;
			result = renderer_resources.context->Map(renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
			memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
			renderer_resources.context->Unmap(renderer_resources.constantBuffer.Get(), 0);

			UINT strides[] = { sizeof(SimpleVertex) };
			UINT offsets[] = { 0 };
			ID3D11Buffer* modelVertexBuffers[] = { current_static_model->vertexBuffer.Get() };
			renderer_resources.context->IASetInputLayout(current_static_model->inputLayout.Get());
			renderer_resources.context->IASetVertexBuffers(0, 1, modelVertexBuffers, strides, offsets);
			renderer_resources.context->IASetIndexBuffer(current_static_model->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			ID3D11Buffer* modelCBuffers[] = { renderer_resources.constantBuffer.Get() };
			renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);

			renderer_resources.context->VSSetShader(renderer_resources.shadowVertex.Get(), 0, 0);
			renderer_resources.context->PSSetShader(renderer_resources.shadowPixel.Get(), 0, 0);

			renderer_resources.context->DrawIndexed(current_static_model->indices.size(), 0, 0);
		}
	}

#pragma region Skybox

	//Disable Z buffer
	renderer_resources.context->OMSetRenderTargets(1, tempRTV, nullptr);

	//Set up Skybox
	UINT skystrices[] = { sizeof(SimpleVertex) };
	UINT skyoffsets[] = { 0 };
	ID3D11Buffer* skyVertexBuffers[] = { ResourceManager::GetSkybox()->vertexBuffer.Get() };
	renderer_resources.context->IASetVertexBuffers(0, 1, skyVertexBuffers, skystrices, skyoffsets);
	renderer_resources.context->IASetIndexBuffer(ResourceManager::GetSkybox()->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pos = ResourceManager::GetSkybox()->position;
	rot = ResourceManager::GetSkybox()->rotation;
	scale = ResourceManager::GetSkybox()->scale;

	temp = XMMatrixScaling(10, 10, 10);
	temp = XMMatrixMultiply(temp, XMMatrixTranslation(campos.x, campos.y, campos.z));
	XMStoreFloat4x4(&WORLD.WorldMatrix, temp);

	// View
	ocamera->GetViewMatrix(temp);
	XMStoreFloat4x4(&WORLD.ViewMatrix, temp);

	// Projection
	temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, ocamera->GetNear(), ocamera->GetFar());
	XMStoreFloat4x4(&WORLD.ProjectionMatrix, temp);

	// Model stuff

	// Render stuff
	renderer_resources.context->IASetInputLayout(ResourceManager::GetSkybox()->inputLayout.Get());

	renderer_resources.context->VSSetShader(ResourceManager::GetSkybox()->vertexShader.Get(), 0, 0);

	// Send the matrix to constant buffer
	D3D11_MAPPED_SUBRESOURCE gpuBuffer;
	result = renderer_resources.context->Map(renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
	renderer_resources.context->Unmap(renderer_resources.constantBuffer.Get(), 0);

	// Connect constant buffer to the pipeline
	ID3D11Buffer* modelCBuffers[] = { renderer_resources.constantBuffer.Get() };
	renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);

	ID3D11ShaderResourceView* resource_views[] = {
	ResourceManager::GetSkybox()->albedo->texture.Get(),
	};

	renderer_resources.context->PSSetShader(ResourceManager::GetSkybox()->pixelShader.Get(), 0, 0);

	renderer_resources.context->PSSetShaderResources(0, 1, resource_views);
	renderer_resources.context->PSSetSamplers(0, 1, ResourceManager::GetSkybox()->sampler.GetAddressOf());

	renderer_resources.context->DrawIndexed(ResourceManager::GetSkybox()->indices.size(), 0, 0);

	// Enable Z buffer
	renderer_resources.context->OMSetRenderTargets(1, tempRTV, renderer_resources.depthStencil.Get());

#pragma endregion


	//Render Loop

	current_obj = nullptr;
	current_model = nullptr;
	current_static_model = nullptr;
	current_animated_model = nullptr;

	while (!ResourceManager::RenderQueueEmpty())
	{
		current_obj = ResourceManager::PopFromRenderQueue();
		current_model = ResourceManager::GetSimpleModelPtrFromMeshId(current_obj->GetMeshId());

		if (current_model == nullptr)
			continue;

		//Animations
		if ((*current_model)->animated)
		{
			current_animated_model = static_cast<SimpleAnimModel*>(*current_model);

			//continue;

			std::vector<XMMATRIX> joints = current_animated_model->UpdateAnimation(delta_time);

			//Update Buffers
			renderer_resources.context->UpdateSubresource(current_animated_model->animationBuffer.Get(), NULL, nullptr, joints.data(), 0, 0);

			UINT strides[] = { sizeof(SimpleVertexAnim) };
			UINT offsets[] = { 0 };
			ID3D11Buffer* modelVertexBuffers[] = { current_animated_model->vertexBuffer.Get() };
			renderer_resources.context->IASetVertexBuffers(0, 1, modelVertexBuffers, strides, offsets);
			renderer_resources.context->IASetIndexBuffer(current_animated_model->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			// Model stuff
			// World matrix projection
			// TODO: Translate rotation to quaternion

			current_obj->GetWorldMatrix(temp);
			XMStoreFloat4x4(&WORLD.WorldMatrix, temp);
			// View

			// Send the matrix to constant buffer
			D3D11_MAPPED_SUBRESOURCE gpuBuffer;
			HRESULT result = renderer_resources.context->Map(renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
			memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
			renderer_resources.context->Unmap(renderer_resources.constantBuffer.Get(), 0);
			// Connect constant buffer to the pipeline
			ID3D11Buffer* modelCBuffers[] = { renderer_resources.constantBuffer.Get(), current_animated_model->animationBuffer.Get() };
			renderer_resources.context->VSSetConstantBuffers(0, 2, modelCBuffers);
			// Model stuff

			// Render stuff
			// Set sampler

			ID3D11ShaderResourceView* resource_views[] = {
				current_animated_model->albedo->texture.Get(),
				current_animated_model->emissive->texture.Get(),
				current_animated_model->normal->texture.Get(),
			};

			renderer_resources.context->PSSetConstantBuffers(0, 1, lightCbuffers);
			renderer_resources.context->PSSetShaderResources(0, 3, resource_views);
			renderer_resources.context->VSSetShaderResources(0, 1, current_animated_model->normal->texture.GetAddressOf());

			renderer_resources.context->VSSetShader(current_animated_model->vertexShader.Get(), 0, 0);
			renderer_resources.context->PSSetShader(current_animated_model->pixelShader.Get(), 0, 0);
			renderer_resources.context->IASetInputLayout(current_animated_model->inputLayout.Get());

			ID3D11SamplerState* current_samplers[] = { current_animated_model->sampler.Get() };

			renderer_resources.context->PSSetSamplers(0, 1, current_samplers);

			//int istart = current_model->desc.index_start;
			//int ibase = current_model->desc.base_vertex_location;
			//int icount = current_model->desc.index_count;
			//pbr_renderer_resources.context->DrawIndexed(icount, istart, ibase);
	
			renderer_resources.context->DrawIndexed(current_animated_model->indices.size(), 0, 0);
		}

		//Statics
		else
		{
			current_static_model = static_cast<SimpleStaticModel*>(*current_model);

			UINT strides[2] = { sizeof(SimpleVertex), sizeof(XMFLOAT3) };
			UINT offsets[2] = { 0, 0 };
			ID3D11Buffer* modelVertexBuffers[2] = { current_static_model->vertexBuffer.Get(), current_static_model->instanceBuffer.Get() };
			renderer_resources.context->IASetVertexBuffers(0, 2, modelVertexBuffers, strides, offsets);
			renderer_resources.context->IASetIndexBuffer(current_static_model->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			// Model stuff
			// World matrix projection
			// TODO: Translate rotation to quaternion
			current_obj->GetWorldMatrix(temp);
			//temp = XMMatrixRotationX(-3.14f / 2) * temp;
			XMStoreFloat4x4(&WORLD.WorldMatrix, temp);

			// Send the matrix to constant buffer
			D3D11_MAPPED_SUBRESOURCE gpuBuffer;
			HRESULT result = renderer_resources.context->Map(renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
			memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
			renderer_resources.context->Unmap(renderer_resources.constantBuffer.Get(), 0);
			// Connect constant buffer to the pipeline
			ID3D11Buffer* modelCBuffers[] = { renderer_resources.constantBuffer.Get() };
			renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);
			// Model stuff
			renderer_resources.context->PSSetConstantBuffers(0, 1, lightCbuffers);
			// Render stuff

			ID3D11ShaderResourceView* resource_views[] = {
				current_static_model->albedo->texture.Get(),
				current_static_model->normal->texture.Get(),
				renderer_resources.shadowView.Get(),
			};

			ID3D11SamplerState* current_samplers[] = { current_static_model->sampler.Get() };

			renderer_resources.context->PSSetShaderResources(0, 3, resource_views);
			renderer_resources.context->PSSetSamplers(0, 1, current_samplers);

			renderer_resources.context->VSSetShaderResources(0, 1, current_static_model->normal->texture.GetAddressOf());

			renderer_resources.context->VSSetShader(current_static_model->vertexShader.Get(), 0, 0);
			renderer_resources.context->PSSetShader(current_static_model->pixelShader.Get(), 0, 0);
			renderer_resources.context->IASetInputLayout(current_static_model->inputLayout.Get());

			renderer_resources.context->DrawIndexedInstanced(current_static_model->indices.size(), current_static_model->instanceCount, 0, 0, 0);
		}

		//bool bruh = current_model->desc.wireframe_mode;
		//if (bruh)
		//{
		//	pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.wireframeRasterizerState.Get());
		//}
		//else
		//{
		//	pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.defaultRasterizerState.Get());
		//}
	}

	delete shadowCamera;

	return true;
}

bool  ADResource::ADRenderer::PBRRenderer::Frame()
{
	renderer_resources.chain->Present(1, 0);

	return true;
}

bool  ADResource::ADRenderer::PBRRenderer::ShutDown()
{
	return true;
}
