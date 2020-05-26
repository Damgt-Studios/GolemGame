#pragma once

#include "pch.h"

#include <vector>
#include <queue>

#include <string>
#include "ADPhysics.h"
#include "MeshLoader.h"

#ifndef AD_MEMORY_DEFAULT
#include "ADMemoryManager.h"
#endif // !1

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
#ifdef AD_MEMORY_DEFAULT
			std::vector<Vertex> vertices;
			std::vector<int> indices;
#else
			ADVector<Vertex> vertices;
			ADVector<int> indices;
#endif

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

		struct SimpleModel
		{
			bool animated = false;

			XMFLOAT3 position;
			XMFLOAT3 rotation;
			XMFLOAT3 scale;

			ComPtr<ID3D11Buffer> vertexBuffer;
			ComPtr<ID3D11Buffer> indexBuffer;

			ComPtr<ID3D11VertexShader> vertexShader;
			ComPtr<ID3D11PixelShader> pixelShader;

			ComPtr<ID3D11InputLayout> inputLayout;

			// Texture stuff
			ComPtr<ID3D11SamplerState> sampler;

			ComPtr<ID3D11ShaderResourceView> albedo;
			ComPtr<ID3D11ShaderResourceView> normal;
			ComPtr<ID3D11ShaderResourceView> emissive;
		};

		struct SimpleStaticModel : public SimpleModel
		{
#ifdef AD_MEMORY_DEFAULT
			std::vector<SimpleVertex> vertices;
			std::vector<int> indices;
#else
			ADVector<SimpleVertex> vertices;
			ADVector<int> indices;
#endif
		};

		struct SimpleAnimModel :  public SimpleModel
		{
#ifdef AD_MEMORY_DEFAULT
			std::vector<SimpleVertexAnim> vertices;
			std::vector<int> indices;
			std::vector<bones> skeleton;
			std::vector<XMMATRIX> inverse_transforms;
			std::vector<anim_clip> animations;

#else
			ADVector<SimpleVertexAnim> vertices;
			ADVector<int> indices;
			ADVector<bones> skeleton;
			ADVector<XMMATRIX> inverse_transforms;
			ADVector<anim_clip> animations;
#endif
			float elapsedTime = 0;
			int counter = 0;

			float modifier = 1;

			ComPtr<ID3D11Buffer> animationBuffer;

			std::vector<XMMATRIX> UpdateAnimation(float delta_time)
			{
				std::vector<XMMATRIX> positions;
				positions.resize(animations[0].frames[0].jointsMatrix.size());

				elapsedTime += delta_time;

				if (animations.size() > 0)
				{
					modifier = animations[0].frames.size();

					//Animations

					if (elapsedTime >= 1.0f / modifier)
					{
						counter++;
						if (counter == animations[0].frames.size())
						{
							counter = 1;
						}

						elapsedTime = 0;
					}

					for (int i = animations[0].frames[counter].jointsMatrix.size() - 1; i >= 0; --i)
					{
						int nextKeyframe = 0;

						if (counter + 1 < animations[0].frames.size())
						{
							nextKeyframe = counter + 1;
						}
						else if (counter + 1 == animations[0].frames.size())
						{
							nextKeyframe = 1;
						}

						XMMATRIX current = animations[0].frames[counter].jointsMatrix[i];
						XMMATRIX next = animations[0].frames[nextKeyframe].jointsMatrix[i];

						XMMATRIX Tween = ADMath::MatrixLerp(current, next, elapsedTime * modifier);

						XMMATRIX matrixToGPU = XMMatrixMultiply(inverse_transforms[i], Tween);
						positions[i] = matrixToGPU;
					}
				}

				return positions;
			}
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
			char atlas[256];
		};

		class UIMessage
		{
		public:
			UIMessage() {};
			~UIMessage() {};

			UINT messageType;
			UINT controllerID;
			union
			{
				UINT number;
				std::string sentence;
				bool boolean;
			};
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
			float atlastID;
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
		private:
			UINT componentType;
		public:
			UINT quadCount = 0;
			UINT labelCount = 0;
			bool active;
			bool visible;
			bool controlFocus;
			bool requiresRefresh;
			virtual void Initialize() {};
			virtual void Update(float delta_time) {};
			virtual UIMessage* ProcessInput() { return nullptr; };
			virtual QuadData** GetQuads() { return nullptr; };
			virtual QuadData* GetQuad() { return nullptr; };
			virtual UINT GetQuadCount() { return quadCount; };
			virtual TextLabel* GetText() { return nullptr; };
			virtual TextLabel** GetTexts() { return nullptr; };
			virtual void Refresh() { requiresRefresh = true; };
			virtual void Enable() { visible = true; active = true; requiresRefresh = true; };
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
			//Start Quad matches up with componentID.  
			//Since some components pass up multiple quads this tells the system where the first quad for this component is in the vertices list so I can replace just that section.
			std::vector<UINT> startQuad;
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

			//void RefreshAll()
			//{
			//	for(int i = 0; i < componentIDs.size(); )
			//}

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
			ComPtr<ID3D11VertexShader> vertexShader;
			ComPtr<ID3D11PixelShader> pixelShader;

			ComPtr<ID3D11InputLayout> vertexBufferLayout;

			ComPtr<ID3D11ShaderResourceView> uiTextures;
			ComPtr<ID3D11ShaderResourceView> uiTextures2;

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
			PLAYER, ENEMY, DESTRUCTABLE, GEM, TRIGGER, STATIC
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
				active = false;
			}

			virtual void CheckCollision(GameObject* obj) {};

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

			void SetRotationMatrix(XMMATRIX newRot)
			{
				transform.r[0] = newRot.r[0];
				transform.r[1] = newRot.r[1];
				transform.r[2] = newRot.r[2];
			}
			void RotationYBasedOnView( XMMATRIX& cam,float angle, float PI)
			{
				angle *= (180.0f / PI);

				cam = XMMatrixInverse(nullptr, cam);
				XMVECTOR cameulerAngles = GetRotation(cam);
				cam = XMMatrixInverse(nullptr, cam);
				cameulerAngles.m128_f32[1] *= (180.0f / PI);

				angle += -cameulerAngles.m128_f32[1];
				angle *= (PI / 180.0f);
 
				XMMATRIX RotationY = XMMatrixRotationAxis({ 0,1,0 }, angle);

				SetRotationMatrix(RotationY);

			}
			XMVECTOR GetRotation()
			{
				float Yaw; float Pitch; float Roll;
				if (transform.r[0].m128_f32[0] == 1.0f)
				{
					Yaw = atan2f(transform.r[0].m128_f32[2], transform.r[2].m128_f32[3]);
					Pitch = 0;
					Roll = 0;

				}
				else if (transform.r[0].m128_f32[0] == -1.0f)
				{
					Yaw = atan2f(transform.r[0].m128_f32[2], transform.r[2].m128_f32[3]);
					Pitch = 0;
					Roll = 0;
				}
				else
				{
					Yaw = atan2(-transform.r[2].m128_f32[0], transform.r[0].m128_f32[0] );
					Pitch = asin(transform.r[1].m128_f32[0]);
					Roll = atan2(-transform.r[1].m128_f32[2], transform.r[1].m128_f32[1]);
				}
				return { Pitch, Yaw, Roll };
			}
			XMVECTOR GetRotation(XMMATRIX matrix)
			{
				float Yaw; float Pitch; float Roll;
				if (matrix.r[0].m128_f32[0] == 1.0f)
				{
					Yaw = atan2f(matrix.r[0].m128_f32[2], matrix.r[2].m128_f32[3]);
					Pitch = 0;
					Roll = 0;

				}
				else if (matrix.r[0].m128_f32[0] == -1.0f)
				{
					Yaw = atan2f(matrix.r[0].m128_f32[2], matrix.r[2].m128_f32[3]);
					Pitch = 0;
					Roll = 0;
				}
				else
				{
					Yaw = atan2(-matrix.r[2].m128_f32[0], matrix.r[0].m128_f32[0]);
					Pitch = asin(matrix.r[1].m128_f32[0]);
					Roll = atan2(-matrix.r[1].m128_f32[2], matrix.r[1].m128_f32[1]);
				}
				return { Pitch, Yaw, Roll };
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
			bool active = true;
			int type;
			int GemCount;
			AD_ULONG meshID;
			OBJECT_DEFENSE defenseType;
			XMFLOAT4 Velocity;
			XMMATRIX transform;
			XMMATRIX postTransform;

			ADPhysics::Collider* colliderPtr = nullptr;
			ADPhysics::PhysicsMaterial pmat = ADPhysics::PhysicsMaterial();

		public:
			bool has_mesh = false;
		};

		struct CollisionPacket
		{
			ADResource::ADGameplay::GameObject* A;
			ADResource::ADGameplay::GameObject* B;
			ADPhysics::Manifold m;

			CollisionPacket() = delete;
			CollisionPacket(GameObject* a, GameObject* b, ADPhysics::Manifold& manifold) : A(a), B(b), m(manifold) {};
		};

		//If multiple instances will select one out of all of them and use only that one. 
		//Not sure if extra ones still take up memory. Don't think they do
		__declspec(selectany) std::queue<CollisionPacket> collisionQueue;

		static void ResolveCollisions()
		{
			while (!collisionQueue.empty()) {
				CollisionPacket current = collisionQueue.front();
				collisionQueue.pop();
				//If the object is of the object type STATIC it will only apply a velocty change to the other objects.
				//Mainly will be used for Spyro against Ground, Enemies against Ground, Chests and Gems against Spyro, etc.
				if (current.B->type == (int)OBJECT_TYPE::STATIC)
				{
					XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
					const ADPhysics::PhysicsMaterial temp = ADPhysics::PhysicsMaterial(0, 0, 0);
					XMFLOAT4 aVelocity = current.A->Velocity;
					const ADPhysics::PhysicsMaterial aMat = current.A->pmat;

					VelocityImpulse(tempV, temp, aVelocity, aMat, current.m);

					(*current.A).Velocity = aVelocity;
					(*current.A).pmat = aMat;

					PositionalCorrection(tempV, temp, (XMFLOAT4&)(current.A->transform.r[3]), current.A->pmat, current.m);
				}
				//Otherwise it will apply a velocity change against both objects. Not sure how often this will be used but it is here for now.
				else 
				{
					XMFLOAT4 aTemp = current.A->Velocity, bTemp = current.B->Velocity;
					const ADPhysics::PhysicsMaterial aMat = current.A->pmat, bMat = current.B->pmat;
					VelocityImpulse(aTemp, aMat, bTemp, bMat, current.m);
					(*current.A).Velocity = aTemp; (*current.B).Velocity = bTemp;
					(*current.A).pmat = aMat; (*current.B).pmat = bMat;
					PositionalCorrection((XMFLOAT4&)current.A->transform.r[3], current.A->pmat, (XMFLOAT4&)current.B->transform.r[3], current.B->pmat, current.m);
				}
			}
		}

		static bool GroundClamping(GameObject* obj, std::vector<ADPhysics::Triangle>& ground, float delta_time) 
		{
			ADPhysics::Segment line = ADPhysics::Segment((XMFLOAT3&)(obj->transform.r[3] + XMVectorSet(0, 1, 0, 0)), (XMFLOAT3&)(obj->transform.r[3] - XMVectorSet(0, 1, 0, 0)));

			for (unsigned int i = 0; i < ground.size(); i++)
			{
				ADPhysics::Manifold m;
				if (LineSegmentToTriangle(line, ground[i], m))
				{
					obj->transform.r[3] = (XMVECTOR&)m.ContactPoint;
					obj->transform.r[3].m128_f32[1] += obj->colliderPtr->GetHeight() / 2;
					obj->Velocity = (XMFLOAT4&)(Float4ToVector(obj->Velocity) + (-Float4ToVector(obj->Velocity) * delta_time * 20));
					return true;
				}
			}

			return false;
		}
	}
};
