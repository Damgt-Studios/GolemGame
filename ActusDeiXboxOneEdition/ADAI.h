#pragma once
#include "Types.h"



namespace ADAI
{
	//Forward Declaration
	class AIUnit;
	
	class State
	{
	protected:
		
	public:
		AIUnit* owner = nullptr;
		XMVECTOR personalTarget;
		
		std::vector<ADResource::ADGameplay::GameObject*> objectsToApproach;
		std::vector<ADResource::ADGameplay::GameObject*> objectsToAvoid;
		virtual void Update(float _deltaTime) = 0;
	};

	class IdleState : public State
	{
		virtual void Update(float _deltaTime);
	};

	class WayPointState : public State
	{
		std::vector<XMFLOAT4> wayPoints;
		//Current Pathfinding Path
		virtual void Update(float _deltaTime);
	};

	class FlockingState : public State
	{
	public:
		XMFLOAT4 VelocityWhenFlocking = { 0,0,0,0 };

		//Current Pathfinding Path
		void Update(float _deltaTime);
	};

	class AIUnit
	{
	public:
		ADResource::ADGameplay::GameObject* owner;
		std::vector<ADAI::State*> states;
		ADAI::State* currentState;

		void SwitchState()
		{
			//currentState = ...
		}

		void Update(float _deltaTime);

	};

	class FlockingGroup
	{
	public:
		std::vector<AIUnit*> flockers;
		std::vector<FlockingState*> flockerState;
		float commandDirectionalStrength = 0.00f;
		float returnDirectionalStrength = 0.00f;
		float alignmentDirectionalStrength = 0.00f;
		float targetCohesionStrength  = 1.0f;
		float cohesionStrength = 0.7f;
		float separationStrength = 1.0f;
		float flockRadius = 10.0;
		float maxSpeed = 1.0f;
		float moveSpeed = 0.1f;
		XMVECTOR averagePosition;
		XMVECTOR averageForward;
		XMVECTOR commandDirection;
		XMMATRIX* groupTarget; //Player
		XMVECTOR commandDestination;

		XMFLOAT3 SetCommandDirection(XMMATRIX _camera)
		{
			for (int i = 0; i < flockers.size(); ++i)
			{
				flockers[i]->currentState = flockers[i]->states[1];
			}
			commandDirectionalStrength = 1.2f;
			returnDirectionalStrength = 0.f;

			//commandDestination = ((groupTarget->r[3] - _camera.r[3]));
			//XMFLOAT3 temp;
			//XMStoreFloat3(&temp, commandDestination);
			//commandDestination = _camera.r[3] + XMLoadFloat3(&temp);

			XMVECTOR camHeadingNormal = XMMatrixInverse(nullptr, _camera).r[2];
			XMFLOAT4 camFN;
			XMStoreFloat4(&camFN, camHeadingNormal);

			XMVECTOR targetPosition = XMMatrixInverse(nullptr, _camera).r[3];
			XMFLOAT3 targetP;
			XMStoreFloat3(&targetP, targetPosition);

			targetP.x += camFN.x*1000;
			targetP.y += camFN.y*1000;
			targetP.z += camFN.z*1000;

			commandDestination = XMLoadFloat3(&targetP);

			return targetP;
		};

		void ReturnCall()
		{
			for (int i = 0; i < flockers.size(); ++i)
			{
				flockers[i]->currentState = flockers[i]->states[1];
;			}
			commandDirectionalStrength = 0.f;
			returnDirectionalStrength = 1.2f;
		};

		void AddUnitToGroup(AIUnit* _flocker, FlockingState* _flockersState)
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
				averagePosition += flockers[i]->owner->transform.r[3];
				averageForward += XMLoadFloat4(&flockers[i]->owner->Velocity);
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
			//XMFLOAT3 sigh = _object->GetPosition();
			//XMVECTOR objectPos = XMLoadFloat3(&sigh);
			//XMFLOAT4X4 f4x4;
			//XMStoreFloat4x4(&f4x4, *groupTarget);
			//XMVECTOR f4 = { f4x4.m[3][0], f4x4.m[3][1], f4x4.m[3][2],f4x4.m[3][3]};
			//XMVECTOR velocity = { 0,0,0,0 };// = f4 - objectPos;

			//velocity = XMVector4Normalize(velocity);

			//velocity *= returnDirectionalStrength;
			//return velocity;


			XMFLOAT3 sigh = _object->GetPosition();
			XMVECTOR objectPos = XMLoadFloat3(&sigh);
			XMVECTOR velocity = groupTarget->r[3] - objectPos;

			velocity = XMVector4Normalize(velocity);

			velocity *= returnDirectionalStrength;
			return velocity;
		}

		XMVECTOR CalculateTargetAcceleration(AIUnit* _object)
		{
			//XMFLOAT3 sigh = _object->GetPosition();
			//XMVECTOR objectPos = XMLoadFloat3(&sigh);
			//XMFLOAT4X4 f4x4;
			//XMStoreFloat4x4(&f4x4, *groupTarget);
			//XMVECTOR f4 = { f4x4.m[3][0], f4x4.m[3][1], f4x4.m[3][2],f4x4.m[3][3]};
			//XMVECTOR velocity = { 0,0,0,0 };// = f4 - objectPos;

			//velocity = XMVector4Normalize(velocity);

			//velocity *= returnDirectionalStrength;
			//return velocity;


			XMFLOAT3 sigh = _object->owner->GetPosition();
			XMVECTOR objectPos = XMLoadFloat3(&sigh);
			XMVECTOR velocity = _object->currentState->personalTarget - objectPos;

			velocity = XMVector4Normalize(velocity);

			velocity *= targetCohesionStrength;
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
				sigh = flockers[i]->owner->GetPosition();
				sigh2 = _object->GetPosition();
				XMVECTOR enemyPOS = XMLoadFloat3(&sigh);
				XMVECTOR myPOS = XMLoadFloat3(&sigh2);
				distance = enemyPOS - myPOS;

				float threshold = flockers[i]->owner->safeRadius + _object->safeRadius;

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
				XMVECTOR velocity = XMLoadFloat4(&flockers[i]->owner->Velocity);
				XMFLOAT4 heading;
				XMStoreFloat4(&heading, XMVector4Normalize(velocity));

				flockerState[i]->VelocityWhenFlocking.x += heading.x * _deltaTime * moveSpeed;
				flockerState[i]->VelocityWhenFlocking.y += heading.y * _deltaTime * moveSpeed;
				flockerState[i]->VelocityWhenFlocking.z += heading.z * _deltaTime * moveSpeed;

				velocity = CalculateAlignmentAcceleration(flockers[i]->owner) + CalculateCohesionAcceleration(flockers[i]->owner) + CalculateSeparationAcceleration(flockers[i]->owner) +CalculateDirectionalAcceleration(flockers[i]->owner) + CalculateReturnAcceleration(flockers[i]->owner) + CalculateTargetAcceleration(flockers[i]);
				velocity *= (maxSpeed * _deltaTime);
				velocity += XMLoadFloat4(&flockers[i]->owner->Velocity);

				XMFLOAT4 yCancel;
				XMStoreFloat4(&yCancel, velocity);
				yCancel.y = 0;
				velocity = XMLoadFloat4(&yCancel);

				XMFLOAT4 length;
				XMStoreFloat4(&length, XMVector4Length(velocity));

				if(length.x > maxSpeed)
				{
					velocity = ShortenLength(velocity, maxSpeed);
				}

				XMStoreFloat4(&flockerState[i]->VelocityWhenFlocking, velocity);
			}
		};
	};

	static float DistanceCalculation(XMFLOAT3 _obj1Pos, XMFLOAT3 _obj2Pos)
	{
		float xDist = abs(_obj1Pos.x - _obj2Pos.x);
		float yDist = abs(_obj1Pos.z - _obj2Pos.z);
		return sqrt((xDist * xDist) + (yDist * yDist));
	}

	static ADResource::ADGameplay::GameObject* FindNearest(ADResource::ADGameplay::GameObject* _gameObject, std::vector<ADResource::ADGameplay::GameObject*> _searchGroup, float distnaceLimit, float desirabilityWeight)
	{
		float currentTargetDistance = 9999999;
		ADResource::ADGameplay::GameObject* currentTarget = nullptr;
		for (int i = 0; i < _searchGroup.size(); i++)
		{
			float distance = DistanceCalculation(_gameObject->GetPosition(), _searchGroup[i]->GetPosition());
			if ((distance < distnaceLimit) && (distance * (_searchGroup[i]->desirability * desirabilityWeight)) <= currentTargetDistance)
			{
				currentTarget = _searchGroup[i];
			}
		}
		return currentTarget;
	}
};

