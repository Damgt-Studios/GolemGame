#include "pchgame.h"
#include "ADAudio.h"

namespace AD_AUDIO
{

    AudioImplementation::AudioImplementation() {
        studioSystem = NULL;
        ADAudio::AudioErrorCheck(FMOD::Studio::System::create(&studioSystem));
        //https://www.fmod.com/resources/documentation-api?version=1.10&page=content/generated/FMOD_STUDIO_INITFLAGS.html
        ADAudio::AudioErrorCheck(studioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

        audioSystem = NULL;
        ADAudio::AudioErrorCheck(studioSystem->getCoreSystem(&audioSystem));

    }

    AudioImplementation::~AudioImplementation() {
        ADAudio::AudioErrorCheck(studioSystem->unloadAll());
        ADAudio::AudioErrorCheck(studioSystem->release());
    }

    void AudioImplementation::Update() {
        std::vector<std::map<int, FMOD::Channel*>::iterator> pStoppedChannels;
        for (auto it = channelsID_map.begin(), itEnd = channelsID_map.end(); it != itEnd; ++it)
        {
            bool bIsPlaying = false;
            it->second->isPlaying(&bIsPlaying);
            if (!bIsPlaying)
            {
                pStoppedChannels.push_back(it);
            }
        }
        for (auto& it : pStoppedChannels)
        {
            channelsID_map.erase(it);
        }
        ADAudio::AudioErrorCheck(studioSystem->update());
    }

    AudioImplementation* audioImp = nullptr;

    void ADAudio::Init() {
        audioImp = new AudioImplementation;
    }

    void ADAudio::Update() {
        audioImp->Update();
    }

    void ADAudio::LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
    {
        auto tFoundIt = audioImp->soundsName_map.find(strSoundName);
        if (tFoundIt != audioImp->soundsName_map.end())
            return;

        FMOD_MODE eMode = FMOD_DEFAULT;
        eMode |= b3d ? FMOD_3D : FMOD_2D;
        eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
        eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

        FMOD::Sound* pSound = nullptr;
        ADAudio::AudioErrorCheck(audioImp->audioSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
        if (pSound) {
            audioImp->soundsName_map[strSoundName] = pSound;
        }

    }

    void ADAudio::UnLoadSound(const std::string& strSoundName)
    {
        auto tFoundIt = audioImp->soundsName_map.find(strSoundName);
        if (tFoundIt == audioImp->soundsName_map.end())
            return;

        ADAudio::AudioErrorCheck(tFoundIt->second->release());
        audioImp->soundsName_map.erase(tFoundIt);
    }

    void ADAudio::Set3dListenerAndOrientation(const FMOD_VECTOR& vPos, float fVolumedB)
    {
        //FMOD_3D_ATTRIBUTES attis;
        //attis.position = vPos;
        //attis.forward = vPos;
        //attis.up = vPos;
        //attis.velocity = vPos;

        //ADAudio::AudioErrorCheck(audioImp->studioSystem->setListenerAttributes(0, &attis, &vPos));
    }

    int ADAudio::PlaySounds(const std::string& strSoundName, const XMFLOAT3& vPosition, float fVolumedB)
    {
        int nChannelId = audioImp->nextChannelId++;
        auto tFoundIt = audioImp->soundsName_map.find(strSoundName);
        if (tFoundIt == audioImp->soundsName_map.end())
        {
            LoadSound(strSoundName);
            tFoundIt = audioImp->soundsName_map.find(strSoundName);
            if (tFoundIt == audioImp->soundsName_map.end())
            {
                return nChannelId;
            }
        }
        FMOD::Channel* pChannel = nullptr;
        ADAudio::AudioErrorCheck(audioImp->audioSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
        if (pChannel)
        {
            FMOD_MODE currMode;
            tFoundIt->second->getMode(&currMode);
            if (currMode & FMOD_3D) {
                FMOD_VECTOR position = VectorToFmod(vPosition);
                ADAudio::AudioErrorCheck(pChannel->set3DAttributes(&position, nullptr));
            }
            //ADAudio::AudioErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
            ADAudio::AudioErrorCheck(pChannel->setPaused(false));
            audioImp->channelsID_map[nChannelId] = pChannel;
            ADAudio::AudioErrorCheck(audioImp->channelsID_map[nChannelId]->setVolume(fVolumedB));
        }
        return nChannelId;
    }

    void ADAudio::StopChannel(int nChannelId)
    {
        auto tFoundIt = audioImp->channelsID_map.find(nChannelId);
        if (tFoundIt == audioImp->channelsID_map.end())
            return;

        audioImp->channelsID_map.erase(tFoundIt);
    }

    void ADAudio::StopAllChannels()
    {
        audioImp->channelsID_map.clear();
    }

    void ADAudio::SetChannel3dPosition(int nChannelId, const XMFLOAT3& vPosition)
    {
        auto tFoundIt = audioImp->channelsID_map.find(nChannelId);
        if (tFoundIt == audioImp->channelsID_map.end())
            return;

        FMOD_VECTOR position = VectorToFmod(vPosition);
        ADAudio::AudioErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
    }

    void ADAudio::SetChannelVolume(int nChannelId, float fVolumedB)
    {
        auto tFoundIt = audioImp->channelsID_map.find(nChannelId);
        if (tFoundIt == audioImp->channelsID_map.end())
            return;

        ADAudio::AudioErrorCheck(tFoundIt->second->setVolume(fVolumedB));
    }

    bool ADAudio::IsPlaying(int nChannelId) const
    {

        bool isPlaying = false;
        auto tFoundIt = audioImp->channelsID_map.find(nChannelId);
        if (tFoundIt != audioImp->channelsID_map.end())
        {
            ADAudio::AudioErrorCheck(audioImp->channelsID_map[nChannelId]->isPlaying(&isPlaying));
        }
        return isPlaying;
    }

    void ADAudio::LoadEvent(const std::string& strEventName)
    {
        auto tFoundit = audioImp->eventsName_map.find(strEventName);
        if (tFoundit != audioImp->eventsName_map.end())
            return;
        FMOD::Studio::EventDescription* pEventDescription = NULL;
        ADAudio::AudioErrorCheck(audioImp->studioSystem->getEvent(strEventName.c_str(), &pEventDescription));
        if (pEventDescription) {
            FMOD::Studio::EventInstance* pEventInstance = NULL;
            ADAudio::AudioErrorCheck(pEventDescription->createInstance(&pEventInstance));
            if (pEventInstance) {
                audioImp->eventsName_map[strEventName] = pEventInstance;
            }
        }
    }

    void ADAudio::PlayEvent(const string& strEventName) {
        auto tFoundit = audioImp->eventsName_map.find(strEventName);
        if (tFoundit == audioImp->eventsName_map.end()) {
            LoadEvent(strEventName);
            tFoundit = audioImp->eventsName_map.find(strEventName);
            if (tFoundit == audioImp->eventsName_map.end())
                return;
        }
        tFoundit->second->start();
    }

    void ADAudio::StopEvent(const string& strEventName, bool bImmediate) {
        auto tFoundIt = audioImp->eventsName_map.find(strEventName);
        if (tFoundIt == audioImp->eventsName_map.end())
            return;
        FMOD_STUDIO_STOP_MODE eMode;
        eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
        ADAudio::AudioErrorCheck(tFoundIt->second->stop(eMode));
    }

    void ADAudio::GetEventParameter(const string& strEventName, const string& strParameterName, float* parameter) {
        //auto tFoundIt = audioImp->eventsName_map.find(strEventName);
        //if (tFoundIt == audioImp->eventsName_map.end())
        //    return;
        //FMOD::Studio::ParameterInstance* pParameter = NULL;
        //ADAudio::AudioErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
        //ADAudio::AudioErrorCheck(pParameter->getValue(parameter));
    }


    void ADAudio::SetEventParameter(const string& strEventName, const string& strParameterName, float fValue) {
        //auto tFoundIt = audioImp->eventsName_map.find(strEventName);
        //if (tFoundIt == audioImp->eventsName_map.end())
        //    return;
        //FMOD::Studio::ParameterInstance* pParameter = NULL;
        //ADAudio::AudioErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
        //ADAudio::AudioErrorCheck(pParameter->setValue(fValue));
    }

    bool ADAudio::IsEventPlaying(const string& strEventName) const {
        auto tFoundIt = audioImp->eventsName_map.find(strEventName);
        if (tFoundIt == audioImp->eventsName_map.end())
            return false;

        FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
        if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
            return true;
        }
        return false;
    }

    FMOD_VECTOR ADAudio::VectorToFmod(const XMFLOAT3& vPosition) {
        FMOD_VECTOR fVec;
        fVec.x = vPosition.x;
        fVec.y = vPosition.y;
        fVec.z = vPosition.z;
        return fVec;
    }

    float  ADAudio::dbToVolume(float dB)
    {
        return powf(10.0f, 0.05f * dB);
    }

    float  ADAudio::VolumeTodb(float volume)
    {
        return 20.0f * log10f(volume);
    }

    void ADAudio::RefreshMusicVolumes()
    {
        for (int i = 0; i < audioImp->musicChannels.size(); ++i)
        {
            audioImp->musicChannels[i]->RefreshVolume();
        }
    }

    int ADAudio::AudioErrorCheck(FMOD_RESULT result) {
        if (result != FMOD_OK) {
            std::cout << "FMOD ERROR " << result << std::endl;
            std::string error;
            error.append("FMOD ERROR ");
            error.append(to_string(result));
            ADUI::MessageReceiver::Log(error.c_str());
            return 1;
        }
        return 0;
    }

    void ADAudio::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
        auto tFoundIt = audioImp->bankName_map.find(strBankName);
        if (tFoundIt != audioImp->bankName_map.end())
            return;
        FMOD::Studio::Bank* pBank;
        ADAudio::AudioErrorCheck(audioImp->studioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
        if (pBank) {
            audioImp->bankName_map[strBankName] = pBank;
        }
    }

    void ADAudio::Shutdown() {
        delete audioImp;
    }

    void AudioSource::LoadSound(bool is3D, bool isLooping, bool isStream)
    {
        engine->LoadSound(soundName, is3D, isLooping, isStream);

        if (isStream)
        {
            audioImp->musicChannels.push_back(this);
        }
    }

    void AudioSource::Play()
    {
        if (restartOnRepeat)
        {
            engine->StopChannel(currentChannel);
        }
        if (!engine->IsPlaying(currentChannel))
        {
            float volume = personalVolume;
            switch (audioSourceType)
            {
            case 0:
                volume *= engine->masterMusicVolume;
                break;
            case 1:
                volume *= engine->masterSoundFXVolume;
                break;
            case 2:
                volume *= engine->masterUISoundFXVolume;
                break;
            default:
                break;
            }
            currentChannel = engine->PlaySounds(soundName, vPos, volume);
        }
    }

    void AudioSource::RefreshVolume()
    {
        if (engine->IsPlaying(currentChannel) && audioSourceType == MUSIC)
        {
            engine->SetChannelVolume(currentChannel, personalVolume * engine->masterMusicVolume);
        }
    }

    void AudioSource::UpdatePosition(XMFLOAT3 pos)
    {
        vPos = pos;
    }
}