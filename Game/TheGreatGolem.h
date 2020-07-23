#pragma once
#include "ADAudio.h"
#include "ADEventSystem.h"
#include "Listeners.h"
#include <Types.h>
#include "GameEffects.h"
#include "GameObjectClasses.h"
#include "GameUtilities.h"
#include "ADUserInterface.h"
#include "GameUserInterface.h"
//#include "AudioManager.h"
#include "MeshLoader.h"
#include "ADAI.h"
#include "ADPathfinding.h"
#include "AnimationStateMachine.h"
#include "Listeners.h"



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
					else if (lhs == "StatMin")
					{
						statSheet->SetMin(currentStat, std::stoi(rhs));
					}
					else if (lhs == "StatMax")
					{
						statSheet->SetMax(currentStat, std::stoi(rhs));
					}
					else if (lhs == "SetEvent")
					{
						statSheet->SetEvent(currentStat, rhs);
					}
					else if (lhs == "SetEventVal")
					{
						statSheet->SetEventVal(currentStat, std::stof(rhs));
					}
					else if (lhs == "StatCurrent")
					{
						statSheet->SetCurrent(currentStat, std::stoi(rhs));
					}
					else
					{
						std::string msg = "Error Loading Data Driven File: ";
						msg.append(lhs);
						msg.append(mid);
						msg.append(rhs);
						ADUI::MessageReceiver::Log(msg);
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
						else if (lhs == "TickDuration")
						{
							effect->tickDuration = std::stof(rhs);
						}
						else if (lhs == "TickOnEnter")
						{
							if (rhs == "true")
								effect->tickOnEnter = true; 
						}
						else if (lhs == "TickOnExit")
						{
							if (rhs == "true")
								effect->tickOnExit = true; 
						}
						else if (lhs == "Stat")
						{
							effect->statsAffected.push_back(rhs);
						}
						else if (lhs == "MinValue")
						{
							int min = std::stoi(rhs);
							effect->minimumChange.push_back(min);
						}
						else if (lhs == "MaxValue")
						{
							int min = std::stoi(rhs);
							effect->maximumChange.push_back(min);
						}
						else
						{
							std::string msg = "Error Loading Data Driven File: ";
							msg.append(lhs);
							msg.append(mid);
							msg.append(rhs);
							ADUI::MessageReceiver::Log(msg);
						}
						_entityStr.erase(0, endPos + 1);
					}
					DefinitionDatabase::Instance()->effectsDatabase[mid] = effect;
					//Needs an ID from a map.
				}
				else if (rhs == "StatBuff")
				{
					_entityStr.erase(0, endPos + 1);
					StatChange* effect = new StatChange();
					while (endPos != std::string::npos)
					{
						midPos = _entityStr.find('=');
						endPos = _entityStr.find('\n');
						lhs = _entityStr.substr(0, midPos);
						rhs = _entityStr.substr(midPos + 1, endPos - (midPos + 1));

						if (lhs == "Duration")
						{
							effect->tickDuration = std::stof(rhs);
						}
						else if (lhs == "Stat")
						{
							effect->statsAffected.push_back(rhs);
						}
						else if (lhs == "MinValue")
						{
							int min = std::stoi(rhs);
							effect->minimumChange.push_back(min);
						}
						else if (lhs == "MaxValue")
						{
							int min = std::stoi(rhs);
							effect->maximumChange.push_back(min);
						}
						else
						{
							std::string msg = "Error Loading Data Driven File: ";
							msg.append(lhs);
							msg.append(mid);
							msg.append(rhs);
							ADUI::MessageReceiver::Log(msg);
						}
						_entityStr.erase(0, endPos + 1);
					}
					DefinitionDatabase::Instance()->effectsDatabase[mid] = effect;
				}
				else if (rhs == "Stun")
				{

					_entityStr.erase(0, endPos + 1);
					StatChange* effect = new StatChange();
					while (endPos != std::string::npos)
					{
						midPos = _entityStr.find('=');
						endPos = _entityStr.find('\n');
						lhs = _entityStr.substr(0, midPos);
						rhs = _entityStr.substr(midPos + 1, endPos - (midPos + 1));

						if (lhs == "Duration")
						{
							effect->tickDuration = std::stof(rhs);
						}
						else
						{
							std::string msg = "Error Loading Data Driven File: ";
							msg.append(lhs);
							msg.append(mid);
							msg.append(rhs);
							ADUI::MessageReceiver::Log(msg);
						}
						_entityStr.erase(0, endPos + 1);
					}
					DefinitionDatabase::Instance()->effectsDatabase[mid] = effect;
				}
			}
			else if (lhs == "Hitbox")
			{
				_entityStr.erase(0, endPos + 1);
				ADResource::ADGameplay::HitBox* trigger = new ADResource::ADGameplay::HitBox();
				XMFLOAT3 scale = XMFLOAT3(1, 1, 1);
				trigger->SetScale(scale);
				trigger->SetRotation(scale);
				trigger->SetPosition(scale);


				//std::vector<std::string> stoneMinionAnimations;
				//stoneMinionAnimations.push_back("files/models/Minion_3_Idle.animfile");
				//AD_ULONG id = ResourceManager::AddAnimatedModel("files/models/Minion_3.AnimMesh", "files/textures/Minion_3.mat", stoneMinionAnimations, XMFLOAT3(300, 0, 100), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0));
				//trigger->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f)); 
				//trigger->SetMeshID(id);

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
						//auto tFoundIt = DefinitionDatabase::Instance()->effectsDatabase.find(rhs);
						//if (tFoundIt == DefinitionDatabase::Instance()->effectsDatabase.end())
						//{
						//	int a = 0;
						//}
						trigger->effects.push_back(DefinitionDatabase::Instance()->effectsDatabase[rhs]->clone());
						trigger->effects[0].get()->sourceID = ResourceManager::GenerateEffectID();
						currentEffect = rhs;
					}
					else if (lhs == "OffsetX")
					{
						trigger->offsetX = std::stof(rhs);
					}
					else if (lhs == "OffsetY")
					{
						trigger->offsetY = std::stof(rhs);
					}
					else if (lhs == "OffsetZ")
					{
						trigger->offsetZ = std::stof(rhs);
					}
					else if (lhs == "Collider_Width")
					{
						scale.x = std::stof(rhs);
					}
					else if (lhs == "Collider_Height")
					{
						scale.y = std::stof(rhs);
					}
					else if (lhs == "Collider_Length")
					{
						scale.z = std::stof(rhs);
					}
					else if (lhs == "Model_Name")
					{
						trigger->modelName = (rhs);
					}
					else if (lhs == "Model_Mat")
					{
						trigger->matName = (rhs);
					}
					else if (lhs == "Model_Width")
					{
						trigger->modelScale.x = std::stof(rhs);
					}
					else if (lhs == "Model_Height")
					{
						trigger->modelScale.y = std::stof(rhs);
					}
					else if (lhs == "Model_Length")
					{
						trigger->modelScale.z = std::stof(rhs);
					}
					else if (lhs == "XRotation")
					{
						trigger->rotation.x = std::stof(rhs);
					}
					else if (lhs == "YRotation")
					{
						trigger->rotation.y = std::stof(rhs);
					}
					else if (lhs == "ZRotation")
					{
						trigger->rotation.z = std::stof(rhs);
					}
					else if (lhs == "XVelocity")
					{
						trigger->vel.x = std::stof(rhs);
					}
					else if (lhs == "YVelocity")
					{
						trigger->vel.y = std::stof(rhs);
					}
					else if (lhs == "ZVelocity")
					{
						trigger->vel.z = std::stof(rhs);
					}
					else if (lhs == "Lifespan")
					{
						trigger->lifespan = std::stof(rhs);
					}
					else if (lhs == "Team")
					{
						trigger->team = std::stoi(rhs);
						if (trigger->team == 3)
						{
							trigger->gamePlayType = ADResource::ADGameplay::CONSUMPTION_HITBOX;
						}
					}
					else if (lhs == "SingleHit")
					{
						if (rhs == "true")
							trigger->isDeactivateOnFirstApplication = true;
					}
					else if (lhs == "SetEvent")
					{
						trigger->eventName = rhs;
					}
					else
					{
						std::string msg = "Error Loading Data Driven File: ";
						msg.append(lhs);
						msg.append(mid);
						msg.append(rhs);
						ADUI::MessageReceiver::Log(msg);
					}
					_entityStr.erase(0, endPos + 1);
				}


				if (trigger->modelName != "")
				{
					AD_ULONG id = ResourceManager::AddSimpleModel(trigger->modelName, trigger->matName, XMFLOAT3(1, 1, 1), trigger->modelScale, { 0,0,0 }); // trigger->modelScale
					trigger->SetMeshID(id);
					////GameUtilities::AttachModelToHitbox(trigger, trigger->modelName, "files/textures/Fireball.mat", XMFLOAT3(1, 1, 1), trigger->modelScale, XMFLOAT3(0, 0, 0));
				}
				else
				{
					AD_ULONG id = ResourceManager::AddRenderableCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), { 0,0,0 });
					trigger->SetMeshID(id);
				}
				trigger->colScale = scale;
				//trigger->transform = XMMatrixRotationX(trigger->rotation.x);// trigger->rotation);
				XMMATRIX matrix1 = XMMatrixTranslation(trigger->offsetX, trigger->offsetY, trigger->offsetZ);
				trigger->collider = ADPhysics::OBB(trigger->transform * matrix1, trigger->colScale); //XMFLOAT3(1, 1, 1)); 
				//trigger->collider.AxisX.x = trigger->colScale.x;
				//trigger->collider.AxisY.y = trigger->colScale.y;
				//trigger->collider.AxisZ.z = trigger->colScale.z;
				trigger->collider.trigger = true;
				trigger->SetScale(trigger->modelScale);//(scale);  
				trigger->colliderPtr = &trigger->collider;
				DefinitionDatabase::Instance()->hitboxDatabase[mid] = trigger;
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
						//auto tFoundIt = DefinitionDatabase::Instance()->hitboxDatabase.find(rhs);
						//if (tFoundIt == DefinitionDatabase::Instance()->hitboxDatabase.end())
						//{
						//	int a = 0;
						//}

						action->hitboxes.push_back(DefinitionDatabase::Instance()->hitboxDatabase[rhs]);
						//ResourceManager::AddGameObject(action->hitboxes[0]);
						action->hitboxFired.push_back(false);
						//HitBox* temp = action->hitbox->Clone();
						//action->hitbox = temp;
						action->hitboxCount++;
					}
					else if (lhs == "HitboxDuplicates")
					{
						action->hitboxCount = std::stoi(rhs);
						for (int i = 1; i < action->hitboxCount; ++i)
						{
							action->hitboxes.push_back(action->hitboxes[0]->Clone());
							action->hitboxFired.push_back(false);
						}
					}
					else if (lhs == "Cooldown")
					{
						action->cooldownDuration = std::stof(rhs);
					}
					else if (lhs == "Duration")
					{
						action->attackDuration = std::stof(rhs);
					}
					else if (lhs == "Persist_After_Attack")
					{
						if (rhs == "true")
							action->removeHbIfEnd = false;
					}
					else if (lhs == "AddEvent")
					{
						action->eventName.push_back(rhs);
						action->eventFired.push_back(false);
					}
					else if (lhs == "SetEventsDelay")
					{
						action->eventDelay.push_back(std::stof(rhs));
					}
					else if (lhs == "SetHitboxDelay")
					{
						action->hitboxDelay = std::stof(rhs);
					}
					else
					{
						std::string msg = "Error Loading Data Driven File: ";
						msg.append(lhs);
						msg.append(mid);
						msg.append(rhs);
						ADUI::MessageReceiver::Log(msg);
					}
					_entityStr.erase(0, endPos + 1);
				}
				//if (action->hitbox == nullptr || action->hitbox->colliderPtr == nullptr)
				//{
				//	std::string msg = "Dick Sauce ";
				//	msg.append(lhs);
				//	msg.append(mid);
				//	msg.append(rhs);
				//	ADUI::MessageReceiver::Log(msg);
				//}

				DefinitionDatabase::Instance()->actionDatabase[mid] = action;
				//Needs an ID from a map.
			}
			else
			{
				std::string msg = "Error Loading Data Driven File: ";
				msg.append(lhs);
				msg.append(mid);
				msg.append(rhs);
				ADUI::MessageReceiver::Log(msg);
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
		else
		{
			ADUI::MessageReceiver::Log("Unable to open master defines file");
		}

		for (int i = 0; i < definitionFiles.size(); i++)
		{
			ReadDefinesFile(definitionFiles[i]);
		}

		return 0;
	}
};


class TheGreatGolem
{
	AD_AUDIO::AudioSource titleMusic;
	AD_AUDIO::AudioSource levelMusic;
	AD_AUDIO::AudioSource endMusic;
	AudioSourceListener playTitleEvent = (titleMusic);
	AudioSourceListener playLevelEvent = (levelMusic);
	AudioSourceListener playEndEvent = (endMusic);
	AudioStopListener stopTitleMusicEvent = (titleMusic);
	AudioStopListener stopLevelMusicEvent = (levelMusic);
	AudioStopListener stopEndMusicEvent = (endMusic);
	AD_AUDIO::AudioSource golemSteps;
	AudioSourceListener golemStepEvent = (golemSteps);
	AD_AUDIO::AudioSource golemPunchSound;
	AudioSourceListener golemPunchEvent = (golemPunchSound);
	AD_AUDIO::AudioSource golemKickSound;
	AudioSourceListener golemKickEvent = (golemKickSound);
	AD_AUDIO::AudioSource golemSlamSound;
	AudioSourceListener golemSlamEvent = (golemSlamSound);
	AD_AUDIO::AudioSource golemWaveSound;
	AudioSourceListener golemWaveEvent = (golemWaveSound);
	AD_AUDIO::AudioSource golemFireball;
	AudioSourceListener golemFireballEvent = (golemFireball);
	AD_AUDIO::AudioSource golemIronHide;
	AudioSourceListener golemTauntEvent = (golemIronHide);
	AD_AUDIO::AudioSource golemRootingSpell;
	AudioSourceListener golemRootEvent = (golemRootingSpell);
	AD_AUDIO::AudioSource golemEatMinion;
	AudioSourceListener golemEatMinionEvent = (golemEatMinion);


	AD_AUDIO::AudioSource menuConfirmSound;
	AudioSourceListener menuConfirmEvent = (menuConfirmSound);
	AD_AUDIO::AudioSource menuReturnSound;
	AudioSourceListener menuReturnEvent = (menuReturnSound);
	AD_AUDIO::AudioSource menuCursorMovementSound;
	AudioSourceListener menuCursorMovementEvent = (menuCursorMovementSound);
	AD_AUDIO::AudioSource golemSliderMovementSound;
	AudioSourceListener golemSliderMovementEvent = (golemSliderMovementSound);

	ApplyEffectListener golemEatingEvent;


	MinionCountListener allMinionDeathListener;
	VillagerDeathListener villagerDeathListener;

	GolemGameUISetup::GameUserInterface gameUI;



	bool LoadGameEmitterListeners()
	{
		/*	ParticleEmitterListener golemPunchParticles(engine->bigCloud);
			golemPunchParticles.lifespan = 20.0f;
			ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemPunch", &golemPunchParticles);

			ParticleEmitterListener bigGolemDustParticles(engine->bigCloud);
			bigGolemDustParticles.lifespan = 0.5f;
			ADEvents::ADEventSystem::Instance()->RegisterClient("BigGolemParticles", &bigGolemDustParticles);

			RecoveryEmitterEvent golemRecoveryParticles(engine->recoveryEmitter);
			golemRecoveryParticles.lifespan = 1.0f;
			ADEvents::ADEventSystem::Instance()->RegisterClient("GolemRecoveryParticles", &golemRecoveryParticles);

			SmallCloudEmitterEvent smallGolemDustParticles(engine->smallCloud);
			smallGolemDustParticles.lifespan = 1.0f;
			ADEvents::ADEventSystem::Instance()->RegisterClient("SmallGolemParticles", &smallGolemDustParticles);

			WaterWaveEmitterEvent waveParticles(engine->waterWave);
			waveParticles.lifespan = 1.0f;
			ADEvents::ADEventSystem::Instance()->RegisterClient("GolemWaveParticles", &waveParticles);

			IronSkinEmitterEvent ironSkinParticles(engine->ironSkin);
			ironSkinParticles.lifespan = 1.0f;
			ADEvents::ADEventSystem::Instance()->RegisterClient("GolemIronSkinParticles", &ironSkinParticles);

			FireballEmitterEvent fireballParticles(engine->fireball);
			fireballParticles.lifespan = 1.0f;
			ADEvents::ADEventSystem::Instance()->RegisterClient("GolemFireballParticles", &fireballParticles);*/

		return true;
	};

	bool LoadGameAnimations()
	{

		return true;
	};

	bool LoadCoreListeners()
	{

		return true;
	};

	//Must Be BEFORE any models or units.
	bool LoadGameData()
	{
		DefinitionReader df;
		df.ReadMasterFile();

		// This is adding the hitboxes for all the spells to the game.  If the hitbox is not a single instance hitbox this will cause issues.  So change this.
		for (auto it = DefinitionDatabase::Instance()->hitboxDatabase.begin(), itEnd = DefinitionDatabase::Instance()->hitboxDatabase.end(); it != itEnd; ++it)
		{
			GameUtilities::AddGameObject(it->second, it->second->has_mesh);
		}
		return true;
	};

	bool LoadGameProfiles()
	{
		return true;

	};

	bool LoadGame()
	{

		return true;

	};

public:

	bool LoadGameAudio(AD_AUDIO::ADAudio* audioEngine)
	{
		//Load the Main Banks
		audioEngine->LoadBank("files//audio//Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
		audioEngine->LoadBank("files//audio//Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);

		//Load the Music and create the Events to Call the Music.
		titleMusic.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::MUSIC;
		titleMusic.engine = audioEngine;
		titleMusic.personalVolume = 0.02f;
		titleMusic.LoadSound("files\\audio\\Opening.mp3", false, false, true, true);
		levelMusic.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::MUSIC;
		levelMusic.engine = audioEngine;
		levelMusic.personalVolume = 1.f;
		levelMusic.LoadSound("files\\audio\\Amysticaljourney3.wav", false, false, true, true);
		endMusic.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::MUSIC;
		endMusic.engine = audioEngine;
		endMusic.personalVolume = 0.8f;
		endMusic.LoadSound("files\\audio\\Ending.mp3", false, false, true, true);

		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayTitle", &stopLevelMusicEvent);
		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayTitle", &stopEndMusicEvent);
		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayTitle", &playTitleEvent);

		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayLevel", &stopTitleMusicEvent);
		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayLevel", &stopEndMusicEvent);
		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayLevel", &playLevelEvent);


		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayEnd", &stopTitleMusicEvent);
		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayEnd", &stopLevelMusicEvent);
		ADEvents::ADEventSystem::Instance()->RegisterClient("PlayEnd", &playEndEvent);



		//Load The Golems Sound Effects

		golemSteps.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemSteps.engine = audioEngine;
		golemSteps.personalVolume = 0.5f;
		golemSteps.restartOnRepeat = true;
		golemSteps.LoadSound("event:/Sfx_GolemStep", true, false, true, true);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemSteps", &golemStepEvent);


		golemPunchSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemPunchSound.engine = audioEngine;
		golemPunchSound.personalVolume = 0.5f;
		golemPunchSound.restartOnRepeat = false;
		golemPunchSound.LoadSound("event:/Sfx_MinorGrunt", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemPunch", &golemPunchEvent);

		golemKickSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemKickSound.engine = audioEngine;
		golemKickSound.personalVolume = 0.5f;
		golemKickSound.restartOnRepeat = false;
		golemKickSound.LoadSound("event:/Sfx_EarthHit2", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemKick", &golemKickEvent);

		golemSlamSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemSlamSound.engine = audioEngine;
		golemSlamSound.personalVolume = 0.5f;
		golemSlamSound.restartOnRepeat = false;
		golemSlamSound.LoadSound("event:/Sfx_EarthHit", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemSlam", &golemSlamEvent);

		golemWaveSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemWaveSound.engine = audioEngine;
		golemWaveSound.personalVolume = 0.5f;
		golemWaveSound.restartOnRepeat = false;
		golemWaveSound.LoadSound("event:/Sfx_WaterWaveSpell", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemWaterWave", &golemWaveEvent);

		golemFireball.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemFireball.engine = audioEngine;
		golemFireball.personalVolume = 0.5f;
		golemFireball.restartOnRepeat = false;
		golemFireball.LoadSound("event:/Sfx_FireBallSpell", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemFireball", &golemFireballEvent);

		golemIronHide.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemIronHide.engine = audioEngine;
		golemIronHide.personalVolume = 0.5f;
		golemIronHide.restartOnRepeat = false;
		golemIronHide.LoadSound("event:/Sfx_IronSkinSpell", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemTaunt", &golemTauntEvent);

		golemRootingSpell.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemRootingSpell.engine = audioEngine;
		golemRootingSpell.personalVolume = 0.5f;
		golemRootingSpell.restartOnRepeat = false;
		golemRootingSpell.LoadSound("event:/Sfx_RootCrushSpell", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemRooting", &golemRootEvent);

		golemEatMinion.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::SOUND_FX;
		golemEatMinion.engine = audioEngine;
		golemEatMinion.personalVolume = 0.5f;
		golemEatMinion.restartOnRepeat = false;
		golemEatMinion.LoadSound("event:/Sfx_MinnionScream", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Sfx_GolemEat", &golemEatMinionEvent);


		menuConfirmSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		menuConfirmSound.engine = audioEngine;
		menuConfirmSound.personalVolume = 0.5f;
		menuConfirmSound.restartOnRepeat = true;
		menuConfirmSound.LoadSound("event:/UI_SFX_GridConfirm", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("UI_Sfx_Confirm", &menuConfirmEvent);

		menuReturnSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		menuReturnSound.engine = audioEngine;
		menuReturnSound.personalVolume = 0.5f;
		menuReturnSound.restartOnRepeat = true;
		menuReturnSound.LoadSound("event:/UI_SFX_MenuReturn", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("UI_Sfx_Return", &menuReturnEvent);

		menuCursorMovementSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		menuCursorMovementSound.engine = audioEngine;
		menuCursorMovementSound.personalVolume = 0.5f;
		menuCursorMovementSound.restartOnRepeat = true;
		menuCursorMovementSound.LoadSound("event:/UI_SFX_GridMovement", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("UI_Sfx_MoveCursor", &menuCursorMovementEvent);

		golemSliderMovementSound.audioSourceType = AD_AUDIO::AUDIO_SOURCE_TYPE::UI_SOUND_FX;
		golemSliderMovementSound.engine = audioEngine;
		golemSliderMovementSound.personalVolume = 0.5f;
		golemSliderMovementSound.restartOnRepeat = true;
		golemSliderMovementSound.LoadSound("event:/UI_SFX_Sliderclick", true, true, false, false);
		ADEvents::ADEventSystem::Instance()->RegisterClient("UI_Sfx_SliderClick", &golemSliderMovementEvent);



		ADEvents::ADEventSystem::Instance()->PrintData();

		return true;
	};

	//Must be AFTER any models or units.
	bool LoadListeners(Golem* _golem, ADGameplay::Scene* _scene)
	{
		golemEatingEvent.SetTarget(_golem, DefinitionDatabase::Instance()->effectsDatabase["GolemEat"]);
		ADEvents::ADEventSystem::Instance()->RegisterClient("Apply_GolemEat", &golemEatingEvent);


		allMinionDeathListener.SetTarget(_scene);
		ADEvents::ADEventSystem::Instance()->RegisterClient("MinionDeath", &allMinionDeathListener);

		villagerDeathListener.SetTarget(_scene);
		ADEvents::ADEventSystem::Instance()->RegisterClient("VillagerDeath", &villagerDeathListener);
		return true;
	};

	bool LoadGameUserInterface(ADUI::ADUI* _userInterface, AD_AUDIO::ADAudio* _audioEngine)
	{
		gameUI.SetupUI(_userInterface, _audioEngine);

		return true;
	};

	struct GameData
	{

	};

	struct ProfileData
	{

	};

	bool Initialize()
	{
		LoadGameEmitterListeners();
		LoadCoreListeners();
		LoadGameData();
		LoadGameProfiles();
		LoadGame();
		return true;
	}

};

