#pragma once

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include <Types.h>

namespace AD_ADUIO
{
    enum AUDIO_SOURCE_TYPE
    {
        MUSIC = 0,
        SOUND_FX,
        UI_SOUND_FX
    };

    struct AudioImplementation {
        AudioImplementation();
        ~AudioImplementation();

        void Update();

        FMOD::Studio::System* studioSystem;
        FMOD::System* audioSystem;

        int nextChannelId;

        std::map<std::string, FMOD::Sound*> soundsByName;
        std::map<int, FMOD::Channel*> channelsByName;
    };

    class ADAudio {
    public:
        float masterMusicVolume = 1;
        float masterSoundFXVolume = 1;
        float masterUISoundFXVolume = 1;

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
        FMOD_VECTOR VectorToFmod(const XMFLOAT3& vPosition);
    };

    class AudioSource
    {
    public:
        ADAudio* engine;
        std::string soundName;
        XMFLOAT3 vPos;
        float personalVolume = 1;
        UINT audioSourceType;
        int currentChannel;
        void LoadSound(bool is3D = true, bool isLooping = false, bool isStream = false);
        void Play();
        void UpdatePosition(XMFLOAT3 pos);
    };
}
