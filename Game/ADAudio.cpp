#include "pchgame.h"
#include "ADAudio.h"

namespace AD_ADUIO
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
        for (auto it = channelsByName.begin(), itEnd = channelsByName.end(); it != itEnd; ++it)
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
            channelsByName.erase(it);
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
        auto tFoundIt = audioImp->soundsByName.find(strSoundName);
        if (tFoundIt != audioImp->soundsByName.end())
            return;

        FMOD_MODE eMode = FMOD_DEFAULT;
        eMode |= b3d ? FMOD_3D : FMOD_2D;
        eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
        eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

        FMOD::Sound* pSound = nullptr;
        ADAudio::AudioErrorCheck(audioImp->audioSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
        if (pSound) {
            audioImp->soundsByName[strSoundName] = pSound;
        }

    }

    void ADAudio::UnLoadSound(const std::string& strSoundName)
    {
        auto tFoundIt = audioImp->soundsByName.find(strSoundName);
        if (tFoundIt == audioImp->soundsByName.end())
            return;

        ADAudio::AudioErrorCheck(tFoundIt->second->release());
        audioImp->soundsByName.erase(tFoundIt);
    }

    void ADAudio::Set3dListenerAndOrientation(const FMOD_VECTOR& vPos, float fVolumedB)
    {
        FMOD_3D_ATTRIBUTES attis;
        attis.position = vPos;
        attis.forward = vPos;
        attis.up = vPos;
        attis.velocity = vPos;

        ADAudio::AudioErrorCheck(audioImp->studioSystem->setListenerAttributes(0, &attis, &vPos));
    }

    int ADAudio::PlaySounds(const std::string& strSoundName, const XMFLOAT3& vPosition, float fVolumedB)
    {
        int nChannelId = audioImp->nextChannelId++;
        auto tFoundIt = audioImp->soundsByName.find(strSoundName);
        if (tFoundIt == audioImp->soundsByName.end())
        {
            LoadSound(strSoundName);
            tFoundIt = audioImp->soundsByName.find(strSoundName);
            if (tFoundIt == audioImp->soundsByName.end())
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
            ADAudio::AudioErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
            ADAudio::AudioErrorCheck(pChannel->setPaused(false));
            audioImp->channelsByName[nChannelId] = pChannel;
        }
        return nChannelId;
    }

    void ADAudio::SetChannel3dPosition(int nChannelId, const XMFLOAT3& vPosition)
    {
        auto tFoundIt = audioImp->channelsByName.find(nChannelId);
        if (tFoundIt == audioImp->channelsByName.end())
            return;

        FMOD_VECTOR position = VectorToFmod(vPosition);
        ADAudio::AudioErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
    }

    void ADAudio::SetChannelVolume(int nChannelId, float fVolumedB)
    {
        auto tFoundIt = audioImp->channelsByName.find(nChannelId);
        if (tFoundIt == audioImp->channelsByName.end())
            return;

        ADAudio::AudioErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
    }

    bool ADAudio::IsPlaying(int nChannelId) const
    {
        bool isPlaying = false;
        ADAudio::AudioErrorCheck(audioImp->channelsByName[nChannelId]->isPlaying(&isPlaying));
        return isPlaying;
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

    int ADAudio::AudioErrorCheck(FMOD_RESULT result) {
        if (result != FMOD_OK) {
            std::cout << "FMOD ERROR " << result << std::endl;
            return 1;
        }
        return 0;
    }

    void ADAudio::Shutdown() {
        delete audioImp;
    }

    void AudioSource::LoadSound(bool is3D, bool isLooping, bool isStream)
    {
        engine->LoadSound(soundName, is3D, isLooping, isStream);
    }

    void AudioSource::Play()
    {
        if (!engine->IsPlaying(currentChannel))
        {
            float volume = personalVolume;
            switch (audioSourceType)
            {
            case 1:
                volume *= engine->masterMusicVolume;
                break;
            case 2:
                volume *= engine->masterSoundFXVolume;
                break;
            case 3:
                volume *= engine->masterUISoundFXVolume;
                break;
            default:
                break;
            }
            currentChannel = engine->PlaySounds(soundName, vPos, volume);
        }
    }

    void AudioSource::UpdatePosition(XMFLOAT3 pos)
    {
        vPos = pos;
    }
}