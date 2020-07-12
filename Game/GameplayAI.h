#pragma once
#include <ADAI.h>
#include <ADCombat.h>
#include "GameObjectClasses.h"

namespace ADAI
{
	class VillagerAI
	{
	public:
		SimpleStateMachine mySSM;
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
			mySSM.Update(_deltaTime);
		};
	};

	class MinionAI
	{
	public:
		SimpleStateMachine mySSM;
		XMFLOAT3 currentTargetLocation;
		ADResource::ADGameplay::GameObject* currentTarget;
		ADResource::ADGameplay::Action* myAttack;

		//Waypoints (both direct and pathing)
		float directionalStrength = 0.00f;

		//Safety for Fleeing and Cowering
		float desirableTargetCohesionStrength = 1.0f;

		//Danger for Fleeing
		float fleeingTargetSeperationStrength = 0.00f;

		void Update(float _deltaTime)
		{
			mySSM.Update(_deltaTime);
			myAttack->Update(_deltaTime);
		};
	};

	class TowerAI
	{
	public:
		SimpleStateMachine mySSM;
		XMFLOAT3 currentTargetLocation;
		ADResource::ADGameplay::Building* building;
		ADResource::ADGameplay::GameObject* currentTarget;

		void Update(float _deltaTime)
		{
			mySSM.Update(_deltaTime);
		};
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
					tower->currentTarget = ADAI::FindNearest(tower->building, *targets, 800.f, 1.f);
				}
			}
			else
			{
				if (!tower->currentTarget->active)
				{
					tower->currentTarget = nullptr;
				}
				else
				{
					XMVECTOR pos = tower->building->transform.r[3];

					XMVECTOR distanceToPlane = XMVector3Dot({ 0,1,0 }, (tower->currentTarget->transform.r[3] - tower->building->GetModelByIndex(0)->transform.r[3]));
					XMVECTOR pointOnPlane = tower->currentTarget->transform.r[3] - (XMVECTOR{ 0.f,1.f,0.f } * distanceToPlane.m128_f32[0]);

					XMVECTOR forward = XMVector3Normalize(tower->building->GetModelByIndex(0)->transform.r[3] - pointOnPlane);

					XMVECTOR right = XMVector3Cross(XMVector3Normalize({ 0.f,1.f,0.f }), forward);
					XMVECTOR up = XMVector3Cross(forward, right);

					tower->building->GetModelByIndex(0)->transform.r[0] = right;
					tower->building->GetModelByIndex(0)->transform.r[1] = up;
					tower->building->GetModelByIndex(0)->transform.r[2] = -forward;


					tower->building->GetModelByIndex(0)->SetScale({ 0.05,0.05,0.05 });
					tower->building->GetModelByIndex(0)->SetPosition({ pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2] });
				}
			}
		};
	};

	class VillagerGroup
	{
	public:
		std::vector<VillagerAI*> flockers;
		std::vector<FlockingState*> flockerState;
		XMVECTOR averagePosition;
		XMVECTOR averageForward;

		float groupCohesionStrength = 0.01f;
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

		XMVECTOR CalculateCohesionAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMFLOAT3 sigh = _object->GetPosition();
			XMVECTOR ap = XMLoadFloat3(&sigh);
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
				XMVECTOR velocity = XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
				XMFLOAT4 heading;
				XMStoreFloat4(&heading, XMVector4Normalize(velocity));

				flockerState[i]->VelocityWhenFlocking.x += heading.x * _deltaTime * moveSpeed;
				flockerState[i]->VelocityWhenFlocking.y += heading.y * _deltaTime * moveSpeed;
				flockerState[i]->VelocityWhenFlocking.z += heading.z * _deltaTime * moveSpeed;


				XMVECTOR direction = XMVector3Dot(velocity, XMVector3Normalize(flockers[i]->mySSM.gameObject->transform.r[0]));
				XMFLOAT3 fvec;
				XMStoreFloat3(&fvec, direction);

				if (fvec.x > 0.2f)
				{
					XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(0.10f);
					flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
				}
				else if (fvec.x < -0.2f)
				{
					XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(-0.10f);
					flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
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
				velocity *= (maxSpeed * _deltaTime);
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
					ADResource::ADGameplay::GameObject* tempTarget = ADAI::FindNearest(mySSM->gameObject, *fearedObjects, 12.f, 1.f);
					if (tempTarget)
					{
						if (tempTarget->gamePlayType != ADResource::ADGameplay::OBJECT_TAG::PLAYER)
						{
							tempTarget->actionLevel += 5;
							villager->mySSM.SwitchState(2, _deltaTime);

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
					villager->currentStructure = ADAI::FindNearest(mySSM->gameObject, safetyStructures, 2000.f, 1.f);
				}
			}
			else
			{
				for (auto& element : safetyStructures)
				{
					villager->currentStructure = ADAI::FindNearest(mySSM->gameObject, safetyStructures, 2000.f, 1.f);
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
			mySSM->gameObject->AddToPositionVector((XMFLOAT3&)mySSM->gameObject->Velocity);
		};
	};

	class MinionGroup
	{
	public:
		std::vector<MinionAI*> flockers;
		std::vector<FlockingState*> flockerState;
		AttackingState* attacking;
		XMVECTOR averagePosition;
		XMVECTOR averageForward;

		//Group Behavior
		float alignmentDirectionalStrength = 0.0f;
		float groupCohesionStrength = 0.7f;
		float groupSeparationStrength = 1.0f;

		float commandDirectionalStrength = 0.f;
		//float returnDirectionalStrength = 0.f;

		float flockRadius = 10.0;
		float maxSpeed = 1.0f;
		float moveSpeed = 0.1f;

		XMVECTOR groupTarget;
		XMMATRIX* player;

		XMVECTOR commandDirection;
		XMVECTOR commandDestination;

		XMFLOAT3 SetCommandDirection(XMMATRIX _camera, float _distance)
		{
			for (int i = 0; i < flockers.size(); ++i)
			{
				flockers[i]->mySSM.currentState = flockers[i]->mySSM.states[1];
			}
			commandDirectionalStrength = 1.2f;
			//returnDirectionalStrength = 0.f;

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

			targetP.x += camFN.x * _distance;
			targetP.y = 0;
			targetP.z += camFN.z * _distance;

			commandDestination = XMLoadFloat3(&targetP);

			return targetP;
		};

		void SetTarget(ADResource::ADGameplay::GameObject* _target)
		{
			for (auto& flocker : flockers)
			{
				flocker->mySSM.currentState = flocker->mySSM.states[1];
				flocker->currentTarget = _target;
				//flocker->currentTargetLocation = _target->GetPosition();
			}
			commandDirectionalStrength = 1.2f;
			//currentTarget = golem->targetMarker;
		}

		void SetDestination(ADResource::ADGameplay::GameObject* _target)
		{
			for (auto& flocker : flockers)
			{
				flocker->mySSM.currentState = flocker->mySSM.states[1];
				flocker->currentTarget = nullptr;
				flocker->currentTargetLocation = _target->GetPosition();
			}
			commandDirectionalStrength = 1.2f;
			//currentTarget = golem->targetMarker;
		}
		//void ReturnCall()
		//{
		//	for (int i = 0; i < flockers.size(); ++i)
		//	{
		//		flockers[i]->mySSM.currentState = flockers[i]->mySSM.states[1];
		//	}
		//	commandDirectionalStrength = 0.f;
		//	returnDirectionalStrength = 1.2f;
		//};

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

		XMVECTOR CalculateAlignmentAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMVECTOR af = averageForward * (1.f / maxSpeed);
			af = XMVector4Normalize(af);
			af *= alignmentDirectionalStrength;
			return af;
		}

		XMVECTOR CalculateTargetAcceleration(MinionAI* _minion)//, ADResource::ADGameplay::GameObject* _target)
		{
			XMVECTOR minionPos = ADMath::Float3ToVector(_minion->mySSM.gameObject->GetPosition());
			XMVECTOR targetPos = ADMath::Float3ToVector(_minion->currentTargetLocation);
			//XMVECTOR targetPos = ADMath::Float3ToVector(_target->GetPosition());
			XMVECTOR velocity = targetPos - minionPos;

			velocity = XMVector4Normalize(velocity);

			velocity *= commandDirectionalStrength;
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


		XMVECTOR CalculateCohesionAcceleration(ADResource::ADGameplay::GameObject* _object)
		{
			XMFLOAT3 sigh = _object->GetPosition();
			XMVECTOR ap = XMLoadFloat3(&sigh);
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
				XMVECTOR velocity = XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);
				XMFLOAT4 heading;
				XMStoreFloat4(&heading, XMVector4Normalize(velocity));

				XMVECTOR direction = XMVector3Dot(velocity, XMVector3Normalize(flockers[i]->mySSM.gameObject->transform.r[0]));
				XMFLOAT3 fvec;
				XMStoreFloat3(&fvec, direction);

				if (fvec.x > 0.2f)
				{
					XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(0.10f);
					flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
				}
				else if (fvec.x < -0.2f)
				{
					XMMATRIX tempMatrix = DirectX::XMMatrixRotationY(-0.10f);
					flockers[i]->mySSM.gameObject->transform = DirectX::XMMatrixMultiply(tempMatrix, flockers[i]->mySSM.gameObject->transform);
				}

				flockerState[i]->VelocityWhenFlocking.x += heading.x * _deltaTime * moveSpeed;
				flockerState[i]->VelocityWhenFlocking.y += heading.y * _deltaTime * moveSpeed;
				flockerState[i]->VelocityWhenFlocking.z += heading.z * _deltaTime * moveSpeed;

				if (flockers[i]->currentTarget)
				{
					flockers[i]->currentTargetLocation = flockers[i]->currentTarget->GetPosition();
				}
				velocity = CalculateCohesionAcceleration(flockers[i]->mySSM.gameObject) + CalculateSeparationAcceleration(flockers[i]->mySSM.gameObject) + CalculateTargetAcceleration(flockers[i]);// +CalculateReturnAcceleration(flockers[i]);
				velocity *= (maxSpeed * _deltaTime);
				velocity += XMLoadFloat4(&flockers[i]->mySSM.gameObject->Velocity);

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
		};
	};

	class CommandState : public FlockingState
	{
	public:
		MinionAI* minion;
		MinionGroup myGroup;
		float timer = 0;
		const float waitDuration = 1.2f;

		std::vector<ADResource::ADGameplay::GameObject*>* targets;

		virtual void Update(float _deltaTime)
		{
			timer += _deltaTime;
			if (timer > waitDuration)
			{
				minion->currentTarget = ADAI::FindNearest(mySSM->gameObject, *targets, 200.f, 1.f);

				if (minion->currentTarget)
				{
					
					float distance = DistanceCalculation(minion->mySSM.gameObject->GetPosition(), minion->currentTarget->GetPosition());
					if (distance < minion->currentTarget->attackRadius + minion->currentTarget->safeRadius && distance > minion->currentTarget->safeRadius)
					{
						if (minion->currentTarget->gamePlayType != ADResource::ADGameplay::OBJECT_TAG::COMMAND_MARKER)
						{
							minion->currentTarget = minion->currentTarget;
							minion->currentTarget->actionLevel += 50;
							minion->mySSM.SwitchState(2, _deltaTime);


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
			}


			if (timer > waitDuration)
			{
				timer = 0;
			}
			mySSM->gameObject->Velocity = VelocityWhenFlocking;
			mySSM->gameObject->AddToPositionVector((XMFLOAT3&)mySSM->gameObject->Velocity);
		};
	};

}