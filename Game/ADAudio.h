#pragma once
//
//#include "fmod_studio.hpp"
//#include "fmod.hpp"
//#include <string>
//#include <map>
//#include <vector>
//#include <math.h>
//#include <iostream>
//#include <Types.h>
//#include "ADUserInterface.h"
//
//namespace AD_AUDIO
//{
//
//    enum AUDIO_SOURCE_TYPE
//    {
//        MUSIC = 0,
//        SOUND_FX,
//        UI_SOUND_FX
//    };
//
//    class ADAudio {
//    public:
//        float masterMusicVolume = 0.5f;
//        float masterSoundFXVolume = 0.5f;
//        float masterUISoundFXVolume = 0.5f;
//
//        static void Init();
//        static void Update();
//        static void Shutdown();
//        static int AudioErrorCheck(FMOD_RESULT result);
//
//        //Load the Banks
//        void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
//
//        //Sounds & Channels
//        void LoadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
//        void UnLoadSound(const std::string& strSoundName);
//        int PlaySounds(const std::string& strSoundName, const XMFLOAT3& vPos = XMFLOAT3{ 0, 0, 0 }, float fVolumedB = 0.0f);
//        void StopChannel(int nChannelId);
//        void StopAllChannels();
//        void RefreshMusicVolumes();
//        void SetChannelVolume(int nChannelId, float fVolumedB);
//        bool IsPlaying(int nChannelId) const;
//
//        //Events
//	    void LoadEvent(const std::string& strEventName);
//        void PlayEvent(const string& strEventName);
//        void StopEvent(const string& strEventName, bool bImmediate = false);
//         bool IsEventPlaying(const string& strEventName) const;
//
//        //Spatial Positioning Functions
//        void Set3dListenerAndOrientation(const FMOD_VECTOR& vPos = FMOD_VECTOR{ 0, 0, 0 }, float fVolumedB = 0.0f);
//        void SetChannel3dPosition(int nChannelId, const XMFLOAT3& vPosition);
//        FMOD_VECTOR VectorToFmod(const XMFLOAT3& vPosition);
//        
//    };
//
//    //A class can contain an audio source and use it to play sounds.  Couples classes but works fine if that's easily managed.
//    class AudioSource
//    {
//    public:
//        ADAudio* engine;
//        std::string soundName;
//        bool isEvent;
//        XMFLOAT3 vPos;
//        float personalVolume;
//        UINT audioSourceType;
//        bool restartOnRepeat = false;
//        int currentChannel;
//        void LoadSound(std::string _soundName, bool _isEvent = true, bool is3D = true, bool isLooping = false, bool isStream = false);
//        void Play();
//        void Stop();
//        void RefreshVolume();
//        void UpdatePosition(XMFLOAT3 pos);
//    };
//
//    struct AudioImplementation {
//        AudioImplementation();
//        ~AudioImplementation();
//
//        void Update();
//
//        FMOD::Studio::System* studioSystem;
//        FMOD::System* audioSystem;
//
//        int nextChannelId = -1;
//
//        std::map<std::string, FMOD::Sound*> soundsName_map;
//        std::map<int, FMOD::Channel*> channelsID_map;
//        std::vector<AudioSource*> musicChannels;
//        std::map<string, FMOD::Studio::EventInstance*> eventsName_map;
//        std::map<string, FMOD::Studio::Bank*> bankName_map;
//    };
//}
