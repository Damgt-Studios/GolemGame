#pragma once

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include <Types.h>
#include <ADEventSystem.h>

namespace AD_ADUIO
{

    enum AUDIO_SOURCE_TYPE
    {
        MUSIC = 0,
        SOUND_FX,
        UI_SOUND_FX
    };

    class ADAudio {
    public:
        float masterMusicVolume = 0.5f;
        float masterSoundFXVolume = 0.5f;
        float masterUISoundFXVolume = 0.5f;

        static void Init();
        static void Update();
        static void Shutdown();
        static int AudioErrorCheck(FMOD_RESULT result);

        void LoadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
        void UnLoadSound(const std::string& strSoundName);
        void Set3dListenerAndOrientation(const FMOD_VECTOR& vPos = FMOD_VECTOR{ 0, 0, 0 }, float fVolumedB = 0.0f);
        int PlaySounds(const std::string& strSoundName, const XMFLOAT3& vPos = XMFLOAT3{ 0, 0, 0 }, float fVolumedB = 0.0f);
        void SetChannel3dPosition(int nChannelId, const XMFLOAT3& vPosition);
        void SetChannelVolume(int nChannelId, float fVolumedB);
        bool IsPlaying(int nChannelId) const;
        float dbToVolume(float db);
        float VolumeTodb(float volume);
        void RefreshMusicVolumes();
        FMOD_VECTOR VectorToFmod(const XMFLOAT3& vPosition);
    };

    //A class can contain an audio source and use it to play sounds.  Couples classes but works fine if that's easily managed.
    class AudioSource
    {
    public:
        ADAudio* engine;
        std::string soundName;
        XMFLOAT3 vPos;
        float personalVolume;
        UINT audioSourceType;
        bool restartOnRepeat;
        int currentChannel;
        void LoadSound(bool is3D = true, bool isLooping = false, bool isStream = false);
        void Play();
        void RefreshVolume();
        void UpdatePosition(XMFLOAT3 pos);
    };

    //Decouples AudioSource form owner if you want by using an event.
    class AudioSourceEvent : public ADEvents::Listener
    {
    private:
        AudioSource& audioSource;
    public:
        AudioSourceEvent(AudioSource& _audioSource) : audioSource(_audioSource) {};
        void HandleEvent(ADEvents::ADEvent* _event) override;
    };

    struct AudioImplementation {
        AudioImplementation();
        ~AudioImplementation();

        void Update();

        FMOD::Studio::System* studioSystem;
        FMOD::System* audioSystem;

        int nextChannelId = -1;

        std::map<std::string, FMOD::Sound*> soundsByName;
        std::vector<AudioSource*> musicChannels;
        std::map<int, FMOD::Channel*> channelsByName;
    };
}
