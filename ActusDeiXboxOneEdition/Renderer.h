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