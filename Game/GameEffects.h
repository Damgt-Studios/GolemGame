#pragma once


class EssenceEffect : public Effect
{
public:
	EssenceEffect()
	{
		tickDuration = 0.2;
		tickTimer = 0;
		currentTick = 0;
		tickCount = 1;
		isFinished = false;
	};

	virtual UINT OnApply(iStatSheet* _targetsStatSheet) override
	{
		if (_targetsStatSheet->RequestStats(HEALTH))
		{
			targetedStats.push_back(_targetsStatSheet->RequestStats(HEALTH));
		}
		else
		{
			isFinished = true;
		}
		tickTimer = tickDuration;
		return 0;
	};

	virtual UINT Tick() override
	{
		targetedStats[0]->currentValue -= 1;
		if (targetedStats[0]->failValue == targetedStats[0]->currentValue)
			return -1;
		targetedStats[0]->currentValue = min(targetedStats[0]->currentValue, targetedStats[0]->maxValue);
		targetedStats[0]->currentValue = max(targetedStats[0]->currentValue, targetedStats[0]->minValue);
		return 0;
	};

	virtual UINT OnExit() override
	{
		return 0;
	};

	std::unique_ptr<EssenceEffect> clone() const
	{
		return std::unique_ptr<EssenceEffect>(this->clone_impl());
	};
protected:
	virtual EssenceEffect* clone_impl() const override
	{
		return new EssenceEffect(*this);
	}
};
