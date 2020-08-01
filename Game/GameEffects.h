#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Types.h>
#include <unordered_map>
//#include "GameUtilities.h"
//#include "GameObjectClasses.h"
#include "ADUserInterface.h"


class StatChange : public ADResource::ADGameplay::Effect
{
public:
	std::vector<std::string> statsAffected;
	std::vector<std::string> counterStatsIncluded;
	std::vector<ADResource::ADGameplay::Stat*> counterStats;
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
		for (auto& stat : statsAffected)
		{
			if (_targetsStatSheet->RequestStats(stat))
			{
				targetedStats.push_back(_targetsStatSheet->RequestStats(stat));
			}
		}
		for (auto& stat : counterStatsIncluded)
		{
			if (_targetsStatSheet->RequestStats(stat))
			{
				counterStats.push_back(_targetsStatSheet->RequestStats(stat));
			}
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
			int change = static_cast<int>(RandFloat(minimumChange[i], maximumChange[i]));
			if (i < counterStats.size())
			{
				if (counterStats[i]->currentValue > abs(change))
				{
					change = 0;
				}
				else
				{
					change -= counterStats[i]->currentValue;
				}
			}
			
			targetedStats[i]->currentValue += change;
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



class StatBuff : public ADResource::ADGameplay::Effect
{
public:
	std::vector<std::string> statsAffected;
	std::vector<int> minimumChange;
	std::vector<int> maximumChange;
	int actualChange = 0;

	StatBuff()
	{
		tickDuration = 0.2;
		tickTimer = 0;
		currentTick = 0;
		tickCount = 0;
		isFinished = false;
	};

	virtual UINT OnApply(ADResource::ADGameplay::StatSheet* _targetsStatSheet) override
	{
		for (auto& stat : statsAffected)
		{
			if (_targetsStatSheet->RequestStats(stat))
			{
				targetedStats.push_back(_targetsStatSheet->RequestStats(stat));
			}
			
		}
		Tick();
		tickTimer = tickDuration;
		return 0;
	};

	virtual UINT Tick() override
	{
		for (int i = 0; i < targetedStats.size(); i++)
		{
			int previousValue = targetedStats[i]->currentValue;
			int change = static_cast<int>(RandFloat(minimumChange[i], maximumChange[i]));
			targetedStats[i]->currentValue += change;
			targetedStats[i]->currentValue = min(targetedStats[i]->currentValue, targetedStats[i]->maxValue);
			targetedStats[i]->Set(max(targetedStats[i]->currentValue, targetedStats[i]->minValue));
			actualChange = previousValue - targetedStats[i]->currentValue;
		}
		return 0;
	};

	virtual UINT OnExit() override
	{
		for (int i = 0; i < targetedStats.size(); i++)
		{
			targetedStats[i]->currentValue += actualChange;
			targetedStats[i]->currentValue = min(targetedStats[i]->currentValue, targetedStats[i]->maxValue);
			targetedStats[i]->Set(max(targetedStats[i]->currentValue, targetedStats[i]->minValue));
		}
		return 0;
	};

	std::unique_ptr<StatBuff> clone() const
	{
		return std::unique_ptr<StatBuff>(this->clone_impl());
	};
protected:
	virtual StatBuff* clone_impl() const override
	{
		return new StatBuff(*this);
	}
};


