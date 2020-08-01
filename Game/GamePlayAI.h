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
		virtual XMVECTOR GetVelocityValue(ADResource::ADGameplay::GameObject* _currentUnit, XMFLOAT3 _currentTarget) = 0;
	};





	class VillagerAI
	{
	public:
		SimpleStateMachine mySSM;
		ADResource::ADGameplay::Destructable* destructable = nullptr;
		ADResource::ADGameplay::GameObject* currentStructure = nullptr;
		ADResource::ADGameplay::GameObject* currentFear = nullptr;
		float moveSpeed = 2.0f;
		float maxSpeed = 10.0f;

		//Waypoints (both direct and pathing)
		float directionalStrength = 0.00f;

		//Safety for Fleeing and Cowering
		float desirableTargetCohesionStrength = 10.0f;

		//Danger for Fleeing
		float fleeingTargetSeperationStrength = 6.00f;

		void Update(float _deltaTime)
		{
			if (mySSM.gameObject->active)
			{
				mySSM.Update(_deltaTime);
			}
		};
	};

	class VillagerGroup
	{
	public:
		std::vector<VillagerAI*> flockers;
		std::vector<FlockingState*> flockerState;
		XMVECTOR averagePosition = { 0,0,0 };
		XMVECTOR averageForward = { 0,0,0 };

		float groupCohesionStrength = 0.00f;
		float groupSeparationStrength = 5.0f;

		float flockRadius = 200.0;
		float maxSpeed = 10.0f;
		float moveSpeed = 3.0f;

		void AddUnitToGroup(VillagerAI* _flocker, FlockingState* _flockersState)
		{
			flockers.push_back(_flocker);
			flockerState.push_back(_flockersState);
		};


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
			if (_deltaTime > 0)
			{
				CalculateAverages();
				for (int i = 0; i < flockers.size(); ++i)
				{
					if (flockers[i]->mySSM.gameObject->active)
					{
						XMVECTOR velocity = XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
						XMFLOAT4 heading;
						XMStoreFloat4(&heading, XMVector4Normalize(velocity));

						velocity *= (_deltaTime * flockers[i]->moveSpeed);


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


						velocity += CalculateCohesionAcceleration(flockers[i]->mySSM.gameObject) + CalculateSeparationAcceleration(flockers[i]->mySSM.gameObject);
						if (flockers[i]->currentFear)
						{
							velocity += CalculateFleeingAcceleration(flockers[i]);
						}
						if (flockers[i]->currentStructure)
						{
							velocity += CalculateSafetyAcceleration(flockers[i]);
						}
						velocity *= (flockers[i]->moveSpeed * _deltaTime);

						XMFLOAT4 yCancel;
						XMStoreFloat4(&yCancel, velocity);
						yCancel.y = 0;
						velocity = XMLoadFloat4(&yCancel);

						XMFLOAT4 length;
						XMStoreFloat4(&length, XMVector4Length(velocity));

						if (length.x > flockers[i]->maxSpeed)
						{
							velocity = ShortenLength(velocity, flockers[i]->maxSpeed);
						}

						XMStoreFloat4(&flockerState[i]->VelocityWhenFlocking, velocity);

					}
				}
			}

		};
	};

	class FleeingState : public FlockingState
	{
	public:
		VillagerAI* villager = nullptr;
		VillagerGroup myGroup;
		float timer = 0;
		const float waitDuration = 1.2f;
		
		std::vector<ADResource::ADGameplay::GameObject*> safetyStructures;
		std::vector<ADResource::ADGameplay::GameObject*>* fearedObjects = nullptr;
		virtual void Update(float _deltaTime)
		{
			if (_deltaTime > 0)
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
						ADResource::ADGameplay::GameObject* tempTarget = ADAI::FindNearest(mySSM->gameObject, *fearedObjects, 200.f, 1.f);
						if (tempTarget)
						{
							if (tempTarget->gamePlayType != ADResource::ADGameplay::OBJECT_TAG::PLAYER)
							{
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

								return;
							}
						}
						if (timer > waitDuration)
						{
							villager->currentFear = ADAI::FindNearest(mySSM->gameObject, *fearedObjects, 100.f, 1.f);
							if (!villager->currentFear)
							{
								villager->currentFear->actionLevel + 10.f;
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
							villager->currentFear->actionLevel + 10.f;
						}
					}
				}

				if (villager->currentStructure)
				{
					if (timer > waitDuration)
					{
						villager->currentStructure = ADAI::FindNearestSafe(mySSM->gameObject, safetyStructures, 500.f, 1.f);
						villager->currentStructure->desirability + 10.f;
					}
				}
				else
				{
					for (auto& element : safetyStructures)
					{
						villager->currentStructure = ADAI::FindNearestSafe(mySSM->gameObject, safetyStructures, 500.f, 1.f);
						if (villager->currentStructure)
						{
							villager->currentStructure->desirability + 10.f;
						}
					}
				}

				if (timer > waitDuration)
				{
					timer = 0;
				}
				mySSM->gameObject->Velocity = VelocityWhenFlocking;
				if (mySSM->gameObject->Velocity.x > 0.08f || mySSM->gameObject->Velocity.z > 0.08f || mySSM->gameObject->Velocity.x < -0.08f || mySSM->gameObject->Velocity.z < -0.08f)
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
			}
		};
	};




	class MinionAI
	{
	public:
		ADResource::ADGameplay::Destructable* destructable = nullptr;
		SimpleStateMachine mySSM;

		bool turning = false;

		XMFLOAT3 currentTargetLocation = { 0,0,0 };
		ADAI::Solution myPath;
		XMFLOAT3 currentPointInPath = { 0,0,0 };
		XMFLOAT3 lastKnownGoodLocation = { 0,0,0 };
		UINT validDestinationX = 0;
		UINT validDestinationZ = 0;
		bool pathingFailed = false;
		bool cancelPath = false;
		bool waitingForPath = false;
		bool hasPath = false;
		bool donePathing = true;
		UINT currentPointIndex = UINT_MAX;
		ADResource::ADGameplay::GameObject* currentTarget = nullptr;
		ADResource::ADGameplay::GameObject* potentialTarget = nullptr;
		float moveSpeed = 1.0f;
		float maxSpeed = 10.0f;

		ADResource::ADGameplay::Action* myAttack = nullptr;

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

	struct queuePair {
		MinionAI* currentUnit = nullptr;
		XMFLOAT3 currentTarget = { 0,0,0 };
	};

	class PathersQueue
	{
	public:
		std::queue<queuePair> pathingQueue;
		MinionAI* minion = nullptr;
		XMFLOAT3 currentTarget = { 0,0,0 };

		void Update(float _deltaTime)
		{
			if (_deltaTime > 0)
			{
				ADAI::ADPathfinding::Instance()->update(0.01f);
				if (ADAI::ADPathfinding::Instance()->isDone())
				{
					if (minion)
					{
						if (!minion->cancelPath)
						{
							Solution tempSolution = ADAI::ADPathfinding::Instance()->getSolutionPoints();
							//minion->myPath 
							if (tempSolution.positions.size() > 0)
							{	
								if (!minion->hasPath)
								{
									//Set new target
									minion->myPath = tempSolution;
									if (minion->potentialTarget)
									{
										if (tempSolution.positions.size() * 12 < 500)
										{
											minion->currentTarget = minion->potentialTarget;
											minion->currentTargetLocation = minion->potentialTarget->GetPosition();
											minion->potentialTarget == nullptr;
										}
									}
									minion->hasPath = true;
									minion->currentPointIndex = (minion->myPath.positions.size() - 1);
									minion->currentPointInPath = minion->myPath.positions[minion->currentPointIndex];
									minion = nullptr;
								}
								else if ((tempSolution.positions.size()) < (minion->currentPointIndex) && (tempSolution.positions.size() * 12) < 500)
								{
									minion->myPath = tempSolution;
									if (minion->potentialTarget)
									{
										minion->currentTarget = minion->potentialTarget;
										minion->currentTargetLocation = minion->potentialTarget->GetPosition();
										minion->potentialTarget == nullptr;
									}
									minion->hasPath = true;
									minion->currentPointIndex = (minion->myPath.positions.size() - 1);
									minion->currentPointInPath = minion->myPath.positions[minion->currentPointIndex];
									minion = nullptr;
								}

							}
							else if (!minion->hasPath)
							{
								minion->pathingFailed = true;
								minion->hasPath = false;
								minion->cancelPath = false;
								minion = nullptr;
							}
						}
						else
						{
							minion->hasPath = false;
							minion->cancelPath = false;
							minion = nullptr;
						}
					}
					if (pathingQueue.size() > 0)
					{
						bool nextMinion = true;
						while (nextMinion && pathingQueue.size() > 0)
						{
							minion = pathingQueue.front().currentUnit;
							currentTarget = pathingQueue.front().currentTarget;
							minion->currentTargetLocation = currentTarget;
							pathingQueue.pop();
							minion->waitingForPath = false;
							if (minion->cancelPath)
							{
								minion->cancelPath = false;
								nextMinion = true;;
							}
							else
							{
								nextMinion = false;
							}
						} 

						UINT srow;
						UINT scolumn;
						UINT drow;
						UINT dcolumn;
						ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(minion->destructable->GetPosition().x, minion->destructable->GetPosition().z), scolumn, srow);
						ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(currentTarget.x, currentTarget.z), dcolumn, drow);
						int start = ADAI::ADPathfinding::Instance()->findAcceptablePoint(scolumn, srow);
						if (start == 0)
						{
								ADAI::ADPathfinding::Instance()->enter(scolumn, srow, minion->validDestinationX, minion->validDestinationZ);
								minion->pathingFailed = false;
								minion->cancelPath = false;
								return;
						}
						//path fail.
						minion->currentTarget = nullptr;
						minion->pathingFailed = true;
						minion->cancelPath = true;
					}
				
				}
			}
			
		}

		void AddToQueueLimited(MinionAI* _currentUnit, XMFLOAT3 _currentTarget)
		{
			if (!_currentUnit->waitingForPath && pathingQueue.size() < 62)
			{
				pathingQueue.push(queuePair{ _currentUnit, _currentTarget });
				_currentUnit->waitingForPath = true;
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

		void AddToQueueNoCheck(MinionAI* _currentUnit, XMFLOAT3 _currentTarget)
		{
			pathingQueue.push(queuePair{ _currentUnit, _currentTarget });
			_currentUnit->waitingForPath = true;
		}
	};

	class CohesionGroup : public MovementGroup
	{
		XMFLOAT3 averagePosition = { 0,0,0 };
		float groupCohesionStrength = 0.01f;
		float cohesionRadius = 60.0;
		float cohesionLimitRadius = 400.0;

		void CalculateAverages()
		{
			averagePosition = { 0,0,0 };
			UINT count = 0;
			for (auto& member : members)
			{
				if (member->active)
				{

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
		float groupSeperationStrength = 1.3f;

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
		float groupTargetingStrength = 2.0f;

	public:


		void Update(float _deltaTime)
		{

		}

		XMVECTOR GetVelocityValue(ADResource::ADGameplay::GameObject* _currentUnit, XMFLOAT3 _currentTarget) override
		{
			XMVECTOR minionPos = ADMath::Float3ToVector(_currentUnit->GetPosition());
			XMVECTOR targetPos = ADMath::Float3ToVector(_currentTarget);
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

	class CommandState : public FlockingState
	{
	public:
		MinionAI* minion = nullptr;
		PathersQueue* pathingQueue = nullptr;
		//MinionGroup* myGroup;
		float targetCheckTimer = 0;
		const float targetCheckDuration = 0.5f;
		float distanceCheckingTimer = 0;
		const float distanceCheckingDuration = 0.8f;

		std::vector<ADResource::ADGameplay::GameObject*>* targets = nullptr;

		virtual void EnterState(float _deltaTime) override
		{
			if (minion->currentTarget)
			{
				if (!minion->currentTarget->active)
				{
					minion->currentTarget = nullptr;

				}
				else
				{
					minion->mySSM.SwitchState(2, _deltaTime);
					minion->destructable->anim_controller->PlayAnimationByName("Attack");

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
				}
			}
		}

		virtual void Update(float _deltaTime)
		{
			if (_deltaTime > 0)
			{

				XMFLOAT3 temp = minion->destructable->GetPosition();
				SearchNode* localChecker = ADAI::ADPathfinding::Instance()->GetTileFromPosition(XMFLOAT2(temp.x, temp.z));
				if (localChecker)
				{
					if (localChecker->tile->walkable)
					{
						minion->lastKnownGoodLocation = ADAI::ADPathfinding::Instance()->AdjustPosition(localChecker->tile->position);

					}
				}


				if (minion->hasPath) 
				{
					float distance2 = DistanceCalculation(minion->mySSM.gameObject->GetPosition(), minion->currentPointInPath);
					if (distance2 < 15)
					{
						if (minion->currentPointIndex > 0)
						{
							minion->currentPointInPath = minion->myPath.positions[--minion->currentPointIndex];
						}
						else
						{
							VelocityWhenFlocking = { 0,0,0,0 };
							minion->hasPath = false;
						}
					}
					else if (distance2 > 300)
					{
						minion->currentPointInPath = FindNearestPoint(minion->mySSM.gameObject, &minion->myPath.positions, 250, 1, minion->currentPointIndex);
						if (minion->currentPointIndex == 0)
						{
							VelocityWhenFlocking = { 0,0,0,0 };
							minion->hasPath = false;
						}
					}
				}
				targetCheckTimer += _deltaTime;
				if (targetCheckTimer > targetCheckDuration)
				{
					targetCheckTimer = 0;
					if (minion->currentTarget)
					{
						minion->potentialTarget = ADAI::FindNearest(mySSM->gameObject, *targets, 150.f, 1.f);
					}
					else
					{
						minion->potentialTarget = ADAI::FindNearest(mySSM->gameObject, *targets, 150.f, 1.f);
					}
					if (minion->potentialTarget)
					{
						if (minion->potentialTarget->active)
						{
							if (minion->currentTarget)
							{
								if (minion->potentialTarget != minion->currentTarget)
								{
									ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(minion->potentialTarget->GetPosition().x, minion->potentialTarget->GetPosition().z), minion->validDestinationX, minion->validDestinationZ);
									int end = ADAI::ADPathfinding::Instance()->findAcceptablePoint(minion->validDestinationX, minion->validDestinationZ);
									if (end == 0)
									{
										minion->potentialTarget->actionLevel += 1.8f;
										pathingQueue->AddToQueue(minion, minion->potentialTarget->GetPosition());
									}
								}
							}
							else
							{
								ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(minion->potentialTarget->GetPosition().x, minion->potentialTarget->GetPosition().z), minion->validDestinationX, minion->validDestinationZ);
								int end = ADAI::ADPathfinding::Instance()->findAcceptablePoint(minion->validDestinationX, minion->validDestinationZ);
								if (end == 0)
								{
									minion->potentialTarget->actionLevel += 1.8f;
									pathingQueue->AddToQueueLimited(minion, minion->potentialTarget->GetPosition());
								}
							}
						}
						else
						{
							targets->erase(std::remove(targets->begin(), targets->end(), minion->potentialTarget));
						}
					}
				}

				distanceCheckingTimer += _deltaTime;
				if (distanceCheckingTimer > distanceCheckingDuration)
				{
					distanceCheckingTimer = 0;
					if (minion->currentTarget)
					{
						if (minion->currentTarget->active)
						{
							float distance = DistanceCalculation(minion->mySSM.gameObject->GetPosition(), minion->currentTarget->GetPosition());
							XMFLOAT3 velocityLength;
							XMStoreFloat3(&velocityLength, XMVector4Length(XMLoadFloat4(&VelocityWhenFlocking)));

							if (minion->currentTarget->gamePlayType != ADResource::ADGameplay::OBJECT_TAG::COMMAND_MARKER && minion->currentTarget->team != minion->destructable->team)
							{
								if (distance < minion->currentTarget->attackRadius + minion->currentTarget->safeRadius)// && distance > minion->currentTarget->safeRadius)
								{
									minion->mySSM.SwitchState(2, _deltaTime);
									minion->destructable->anim_controller->PlayAnimationByName("Attack");

									minion->currentTarget->actionLevel += 8;

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
								else if (velocityLength.x < 0.1f)
								{
									ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(minion->currentTarget->GetPosition().x, minion->currentTarget->GetPosition().z), minion->validDestinationX, minion->validDestinationZ);
									int end = ADAI::ADPathfinding::Instance()->findAcceptablePoint(minion->validDestinationX, minion->validDestinationZ);
									if (end == 0)
									{
										pathingQueue->AddToQueueLimited(minion, minion->currentTargetLocation);
									}
								}
							}
						}
						else
						{
							minion->currentTarget = nullptr;
							minion->currentTargetLocation = minion->lastKnownGoodLocation;
							//minion->donePathing = true;
							minion->hasPath = false;
							minion->pathingFailed = false;
							minion->waitingForPath = false;
						}
					}
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

			}
		};
	};

	class MinionGroup
	{
	public:
		std::vector<MinionAI*> flockers;
		std::vector<CommandState*> flockerState;

		PathersQueue* pathingQueue = nullptr;
		std::vector<MovementGroup*> myGroups;
		std::vector<TargetedMovementGroup*> myTargetGroups;

		std::vector<ADResource::ADGameplay::GameObject*>* avoidanceGroup = nullptr;

		AttackingState* attacking = nullptr;
		float groupAvoidanceStrength = 0.08f;
		float pathingStrength = 2.0f;

		float flockRadius = 10.0;


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

			return targetP;
		};

		void ReviveAll(ADResource::ADGameplay::GameObject* _target)
		{
			for (auto& flocker : flockers)
			{
				if (flocker->destructable->active == false)
				{
					ADResource::ADGameplay::Stat* tempStat = flocker->destructable->GetStatSheet()->RequestStats("Health");
					tempStat->Set(999999);
					flocker->destructable->active = true;
					flocker->destructable->SetPosition(_target->GetPosition());
					flocker->currentTarget = nullptr;
				}
			}
		}

		void SetTarget(ADResource::ADGameplay::GameObject* _target)
		{
			UINT dColumn = 0;
			UINT dRow = 0;
			ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(_target->GetPosition().x, _target->GetPosition().z), dColumn, dRow);
			int end = ADAI::ADPathfinding::Instance()->findAcceptablePoint(dColumn, dRow);
			if (end == 0)
			{
				for (auto& flocker : flockers)
				{
					flocker->mySSM.currentState = flocker->mySSM.states[1];
					flocker->potentialTarget = nullptr;
					flocker->currentTarget = _target;
					flocker->currentTargetLocation = _target->GetPosition();
					flocker->validDestinationX = dColumn;
					flocker->validDestinationZ = dRow;
					pathingQueue->AddToQueue(flocker, flocker->currentTargetLocation);
				}

			}
		}

		void SetDestination(ADResource::ADGameplay::GameObject* _target)
		{
			UINT dColumn = 0;
			UINT dRow = 0;
			ADAI::ADPathfinding::Instance()->tileMap.GetColumnRowFromPosition(XMFLOAT2(_target->GetPosition().x, _target->GetPosition().z), dColumn, dRow);
			int end = ADAI::ADPathfinding::Instance()->findAcceptablePoint(dColumn, dRow);
			if (end == 0)
			{
				for (auto& flocker : flockers)
				{
					flocker->mySSM.currentState = flocker->mySSM.states[1];
					flocker->potentialTarget = nullptr;
					flocker->currentTarget = nullptr;
					flocker->hasPath = false;
					flocker->pathingFailed = false;
					flocker->donePathing = false;
					flocker->validDestinationX = dColumn;
					flocker->validDestinationZ = dRow;

					flocker->currentTargetLocation = _target->GetPosition();
					pathingQueue->AddToQueueNoCheck(flocker, flocker->currentTargetLocation);
				}
			}
		}

		void AddUnitToGroup(MinionAI* _flocker, CommandState* _flockersState)
		{
			flockers.push_back(_flocker);
			flockerState.push_back(_flockersState);
		};

		XMVECTOR CalculatePathingAcceleration(MinionAI* _minion)//, ADResource::ADGameplay::GameObject* _target)
		{
			XMVECTOR velocity = { 0,0,0 }; 
			XMVECTOR currentPos = ADMath::Float3ToVector(_minion->destructable->GetPosition());
			XMVECTOR targetPos = ADMath::Float3ToVector(_minion->myPath.positions[_minion->currentPointIndex]);

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
			velocity *= pathingStrength;
			return velocity;
		}

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

					float threshold = avoids->avoidRadius + _currentUnit->safeRadius+ _currentUnit->attackRadius;

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

		XMFLOAT4 HandleFacing(MinionAI* _currentUnit)
		{
			XMFLOAT4 heading;
			XMVECTOR velocity = XMLoadFloat4(&_currentUnit->destructable->Velocity);
			XMStoreFloat4(&heading, XMVector3Length(velocity));
			if (heading.x > 0.1f)
			{

				XMStoreFloat4(&heading, XMVector4Normalize(velocity));

				XMVECTOR direction = XMVector3Dot(velocity, XMVector3Normalize(_currentUnit->destructable->transform.r[0]));
				XMVECTOR backCheck = XMVector3Dot(velocity, XMVector3Normalize(_currentUnit->destructable->transform.r[2]));

				XMFLOAT3 fvec;
				XMStoreFloat3(&fvec, direction);
				XMFLOAT3 zvec;
				XMStoreFloat3(&zvec, backCheck);


				if (fvec.x > 0.15f)
				{
					XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(0.10f);
					_currentUnit->destructable->transform = DirectX::XMMatrixMultiply(tempMatrix, _currentUnit->destructable->transform);
					_currentUnit->turning = true;
				}
				else if (fvec.x < -0.15f)
				{
					XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(-0.10f);
					_currentUnit->destructable->transform = DirectX::XMMatrixMultiply(tempMatrix, _currentUnit->destructable->transform);
					_currentUnit->turning = true;
				}
				else if (zvec.x < 0)
				{
					XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(0.25f);
					_currentUnit->destructable->transform = DirectX::XMMatrixMultiply(tempMatrix, _currentUnit->destructable->transform);
					_currentUnit->turning = true;
				}
				else
				{
					_currentUnit->turning = false;
				}

			}
			return heading;
		}

		void Update(float _deltaTime)
		{
			if (_deltaTime > 0)
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

				for (int i = 0; i < flockers.size(); ++i)
				{
					if (flockers[i]->mySSM.gameObject->active)
					{
						XMFLOAT4 heading = HandleFacing(flockers[i]);
						XMVECTOR velocity = { 0,0,0, };


						if (flockers[i]->hasPath)
						{
							if (flockers[i]->currentTarget) //Path and Target
							{
								float distance = DistanceCalculation(flockers[i]->destructable->GetPosition(), flockers[i]->currentTarget->GetPosition());
								if (distance > 50.f)
								{

									for (auto& group : myGroups)
									{
										velocity += group->GetVelocityValue(flockers[i]->destructable);
									}
									velocity += CalculatePathingAcceleration(flockers[i]);
								
								}
								else						//Has target but is close enough to chase.
								{
									for (auto& group : myTargetGroups)
									{
										velocity += group->GetVelocityValue(flockers[i]->destructable, flockers[i]->currentTarget->GetPosition());
									}
								}
							}
							else
							{
								for (auto& group : myGroups)
								{
									velocity += group->GetVelocityValue(flockers[i]->destructable);
								}
								velocity += CalculatePathingAcceleration(flockers[i]);
							}
						}
						else //No Path
						{
							if (flockers[i]->pathingFailed)
							{

								for (auto& group : myTargetGroups)
								{
									velocity += group->GetVelocityValue(flockers[i]->destructable, flockers[i]->lastKnownGoodLocation);
								}
								velocity += CalculateAvoidanceAcceleration(flockers[i]->mySSM.gameObject);
							}
							else if (flockers[i]->currentTarget)
							{
								float distance = DistanceCalculation(flockers[i]->destructable->GetPosition(), flockers[i]->currentTarget->GetPosition());
								
								if (distance < 50.f && distance > flockers[i]->currentTarget->safeRadius+5.f)
								{
									for (auto& group : myTargetGroups)
									{
										velocity += group->GetVelocityValue(flockers[i]->destructable, flockers[i]->currentTarget->GetPosition());
									}

									velocity += CalculateAvoidanceAcceleration(flockers[i]->mySSM.gameObject);
								}
							}
							for (auto& group : myGroups)
							{
								velocity += group->GetVelocityValue(flockers[i]->destructable);
							}
						}



						velocity *= (flockers[i]->moveSpeed * _deltaTime);
						velocity += XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
						float decellerationRate = (5.f * _deltaTime) + 1.f;
						velocity /= decellerationRate;


						XMFLOAT4 length;
						XMStoreFloat4(&length, XMVector4Length(velocity));

						if (flockers[i]->pathingFailed && length.x < 0.08f)
						{
							flockers[i]->pathingFailed = false;
							flockers[i]->currentTarget = nullptr;
							flockers[i]->currentTargetLocation = flockers[i]->lastKnownGoodLocation;
						}
						else
						{
							if (length.x > flockers[i]->maxSpeed)
							{
								velocity = ShortenLength(velocity, flockers[i]->maxSpeed);
							}

						}
						XMFLOAT4 yCancel;
						XMStoreFloat4(&yCancel, velocity);
						yCancel.y = 0;
						velocity = XMLoadFloat4(&yCancel);
							XMStoreFloat4(&flockerState[i]->VelocityWhenFlocking, velocity);

					}
				}
			}

		};
	};





	class TowerAI
	{
	public:
		SimpleStateMachine mySSM;
		XMFLOAT3 currentTargetLocation = { 0,0,0 };
		ADResource::ADGameplay::Action* myAttack = nullptr;
		ADResource::ADGameplay::Building* building = nullptr;
		ADResource::ADGameplay::GameObject* currentTarget = nullptr;

		void Update(float _deltaTime)
		{
			if (mySSM.gameObject->active)
			{
				mySSM.Update(_deltaTime);
				myAttack->Update(_deltaTime);
			}
		};
	};

	class TowerState : public State
	{
	public:
		TowerAI* tower = nullptr;
		std::vector<ADResource::ADGameplay::GameObject*>* targets = nullptr;
		float timer = 0;
		const float waitDuration = 0.2f;

		virtual void Update(float _deltaTime)
		{
			if (!tower->currentTarget)
			{
				timer += _deltaTime;
				if (timer > waitDuration)
				{
					tower->currentTarget = ADAI::FindNearest(tower->building, *targets, 200.f, 1.f);
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

}


