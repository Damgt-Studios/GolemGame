#pragma once
#include "Types.h"


namespace ADAI
{

	class State
	{

	};

	class TestingState : public State
	{

	};

	class FlockingState : public State
	{
	public:
		//XMVECTOR personalTarget;
		XMVECTOR personalTarget;

	};

	class FlockingGroup
	{
	public:
		std::vector<ADResource::ADGameplay::GameObject*> flockers;
		std::vector<FlockingState*> flockerState;
		float commandDirectionalStrength = 0.00f;
		float returnDirectionalStrength = 0.00f;
		float alignmentDirectionalStrength = 0.00f;
		float targetCohesionStrength  = 1.0f;
		float cohesionStrength = 0.7f;
		float separationStrength = 1.0f;
		float flockRadius = 10.0;
		float maxSpeed = 0.1f;
		float moveSpeed = 0.01f;
		XMVECTOR averagePosition;
		XMVECTOR averageForward;
		XMVECTOR commandDirection;
		XMMATRIX* groupTarget; //Player
		XMVECTOR commandDestination;

		void SetCommandDirection(XMVECTOR _direction)
		{
			commandDirection = _direction;
			commandDirectionalStrength = 1.2f;
			returnDirectionalStrength = 0.f;
			commandDestination = groupTarget->r[3] - (commandDirection - groupTarget->r[3]);
		};

		void ReturnCall()
		{
			commandDirectionalStrength = 0.f;
			returnDirectionalStrength = 1.2f;
		};

		void AddUnitToGroup(ADResource::ADGameplay::GameObject* _flocker, FlockingState* _flockersState)
		{
			flockers.push_back(_flocker);
			flockerState.push_back(_flockersState);
		};

		void CalculateAverages()
		{
			averagePosition = {0,0,0};
			averageForward = {0,0,0};
			int count = flockers.size();
			for (int i = 0; i < count; ++i)
			{
				averagePosition += flockers[i]->transform.r[3];
				averageForward += XMLoadFloat4(&flockers[i]->Velocity);
			}
			XMVECTOR cnt = { count, count, count, count };
			XMVECTOR forward = { count, count, count, count };
			averagePosition /= cnt;
			averageForward /= forward;
			return;
		};

		XMVECTOR CalculateAlignmentAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMVECTOR af = averageForward * (1.f / maxSpeed);
			af = XMVector4Normalize(af);
			af *= alignmentDirectionalStrength;
			return af;
		}

		XMVECTOR CalculateDirectionalAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMFLOAT3 sigh = _object->GetPosition();
			XMVECTOR objectPos = XMLoadFloat3(&sigh);
			XMVECTOR velocity = commandDestination - objectPos;

			velocity = XMVector4Normalize(velocity);

			velocity *= commandDirectionalStrength;
			return velocity;
		}

		XMVECTOR CalculateReturnAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMFLOAT3 sigh = _object->GetPosition();
			XMVECTOR objectPos = XMLoadFloat3(&sigh);
			XMVECTOR velocity = groupTarget->r[3] - objectPos;

			velocity = XMVector4Normalize(velocity);

			velocity *= returnDirectionalStrength;
			return velocity;
		}


		XMVECTOR CalculateCohesionAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMFLOAT3 sigh = _object->GetPosition();
			XMVECTOR ap = XMLoadFloat3(&sigh);
			ap  = averagePosition - ap;

			XMFLOAT4 length;
			XMStoreFloat4(&length, XMVector4Length(ap));

			if (length.x < flockRadius)
			{
				ap = XMVector4Normalize(ap);
				ap *= (length.x / flockRadius);
			}
			else
			{
				ap = XMVector4Normalize(ap);
			}
			ap *= cohesionStrength;
			return ap;
		}

		XMVECTOR CalculateSeparationAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMVECTOR B = { 0, 0, 0 };
			// int distanceThreshold =

			XMFLOAT3 sigh;
			XMFLOAT3 sigh2;
			XMVECTOR distance;

			int count = flockers.size();
			for (int i = 0; i < count; ++i)
			{
				sigh = flockers[i]->GetPosition();
				sigh2 = _object->GetPosition();
				XMVECTOR enemyPOS = XMLoadFloat3(&sigh);
				XMVECTOR myPOS = XMLoadFloat3(&sigh2);
				distance = enemyPOS - myPOS;

				float threshold = flockers[i]->safeRadius + _object->safeRadius;

				XMFLOAT4 length;
				XMStoreFloat4(&length, XMVector4Length(distance));
				if (length.x < threshold)
				{
					XMVECTOR C = myPOS - enemyPOS;
					C = XMVector4Normalize(C);
					C *= (threshold - length.x) / threshold;
					B += C;
				}
			}
			//Sum the distances
			//Negate the new value
			B = XMVector4Normalize(B);
			B *= separationStrength;
			return B;
		}


		void CalculateUnitVelocities()
		{

		}

		XMVECTOR ShortenLength(XMVECTOR A, float reductionLength)
		{
			XMVECTOR B = A;
			B = XMVector4Normalize(B);
			B *= reductionLength;
			return B;
		}

		void Update(float _deltaTime)
		{
			CalculateAverages();
			for (int i = 0; i < flockers.size(); ++i)
			{
				XMVECTOR velocity = XMLoadFloat4(&flockers[i]->Velocity);
				XMFLOAT4 heading;
				XMStoreFloat4(&heading, XMVector4Normalize(velocity));

				flockers[i]->Velocity.x += heading.x * _deltaTime * moveSpeed;
				flockers[i]->Velocity.y += heading.y * _deltaTime * moveSpeed;
				flockers[i]->Velocity.z += heading.z * _deltaTime * moveSpeed;

				velocity = CalculateAlignmentAcceleration(flockers[i]) + CalculateCohesionAcceleration(flockers[i]) + CalculateSeparationAcceleration(flockers[i]) +CalculateDirectionalAcceleration(flockers[i]) + CalculateReturnAcceleration(flockers[i]);
				velocity *= (maxSpeed * _deltaTime);
				velocity += XMLoadFloat4(&flockers[i]->Velocity);
				XMFLOAT4 length;
				XMStoreFloat4(&length, XMVector4Length(velocity));

				if(length.x > maxSpeed)
				{
					velocity = ShortenLength(velocity, maxSpeed);
				}

				XMStoreFloat4(&flockers[i]->Velocity, velocity);
				flockers[i]->AddToPositionVector((XMFLOAT3&)flockers[i]->Velocity);

			}
		};
	};

	class AIUnit
	{
	public:
		ADResource::ADGameplay::GameObject* owner;
		std::vector<State*> states;
		State* currentState;

		void SwitchState()
		{
			//currentState = ...
		}

		void Update()
		{
			// currentState.Run();
		}

	};
};

