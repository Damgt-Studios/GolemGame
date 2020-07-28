#pragma once
#include "pch.h"
#include "MeshLoader.h"
#include "Renderable.h"
//#include "../Game/GameplayBaseClasses.h"
using namespace std;
class AnimationStateMachine
{
	struct AnimationState
	{
		string name;
		double duration;
		bool boolCondition = false;
		float valueCondition = 0;
		int index;

		inline bool operator=(AnimationState a)
		{
			this->name = a.name;
			this->boolCondition = a.boolCondition;
			this->valueCondition = a.valueCondition;
			this->index = a.index;

			return true;
		}
	};
	AnimationState currentState;
	SimpleAnimModel* modAnim;
public:
	vector<AnimationState> A_States;
	inline bool operator=(AnimationStateMachine a)
	{
		this->currentState = a.currentState;
		this->modAnim = a.modAnim;
		this->A_States = a.A_States;
	}
	inline bool operator=(AnimationStateMachine* a)
	{
		this->currentState = a->currentState;
		this->modAnim = a->modAnim;
		this->A_States = a->A_States;
	}
	inline bool operator=(AnimationStateMachine& a)
	{
		this->currentState = a.currentState;
		this->modAnim = a.modAnim;
		this->A_States = a.A_States;
	}


	void Initialize(ADResource::ADGameplay::GameObject* obj);


	void CreateAnimationState(string name, double duration, int index, bool truefalseCondition = false, float valueCondition = 0);
	void GenerationAnimationStates(vector<anim_clip> animations);
	void PlayAnimationByName(string name);
	void PlayAnimationByNameBasedOnBoolCondition(string name, bool condition);
	void PlayAnimationByNameBasedOnValueCondition(string name, float value);
	void PlayAnimationByNameBasedOnAllCondition(string name, bool condition, float value);
	void SetAnimationConditionBool(string name, bool conditionBool);
	void SetAnimationConditionValue(string name, float value);
	void SetBothConditions(string name, bool condition, float valuecondition);
	void SetName(string prevName, const string newName);
	void SetNamebyIndex(int index, const string name);
	double GetDurationByName(string name);
	int GetCurrentAnimation();
	void SetModel_To_CurrentAnimation();







};

