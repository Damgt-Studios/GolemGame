#pragma once

#include "pch.h"

#include <vector>
#include <queue>

#include <string>
#include <unordered_map>
#include "ADPhysics.h"
#include "ADQuadTree.h"
#include "MeshLoader.h"
#include "ADEventSystem.h"
#include "XTime.h"

#ifndef AD_MEMORY_DEFAULT
#include "ADMemoryManager.h"
#endif // !1

using namespace DirectX;
using namespace Microsoft::WRL;

// Global type definitions
typedef unsigned long long AD_ULONG;

enum ENGINE_STATE {
	GAMEPLAY = 0,
	PAUSED,
	CLOSE
};

enum class ADResourceType {
	PhongRenderer = 0,
	Light,
	Gem,
	Destructable,
	Trigger,
	Enemy
};

enum class RotationType
{
	xyz, xzy, yxz, yzx, zxy, zyx
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


		//		struct Model
		//		{
		//#ifdef AD_MEMORY_DEFAULT
		//			std::vector<Vertex> vertices;
		//			std::vector<int> indices;
		//#else
		//			ADVector<Vertex> vertices;
		//			ADVector<int> indices;
		//#endif
		//
		//			XMFLOAT3 position;
		//			XMFLOAT3 rotation;
		//			XMFLOAT3 scale;
		//
		//			PBRVertexBufferDesc desc;
		//
		//			ComPtr<ID3D11Buffer> vertexBuffer;
		//			ComPtr<ID3D11Buffer> indexBuffer;
		//
		//			ComPtr<ID3D11VertexShader> vertexShader;
		//			ComPtr<ID3D11PixelShader> pixelShader;
		//
		//			ComPtr<ID3D11InputLayout> vertexBufferLayout;
		//
		//			// Texture stuff
		//			ComPtr<ID3D11SamplerState> sampler;
		//
		//			ComPtr<ID3D11ShaderResourceView> albedo;
		//			ComPtr<ID3D11ShaderResourceView> normal;
		//			ComPtr<ID3D11ShaderResourceView> roughness;
		//			ComPtr<ID3D11ShaderResourceView> metallic;
		//			ComPtr<ID3D11ShaderResourceView> ambient_occlusion;
		//		};

		struct ADTexture
		{
			bool shared = true;
			ComPtr<ID3D11ShaderResourceView> texture;
		};

		class SimpleModel
		{
		public:
			~SimpleModel()
			{
				if (!albedo->shared)
					delete albedo;
				if (!normal->shared)
					delete normal;
				if (!emissive->shared)
					delete emissive;
			}

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

			ADTexture* albedo;
			ADTexture* normal;
			ADTexture* emissive;

		};

		struct SimpleStaticModel : public SimpleModel
		{
#ifdef AD_MEMORY_DEFAULT
			std::vector<SimpleVertex> vertices;
			std::vector<int> indices;
			UINT instanceCount;
			std::vector<XMFLOAT3> positions;
			ComPtr<ID3D11Buffer> instanceBuffer;
#else
			ADVector<SimpleVertex> vertices;
			ADVector<int> indices;
#endif
		};

		struct SimpleAnimModel : public SimpleModel
		{
#ifdef AD_MEMORY_DEFAULT
			std::vector<SimpleVertexAnim> vertices;
			std::vector<int> indices;
			std::vector<bones> skeleton;
			std::vector<XMMATRIX> inverse_transforms;
			std::vector<anim_clip> animations;
			int animationNewIndex = 0;
			int animationCurrentIndex = 0;

			bool animationChange = false;

#else
			ADVector<SimpleVertexAnim> vertices;
			ADVector<int> indices;
			//ADVector<bones> skeleton;
			ADVector<XMMATRIX> inverse_transforms;
			ADVector<anim_clip> animations;
#endif

			float elapsedTime = 0;
			int counter = 0;

			float modifier = 1;

			ComPtr<ID3D11Buffer> animationBuffer;
			void SetAnimCurrentState(int index)
			{
				if (animationNewIndex != index)
				{
					animationNewIndex = index;
					animationChange = true;
				}
			}

			std::vector<XMMATRIX> UpdateAnimation(float delta_time)
			{
				std::vector<XMMATRIX> positions;


				positions.resize(animations[animationCurrentIndex].frames[0].jointsMatrix.size());

				elapsedTime += delta_time;

				if (animations.size() > 0)
				{
					modifier = animations[animationCurrentIndex].frames.size();

					//Animations

					if (elapsedTime >= 1.0f / modifier)
					{
						counter++;
						if (counter == animations[animationCurrentIndex].frames.size())
						{
							counter = 1;
						}

						elapsedTime = 0;
					}

					for (int i = animations[animationCurrentIndex].frames[counter].jointsMatrix.size() - 1; i >= 0; --i)
					{
						int nextKeyframe = 0;

						if (counter + 1 < animations[animationCurrentIndex].frames.size())
						{
							nextKeyframe = counter + 1;
						}
						else if (counter + 1 == animations[animationCurrentIndex].frames.size())
						{
							nextKeyframe = 1;
						}
						XMMATRIX current = animations[animationCurrentIndex].frames[counter].jointsMatrix[i];
						XMMATRIX next = animations[animationCurrentIndex].frames[nextKeyframe].jointsMatrix[i];

						if (animationChange == true)
						{
							current = animations[animationCurrentIndex].frames[counter].jointsMatrix[i];
							next = animations[animationNewIndex].frames[0].jointsMatrix[i];

							animationCurrentIndex = animationNewIndex;
							counter = 0;
							animationChange = false;
							i = animations[animationCurrentIndex].frames[counter].jointsMatrix.size() - 1;

						}

						XMMATRIX Tween = ADMath::MatrixLerp(current, next, elapsedTime * modifier);

						XMMATRIX matrixToGPU = XMMatrixMultiply(inverse_transforms[i], Tween);
						positions[i] = matrixToGPU;
					}

				}
				animationChange = false;
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

		struct LVP
		{
			XMFLOAT4X4 ViewMatrix;
			XMFLOAT4X4 ProjectionMatrix;
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
			XMFLOAT4 position, lightDirection, diffuse, ambient;
			unsigned int lightType; float lightRadius;
			float diffuseIntensity, ambientIntensity;
		};
	}

	namespace ADGameplay
	{

		enum OBJECT_PHYSICS_TYPE
		{
			TRIGGER, COLLIDABLE, STATIC
		};

		class Stat
		{
		public:
			std::string name = "";
			std::string eventName = "";
			std::string eventMessage="";
			int eventDataType = 0;
			int currentValue = 0;
			int maxValue = 0;
			int minValue = 0;
			int eventValue = 0;



			void Set(int _val)
			{
				currentValue = _val;

				if (eventName != "")
				{
					if (eventValue == -1 || currentValue == eventValue)
					{
						if (eventDataType == 0)
						{
							ADEvents::ADEventSystem::Instance()->SendEvent(eventName, (void*)(currentValue));	//Allow me to send an int or string based on dd.
						}
						else if (eventDataType == 1)
						{
							eventMessage.clear();
							eventMessage.append(to_string(currentValue));
							ADEvents::ADEventSystem::Instance()->SendEvent(eventName, static_cast<void*>(&eventMessage));
						}
					}
				}
			}

		};

		class StatSheet
		{
			std::unordered_map<std::string, Stat> stats;
		public:
			StatSheet() {};
			~StatSheet() {};
			void AddStat(std::string _name)
			{
				Stat stat;
				stats[_name] = stat;
			};
			void SetMin(std::string _name, int _val)
			{
				stats[_name].minValue = _val;
			};
			void SetMax(std::string _name, int _val)
			{
				stats[_name].maxValue = _val;
			};
			void SetCurrent(std::string _name, int _val)
			{
				stats[_name].currentValue = _val;
			};
			void SetEvent(std::string _name, std::string _eventName)
			{
				stats[_name].eventName = _eventName;
			};
			void SetEventType(std::string _name, int _eventDT)
			{
				stats[_name].eventDataType = _eventDT;
			};
			void SetEventVal(std::string _name, int _val)
			{
				stats[_name].eventValue = _val;
			};
			Stat* RequestStats(std::string _name)
			{
				auto tFoundIt = stats.find(_name);
				if (tFoundIt == stats.end())
					return nullptr;
				return &stats[_name];
			};
		};


		class Effect
		{
		public:
			std::string name;
			std::vector<Stat*> targetedStats;
			float tickDuration;
			float currentTick;
			float tickTimer;
			bool isFinished;
			bool tickOnEnter;
			bool tickOnExit;
			UINT tickCount;
			UINT sourceID;
			UINT instanceID;

			virtual UINT OnApply(StatSheet* _targetsStatSheet) { return 0; };

			virtual void Update(float _deltaTime)
			{
				if (tickTimer > 0)
				{
					tickTimer -= _deltaTime;
					if (tickTimer <= 0)
					{
						++currentTick;
						if (currentTick <= tickCount)
						{
							Tick();
							if (currentTick == tickCount)
							{
								isFinished = true;
							}
							else
							{
								tickTimer = tickDuration;
							}
						}
						else
						{
							isFinished = true;
						}
					}
				}
			};
			virtual UINT Tick() { return 0; };
			virtual UINT OnExit() { return 0; };
			std::unique_ptr<Effect> clone() const
			{
				return std::unique_ptr<Effect>(this->clone_impl());
			}
		protected:
			virtual Effect* clone_impl() const { return nullptr; };
		};


		class GameObject
		{
		public:
			std::vector<std::unique_ptr<Effect>> effects;

			GameObject()
			{
				transform = postTransform = XMMatrixIdentity();
				gamePlayType = -1;
			}
			virtual ~GameObject() = default;

			virtual void Render() {};

			virtual void Update(float delta_time) {};

			virtual void CheckCollision(GameObject* obj) {};

			virtual StatSheet* GetStatSheet() { return nullptr; };

			bool HasEffectID(UINT _sourceID, UINT _instanceID)
			{
				for (int i = 0; i < effects.size(); i++)
				{
					if (effects[i].get()->sourceID == _sourceID)
					{
						if (effects[i].get()->instanceID == _instanceID)
						{
							return true;
						}
					}
				}
				return false;
			};


			virtual void Remove()
			{
				active = false;
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
			void SetMeshID(AD_ULONG id) { meshID = id; has_mesh = true; };
			AD_ULONG GetMeshId() { return meshID; }
			// Rotations in degrees
			void SetRotation(XMFLOAT3 rotation, RotationType type = RotationType::xyz)
			{
				XMMATRIX X = XMMatrixRotationX(XMConvertToRadians(rotation.x)),
					Y = XMMatrixRotationY(XMConvertToRadians(rotation.y)),
					Z = XMMatrixRotationZ(XMConvertToRadians(rotation.z));

				switch (type)
				{
				case RotationType::xyz:
					transform = Z * Y * X * transform;
					break;
				case RotationType::xzy:
					transform = Y * Z * X * transform;
					break;
				case RotationType::yxz:
					transform = Z * X * Y * transform;
					break;
				case RotationType::yzx:
					transform = X * Z * Y * transform;
					break;
				case RotationType::zxy:
					transform = Y * X * Z * transform;
					break;
				case RotationType::zyx:
					transform = X * Y * Z * transform;
					break;
				}
			}

			void SetRotationMatrix(XMMATRIX newRot)
			{
				transform.r[0] = newRot.r[0];
				transform.r[1] = newRot.r[1];
				transform.r[2] = newRot.r[2];
			}
			void RotationYBasedOnView(XMMATRIX& cam, float angle, float PI)
			{
				XMVECTOR camRot = GetRotation(cam);
				bool isStraight = false;
				if (((angle > -0.75f && angle < 0.75f) || ((angle > 2.39f && angle < 3.89f) || (angle < -2.39f && angle > -3.89f))) && ((camRot.m128_f32[1] > 1.57 && camRot.m128_f32[1] < 1.58)|| (camRot.m128_f32[1] > -0.1f && camRot.m128_f32[1] < 0.1f)))
					isStraight = true;
				angle *= (180.0f / PI);

				cam = XMMatrixInverse(nullptr, cam);
				XMVECTOR cameulerAngles = GetRotation(cam);
				cam = XMMatrixInverse(nullptr, cam);
				cameulerAngles.m128_f32[1] *= (180.0f / PI);

				if (!isStraight)
					angle += -cameulerAngles.m128_f32[1];
				angle *= (PI / 180.0f);

				XMMATRIX RotationY = XMMatrixRotationAxis({ 0,1,0 }, angle);

				SetRotationMatrix(RotationY);

			}
			XMFLOAT3 GetRotationDegrees()
			{
				return rot;
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
					Yaw = atan2(-transform.r[2].m128_f32[0], transform.r[0].m128_f32[0]);
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
		protected:
			XMFLOAT3 rot;

		public:
			bool active = true;
			float safeRadius = 5.0f;
			float avoidRadius = 5.0f;
			float attackRadius = 5.0f;
			float desirability = 0;

			UINT physicsType;
			UINT gamePlayType;
			UINT team = 0;
			UINT actionLevel = 0;
			AD_ULONG meshID;
			//OBJECT_DEFENSE defenseType;
			XMFLOAT4 Velocity = { 0,0,0,0 };
			XMMATRIX transform;
			XMMATRIX postTransform;

			ADPhysics::Collider* colliderPtr = nullptr;
			ADPhysics::PhysicsMaterial pmat = ADPhysics::PhysicsMaterial();

		public:
			bool has_mesh = false;
			bool has_stats = false;
		};

		struct CollisionPacket
		{
			ADResource::ADGameplay::GameObject* A;
			ADResource::ADGameplay::GameObject* B;
			ADPhysics::Manifold m;

			CollisionPacket() = delete;
			CollisionPacket(GameObject* a, GameObject* b, ADPhysics::Manifold& manifold) : A(a), B(b), m(manifold) {};
		};

		struct ClampingArgs
		{
			GameObject* golem;
			QuadTree<ADPhysics::Triangle>* tree;
			float time;
		};

		//If multiple instances will select one out of all of them and use only that one. 
		//Not sure if extra ones still take up memory. Don't think they do
		__declspec(selectany) std::queue<CollisionPacket> collisionQueue;

		static void ResolveCollisions()
		{
			while (!collisionQueue.empty()) {
				CollisionPacket current = collisionQueue.front();
				collisionQueue.pop();

				if (current.A->physicsType == (int)OBJECT_PHYSICS_TYPE::TRIGGER && current.B->physicsType == (int)OBJECT_PHYSICS_TYPE::TRIGGER)
					continue;
				//If the object is of the object type STATIC it will only apply a velocty change to the other objects.
				//Mainly will be used for Spyro against Ground, Enemies against Ground, Chests and Gems against Spyro, etc.
				if (current.B->physicsType == (int)OBJECT_PHYSICS_TYPE::STATIC || current.B->physicsType == (int)OBJECT_PHYSICS_TYPE::TRIGGER)
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
				else if (current.A->physicsType == (int)OBJECT_PHYSICS_TYPE::STATIC || current.A->physicsType == (int)OBJECT_PHYSICS_TYPE::TRIGGER)
				{
					XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
					const ADPhysics::PhysicsMaterial temp = ADPhysics::PhysicsMaterial(0, 0, 0);
					XMFLOAT4 bVelocity = current.B->Velocity;
					const ADPhysics::PhysicsMaterial bMat = current.B->pmat;

					VelocityImpulse(tempV, temp, bVelocity, bMat, current.m);

					(*current.B).Velocity = bVelocity;
					(*current.B).pmat = bMat;

					PositionalCorrection(tempV, temp, (XMFLOAT4&)(current.B->transform.r[3]), current.B->pmat, current.m);
				}
				//Otherwise it will apply a velocity change against both objects. Not sure how often this will be used but it is here for now.
				else
				{
					//XMFLOAT4 aTemp = current.A->Velocity, bTemp = current.B->Velocity;
					//const ADPhysics::PhysicsMaterial aMat = current.A->pmat, bMat = current.B->pmat;
					//VelocityImpulse(aTemp, aMat, bTemp, bMat, current.m);
					//(*current.A).Velocity = aTemp; (*current.B).Velocity = bTemp;
					//(*current.A).pmat = aMat; (*current.B).pmat = bMat;
					PositionalCorrection((XMFLOAT4&)current.A->transform.r[3], current.A->pmat, (XMFLOAT4&)current.B->transform.r[3], current.B->pmat, current.m);
				}
			}
		}

		static void ResolveCollisionsWrapper(void* args, int index)
		{
			ResolveCollisions();
		}


		static bool GroundClamping(GameObject* obj, QuadTree<ADPhysics::Triangle>* tree, float delta_time)
		{
			std::vector<ADQuadTreePoint<ADPhysics::Triangle>> ground = tree->Query(ADQuad(obj->transform.r[3].m128_f32[0], obj->transform.r[3].m128_f32[2], 50, 50));

			ADPhysics::Segment line = ADPhysics::Segment((XMFLOAT3&)(obj->transform.r[3] + XMVectorSet(0, 100, 0, 0)), (XMFLOAT3&)(obj->transform.r[3] - XMVectorSet(0, 100, 0, 0)));

			for (unsigned int i = 0; i < ground.size(); i++)
			{
				ADPhysics::Manifold m;
				if (LineSegmentToTriangle(line, *ground[i].data, m))
				{
					obj->transform.r[3] = (XMVECTOR&)m.ContactPoint;
					return true;
				}
			}

			return false;
		}

		static void GroundClampingWrapper(void* args, int index)
		{
			ClampingArgs* temp = static_cast<ClampingArgs*>(args);
			GroundClamping(temp->golem, temp->tree, temp->time);
		}
	}
};
