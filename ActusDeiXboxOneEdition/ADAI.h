#pragma once
#include "Types.h"
#include "ADCombat.h"

namespace ADAI
{
	class State
	{
	public:
		virtual void Update(float _deltaTime) = 0;
		virtual void EnterState(float _deltaTime) {};
		virtual void ExitState(float _deltaTime) {};
	};

	class SimpleStateMachine
	{
	public:
		ADResource::ADGameplay::GameObject* gameObject;
		std::vector<ADAI::State*> states;
		ADAI::State* currentState;

		void SwitchState(ADAI::State* _state, float _deltaTime = 0.f)
		{
			currentState->ExitState(_deltaTime);
			currentState = _state;
			currentState->EnterState(_deltaTime);
		};
		void SwitchState(int _stateID, float _deltaTime = 0.f)
		{
			currentState->ExitState(_deltaTime);
			currentState = states[_stateID];
			currentState->EnterState(_deltaTime);
		};
		void Update(float _deltaTime)
		{
			currentState->Update(_deltaTime);
		};
	};

	class FlockingState : public State
	{
	public:
		SimpleStateMachine* mySSM;
		XMFLOAT4 VelocityWhenFlocking = { 0,0,0,0 };

		//Current Pathfinding Path
		void Update(float _deltaTime)
		{
			mySSM->gameObject->Velocity = VelocityWhenFlocking;
			mySSM->gameObject->AddToPositionVector((XMFLOAT3&)mySSM->gameObject->Velocity);
		};
	};

	class AttackingState : public State
	{
	public:
		SimpleStateMachine* mySSM;
		UINT returnIndex;
		//ADResource::ADGameplay::GameObject* target;
		UINT attackCount = 1;
		UINT attackCounter = 0;
		ADResource::ADGameplay::Action* myAttack;

		virtual void OnEnter()
		{
			mySSM->gameObject->Velocity = { 0,0,0,0 };
		}

		//Current Pathfinding Path
		void Update(float _deltaTime)
		{
			//XMVECTOR zVec = minion->currentTarget->transform.r[3] - minion->mySSM.gameObject->transform.r[3];
			//zVec = XMVector3Normalize(zVec);

			//XMVECTOR direction = XMVector3Dot(zVec, XMVector3Normalize(minion->mySSM.gameObject->transform.r[0]));
			//XMFLOAT3 fvec;
			//XMStoreFloat3(&fvec, direction);

			//XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(fvec.x);
			//minion->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, minion->mySSM.gameObject->transform);

			if (myAttack->active)
			{
				myAttack->Update(_deltaTime);
			}
			else
			{
				if (attackCounter < attackCount)
				{
					attackCounter++;
					myAttack->StartAction(&mySSM->gameObject->transform);
				}
				else
				{
					attackCounter = 0;
					mySSM->gameObject->actionLevel = 0;
					mySSM->SwitchState(returnIndex);
				}
			}
		};
	};

	static XMVECTOR ShortenLength(XMVECTOR A, float reductionLength)
	{
		XMVECTOR B = A;
		B = XMVector4Normalize(B);
		B *= reductionLength;
		return B;
	}

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
			if (_searchGroup[i]->active)
			{
				float distance = DistanceCalculation(_gameObject->GetPosition(), _searchGroup[i]->GetPosition());
				if (distance < distnaceLimit)
				{
					if (distance * ((2 + _searchGroup[i]->actionLevel) - (_searchGroup[i]->desirability * desirabilityWeight)) <= currentTargetDistance)
					{
						currentTarget = _searchGroup[i];
						currentTargetDistance = distance * (2 - (_searchGroup[i]->desirability * desirabilityWeight));
					}
				}
			}
		}
		return currentTarget;
	}

	static void turnTo(XMMATRIX _turner, XMMATRIX _target)
	{
	}

};

