#pragma once
//#include "GameUtilities.h"
//#include "GameObjectClasses.h"
#include "ADUserInterface.h"

#include <iostream>
#include <fstream>
#include <string>
#include <Types.h>
#include <unordered_map>


class StatChange : public ADResource::ADGameplay::Effect
{
public:
	std::vector<std::string> statsAffected;
	//Index aligns with statsAffected.
	std::vector<int> minimumChange;
	//Index aligns with statsAffected.
	std::vector<int> maximumChange;

	StatChange()
	{
		tickDuration = 0.2;
		tickTimer = 0;
		currentTick = 0;
		tickCount = 1;
		isFinished = false;
	};

	virtual UINT OnApply(ADResource::ADGameplay::StatSheet* _targetsStatSheet) override
	{
		if (_targetsStatSheet->RequestStats(statsAffected[0]))
		{
			targetedStats.push_back(_targetsStatSheet->RequestStats(statsAffected[0]));
		}
		else
		{
			isFinished = true;
		}
		if (tickOnEnter)
		{
			Tick();
		}
		tickTimer = tickDuration;
		return 0;
	};

	virtual UINT Tick() override
	{
		for (int i = 0; i < targetedStats.size(); i++)
		{
			targetedStats[i]->currentValue += minimumChange[i];
			targetedStats[i]->currentValue = min(targetedStats[i]->currentValue, targetedStats[i]->maxValue);
			targetedStats[i]->Set(max(targetedStats[i]->currentValue, targetedStats[i]->minValue));
		}
		return 0;
	};

	virtual UINT OnExit() override
	{
		if (tickOnExit)
		{
			Tick();
		}
		return 0;
	};

	std::unique_ptr<StatChange> clone() const
	{
		return std::unique_ptr<StatChange>(this->clone_impl());
	};
protected:
	virtual StatChange* clone_impl() const override
	{
		return new StatChange(*this);
	}
};



