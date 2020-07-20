#pragma once
#include "ADAI.h"
#include "ADCombat.h"
#include "GameObjectClasses.h"

//#include <queue>

namespace ADAI
{
	class MovementGroup
	{
	public:
		virtual void Update(float _deltaTime) = 0;
		virtual XMVECTOR GetVelocityValue(ADResource::ADGameplay::GameObject* _currentUnit) = 0;
	};

	class TargetedMovementGroup
	{
	public:
		virtual void Update(float _deltaTime) = 0;
		virtual XMVECTOR GetVelocityValue(ADResource::ADGameplay::GameObject* _currentUnit, ADResource::ADGameplay::GameObject* _currentTarget) = 0;
	};

	class VillagerAI
	{
	public:
		SimpleStateMachine mySSM;
		ADResource::ADGameplay::Destructable* destructable;
		ADResource::ADGameplay::GameObject* currentStructure;
		ADResource::ADGameplay::GameObject* currentFear;

		//Waypoints (both direct and pathing)
		float directionalStrength = 0.00f;

		//Safety for Fleeing and Cowering
		float desirableTargetCohesionStrength = 1.0f;

		//Danger for Fleeing
		float fleeingTargetSeperationStrength = 0.00f;

		void Update(float _deltaTime)
		{
			if (mySSM.gameObject->active)
			{
				mySSM.Update(_deltaTime);
			}
		};
	};

	class MinionAI
	{
	public:
		SimpleStateMachine mySSM;
		XMFLOAT3 currentTargetLocation;
		std::vector<XMFLOAT3> myPath;
		ADResource::ADGameplay::GameObject* currentTarget;
		ADResource::ADGameplay::Destructable* destructable;
		ADResource::ADGameplay::Action* myAttack;
		XMFLOAT3 currentPointInPath = { 0,0,0 };
		bool pathingFailed = false;
		bool cancelPath = false;
		bool waitingForPath = false;
		bool hasPath = false;
		bool donePathing = true;
		UINT currentPointIndex = UINT_MAX;

		//Waypoints (both direct and pathing)
		float directionalStrength = 0.00f;

		//Safety for Fleeing and Cowering
		float desirableTargetCohesionStrength = 1.0f;

		//Danger for Fleeing
		//float fleeingTargetSeperationStrength = 0.00f;

		void Update(float _deltaTime)
		{
			if (mySSM.gameObject->active)
			{

				mySSM.Update(_deltaTime);
				myAttack->Update(_deltaTime);
			}
		};
	};

	class TowerAI
	{
	public:
		SimpleStateMachine mySSM;
		XMFLOAT3 currentTargetLocation;
		ADResource::ADGameplay::Action* myAttack;
		ADResource::ADGameplay::Building* building;
		ADResource::ADGameplay::GameObject* currentTarget;

		void Update(float _deltaTime)
		{
			if (mySSM.gameObject->active)
			{
				mySSM.Update(_deltaTime);
				myAttack->Update(_deltaTime);
			}
		};
	};

	struct queuePair {
		MinionAI* currentUnit;
		XMFLOAT3 currentTarget;
	};

	class PathersQueue
	{
	public:
		std::queue<queuePair> pathingQueue;
		MinionAI* minion = nullptr;
		XMFLOAT3 currentTarget = { 0,0,0 };

		void Update(float _deltaTime)
		{
			ADAI::ADPathfinding::Instance()->update(0.005f);
			if (ADAI::ADPathfinding::Instance()->isDone() && pathingQueue.size() > 0)
			{
				if (minion)
				{
					if (!minion->cancelPath)
					{
						minion->myPath = ADAI::ADPathfinding::Instance()->getSolutionPoints();
						if (minion->myPath.size() > 0)
						{
							minion->donePathing = false;
							minion->hasPath = true;
							minion->currentPointIndex = (minion->myPath.size() - 1);
							minion->currentPointInPath = minion->myPath[minion->currentPointIndex];
						}
						else
						{
							minion->pathingFailed = true;
							minion->donePathing = true;
							minion->hasPath = false;
							minion->cancelPath = false;
						}
					}
					else
					{
						minion->donePathing = true;
						minion->hasPath = false;
						minion->cancelPath = false;

					}
				}
				bool nextMinion = false;
				do {
					minion = pathingQueue.front().currentUnit;
					currentTarget = pathingQueue.front().currentTarget;
					pathingQueue.pop();
					minion->waitingForPath = false;
					if (minion->cancelPath)
					{
						minion->cancelPath = false;
						nextMinion = true;;
					}
				} while (nextMinion && pathingQueue.size() > 0);

				UINT srow;
				UINT scolumn;
				UINT drow;
				UINT dcolumn;
				ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(minion->destructable->GetPosition().x, minion->destructable->GetPosition().z), scolumn, srow);
				ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(currentTarget.x, currentTarget.z), dcolumn, drow);
				int pathSuccess = ADAI::ADPathfinding::Instance()->enter(scolumn, srow, dcolumn, drow);
				if (pathSuccess == 0)
				{
					minion->pathingFailed = false;
					minion->cancelPath = false;
				}
				else if (pathSuccess == 1)
				{
					//start fail back up.
					minion->pathingFailed = true;
					minion->cancelPath = true;
				}

			}
		}

		void AddToQueue(MinionAI* _currentUnit, XMFLOAT3 _currentTarget)
		{
			if (!_currentUnit->waitingForPath)
			{
				pathingQueue.push(queuePair{ _currentUnit, _currentTarget });
				_currentUnit->waitingForPath = true;
			}
		}
	};




	class TowerState : public State
	{
	public:
		TowerAI* tower;
		std::vector<ADResource::ADGameplay::GameObject*>* targets;
		float timer = 0;
		const float waitDuration = 0.2f;

		virtual void Update(float _deltaTime)
		{
			if (!tower->currentTarget)
			{
				timer += _deltaTime;
				if (timer > waitDuration)
				{
					tower->currentTarget = ADAI::FindNearest(tower->building, *targets, 250.f, 1.f);
				}
			}
			else
			{
				if (!tower->currentTarget->active || DistanceCalculation(tower->currentTarget->GetPosition(), tower->building->GetPosition()) > 300.f)
				{
					tower->currentTarget = nullptr;

				}
				else
				{
					tower->mySSM.SwitchState(1, _deltaTime);

					XMVECTOR pos = tower->building->transform.r[3];

					XMVECTOR distanceToPlane = XMVector3Dot({ 0,1,0 }, (tower->currentTarget->transform.r[3] - tower->building->GetModelByIndex(0)->transform.r[3]));
					XMVECTOR pointOnPlane = tower->currentTarget->transform.r[3] - (XMVECTOR{ 0.f,1.f,0.f } *distanceToPlane.m128_f32[0]);

					XMVECTOR forward = XMVector3Normalize(tower->building->GetModelByIndex(0)->transform.r[3] - pointOnPlane);

					XMVECTOR right = XMVector3Cross(XMVector3Normalize({ 0.f,1.f,0.f }), forward);
					XMVECTOR up = XMVector3Cross(forward, right);

					tower->building->GetModelByIndex(0)->transform.r[0] = right;
					tower->building->GetModelByIndex(0)->transform.r[1] = up;
					tower->building->GetModelByIndex(0)->transform.r[2] = -forward;


					tower->building->GetModelByIndex(0)->SetScale({ 0.05,0.05,0.05 });
					tower->building->GetModelByIndex(0)->SetPosition({ pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2] });
					tower->building->transform = tower->building->GetModelByIndex(0)->transform;
				}
			}
		};
	};

	class CohesionGroup : public MovementGroup
	{
		XMFLOAT3 averagePosition = { 0,0,0 };
		float groupCohesionStrength = 0.2f;
		float cohesionRadius = 20.0;
		float cohesionLimitRadius = 400.0;

		void CalculateAverages()
		{
			averagePosition = { 0,0,0 };
			UINT count = 0;
			for (auto& member : members)
			{
				if (member->active)
				{
					//XMVECTOR memberTransform = member->transform.r[3];
					//XMVECTOR aPos = Float3ToVector(averagePosition);
					//aPos += memberTransform;
					//XMStoreFloat3(&averagePosition, aPos);

					XMFLOAT4X4 memberTransform = MatrixToFloat4x4(member->transform);
					memberTransform.m[3];
					averagePosition.x += memberTransform.m[3][0];
					averagePosition.y += memberTransform.m[3][1];
					averagePosition.z += memberTransform.m[3][2];
					count++;
				}
			}
			averagePosition.x /= count;
			averagePosition.y /= count;
			averagePosition.z /= count;
		};

	public:
		std::vector<ADResource::ADGameplay::GameObject*> members;

		void Update(float _deltaTime)
		{
			CalculateAverages();
		}

		XMVECTOR GetVelocityValue(ADResource::ADGameplay::GameObject* _currentUnit) override
		{
			XMVECTOR cuPos = Float3ToVector(_currentUnit->GetPosition());
			XMVECTOR aPos = Float3ToVector(averagePosition);
			cuPos = aPos - cuPos;

			XMFLOAT4 length;
			XMStoreFloat4(&length, XMVector4Length(cuPos));

			if (length.x > cohesionLimitRadius)
			{
				if (length.x < cohesionRadius)
				{
					cuPos = XMVector4Normalize(cuPos);
					cuPos *= (length.x / cohesionRadius);
				}
				else
				{
					cuPos = XMVector4Normalize(cuPos);
				}
				cuPos *= groupCohesionStrength;
				return cuPos;
			}
			return { 0,0,0 };
		}
	};

	class SeperationGroup : public MovementGroup
	{
		float groupSeperationStrength = 1.0f;
		//Seperation is calculated by the safe radius of each unit. This value increases that seperation radius.
		float additionalSeperation = 0.0f;

	public:
		std::vector<ADResource::ADGameplay::GameObject*> members;

		void Update(float _deltaTime)
		{
		}

		XMVECTOR GetVelocityValue(ADResource::ADGameplay::GameObject* _currentUnit) override
		{
			XMVECTOR changeFromTarget = { 0,0,0 };
			XMVECTOR targetPos = { 0,0,0 };
			XMVECTOR velocity = { 0, 0, 0 };
			XMVECTOR distance = { 0, 0, 0 };
			XMFLOAT4 length = { 0,0,0,0 };
			XMVECTOR curentPos = Float3ToVector(_currentUnit->GetPosition());

			for (auto& member : members)
			{
				if (member->active)
				{
					targetPos = Float3ToVector(member->GetPosition());
					distance = targetPos - curentPos;

					float threshold = member->safeRadius + _currentUnit->safeRadius + additionalSeperation;

					XMStoreFloat4(&length, XMVector4Length(distance));
					if (length.x < threshold)
					{
						changeFromTarget = curentPos - targetPos;
						changeFromTarget = XMVector4Normalize(changeFromTarget);
						changeFromTarget *= (threshold - length.x) / threshold;
						velocity += changeFromTarget;
					}
				}

			}
			velocity = XMVector4Normalize(velocity);
			velocity *= groupSeperationStrength;
			return velocity;
		}
	};


	class TargetingGroup : public TargetedMovementGroup
	{
		float groupTargetingStrength = 1.0f;

	public:
		std::vector<ADResource::ADGameplay::GameObject*> members;
		std::vector<ADResource::ADGameplay::GameObject*> targets;

		void Update(float _deltaTime)
		{

		}

		XMVECTOR GetVelocityValue(ADResource::ADGameplay::GameObject* _currentUnit, ADResource::ADGameplay::GameObject* _currentTarget) override
		{
			XMVECTOR minionPos = ADMath::Float3ToVector(_currentUnit->GetPosition());
			XMVECTOR targetPos = ADMath::Float3ToVector(_currentTarget->GetPosition());
			XMVECTOR velocity = targetPos - minionPos;

			XMFLOAT4 length;
			XMStoreFloat4(&length, XMVector4Length(velocity));

			if (length.x < 20.f)
			{
				velocity = XMVector4Normalize(velocity);
				velocity *= (length.x / 20.f);
			}
			else
			{
				velocity = XMVector4Normalize(velocity);
			}

			velocity *= groupTargetingStrength;
			return velocity;
		}
	};



	class VillagerGroup
	{
	public:
		std::vector<VillagerAI*> flockers;
		std::vector<FlockingState*> flockerState;
		XMVECTOR averagePosition;
		XMVECTOR averageForward;

		float groupCohesionStrength = 1.f;
		float groupSeparationStrength = 1.0f;

		float flockRadius = 10.0;
		float maxSpeed = 0.8f;
		float moveSpeed = 0.1f;

		void AddUnitToGroup(VillagerAI* _flocker, FlockingState* _flockersState)
		{
			flockers.push_back(_flocker);
			flockerState.push_back(_flockersState);
		};

		//void RemoveUnitFromGroup(AIUnit* _flocker, FlockingState* _flockersState)
		//{
		//	flockers.push_back(_flocker);
		//	flockerState.push_back(_flockersState);
		//};


		void CalculateAverages()
		{
			averagePosition = { 0,0,0 };
			averageForward = { 0,0,0 };
			int count = flockers.size();
			for (int i = 0; i < count; ++i)
			{
				averagePosition += flockers[i]->mySSM.gameObject->transform.r[3];
				averageForward += XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
			}
			XMVECTOR cnt = { count, count, count, count };
			XMVECTOR forward = { count, count, count, count };
			averagePosition /= cnt;
			averageForward /= forward;
			return;
		};

		XMVECTOR CalculateFleeingAcceleration(VillagerAI* _villager)
		{
			XMVECTOR villagerPos = ADMath::Float3ToVector(_villager->mySSM.gameObject->GetPosition());
			XMVECTOR targetPos = ADMath::Float3ToVector(_villager->currentFear->GetPosition());
			XMVECTOR velocity = villagerPos - targetPos;

			velocity = XMVector4Normalize(velocity);

			velocity *= _villager->fleeingTargetSeperationStrength;
			return velocity;
		}

		XMVECTOR CalculateSafetyAcceleration(VillagerAI* _villager)
		{
			XMVECTOR villagerPos = ADMath::Float3ToVector(_villager->mySSM.gameObject->GetPosition());
			XMVECTOR targetPos = ADMath::Float3ToVector(_villager->currentStructure->GetPosition());
			XMVECTOR velocity = targetPos - villagerPos;

			velocity = XMVector4Normalize(velocity);

			velocity *= _villager->desirableTargetCohesionStrength;
			return velocity;
		}

		XMVECTOR CalculateCohesionAcceleration(ADResource::ADGameplay::GameObject* _currentUnit)
		{
			XMVECTOR ap = Float3ToVector(_currentUnit->GetPosition());
			ap = averagePosition - ap;

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
			ap *= groupCohesionStrength;
			return ap;
		}

		XMVECTOR CalculateSeparationAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMVECTOR B = { 0, 0, 0 };

			XMFLOAT3 sigh;
			XMFLOAT3 sigh2;
			XMVECTOR distance;

			int count = flockers.size();
			for (int i = 0; i < count; ++i)
			{
				sigh = flockers[i]->mySSM.gameObject->GetPosition();
				sigh2 = _object->GetPosition();
				XMVECTOR enemyPOS = XMLoadFloat3(&sigh);
				XMVECTOR myPOS = XMLoadFloat3(&sigh2);
				distance = enemyPOS - myPOS;

				float threshold = flockers[i]->mySSM.gameObject->safeRadius + _object->safeRadius;

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
			B = XMVector4Normalize(B);
			B *= groupSeparationStrength;
			return B;
		}

		void Update(float _deltaTime)
		{
			CalculateAverages();
			for (int i = 0; i < flockers.size(); ++i)
			{
				if (flockers[i]->mySSM.gameObject->active)
				{
					XMVECTOR velocity = XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
					XMFLOAT4 heading;
					XMStoreFloat4(&heading, XMVector4Normalize(velocity));

					flockerState[i]->VelocityWhenFlocking.x += heading.x * _deltaTime * moveSpeed;
					flockerState[i]->VelocityWhenFlocking.y += heading.y * _deltaTime * moveSpeed;
					flockerState[i]->VelocityWhenFlocking.z += heading.z * _deltaTime * moveSpeed;


					XMVECTOR direction = XMVector3Dot(velocity, XMVector3Normalize(flockers[i]->mySSM.gameObject->transform.r[0]));
					XMVECTOR backCheck = XMVector3Dot(velocity, XMVector3Normalize(flockers[i]->mySSM.gameObject->transform.r[2]));
					XMFLOAT3 fvec;
					XMStoreFloat3(&fvec, direction);
					XMFLOAT3 zvec;
					XMStoreFloat3(&zvec, backCheck);


					if (fvec.x > 0.2f)
					{
						XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(0.10f);
						flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
						flockerState[i]->turning = true;
					}
					else if (fvec.x < -0.2f)
					{
						XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(-0.10f);
						flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
						flockerState[i]->turning = true;
					}
					else if (zvec.x < 0)
					{
						XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(0.25f);
						flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
						flockerState[i]->turning = true;
					}
					else
					{
						flockerState[i]->turning = false;
					}


					velocity = CalculateCohesionAcceleration(flockers[i]->mySSM.gameObject) + CalculateSeparationAcceleration(flockers[i]->mySSM.gameObject);
					if (flockers[i]->currentFear)
					{
						velocity += CalculateFleeingAcceleration(flockers[i]);
					}
					if (flockers[i]->currentStructure)
					{
						velocity += CalculateSafetyAcceleration(flockers[i]);
					}
					velocity *= (moveSpeed * _deltaTime);
					velocity += (XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity) * 0.9f);

					XMFLOAT4 yCancel;
					XMStoreFloat4(&yCancel, velocity);
					yCancel.y = 0;
					velocity = XMLoadFloat4(&yCancel);

					XMFLOAT4 length;
					XMStoreFloat4(&length, XMVector4Length(velocity));

					if (length.x > maxSpeed)
					{
						velocity = ShortenLength(velocity, maxSpeed);
					}

					XMStoreFloat4(&flockerState[i]->VelocityWhenFlocking, velocity);

				}
			}
		};
	};

	class FleeingState : public FlockingState
	{
	public:
		VillagerAI* villager;
		VillagerGroup myGroup;
		float timer = 0;
		const float waitDuration = 1.2f;

		std::vector<ADResource::ADGameplay::GameObject*> safetyStructures;
		std::vector<ADResource::ADGameplay::GameObject*>* fearedObjects;
		virtual void Update(float _deltaTime)
		{
			timer += _deltaTime;
			if (villager->currentFear)
			{
				if (!villager->currentFear->active)
				{
					villager->currentFear = nullptr;
				}
				else
				{
					ADResource::ADGameplay::GameObject* tempTarget = ADAI::FindNearest(mySSM->gameObject, *fearedObjects, 20.f, 1.f);
					if (tempTarget)
					{
						if (tempTarget->gamePlayType != ADResource::ADGameplay::OBJECT_TAG::PLAYER)
						{
							tempTarget->actionLevel += 5;
							villager->mySSM.SwitchState(2, _deltaTime);
							villager->destructable->anim_controller->PlayAnimationByName("Cower");

							XMVECTOR pos = villager->mySSM.gameObject->transform.r[3];

							XMVECTOR distanceToPlane = XMVector3Dot({ 0,1,0 }, (villager->currentFear->transform.r[3] - villager->mySSM.gameObject->transform.r[3]));
							XMVECTOR pointOnPlane = villager->currentFear->transform.r[3] - (XMVECTOR{ 0.f,1.f,0.f } *distanceToPlane.m128_f32[0]);

							XMVECTOR forward = XMVector3Normalize(villager->mySSM.gameObject->transform.r[3] - pointOnPlane);

							XMVECTOR right = XMVector3Cross(XMVector3Normalize({ 0.f,1.f,0.f }), forward);
							XMVECTOR up = XMVector3Cross(forward, right);

							villager->mySSM.gameObject->transform.r[0] = right;
							villager->mySSM.gameObject->transform.r[1] = up;
							villager->mySSM.gameObject->transform.r[2] = -forward;


							villager->mySSM.gameObject->SetScale({ 0.1,0.1,0.1 });
							villager->mySSM.gameObject->SetPosition({ pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2] });

							//XMVECTOR zVec = tempTarget->transform.r[3] - villager->mySSM.gameObject->transform.r[3];
							//zVec = XMVector3Normalize(zVec);

							//XMVECTOR direction = XMVector3Dot(zVec, XMVector3Normalize(villager->mySSM.gameObject->transform.r[0]));
							//XMFLOAT3 fvec;
							//XMStoreFloat3(&fvec, direction);

							//XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(fvec.x);
							//villager->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, villager->mySSM.gameObject->transform);
							return;
						}
					}
					if (timer > waitDuration)
					{
						villager->currentFear = ADAI::FindNearest(mySSM->gameObject, *fearedObjects, 500.f, 1.f);
						if (!villager->currentFear)
						{
							villager->fleeingTargetSeperationStrength = 0.0f;
						}
					}
				}
			}
			else
			{
				for (auto& element : *fearedObjects)
				{
					villager->currentFear = ADAI::FindNearest(mySSM->gameObject, *fearedObjects, 100.f, 1.f);
					if (villager->currentFear)
					{
						villager->fleeingTargetSeperationStrength = 1.0f;
					}
				}
			}

			if (villager->currentStructure)
			{
				if (timer > waitDuration)
				{
					villager->currentStructure = ADAI::FindNearest(mySSM->gameObject, safetyStructures, 700.f, 1.f);
				}
			}
			else
			{
				for (auto& element : safetyStructures)
				{
					villager->currentStructure = ADAI::FindNearest(mySSM->gameObject, safetyStructures, 700.f, 1.f);
					if (villager->currentStructure)
					{
						villager->desirableTargetCohesionStrength = 0.5f;
					}
				}
			}

			if (timer > waitDuration)
			{
				timer = 0;
			}
			mySSM->gameObject->Velocity = VelocityWhenFlocking;
			if (mySSM->gameObject->Velocity.x > 0 || mySSM->gameObject->Velocity.z > 0 || mySSM->gameObject->Velocity.x < 0 || mySSM->gameObject->Velocity.z < 0)
			{
				villager->destructable->anim_controller->PlayAnimationByName("Run");
			}
			else
			{
				villager->destructable->anim_controller->PlayAnimationByName("Idle");
			}
			if (!turning)
			{
				mySSM->gameObject->AddToPositionVector((XMFLOAT3&)mySSM->gameObject->Velocity);
			}
		};
	};

	class MinionGroup
	{
	public:
		std::vector<MinionAI*> flockers;
		std::vector<FlockingState*> flockerState;

		PathersQueue* pathingQueue = nullptr;
		std::vector<MovementGroup*> myGroups;
		std::vector<TargetedMovementGroup*> myTargetGroups;

		std::vector<ADResource::ADGameplay::GameObject*>* avoidanceGroup;

		AttackingState* attacking;
		//XMVECTOR averagePosition;
		//XMVECTOR averageForward;
		//std::queue<MinionAI*> waitingForPath;
		//bool pathingReady = true;

		//Group Behavior
		//float alignmentDirectionalStrength = 0.000f;
		//float groupCohesionStrength = 0.3f;
		float groupSeparationStrength = 0.7f;
		float groupAvoidanceStrength = 1.2f;

		//float commandDirectionalStrength = 1.2f;
		//float commandPathingStrength = 5.f;
		//float returnDirectionalStrength = 0.f;

		float flockRadius = 10.0;
		float maxSpeed = 1.0f;
		float moveSpeed = 0.7f;

		//XMVECTOR groupTarget;
		//XMMATRIX* player;

		//XMVECTOR commandDirection;
		XMFLOAT3 commandDestination;

		XMFLOAT3 SetCommandDirection(XMMATRIX _camera, float _distance)
		{
			XMVECTOR camHeadingNormal = XMMatrixInverse(nullptr, _camera).r[2];
			XMFLOAT4 camFN;
			XMStoreFloat4(&camFN, camHeadingNormal);

			XMVECTOR targetPosition = XMMatrixInverse(nullptr, _camera).r[3];
			XMFLOAT3 targetP;
			XMStoreFloat3(&targetP, targetPosition);

			targetP.x += camFN.x * _distance;
			targetP.y = 0;
			targetP.z += camFN.z * _distance;

			commandDestination = targetP;

			return targetP;
		};

		void SetTarget(ADResource::ADGameplay::GameObject* _target)
		{
			for (auto& flocker : flockers)
			{
				flocker->mySSM.currentState = flocker->mySSM.states[1];
				flocker->currentTarget = _target;
				flocker->currentTargetLocation = _target->GetPosition();
				pathingQueue->AddToQueue(flocker, flocker->currentTargetLocation);
			}
		}

		void SetDestination(ADResource::ADGameplay::GameObject* _target)
		{
			for (auto& flocker : flockers)
			{
				flocker->mySSM.currentState = flocker->mySSM.states[1];
				flocker->currentTarget = nullptr;
				flocker->currentTargetLocation = _target->GetPosition();
				pathingQueue->AddToQueue(flocker, flocker->currentTargetLocation);
			}
		}

		void AddUnitToGroup(MinionAI* _flocker, FlockingState* _flockersState)
		{
			flockers.push_back(_flocker);
			flockerState.push_back(_flockersState);
		};

		//void RemoveUnitFromGroup(AIUnit* _flocker, FlockingState* _flockersState)
		//{
		//	flockers.push_back(_flocker);
		//	flockerState.push_back(_flockersState);
		//};

		//void CalculateAverages()
		//{
		//	averagePosition = { 0,0,0 };
		//	averageForward = { 0,0,0 };
		//	int count = flockers.size();
		//	for (int i = 0; i < count; ++i)
		//	{
		//		averagePosition += flockers[i]->mySSM.gameObject->transform.r[3];
		//		averageForward += XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
		//	}
		//	XMVECTOR cnt = { count, count, count, count };
		//	XMVECTOR forward = { count, count, count, count };
		//	averagePosition /= cnt;
		//	averageForward /= forward;
		//	return;
		//};

		//XMVECTOR CalculateAlignmentAcceleration(ADResource::ADGameplay::GameObject* _object)
		//{
		//	XMVECTOR af = averageForward * (1.f / maxSpeed);
		//	af = XMVector4Normalize(af);
		//	af *= alignmentDirectionalStrength;
		//	return af;
		//}

		//XMVECTOR CalculateTargetAcceleration(MinionAI* _minion)//, ADResource::ADGameplay::GameObject* _target)
		//{
		//	XMVECTOR minionPos = ADMath::Float3ToVector(_minion->mySSM.gameObject->GetPosition());
		//	XMVECTOR targetPos = ADMath::Float3ToVector(_minion->currentTargetLocation);
		//	//XMVECTOR targetPos = ADMath::Float3ToVector(_target->GetPosition());
		//	XMVECTOR velocity = targetPos - minionPos;

		//	XMFLOAT4 length;
		//	XMStoreFloat4(&length, XMVector4Length(velocity));

		//	if (length.x < 20)
		//	{
		//		velocity = XMVector4Normalize(velocity);
		//		velocity *= (length.x * 0.01f);
		//	}
		//	else
		//	{
		//		velocity = XMVector4Normalize(velocity);
		//	}

		//	velocity *= commandDirectionalStrength;
		//	return velocity;
		//}

		XMVECTOR CalculatePathingAcceleration(MinionAI* _minion)//, ADResource::ADGameplay::GameObject* _target)
		{
			XMVECTOR velocity = { 0,0,0 }; \
				XMVECTOR currentPos = ADMath::Float3ToVector(_minion->destructable->GetPosition());
			XMVECTOR targetPos = ADMath::Float3ToVector(_minion->myPath[_minion->currentPointIndex]);

			velocity = targetPos - currentPos;

			XMFLOAT4 length;
			XMStoreFloat4(&length, XMVector4Length(velocity));

			if (length.x < 15)
			{
				velocity = XMVector4Normalize(velocity);
				velocity *= (length.x / 20.f);
			}
			else
			{
				velocity = XMVector4Normalize(velocity);
			}
			return velocity;
		}

		//XMVECTOR CalculateReturnAcceleration(MinionAI* _minion)
		//{
		//	//XMFLOAT3 sigh = _object->GetPosition();
		//	//XMVECTOR objectPos = XMLoadFloat3(&sigh);
		//	//XMFLOAT4X4 f4x4;
		//	//XMStoreFloat4x4(&f4x4, *groupTarget);
		//	//XMVECTOR f4 = { f4x4.m[3][0], f4x4.m[3][1], f4x4.m[3][2],f4x4.m[3][3]};
		//	//XMVECTOR velocity = { 0,0,0,0 };// = f4 - objectPos;
		//	//velocity = XMVector4Normalize(velocity);
		//	//velocity *= returnDirectionalStrength;
		//	//return velocity;
		//	XMFLOAT3 sigh = _minion->mySSM.gameObject->GetPosition();
		//	XMVECTOR objectPos = XMLoadFloat3(&sigh);
		//	XMVECTOR velocity = player->r[3] - objectPos;
		//	velocity = XMVector4Normalize(velocity);
		//	velocity *= returnDirectionalStrength;
		//	return velocity;
		//}
		//XMVECTOR CalculateTargetAcceleration(AIUnit* _object)
		//{
		//	//XMFLOAT3 sigh = _object->GetPosition();
		//	//XMVECTOR objectPos = XMLoadFloat3(&sigh);
		//	//XMFLOAT4X4 f4x4;
		//	//XMStoreFloat4x4(&f4x4, *groupTarget);
		//	//XMVECTOR f4 = { f4x4.m[3][0], f4x4.m[3][1], f4x4.m[3][2],f4x4.m[3][3]};
		//	//XMVECTOR velocity = { 0,0,0,0 };// = f4 - objectPos;
		//	//velocity = XMVector4Normalize(velocity);
		//	//velocity *= returnDirectionalStrength;
		//	//return velocity;
		//	XMFLOAT3 sigh = _object->owner->GetPosition();
		//	XMVECTOR objectPos = XMLoadFloat3(&sigh);
		//	XMVECTOR velocity = _object->currentState->personalTarget - objectPos;
		//	velocity = XMVector4Normalize(velocity);
		//	velocity *= targetCohesionStrength;
		//	return velocity;
		//}
		//XMVECTOR CalculateTargetAcceleration(AIUnit* _object)
		//{
		//	//XMFLOAT3 sigh = _object->GetPosition();
		//	//XMVECTOR objectPos = XMLoadFloat3(&sigh);
		//	//XMFLOAT4X4 f4x4;
		//	//XMStoreFloat4x4(&f4x4, *groupTarget);
		//	//XMVECTOR f4 = { f4x4.m[3][0], f4x4.m[3][1], f4x4.m[3][2],f4x4.m[3][3]};
		//	//XMVECTOR velocity = { 0,0,0,0 };// = f4 - objectPos;
		//	//velocity = XMVector4Normalize(velocity);
		//	//velocity *= returnDirectionalStrength;
		//	//return velocity;
		//	XMFLOAT3 sigh = _object->owner->GetPosition();
		//	XMVECTOR objectPos = XMLoadFloat3(&sigh);
		//	XMVECTOR velocity = _object->currentState->personalTarget - objectPos;
		//	velocity = XMVector4Normalize(velocity);
		//	velocity *= targetCohesionStrength;
		//	return velocity;
		//}


		//XMVECTOR CalculateCohesionAcceleration(ADResource::ADGameplay::GameObject* _currentUnit)
		//{
		//	XMVECTOR ap = Float3ToVector(_currentUnit->GetPosition());
		//	ap = averagePosition - ap;

		//	XMFLOAT4 length;
		//	XMStoreFloat4(&length, XMVector4Length(ap));

		//	if (length.x < flockRadius)
		//	{
		//		ap = XMVector4Normalize(ap);
		//		ap *= (length.x / flockRadius);
		//	}
		//	else
		//	{
		//		ap = XMVector4Normalize(ap);
		//	}
		//	ap *= groupCohesionStrength;
		//	return ap;
		//}

		//XMVECTOR CalculateSeparationAcceleration(ADResource::ADGameplay::GameObject* _object)
		//{
		//	XMVECTOR B = { 0, 0, 0 };

		//	XMFLOAT3 sigh;
		//	XMFLOAT3 sigh2;
		//	XMVECTOR distance;

		//	int count = flockers.size();
		//	for (int i = 0; i < count; ++i)
		//	{
		//		sigh = flockers[i]->mySSM.gameObject->GetPosition();
		//		sigh2 = _object->GetPosition();
		//		XMVECTOR enemyPOS = XMLoadFloat3(&sigh);
		//		XMVECTOR myPOS = XMLoadFloat3(&sigh2);
		//		distance = enemyPOS - myPOS;

		//		float threshold = flockers[i]->mySSM.gameObject->safeRadius + _object->safeRadius;

		//		XMFLOAT4 length;
		//		XMStoreFloat4(&length, XMVector4Length(distance));
		//		if (length.x < threshold)
		//		{
		//			XMVECTOR C = myPOS - enemyPOS;
		//			C = XMVector4Normalize(C);
		//			C *= (threshold - length.x) / threshold;
		//			B += C;
		//		}
		//	}
		//	B = XMVector4Normalize(B);
		//	B *= groupSeparationStrength;
		//	return B;
		//}

		XMVECTOR CalculateAvoidanceAcceleration(ADResource::ADGameplay::GameObject* _currentUnit)
		{

			XMVECTOR changeFromTarget = { 0,0,0 };
			XMVECTOR targetPos = { 0,0,0 };
			XMVECTOR velocity = { 0, 0, 0 };
			XMVECTOR distance = { 0, 0, 0 };
			XMFLOAT4 length = { 0,0,0,0 };
			XMVECTOR curentPos = Float3ToVector(_currentUnit->GetPosition());

			for (auto& avoids : *avoidanceGroup)
			{
				if (avoids->active)
				{
					targetPos = Float3ToVector(avoids->GetPosition());
					distance = targetPos - curentPos;

					float threshold = avoids->safeRadius + _currentUnit->safeRadius;

					XMStoreFloat4(&length, XMVector4Length(distance));
					if (length.x < threshold)
					{
						changeFromTarget = curentPos - targetPos;
						changeFromTarget = XMVector4Normalize(changeFromTarget);
						changeFromTarget *= (threshold - length.x) / threshold;
						velocity += changeFromTarget;
					}
				}

			}
			velocity = XMVector4Normalize(velocity);
			velocity *= groupAvoidanceStrength;
			return velocity;
		}

		void Update(float _deltaTime)
		{
			pathingQueue->Update(_deltaTime);
			for (auto& group : myGroups)
			{
				group->Update(_deltaTime);
			}
			for (auto& group : myTargetGroups)
			{
				group->Update(_deltaTime);
			}
			//CalculateAverages();

			for (int i = 0; i < flockers.size(); ++i)
			{
				if (flockers[i]->mySSM.gameObject->active)
				{
					XMVECTOR velocity = XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
					XMFLOAT4 heading;
					XMStoreFloat4(&heading, XMVector4Normalize(velocity));

					XMVECTOR direction = XMVector3Dot(velocity, XMVector3Normalize(flockers[i]->mySSM.gameObject->transform.r[0]));
					XMVECTOR backCheck = XMVector3Dot(velocity, XMVector3Normalize(flockers[i]->mySSM.gameObject->transform.r[2]));

					XMFLOAT3 fvec;
					XMStoreFloat3(&fvec, direction);
					XMFLOAT3 zvec;
					XMStoreFloat3(&zvec, backCheck);


					if (fvec.x > 0.15f)
					{
						XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(0.10f);
						flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
						flockerState[i]->turning = true;
					}
					else if (fvec.x < -0.15f)
					{
						XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(-0.10f);
						flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
						flockerState[i]->turning = true;
					}
					else if (zvec.x < 0)
					{
						XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(0.25f);
						flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
						flockerState[i]->turning = true;
					}
					else
					{
						flockerState[i]->turning = false;
					}

					flockerState[i]->VelocityWhenFlocking.x += heading.x * _deltaTime * moveSpeed;
					flockerState[i]->VelocityWhenFlocking.y += heading.y * _deltaTime * moveSpeed;
					flockerState[i]->VelocityWhenFlocking.z += heading.z * _deltaTime * moveSpeed;


					if (flockers[i]->hasPath)
					{
						velocity = CalculatePathingAcceleration(flockers[i]);
						groupAvoidanceStrength = 0.0f;
					}
					else
					{
						velocity = { 0,0,0 };
						groupAvoidanceStrength = 1.0f;
						for (auto& group : myGroups)
						{
							velocity += group->GetVelocityValue(flockers[i]->mySSM.gameObject);
						}
						for (auto& group : myTargetGroups)
						{
							if (flockers[i]->currentTarget)
							{
								velocity += group->GetVelocityValue(flockers[i]->mySSM.gameObject, flockers[i]->currentTarget);
							}
						}
					}
					velocity *= (moveSpeed * _deltaTime);
					velocity += XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
					float decellerationRate = (1.0f * _deltaTime) + 1.f;
					velocity /= decellerationRate;
					if (flockers[i]->pathingFailed)
					{
						groupAvoidanceStrength = 2.0f;
						pathingQueue->AddToQueue(flockers[i], flockers[i]->currentTargetLocation);

					}
					velocity += CalculateAvoidanceAcceleration(flockers[i]->mySSM.gameObject);
					XMFLOAT4 yCancel;
					XMStoreFloat4(&yCancel, velocity);
					yCancel.y = 0;
					velocity = XMLoadFloat4(&yCancel);

					XMFLOAT4 length;
					XMStoreFloat4(&length, XMVector4Length(velocity));

					if (flockers[i]->pathingFailed && length.x < 0.01f)
					{
						velocity += {RandFloat(-0.5f, 0.5f), 0, RandFloat(-0.5f, 0.5f)};
					}
					if (length.x > maxSpeed)
					{
						velocity = ShortenLength(velocity, maxSpeed);
					}

					XMStoreFloat4(&flockerState[i]->VelocityWhenFlocking, velocity);
				}
			}
		};
	};

	class CommandState : public FlockingState
	{
	public:
		MinionAI* minion;
		PathersQueue* pathingQueue = nullptr;
		//MinionGroup* myGroup;
		float timer = 0;
		const float waitDuration = 1.2f;

		std::vector<ADResource::ADGameplay::GameObject*>* targets;

		virtual void EnterState(float _deltaTime) override
		{
			if (!minion->currentTarget->active)
			{
				minion->currentTarget = nullptr;
			}
		}

		virtual void Update(float _deltaTime)
		{
			if (minion->donePathing == false && minion->hasPath)
			{
				float distance2 = DistanceCalculation(minion->mySSM.gameObject->GetPosition(), minion->currentPointInPath);
				if (distance2 < 15)
				{
					if (minion->currentPointIndex > 0)
					{
						minion->currentPointInPath = minion->myPath[--minion->currentPointIndex];
					}
					else
					{
						VelocityWhenFlocking = { 0,0,0,0 };
						minion->donePathing = true;
						minion->hasPath = false;
					}
				}
				else if (distance2 > 300)
				{
					minion->currentPointInPath = FindNearestPoint(minion->mySSM.gameObject, &minion->myPath, 400, 1, minion->currentPointIndex);
					if (minion->currentPointIndex == 0)
					{
						VelocityWhenFlocking = { 0,0,0,0 };
						minion->donePathing = true;
						minion->hasPath = false;
					}
				}
			}
			timer += _deltaTime;
			if (timer > waitDuration)
			{
				ADResource::ADGameplay::GameObject* tempTarget = ADAI::FindNearest(mySSM->gameObject, *targets, 200.f, 1.f);
				if (tempTarget)
				{
					if (tempTarget != minion->currentTarget)
					{
						if (tempTarget->active)
						{
							minion->currentTarget = tempTarget;
							pathingQueue->AddToQueue(minion, minion->currentTargetLocation);
						}
					}

				}

				if (minion->currentTarget)
				{
					if (minion->currentTarget->active)
					{
						float distance = DistanceCalculation(minion->mySSM.gameObject->GetPosition(), minion->currentTarget->GetPosition());

						if (minion->currentTarget->gamePlayType != ADResource::ADGameplay::OBJECT_TAG::COMMAND_MARKER && minion->currentTarget->team != minion->destructable->team)
						{
							if (distance < minion->currentTarget->attackRadius + minion->currentTarget->safeRadius && distance > minion->currentTarget->safeRadius)
							{
								minion->mySSM.SwitchState(2, _deltaTime);
								minion->destructable->anim_controller->PlayAnimationByName("Attack");

								if (minion->currentTarget->gamePlayType == ADResource::ADGameplay::OBJECT_TAG::PEASANT)
								{
									//use a function to engage the target this isn't working well
									minion->currentTarget->actionLevel += 25;
								}

								minion->donePathing = true;
								minion->hasPath = false;
								minion->cancelPath = true;

								XMVECTOR pos = minion->mySSM.gameObject->transform.r[3];

								XMVECTOR distanceToPlane = XMVector3Dot({ 0,1,0 }, (minion->currentTarget->transform.r[3] - minion->mySSM.gameObject->transform.r[3]));
								XMVECTOR pointOnPlane = minion->currentTarget->transform.r[3] - (XMVECTOR{ 0.f,1.f,0.f } *distanceToPlane.m128_f32[0]);

								XMVECTOR forward = XMVector3Normalize(minion->mySSM.gameObject->transform.r[3] - pointOnPlane);

								XMVECTOR right = XMVector3Cross(XMVector3Normalize({ 0.f,1.f,0.f }), forward);
								XMVECTOR up = XMVector3Cross(forward, right);

								minion->mySSM.gameObject->transform.r[0] = right;
								minion->mySSM.gameObject->transform.r[1] = up;
								minion->mySSM.gameObject->transform.r[2] = -forward;


								minion->mySSM.gameObject->SetScale({ 0.03,0.03,0.03 });
								minion->mySSM.gameObject->SetPosition({ pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2] });

								return;
							}
						}
					}
					else
					{
						minion->currentTarget = nullptr;
					}
				}
			}


			if (timer > waitDuration)
			{
				timer = 0;
			}
			mySSM->gameObject->Velocity = VelocityWhenFlocking;

			if (mySSM->gameObject->Velocity.x > 0.05f || mySSM->gameObject->Velocity.z > 0.05f || mySSM->gameObject->Velocity.x < -0.05f || mySSM->gameObject->Velocity.z < -0.05f)
			{
				minion->destructable->anim_controller->PlayAnimationByName("Run");
			}
			else
			{
				minion->destructable->anim_controller->PlayAnimationByName("Idle");
			}
			if (!turning)
			{
				mySSM->gameObject->AddToPositionVector((XMFLOAT3&)mySSM->gameObject->Velocity);
			}
		};
	};
}


