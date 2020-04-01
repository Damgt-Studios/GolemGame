#include "pch.h"

#include "Renderer.h"
#include "Utils.h"


ADResource::ADRenderer::PBRRenderer::PBRRenderer()
{
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
	result = pbr_renderer_resources.device->CreateRasterizerState(&rdesc, &traster);
	assert(!FAILED(result));
	result = traster.As(&pbr_renderer_resources.defaultRasterizerState);
	assert(!FAILED(result));

	pbr_renderer_resources.context->RSSetState(traster.Get());
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

	return true;
}

bool ADResource::ADRenderer::PBRRenderer::Update(FPSCamera* camera)
{
	float color[4] = { 0, 0, 0, 1 };

	ID3D11RenderTargetView* tempRTV[] = { pbr_renderer_resources.render_target_view.Get() };
	pbr_renderer_resources.context->OMSetRenderTargets(1, tempRTV, pbr_renderer_resources.depthStencil.Get());
	pbr_renderer_resources.context->ClearDepthStencilView(pbr_renderer_resources.depthStencil.Get(), D3D11_CLEAR_DEPTH, 1, 0);

	pbr_renderer_resources.context->ClearRenderTargetView(pbr_renderer_resources.render_target_view.Get(), color);
	pbr_renderer_resources.context->RSSetViewports(1, &pbr_renderer_resources.viewport);

	Windows::UI::Core::CoreWindow^ Window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	float aspectRatio = Window->Bounds.Width / Window->Bounds.Height;


	unsigned int model_count = ResourceManager::GetPBRModelCount();
	for (int i = 0; i < model_count; i++)
	{
		// Model stuff
		// World matrix projection
		XMMATRIX temp = XMMatrixIdentity();
		temp = XMMatrixMultiply(temp, XMMatrixScaling(ResourceManager::GetPBRPtr()[i].scale.x, ResourceManager::GetPBRPtr()[i].scale.y, ResourceManager::GetPBRPtr()[i].scale.z));
		XMFLOAT3 pos = ResourceManager::GetPBRPtr()[i].position;
		temp = XMMatrixMultiply(temp, XMMatrixTranslation(pos.x, pos.y, pos.z));
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
		HRESULT result = pbr_renderer_resources.context->Map(pbr_renderer_resources.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
		memcpy(gpuBuffer.pData, &WORLD, sizeof(WORLD));
		pbr_renderer_resources.context->Unmap(pbr_renderer_resources.constantBuffer.Get(), 0);
		// Connect constant buffer to the pipeline
		ID3D11Buffer* modelCBuffers[] = { pbr_renderer_resources.constantBuffer.Get() };
		pbr_renderer_resources.context->VSSetConstantBuffers(0, 1, modelCBuffers);
		// Model stuff

		// Render stuff
		// sET THE PIPELINE
		UINT strides[] = { sizeof(Vertex) };
		UINT offsets[] = { ResourceManager::GetPBRPtr()[i].desc.base_vertex_location * sizeof(Vertex) };
		ID3D11Buffer* moelVertexBuffers[] = { ResourceManager::GetVertexBuffer().Get() };
		pbr_renderer_resources.context->IASetVertexBuffers(0, 1, moelVertexBuffers, strides, offsets);
		int ioffset = ResourceManager::GetPBRPtr()[i].desc.index_start * 4;
		pbr_renderer_resources.context->IASetIndexBuffer(ResourceManager::GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, ioffset);

		// Set sampler
		pbr_renderer_resources.context->PSSetSamplers(0, 1, ResourceManager::GetPBRPtr()[i].sampler.GetAddressOf());

		ID3D11ShaderResourceView* resource_views[] = {
			ResourceManager::GetPBRPtr()[i].albedo.Get(),
			ResourceManager::GetPBRPtr()[i].normal.Get(),
			ResourceManager::GetPBRPtr()[i].metallic.Get(),
			ResourceManager::GetPBRPtr()[i].roughness.Get(),
			ResourceManager::GetPBRPtr()[i].ambient_occlusion.Get(),
		};

		pbr_renderer_resources.context->PSSetShaderResources(0, 5, resource_views);

		pbr_renderer_resources.context->VSSetShader(ResourceManager::GetPBRPtr()[i].vertexShader.Get(), 0, 0);
		pbr_renderer_resources.context->PSSetShader(ResourceManager::GetPBRPtr()[i].pixelShader.Get(), 0, 0);
		pbr_renderer_resources.context->IASetInputLayout(ResourceManager::GetPBRPtr()[i].vertexBufferLayout.Get());

		int istart = ResourceManager::GetPBRPtr()[i].desc.index_start;
		int ibase = ResourceManager::GetPBRPtr()[i].desc.base_vertex_location;
		int icount = ResourceManager::GetPBRPtr()[i].desc.index_count;
		pbr_renderer_resources.context->DrawIndexed(icount, 0, 0);
	}

	// Send the lights to constant buffer
	D3D11_MAPPED_SUBRESOURCE lightSub;
	HRESULT result = pbr_renderer_resources.context->Map(pbr_renderer_resources.lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightSub);
	assert(!FAILED(result));
	memcpy(lightSub.pData, ResourceManager::GetLightDataPtr(), sizeof(Light) * ResourceManager::GetLightCount());
	pbr_renderer_resources.context->Unmap(pbr_renderer_resources.lightBuffer.Get(), 0);
	// Connect constant buffer to the pipeline
	ID3D11Buffer* lightCbuffers[] = { pbr_renderer_resources.lightBuffer.Get() };
	pbr_renderer_resources.context->PSSetConstantBuffers(0, 1, lightCbuffers);

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
