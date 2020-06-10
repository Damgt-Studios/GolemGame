#include "pch.h"
#include "AnimationStateMachine.h"

void AnimationStateMachine::Initialize(ADResource::ADGameplay::GameObject* obj)
{

	SimpleModel** mod = ResourceManager::GetSimpleModelPtrFromMeshId(obj->GetMeshId());
	 modAnim = static_cast<SimpleAnimModel*>(*mod);

	GenerationAnimationStates(modAnim->animations);
	

}

void AnimationStateMachine::CreateAnimationState( string name, double duration, int index, bool truefalseCondition, float valueCondition)
{
	AnimationState n_AnimState{ name, duration, truefalseCondition,valueCondition, index };

	A_States.push_back(n_AnimState);



}

void AnimationStateMachine::GenerationAnimationStates(vector<anim_clip> animations)
{
	for (unsigned int i = 0; i < animations.size(); i++)
	{
		CreateAnimationState(animations[i].animfilename, animations[i].duration, i);
	}

}


void AnimationStateMachine::PlayAnimationByName(string name)
{
	if (currentState.name != name)
	{
		for (unsigned int i = 0; i < A_States.size(); i++)
		{
			if (A_States[i].name == name)
			{
				currentState = A_States[i];
				break;
			}
		}
	};
}

void AnimationStateMachine::PlayAnimationByNameBasedOnBoolCondition(string name, bool condition)
{
	if (currentState.name != name)
	{
		for (unsigned int i = 0; i < A_States.size(); i++)
		{
			if (A_States[i].name == name && A_States[i].boolCondition == condition)
			{
				currentState = A_States[i];
				break;
			}
		}
	}
}

void AnimationStateMachine::PlayAnimationByNameBasedOnValueCondition(string name, float value)
{
	if (currentState.name != name)
	{
		for (unsigned int i = 0; i < A_States.size(); i++)
		{
			if (A_States[i].name == name && A_States[i].valueCondition == value)
			{
				currentState = A_States[i];
				break;
			}
		}
	}
}

void AnimationStateMachine::PlayAnimationByNameBasedOnAllCondition(string name, bool condition, float value)
{
	if (currentState.name != name)
	{
		for (unsigned int i = 0; i < A_States.size(); i++)
		{
			if (A_States[i].name == name && A_States[i].valueCondition == value && A_States[i].boolCondition == condition)
			{
				currentState = A_States[i];
				break;
			}
		}
	}
}

void AnimationStateMachine::SetAnimationConditionBool(string name, bool conditionBool)
{
	for (unsigned int i = 0; i < A_States.size(); i++)
	{
		if (A_States[i].name == name)
		{
			A_States[i].boolCondition = conditionBool;
			break;
		}
	}
}

void AnimationStateMachine::SetAnimationConditionValue(string name, float value)
{
	for (unsigned int i = 0; i < A_States.size(); i++)
	{
		if (A_States[i].name == name)
		{
			A_States[i].valueCondition = value;
			break;
		}
	}


}

void AnimationStateMachine::SetBothConditions(string name, bool conditionBool, float value)
{
	for (unsigned int i = 0; i < A_States.size(); i++)
	{
		if (A_States[i].name == name)
		{
			A_States[i].valueCondition = value;
			A_States[i].boolCondition = conditionBool;
			break;
		}
	}
}

void AnimationStateMachine::SetName(string prevName,  string newName)
{
	for (unsigned int i = 0; i < A_States.size(); i++)
	{
		if (A_States[i].name == prevName)
		{
			A_States[i].name = newName;
			break;
		}
	}

}

void AnimationStateMachine::SetNamebyIndex(int index,  string name)
{
	A_States[index].name = name;
}

double AnimationStateMachine::GetDurationByName(string name)
{
	for (unsigned int i = 0; i < A_States.size(); i++)
	{
		if (A_States[i].name == name)
		{
			return A_States[i].duration;
		}
	}
}




int AnimationStateMachine::GetCurrentAnimation()
{

	return currentState.index;
}

void AnimationStateMachine::SetModel_To_CurrentAnimation()
{
	
	modAnim->SetAnimCurrentState(currentState.index);
}


