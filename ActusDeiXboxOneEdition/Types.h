#pragma once

#include "pch.h"

#include <vector>

#include "ADPhysics.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// Global type definitions
typedef unsigned long long AD_ULONG;

enum class ADResourceType {
	PhongRenderer = 0,
	Light,
	Gem,
	Destructable,
	Trigger,
	Enemy
};

namespace ADResource
{
	namespace ADRenderer
	{
		// Primitives
		struct Vertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT3 Tangent;
			XMFLOAT3 Bitangent;
			XMFLOAT2 Tex;
		};

		struct Header
		{
			int indexcount, vertexcount;
			int indexstart, vertexstart;
			char t_albedo[256];
			char t_normal[256];
			char t_metallic[256];
			char t_roughness[256];
			char t_ambient_occlusion[256];
			char emissive[256];
		};

		struct Model
		{
			std::vector<Vertex> vertices;
			std::vector<int> indices;

			XMFLOAT3 position;

			ComPtr<ID3D11Buffer> vertexBuffer;
			ComPtr<ID3D11Buffer> indexBuffer;

			ComPtr<ID3D11VertexShader> vertexShader;
			ComPtr<ID3D11PixelShader> pixelShader;

			ComPtr<ID3D11InputLayout> vertexBufferLayout;

			// Texture stuff
			ComPtr<ID3D11SamplerState> sampler;

			ComPtr<ID3D11ShaderResourceView> albedo;
			ComPtr<ID3D11ShaderResourceView> normal;
			ComPtr<ID3D11ShaderResourceView> roughness;
			ComPtr<ID3D11ShaderResourceView> metallic;
			ComPtr<ID3D11ShaderResourceView> ambient_occlusion;
		};

		struct WVP
		{
			XMFLOAT4X4 WorldMatrix;
			XMFLOAT4X4 ViewMatrix;
			XMFLOAT4X4 ProjectionMatrix;
			XMFLOAT4   CameraPosition;
		};

		enum class LIGHTTYPE
		{
			DIRECTIONAL = 0,
			POINT,
			SPOT,
			CAPSULE
		};

		struct Light
		{
			XMFLOAT4 position, lightDirection;
			XMFLOAT4 ambientUp, ambientDown, diffuse, specular;
			unsigned int lightType;
			float lightRadius;
			float cosineInnerCone, cosineOuterCone;
			float ambientIntensityUp, ambientIntensityDown, diffuseIntensity, specularIntensity;
			float lightLength, p1, p2, p3;
		};

		// Renderer Types
		struct PBRRendererResources
		{
			ComPtr<ID3D11Device1> device;
			ComPtr<ID3D11DeviceContext1> context;
			ComPtr<IDXGISwapChain1> chain;

			ComPtr<ID3D11RenderTargetView> render_target_view;
			D3D11_VIEWPORT viewport;

			// States
			ComPtr<ID3D11RasterizerState1> defaultRasterizerState;

			// Z Buffer
			ComPtr<ID3D11Texture2D> zBuffer;
			ComPtr<ID3D11DepthStencilView> depthStencil;

			// Cbuffers
			ComPtr<ID3D11Buffer> constantBuffer;
			ComPtr<ID3D11Buffer> lightBuffer;
		};
	}

	namespace AD_AI
	{

		enum AITYPES
		{
			RUNNER, STRIKER, SHOOTER, BOSS, COUNT
		};

		enum BEHAVIORS
		{
			IDLE, WANDER, DIRECT, WAYPOINT, ATTACK, SHOOT, DEATH, GUARD, TAUNT
		};

		class AI
		{
		public:
			void Update()
			{
				//Traverse Behavior Tree and Enact Behavior
				//Updates the postTransform of a Game Object.
				//Communicates with EventSystem to spawn projectiles and Gems.
			}
		};

		//For constructing AI's from binary.
		class AIData
		{
			int ObjectType;

			AI ToAI()
			{
				switch (ObjectType)
				{
				case AITYPES::RUNNER:

					break;
				case AITYPES::STRIKER:

					break;
				case AITYPES::SHOOTER:

					break;
				case AITYPES::BOSS:

					break;
				default:
					break;
				}
			}
		};

	}

	namespace ADGameplay
	{
		//This could be part of the player object.  You guys decide how you want the event system to determine things.
		struct GameState
		{
			int Health;
			int Gems;
		};

		enum OBJECT_TYPE
		{
			PLAYER, ENEMY, DESTRUCTABLE, GEM, HITBOX, TRIGGER  //We should replace trigger with the types of trigger to avoid an extra var for trigger type.
		};
		enum OBJECT_DEFENSE
		{
			NONE, IGNORE_RAM, IGNORE_FIRE, INVULNERABLE
		};
		enum DAMAGE_TYPE
		{
			RAM = 1, FIRE = 2
		};

		class GameObject
		{
		public:
			//TODO: Whittington.
			virtual void Render() {};

			virtual void Damage(DAMAGE_TYPE damageType)
			{
				if (type == DESTRUCTABLE)
				{
					if (defenseType != INVULNERABLE && defenseType != damageType)
					{
						Remove();
					}
				}
			};
			virtual void Remove()
			{
				//Drop your Gems and get return to pool,  You're done son.
			}

		public:
			bool active;
			int type;
			int GemCount;
			AD_ULONG meshID;
			OBJECT_DEFENSE defenseType;
			XMFLOAT4X4 transform;
			XMFLOAT4X4 postTransform;
			//ADPhysics::Collider collider;
		};

		class Enemy : GameObject
		{
			AD_AI::AI ai;
			int health;
			void Update()
			{
				ai.Update();
			}
			void Damage(DAMAGE_TYPE damageType) override
			{
				if (defenseType != INVULNERABLE && defenseType != damageType)
				{
					health--;
					if (health < 1)
					{
						Remove();
					}
				}
			};
		};
	}

	


	//TODO Gage  (This is rough draft crap made by Dan.  Just do your thing.)
	namespace ADPhysics
	{
		struct Collider
		{
			enum TYPE
			{
				AABB, SPHERE
			};

			int type;
			int centerAnchor;
			XMFLOAT3 sizeOffsets;
			XMFLOAT3 positionOffset;
		};

	}
}