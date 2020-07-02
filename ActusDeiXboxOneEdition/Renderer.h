#pragma once

#include <string>
#include <fstream>

#include "Types.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "ADParticles.h"

using namespace ADResource::ADRenderer;

namespace ADResource
{
	namespace ADRenderer
	{
		class PBRRenderer
		{
		public:
			struct RendererResources
			{

				ComPtr<ID3D11Device1> device;
				ComPtr<ID3D11DeviceContext1> context;
				ComPtr<IDXGISwapChain1> chain;

				ComPtr<ID3D11RenderTargetView> render_target_view;
				D3D11_VIEWPORT viewport;

				// States
				ComPtr<ID3D11RasterizerState> defaultRasterizerState;
				ComPtr<ID3D11RasterizerState> wireframeRasterizerState;

				// Z Buffer
				ComPtr<ID3D11Texture2D> zBuffer;
				ComPtr<ID3D11DepthStencilView> depthStencil;

				// Cbuffers
				ComPtr<ID3D11Buffer> constantBuffer;
				ComPtr<ID3D11Buffer> lightBuffer;

				// Samplers
				ComPtr<ID3D11SamplerState> normal_sampler;

				//Shadows
				ComPtr<ID3D11Texture2D> shadowTexture;
				ComPtr<ID3D11ShaderResourceView> shadowView;
				ComPtr<ID3D11DepthStencilView> shadowDepth;
				ComPtr<ID3D11SamplerState> shadowSampler;

				ComPtr<ID3D11VertexShader> shadowVertex;
				ComPtr<ID3D11PixelShader> shadowPixel;

				RendererResources()
				{
					device = nullptr;
					context = nullptr;
					chain = nullptr;
					render_target_view = nullptr;
					defaultRasterizerState = nullptr;
					wireframeRasterizerState = nullptr;
					zBuffer = nullptr;
					depthStencil = nullptr;
					constantBuffer = nullptr;
					lightBuffer = nullptr;

					shadowTexture = nullptr;
					shadowView = nullptr;
					shadowDepth = nullptr;
					shadowSampler = nullptr;

					shadowVertex = nullptr;
					shadowPixel = nullptr;
				};
			};
			
			static RendererResources renderer_resources;
		public:
			PBRRenderer();
			~PBRRenderer() = default;

			// Lifecycle Methods
			bool Initialize();
			bool Update(FPSCamera* camera, OrbitCamera* ocamera);
			bool Render(FPSCamera* camera, OrbitCamera* ocamera, float delta_time);
			bool Frame();
			bool ShutDown();

			static RendererResources* GetRendererResources() { return &renderer_resources; };


		public:

			// Temp
		private:
			WVP WORLD;

			// Temp
			ComPtr<IDXGIDevice1> dxgiDevice;
		};
	};
}