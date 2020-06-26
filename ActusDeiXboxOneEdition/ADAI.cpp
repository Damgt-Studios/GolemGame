#include "pch.h"
#include "ADAI.h"

void ADAI::IdleState::Update(float _deltaTime)
{
	ADResource::ADGameplay::GameObject* target = FindNearest(owner->owner, objectsToAvoid, 50, 0);
	if (target != nullptr)
	{
		owner->currentState = owner->states[1];
		owner->currentState->personalTarget = target->transform.r[3];
	}
}

void ADAI::WayPointState::Update(float _deltaTime)
{
	if (FindNearest(owner->owner, objectsToAvoid, 50, 0) != nullptr)
	{
		owner->currentState = owner->states[1];
	}
}

void ADAI::FlockingState::Update(float _deltaTime)
{
	ADResource::ADGameplay::GameObject* target = FindNearest(owner->owner, objectsToAvoid, 50, 0);
	if (target != nullptr)
	{
		owner->currentState = owner->states[1];
		owner->currentState->personalTarget = target->transform.r[3];
	}
	//if (FindNearest(owner->owner, objectsToApproach, 50, 0) != nullptr)
	//{
	//	owner->currentState = owner->states[1];
	//}
	owner->owner->Velocity = VelocityWhenFlocking;
	owner->owner->AddToPositionVector((XMFLOAT3&)owner->owner->Velocity);
}

void ADAI::AIUnit::Update(float _deltaTime)
{
	currentState->Update(_deltaTime);
}
