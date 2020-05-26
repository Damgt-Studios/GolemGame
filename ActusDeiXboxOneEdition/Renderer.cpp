#include "pch.h"

#include "Renderer.h"
#include "Utils.h"


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
	result = tdevice.As(&pbr_renderer_resources.device); assert(!FAILED(result));
	result = tcontext.As(&pbr_renderer_resources.context); assert(!FAILED(result));

	// Grab the adapter to set up the swapchain
	result = pbr_renderer_resources.device.As(&dxgiDevice); assert(!FAILED(result));
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
		pbr_renderer_resources.device.Get(),
		reinterpret_cast<IUnknown*>(Window),
		&scd,
		nullptr,
		&pbr_renderer_resources.chain
		);
	assert(!FAILED(result));

	// Setup viewport
	pbr_renderer_resources.viewport.Width = scd.Width;
	pbr_renderer_resources.viewport.Height = scd.Height;
	pbr_renderer_resources.viewport.TopLeftY = pbr_renderer_resources.viewport.TopLeftX = 0;
	pbr_renderer_resources.viewport.MinDepth = 0;
	pbr_renderer_resources.viewport.MaxDepth = 1;

	// Create render target
	ComPtr<ID3D11Texture2D> backbuffer;
	result = pbr_renderer_resources.chain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);
	assert(!FAILED(result));

	// Render target
	result = pbr_renderer_resources.device->CreateRenderTargetView(backbuffer.Get(), nullptr, &pbr_renderer_resources.render_target_view);
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
	result = pbr_renderer_resources.device->CreateRasterizerState(&rdesc, &traster);
	assert(!FAILED(result));
	pbr_renderer_resources.defaultRasterizerState = traster;

	// Wireframe raster
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	result = pbr_renderer_resources.device->CreateRasterizerState(&rdesc, &twireraster);
	assert(!FAILED(result));
	pbr_renderer_resources.wireframeRasterizerState = twireraster;

	pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.defaultRasterizerState.Get());
	// Rasterizer state

	// Set primitive topology
	pbr_renderer_resources.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

	result = pbr_renderer_resources.device->CreateBuffer(&bDesc, nullptr, &pbr_renderer_resources.constantBuffer);
	assert(!FAILED(result));

	// Create light buffer
	bDesc.ByteWidth = ResourceManager::GetLightCount() * sizeof(Light);

	result = pbr_renderer_resources.device->CreateBuffer(&bDesc, nullptr, &pbr_renderer_resources.lightBuffer);
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

	result = pbr_renderer_resources.device->CreateTexture2D(&zDesc, nullptr, &pbr_renderer_resources.zBuffer);
	assert(!FAILED(result));

	D3D11_DEPTH_STENCIL_VIEW_DESC zViewDesc;
	ZeroMemory(&zViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	zViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	zViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	result = pbr_renderer_resources.device->CreateDepthStencilView(pbr_renderer_resources.zBuffer.Get(), nullptr, &pbr_renderer_resources.depthStencil);
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
	result = pbr_renderer_resources.device->CreateSamplerState(&sdesc, &pbr_renderer_resources.normal_sampler);
	assert(!FAILED(result));

	return true;
}

bool ADResource::ADRenderer::PBRRenderer::Update(FPSCamera* camera, OrbitCamera* ocamera)
{
	float color[4] = { 0, 0, 0, 1 };

	ID3D11RenderTargetView* tempRTV[] = { pbr_renderer_resources.render_target_view.Get() };
	pbr_renderer_resources.context->OMSetRenderTargets(1, tempRTV, pbr_renderer_resources.depthStencil.Get());
	pbr_renderer_resources.context->ClearDepthStencilView(pbr_renderer_resources.depthStencil.Get(), D3D11_CLEAR_DEPTH, 1, 0);

	pbr_renderer_resources.context->ClearRenderTargetView(pbr_renderer_resources.render_target_view.Get(), color);
	pbr_renderer_resources.context->RSSetViewports(1, &pbr_renderer_resources.viewport);
	pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.defaultRasterizerState.Get());

	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;

	// Send the lights to constant buffer
	D3D11_MAPPED_SUBRESOURCE lightSub;
	HRESULT result = pbr_renderer_resources.context->Map(pbr_renderer_resources.lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightSub);
	assert(!FAILED(result));
	memcpy(lightSub.pData, ResourceManager::GetLightDataPtr(), sizeof(Light) * ResourceManager::GetLightCount());
	pbr_renderer_resources.context->Unmap(pbr_renderer_resources.lightBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* lightCbuffers[] = { pbr_renderer_resources.lightBuffer.Get() };
	pbr_renderer_resources.context->PSSetConstantBuffers(0, 1, lightCbuffers);

	//XMFLOAT3 campos = camera->GetPosition();
	XMFLOAT3 campos = ocamera->GetPosition();
	XMFLOAT3 pos, rot, scale;
	XMMATRIX temp;

	// Skybox
	// disable depth buffer
	pbr_renderer_resources.context->OMSetRenderTargets(1, tempRTV, nullptr);

	// sET THE PIPELINE
	UINT skystrices[] = { sizeof(Vertex) };
	UINT skyoffsets[] = { 0 };
	ID3D11Buffer* skyVertexBuffers[] = { ResourceManager::GetSkybox()->vertexBuffer.Get() };
	pbr_renderer_resources.context->IASetVertexBuffers(0, 1, skyVertexBuffers, skystrices, skyoffsets);
	pbr_renderer_resources.context->IASetIndexBuffer(ResourceManager::GetSkybox()->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

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
	result = pbr_renderer_resources.context->Map(pbr_renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
	pbr_renderer_resources.context->Unmap(pbr_renderer_resources.constantBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* modelCBuffers[] = { pbr_renderer_resources.constantBuffer.Get() };
	pbr_renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);
	// Model stuff

	// Render stuff
	// Set sampler
	pbr_renderer_resources.context->PSSetSamplers(0, 1, ResourceManager::GetSkybox()->sampler.GetAddressOf());

	ID3D11ShaderResourceView* resource_views[] = {
		ResourceManager::GetSkybox()->albedo.Get(),
	};

	pbr_renderer_resources.context->PSSetShaderResources(0, 1, resource_views);

	pbr_renderer_resources.context->VSSetShader(ResourceManager::GetSkybox()->vertexShader.Get(), 0, 0);
	pbr_renderer_resources.context->PSSetShader(ResourceManager::GetSkybox()->pixelShader.Get(), 0, 0);
	pbr_renderer_resources.context->IASetInputLayout(ResourceManager::GetSkybox()->vertexBufferLayout.Get());

	pbr_renderer_resources.context->DrawIndexed(ResourceManager::GetSkybox()->indices.size(), 0, 0);
	// Skybox

	// Set depth buffer
	pbr_renderer_resources.context->OMSetRenderTargets(1, tempRTV, pbr_renderer_resources.depthStencil.Get());

	// sET THE PIPELINE
	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };
	ID3D11Buffer* moelVertexBuffers[] = { ResourceManager::GetVertexBuffer().Get() };
	pbr_renderer_resources.context->IASetVertexBuffers(0, 1, moelVertexBuffers, strides, offsets);
	pbr_renderer_resources.context->IASetIndexBuffer(ResourceManager::GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	unsigned int model_count = ResourceManager::GetPBRModelCount();


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

	ID3D11RenderTargetView* tempRTV[] = { pbr_renderer_resources.render_target_view.Get() };
	pbr_renderer_resources.context->OMSetRenderTargets(1, tempRTV, pbr_renderer_resources.depthStencil.Get());
	pbr_renderer_resources.context->ClearDepthStencilView(pbr_renderer_resources.depthStencil.Get(), D3D11_CLEAR_DEPTH, 1, 0);

	pbr_renderer_resources.context->ClearRenderTargetView(pbr_renderer_resources.render_target_view.Get(), color);
	pbr_renderer_resources.context->RSSetViewports(1, &pbr_renderer_resources.viewport);
	pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.defaultRasterizerState.Get());

	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;

	// Send the lights to constant buffer
	D3D11_MAPPED_SUBRESOURCE lightSub;
	HRESULT result = pbr_renderer_resources.context->Map(pbr_renderer_resources.lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightSub);
	assert(!FAILED(result));
	memcpy(lightSub.pData, ResourceManager::GetLightDataPtr(), sizeof(Light) * ResourceManager::GetLightCount());
	pbr_renderer_resources.context->Unmap(pbr_renderer_resources.lightBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* lightCbuffers[] = { pbr_renderer_resources.lightBuffer.Get() };
	pbr_renderer_resources.context->PSSetConstantBuffers(0, 1, lightCbuffers);

	//XMFLOAT3 campos = camera->GetPosition();
	XMFLOAT3 campos = ocamera->GetPosition();
	XMFLOAT3 pos, rot, scale;
	XMMATRIX temp;

	// Skybox
	// disable depth buffer
	pbr_renderer_resources.context->OMSetRenderTargets(1, tempRTV, nullptr);

	// sET THE PIPELINE
	UINT skystrices[] = { sizeof(Vertex) };
	UINT skyoffsets[] = { 0 };
	ID3D11Buffer* skyVertexBuffers[] = { ResourceManager::GetSkybox()->vertexBuffer.Get() };
	pbr_renderer_resources.context->IASetVertexBuffers(0, 1, skyVertexBuffers, skystrices, skyoffsets);
	pbr_renderer_resources.context->IASetIndexBuffer(ResourceManager::GetSkybox()->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

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
	result = pbr_renderer_resources.context->Map(pbr_renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
	pbr_renderer_resources.context->Unmap(pbr_renderer_resources.constantBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* modelCBuffers[] = { pbr_renderer_resources.constantBuffer.Get() };
	pbr_renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);
	// Model stuff

	// Render stuff
	// Set sampler
	pbr_renderer_resources.context->PSSetSamplers(0, 1, ResourceManager::GetSkybox()->sampler.GetAddressOf());

	ID3D11ShaderResourceView* resource_views[] = {
		ResourceManager::GetSkybox()->albedo.Get(),
	};

	pbr_renderer_resources.context->PSSetShaderResources(0, 1, resource_views);

	pbr_renderer_resources.context->VSSetShader(ResourceManager::GetSkybox()->vertexShader.Get(), 0, 0);
	pbr_renderer_resources.context->PSSetShader(ResourceManager::GetSkybox()->pixelShader.Get(), 0, 0);
	pbr_renderer_resources.context->IASetInputLayout(ResourceManager::GetSkybox()->vertexBufferLayout.Get());

	pbr_renderer_resources.context->DrawIndexed(ResourceManager::GetSkybox()->indices.size(), 0, 0);
	// Skybox

	// Set depth buffer
	pbr_renderer_resources.context->OMSetRenderTargets(1, tempRTV, pbr_renderer_resources.depthStencil.Get());

	//// sET THE PIPELINE
	//UINT strides[] = { sizeof(Vertex) };
	//UINT offsets[] = { 0 };
	//ID3D11Buffer* moelVertexBuffers[] = { ResourceManager::GetVertexBuffer().Get() };
	//pbr_renderer_resources.context->IASetVertexBuffers(0, 1, moelVertexBuffers, strides, offsets);
	//pbr_renderer_resources.context->IASetIndexBuffer(ResourceManager::GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	ADResource::ADGameplay::GameObject* current_obj = nullptr;
	SimpleModel** current_model = nullptr;
	SimpleStaticModel* current_static_model = nullptr;
	SimpleAnimModel* current_animated_model = nullptr;
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

			std::vector<XMMATRIX> joints = current_animated_model->UpdateAnimation(delta_time);

			//Update Buffers
			pbr_renderer_resources.context->UpdateSubresource(current_animated_model->animationBuffer.Get(), NULL, nullptr, joints.data(), 0, 0);

			//pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.defaultRasterizerState.Get());

			UINT strides[] = { sizeof(SimpleVertexAnim) };
			UINT offsets[] = { 0 };
			ID3D11Buffer* modelVertexBuffers[] = { current_animated_model->vertexBuffer.Get() };
			pbr_renderer_resources.context->IASetVertexBuffers(0, 1, modelVertexBuffers, strides, offsets);
			pbr_renderer_resources.context->IASetIndexBuffer(current_animated_model->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			// Model stuff
			// World matrix projection
			// TODO: Translate rotation to quaternion
			fbxmodel_map;

			current_obj->GetWorldMatrix(temp);
			temp.r[3].m128_f32[1] = -5;
			//temp = XMMatrixRotationX(-3.14f / 2) * temp;
			//temp = XMMatrixRotationZ(3.14f) * temp;
			temp = XMMatrixRotationY(3.14f) * temp;
			XMStoreFloat4x4(&WORLD.WorldMatrix, temp);
			// View

			ocamera->GetViewMatrix(temp);
			XMStoreFloat4x4(&WORLD.ViewMatrix, temp);
			// Projection

			temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, 0.1f, 3000);
			XMStoreFloat4x4(&WORLD.ProjectionMatrix, temp);

			WORLD.CameraPosition = XMFLOAT4(campos.x, campos.y, campos.z, 1);

			// Send the matrix to constant buffer
			D3D11_MAPPED_SUBRESOURCE gpuBuffer;
			HRESULT result = pbr_renderer_resources.context->Map(pbr_renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
			memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
			pbr_renderer_resources.context->Unmap(pbr_renderer_resources.constantBuffer.Get(), 0);
			// Connect constant buffer to the pipeline
			ID3D11Buffer* modelCBuffers[] = { pbr_renderer_resources.constantBuffer.Get(), current_animated_model->animationBuffer.Get() };
			pbr_renderer_resources.context->VSSetConstantBuffers(0, 2, modelCBuffers);
			// Model stuff

			// Render stuff
			// Set sampler
			ID3D11SamplerState* samplers[] = { current_animated_model->sampler.Get(), pbr_renderer_resources.normal_sampler.Get() };
			pbr_renderer_resources.context->PSSetSamplers(0, 2, samplers);

			ID3D11ShaderResourceView* resource_views[] = {
				current_animated_model->albedo.Get(),
				current_animated_model->normal.Get(),
				current_animated_model->emissive.Get()
			};

			pbr_renderer_resources.context->PSSetShaderResources(0, 3, resource_views);
			pbr_renderer_resources.context->VSSetShaderResources(0, 1, current_animated_model->normal.GetAddressOf());

			pbr_renderer_resources.context->VSSetShader(current_animated_model->vertexShader.Get(), 0, 0);
			pbr_renderer_resources.context->PSSetShader(current_animated_model->pixelShader.Get(), 0, 0);
			pbr_renderer_resources.context->IASetInputLayout(current_animated_model->inputLayout.Get());

			ID3D11SamplerState* current_samplers[] = { current_animated_model->sampler.Get() };

			pbr_renderer_resources.context->PSSetSamplers(0, 1, current_samplers);

			//int istart = current_model->desc.index_start;
			//int ibase = current_model->desc.base_vertex_location;
			//int icount = current_model->desc.index_count;
			//pbr_renderer_resources.context->DrawIndexed(icount, istart, ibase);
			pbr_renderer_resources.context->DrawIndexed(current_animated_model->indices.size(), 0, 0);
		}

		//Statics
		else
		{
			current_static_model = static_cast<SimpleStaticModel*>(*current_model);

			pbr_renderer_resources.context->RSSetState(pbr_renderer_resources.defaultRasterizerState.Get());

			UINT strides[] = { sizeof(SimpleVertex) };
			UINT offsets[] = { 0 };
			ID3D11Buffer* modelVertexBuffers[] = { current_static_model->vertexBuffer.Get() };
			pbr_renderer_resources.context->IASetVertexBuffers(0, 1, modelVertexBuffers, strides, offsets);
			pbr_renderer_resources.context->IASetIndexBuffer(current_static_model->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			// Model stuff
			// World matrix projection
			// TODO: Translate rotation to quaternion
			current_obj->GetWorldMatrix(temp);
			temp.r[3].m128_f32[1] = -5;
			temp.r[3].m128_f32[2] = -10;
			temp = XMMatrixRotationX(-3.14f / 2) * temp;
			XMStoreFloat4x4(&WORLD.WorldMatrix, temp);
			// View

			ocamera->GetViewMatrix(temp);
			XMStoreFloat4x4(&WORLD.ViewMatrix, temp);
			// Projection

			temp = XMMatrixPerspectiveFovLH(ocamera->GetFOV(), aspectRatio, 0.1f, 3000);
			XMStoreFloat4x4(&WORLD.ProjectionMatrix, temp);

			WORLD.CameraPosition = XMFLOAT4(campos.x, campos.y, campos.z, 1);

			// Send the matrix to constant buffer
			D3D11_MAPPED_SUBRESOURCE gpuBuffer;
			HRESULT result = pbr_renderer_resources.context->Map(pbr_renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
			memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
			pbr_renderer_resources.context->Unmap(pbr_renderer_resources.constantBuffer.Get(), 0);
			// Connect constant buffer to the pipeline
			ID3D11Buffer* modelCBuffers[] = { pbr_renderer_resources.constantBuffer.Get() };
			pbr_renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);
			// Model stuff

			// Render stuff
			// Set sampler
			ID3D11SamplerState* samplers[] = { current_static_model->sampler.Get(), pbr_renderer_resources.normal_sampler.Get() };
			pbr_renderer_resources.context->PSSetSamplers(0, 2, samplers);

			ID3D11ShaderResourceView* resource_views[] = {
				current_static_model->albedo.Get(),
				current_static_model->normal.Get(),
				current_static_model->emissive.Get()
			};

			ID3D11SamplerState* current_samplers[] = { current_static_model->sampler.Get() };

			pbr_renderer_resources.context->PSSetShaderResources(0, 3, resource_views);
			pbr_renderer_resources.context->PSSetSamplers(0, 1, current_samplers);


			pbr_renderer_resources.context->VSSetShaderResources(0, 1, current_static_model->normal.GetAddressOf());

			pbr_renderer_resources.context->VSSetShader(current_static_model->vertexShader.Get(), 0, 0);
			pbr_renderer_resources.context->PSSetShader(current_static_model->pixelShader.Get(), 0, 0);
			pbr_renderer_resources.context->IASetInputLayout(current_static_model->inputLayout.Get());


			//int istart = current_model->desc.index_start;
			//int ibase = current_model->desc.base_vertex_location;
			//int icount = current_model->desc.index_count;
			//pbr_renderer_resources.context->DrawIndexed(icount, istart, ibase);
			pbr_renderer_resources.context->DrawIndexed(current_static_model->indices.size(), 0, 0);
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

	return true;
}

bool  ADResource::ADRenderer::PBRRenderer::Frame()
{
	pbr_renderer_resources.chain->Present(1, 0);

	return true;
}

bool  ADResource::ADRenderer::PBRRenderer::ShutDown()
{
	return true;
}

ADResource::ADRenderer::PBRRendererResources* ADResource::ADRenderer::PBRRenderer::GetPBRRendererResources()
{
	return &pbr_renderer_resources;
}
