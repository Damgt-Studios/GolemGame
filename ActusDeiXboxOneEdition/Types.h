#pragma once

#include "pch.h"

#include <vector>

#include <string>
#include "ADPhysics.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// Global type definitions
typedef unsigned long long AD_ULONG;

enum class ENGINE_STATE {
	GAMEPLAY = 0,
	PAUSED,
	MENUSCREEN
};

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

		struct PBRVertexBufferDesc
		{
			unsigned int index_start;
			unsigned int index_count;
			unsigned int index_offset;
			unsigned int vertex_count;
			unsigned int base_vertex_location;

			bool wireframe_mode;
		};

		struct Model
		{
			std::vector<Vertex> vertices;
			std::vector<int> indices;

			XMFLOAT3 position;
			XMFLOAT3 rotation;
			XMFLOAT3 scale;

			PBRVertexBufferDesc desc;

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
		};
	}

	namespace AD_UI
	{

		struct UIVertex
		{
			XMFLOAT4 Color;     //If a pixel is white hue (grey/etc) it will bleech it this color.  Starts white.
			XMFLOAT3 Pos;
			XMFLOAT2 Tex;
		};

		struct UIHeader
		{
			char t_albedo[256];
		};

		struct QuadData
		{
			float x, y;
			float quadWidth;
			float quadHeight;
			float minU;
			float maxU;
			float minV;
			float maxV;
		};

		struct AnimData2d
		{
			UINT frameCount;
			UINT startFrame;
			UINT columns;
			float fps;
		};

		struct TextLabel
		{
			bool visible;
			XMFLOAT2 position;
			std::string output;
		};

		class UIComponent
		{
		public:
			UINT quadCount = 0;
			bool active;
			bool visible;
			bool controlFocus;
			virtual void Initialize() {};
			virtual void Update(float delta_time) {};
			virtual int ProcessInput() { return 0; };
			virtual QuadData** GetQuads() { return nullptr; };
			virtual QuadData* GetQuad() { return nullptr; };
			virtual UINT GetQuadCount() { return quadCount; };
			virtual TextLabel* GetText() { return nullptr; };
			virtual void Enable() { visible = true; active = true; };
			virtual void Disable() { visible = false; active = false; };
			virtual void CleanUp() {};
		};

		class Overlay2D
		{
		private:
			bool dynamic = false;
			UINT myId;

		public:
			bool active = true;
			bool visible = true;
			std::vector<UINT> componentIDs;
			std::vector<ADResource::AD_UI::UIVertex> vertices;
			std::vector<int> indices;
			ComPtr<ID3D11Buffer> vertexBuffer;
			ComPtr<ID3D11Buffer> indexBuffer;
			
			Overlay2D(UINT _myID, bool _visible = false, bool _active = false, bool _dynamic = true)
			{
				myId = _myID;
				visible = _visible;
				active = _active;
				dynamic = _dynamic;
			};

			void AddComponent(UINT _compID)
			{
				componentIDs.push_back(_compID);
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

			bool IsDynamic()
			{
				return dynamic;
			}

			UINT GetID()
			{
				return myId;
			}
		};

		struct UIRendererResources
		{
			std::vector<ADResource::AD_UI::UIVertex> vertices;
			std::vector<int> indices;

			ComPtr<ID3D11Buffer> vertexBuffer;
			ComPtr<ID3D11Buffer> indexBuffer;

			ComPtr<ID3D11VertexShader> vertexShader;
			ComPtr<ID3D11PixelShader> pixelShader;

			ComPtr<ID3D11InputLayout> vertexBufferLayout;

			ComPtr<ID3D11ShaderResourceView> uiTextures;

			// Cbuffers - Orthogonal projection matrix for 2D
			ComPtr<ID3D11Buffer> constantBuffer;
			XMFLOAT4X4 ortoprojMatrix;

			// For drawing to the front by turning off the zbuffer.
			ComPtr<ID3D11DepthStencilState> depthStencilState;
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
			PLAYER, ENEMY, DESTRUCTABLE, GEM, HITBOX, TRIGGER  
			// We should replace trigger with the types of trigger to avoid an extra var for trigger type.
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
			GameObject()
			{
				transform = postTransform = XMMatrixIdentity();
			}

			//TODO: Whittington.
			virtual void Render() {};

			virtual void Update(float delta_time) {};

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

			// Nesessary utilities
			virtual void SetPosition(XMFLOAT3 pos)
			{
				transform.r[3].m128_f32[0] = pos.x;
				transform.r[3].m128_f32[1] = pos.y;
				transform.r[3].m128_f32[2] = pos.z;
			}

			virtual void AddToPositionVector(XMFLOAT3 pos)
			{
				transform.r[3].m128_f32[0] += pos.x;
				transform.r[3].m128_f32[1] += pos.y;
				transform.r[3].m128_f32[2] += pos.z;
			}

			XMFLOAT3 GetPosition()
			{
				XMFLOAT3 pos;
				XMVECTOR posv = transform.r[3];
				XMStoreFloat3(&pos, posv);

				return pos;
			}

		public:
			// Setters/ Getters
			void SetMeshID(AD_ULONG id) { meshID = id; };
			AD_ULONG GetMeshId() { return meshID; }
			// Rotations in degrees
			void SetRotation(XMFLOAT3 rotation)
			{
			}

			void SetScale(XMFLOAT3 scale)
			{
				transform = XMMatrixMultiply(transform, XMMatrixScaling(scale.x, scale.y, scale.z));
			}

			void GetWorldMatrix(XMMATRIX& mat)
			{
				mat = transform;
			}

		public:
			bool active;
			int type;
			int GemCount;
			AD_ULONG meshID;
			OBJECT_DEFENSE defenseType;
			XMMATRIX transform;
			XMMATRIX postTransform;

		public:
			bool has_mesh = false;
		};

		class Enemy : public GameObject
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