#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Types.h>
#include <unordered_map>
//#include "GameUtilities.h"
//#include "GameObjectClasses.h"


class DefinitionDatabase
{
	DefinitionDatabase() {};
	~DefinitionDatabase() { Shutdown(); };
	DefinitionDatabase(const DefinitionDatabase& _rhs) {};
	DefinitionDatabase& operator =(const DefinitionDatabase& _rhs) {};

public:

	std::unordered_map<std::string, ADResource::ADGameplay::Effect*> effectsDatabase;
	std::unordered_map<std::string, ADResource::ADGameplay::StatSheet*> statsheetDatabase;
	std::unordered_map<std::string, ADResource::ADGameplay::Trigger*> hitboxDatabase;
	std::unordered_map<std::string, ADResource::ADGameplay::Action*> actionDatabase;

	static DefinitionDatabase* Instance()
	{
		static DefinitionDatabase instance;
		return &instance;
	};

	void Shutdown()
	{
		for (auto it = effectsDatabase.begin(), itEnd = effectsDatabase.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
		effectsDatabase.clear();
		for (auto it = statsheetDatabase.begin(), itEnd = statsheetDatabase.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
		statsheetDatabase.clear();
		for (auto it = hitboxDatabase.begin(), itEnd = hitboxDatabase.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
		hitboxDatabase.clear();
		for (auto it = actionDatabase.begin(), itEnd = actionDatabase.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
		actionDatabase.clear();
	};
};

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
		tickTimer = tickDuration;
		return 0;
	};

	virtual UINT Tick() override
	{
		for (int i = 0; i < targetedStats.size(); i++)
		{
			targetedStats[i]->currentValue += minimumChange[i];
			//if (targetedStats[i]->failValue == targetedStats[i]->currentValue)
			//	return -1;
			targetedStats[i]->currentValue = min(targetedStats[i]->currentValue, targetedStats[i]->maxValue);
			targetedStats[i]->currentValue = max(targetedStats[i]->currentValue, targetedStats[i]->minValue);
		}
		return 0;
	};

	virtual UINT OnExit() override
	{
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

class EssenceEffect : public ADResource::ADGameplay::Effect
{
public:
	EssenceEffect()
	{
		tickDuration = 0.2f;
		tickTimer = 0;
		currentTick = 0;
		tickCount = 1;
		isFinished = false;
	};

	virtual UINT OnApply(ADResource::ADGameplay::StatSheet* _targetsStatSheet) override
	{
		if (_targetsStatSheet->RequestStats("Health"))
		{
			targetedStats.push_back(_targetsStatSheet->RequestStats("Health"));
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
		for (int i = 0; i < targetedStats.size(); i++)
		{
			targetedStats[i]->currentValue -= 1;
			//if (targetedStats[0]->failValue == targetedStats[0]->currentValue)
			//	return -1;
			targetedStats[i]->currentValue = min(targetedStats[i]->currentValue, targetedStats[i]->maxValue);
			targetedStats[i]->currentValue = max(targetedStats[i]->currentValue, targetedStats[i]->minValue);
		}
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






class DefinitionReader
{
public:
	std::vector<std::string> definitionFiles;

	bool ReadEntity(std::string _entityStr)
	{
		const std::string delimiter = "=";
		std::string lhs;
		std::string mid = "";
		std::string rhs = "";
		size_t pos = 0;
		size_t midPos = 0;
		size_t endPos = 0;
		while (_entityStr.substr(0, 1) == "\n")
		{
			_entityStr.erase(0, 1);
		}
		while ((pos = _entityStr.find(delimiter)) != std::string::npos) {
			lhs = _entityStr.substr(0, pos);
			if ((endPos = _entityStr.find(':')) != std::string::npos)
			{
				mid = _entityStr.substr(pos + 1, endPos - (pos + 1));
				pos = endPos;
				endPos = _entityStr.find('\n');
				rhs = _entityStr.substr(pos + 1, endPos - (pos + 1));
				pos = endPos;
			}
			if (lhs == "StatSheet")
			{
				_entityStr.erase(0, endPos + 1);
				ADResource::ADGameplay::StatSheet* statSheet = new ADResource::ADGameplay::StatSheet();
				std::string currentStat;
				while (endPos != std::string::npos)
				{
					midPos = _entityStr.find('=');
					endPos = _entityStr.find('\n');
					lhs = _entityStr.substr(0, midPos);
					rhs = _entityStr.substr(midPos + 1, endPos - (midPos + 1));

					if (lhs == "AddStat")
					{
						statSheet->AddStat(rhs);
						currentStat = rhs;
					}
					if (lhs == "StatMin")
					{
						statSheet->SetMin(currentStat, std::stoi(rhs));
					}
					if (lhs == "StatMax")
					{
						statSheet->SetMax(currentStat, std::stoi(rhs));
					}
					if (lhs == "StatFail")
					{
						statSheet->SetEvent(currentStat, std::stoi(rhs));
					}
					if (lhs == "StatCurrent")
					{
						statSheet->SetCurrent(currentStat, std::stoi(rhs));
					}
					_entityStr.erase(0, endPos + 1);
				}
				//mid
				DefinitionDatabase::Instance()->statsheetDatabase[mid] = statSheet;
				//Needs an ID from a map.
			}
			else if (lhs == "Effect")
			{
				if (rhs == "StatChange")
				{
					_entityStr.erase(0, endPos + 1);
					StatChange* effect = new StatChange();
					while (endPos != std::string::npos)
					{
						midPos = _entityStr.find('=');
						endPos = _entityStr.find('\n');
						lhs = _entityStr.substr(0, midPos);
						rhs = _entityStr.substr(midPos + 1, endPos - (midPos + 1));

						if (lhs == "TickCount")
						{
							effect->tickCount = std::stoi(rhs);
						}
						if (lhs == "TickOnEnter")
						{
							if (rhs == "true")
								effect->tickOnEnter = true; \
						}
						if (lhs == "TickOnExit")
						{
							if (rhs == "true")
								effect->tickOnExit = true; \
						}
						if (lhs == "Stat")
						{
							effect->statsAffected.push_back(rhs);
						}
						if (lhs == "MinValue")
						{
							int min = std::stoi(rhs);
							effect->minimumChange.push_back(min);
						}
						if (lhs == "MaxValue")
						{
							int min = std::stoi(rhs);
							effect->maximumChange.push_back(min);
						}
						_entityStr.erase(0, endPos + 1);
					}
					DefinitionDatabase::Instance()->effectsDatabase[mid] = effect;
					//Needs an ID from a map.
				}
			}
			else if (lhs == "Hitbox")
			{
				_entityStr.erase(0, endPos + 1);
				ADResource::ADGameplay::Trigger* trigger = new ADResource::ADGameplay::Trigger();
				XMFLOAT3 scale = XMFLOAT3(1,1,1);
				trigger->SetScale(scale);
				trigger->SetRotation(scale);
				trigger->SetPosition(scale);
				std::vector<std::string> stoneMinionAnimations;
				stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");
				AD_ULONG id = ResourceManager::AddAnimatedModel("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, XMFLOAT3(300, 0, 100), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));

				trigger->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f)); 
				trigger->SetMeshID(id);
				trigger->active = false;
				trigger->team = 1;


				std:string currentEffect;
				while (endPos != std::string::npos)
				{
					midPos = _entityStr.find('=');
					endPos = _entityStr.find('\n');
					lhs = _entityStr.substr(0, midPos);
					rhs = _entityStr.substr(midPos + 1, endPos - (midPos + 1));

					if (lhs == "AddEffect")
					{
						trigger->effects.push_back(DefinitionDatabase::Instance()->effectsDatabase[rhs]->clone());
						trigger->effects[0].get()->sourceID = ResourceManager::GenerateEffectID();
						currentEffect = rhs;
					}
					if (lhs == "OffsetX")
					{
						trigger->offsetX = std::stof(rhs);
					}
					if (lhs == "OffsetZ")
					{
						trigger->offsetZ = std::stof(rhs);
					}
					if (lhs == "Collider_Width")
					{
						scale.x = std::stof(rhs);
					}
					if (lhs == "Collider_Height")
					{
						scale.y = std::stof(rhs);
					}
					if (lhs == "Collider_Length")
					{
						scale.z = std::stof(rhs);
					}
					if (lhs == "Model_Scale_Width")
					{
						scale.x = std::stof(rhs);
					}
					if (lhs == "Model_Scale_Height")
					{
						scale.y = std::stof(rhs);
					}
					if (lhs == "Model_Scale_Length")
					{
						scale.z = std::stof(rhs);
					}
					if (lhs == "SingleHit")
					{
						if (rhs == "true")
							trigger->isDeactivateOnFirstApplication = true;
					}
					_entityStr.erase(0, endPos + 1);
				}
				trigger->colScale = scale;
				trigger->collider = ADPhysics::AABB(XMFLOAT3(1,1,1), scale);
				trigger->collider.trigger = true;
				trigger->colliderPtr = &trigger->collider;
				DefinitionDatabase::Instance()->hitboxDatabase[mid] = trigger;
				//GameUtilities::AddGameObject(trigger);
				//Needs an ID from a map.
			}
			else if (lhs == "Action")
			{
				ADResource::ADGameplay::Action* action = new ADResource::ADGameplay::Action();
				action->hitboxCount = 0;
				while (endPos != std::string::npos)
				{
					midPos = _entityStr.find('=');
					endPos = _entityStr.find('\n');
					lhs = _entityStr.substr(0, midPos);
					rhs = _entityStr.substr(midPos + 1, endPos - (midPos + 1));

					if (lhs == "AddHitBox")
					{
						action->hitbox = DefinitionDatabase::Instance()->hitboxDatabase[rhs];
						action->hitboxCount++;
					}
					if (lhs == "Cooldown")
					{
						action->cooldownDuration = std::stof(rhs);
					}
					if (lhs == "Duration")
					{
						action->attackDuration = std::stof(rhs);
					}
					if (lhs == "Persist_After_Attack")
					{

					}
					_entityStr.erase(0, endPos + 1);
				}
				DefinitionDatabase::Instance()->actionDatabase[mid] = action;
				//Needs an ID from a map.
			}
		}

		return 0;
	}

	bool ReadDefinesFile(std::string _filename) {
		std::string entity;
		ifstream myfile(_filename);

		if (myfile.is_open())
		{
			while (std::getline(myfile, entity, ';')) {
				ReadEntity(entity);
			}
			myfile.close();
		}

		return 0;
	}

	bool ReadMasterFile() {
		string line;
		ifstream myfile("files\\definitions\\MasterDefinesList.txt");
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				definitionFiles.push_back(line);
			}
			myfile.close();
		}
		else cout << "Unable to open master defines file";

		for (int i = 0; i < definitionFiles.size(); i++)
		{
			ReadDefinesFile(definitionFiles[i]);
		}

		return 0;
	}
};
