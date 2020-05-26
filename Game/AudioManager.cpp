#include "pchgame.h"
#include "AudioManager.h"
//#include "Utils.h"
//
//bool AudioManager::Initialize(std::string thing, std::vector<std::string> noises /*= std::vector<std::string>()*/)
//{
//	// Initialize the music loops
//	music_loop = ref new MediaPlayer();
//
//	// Set up media players
//	character_player = ref new MediaPlayer();
//	sfx_player.resize(SFX_TRACK_COUNT);
//	for (int i = 0; i < SFX_TRACK_COUNT; i++)
//	{
//		sfx_player[i] = ref new MediaPlayer();
//	}
//
//	Platform::String^ appInstallFolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
//	std::string READ_PATH = std::string(appInstallFolder->Begin(), appInstallFolder->End()).append("\\");
//
//	thing = std::string(READ_PATH).append(thing);
//	std::wstring w;
//	const wchar_t* wchar;
//	String^ str = nullptr;
//	Uri^ uri = nullptr;
//
//	w = std::wstring(thing.begin(), thing.end());
//	wchar = w.c_str();
//	str = ref new String(wchar);
//	uri = ref new Uri(str);
//
//	bg_loop = MediaSource::CreateFromUri(uri);
//
//	// SFX
//	// Resize the media sources to hold the audio
//	effects.resize(noises.size());
//	for (int i = 0; i < noises.size(); i++)
//	{
//		thing = std::string(READ_PATH).append(noises[i]);
//		w = std::wstring(thing.begin(), thing.end());
//		wchar = w.c_str();
//		str = ref new String(wchar);
//		uri = ref new Uri(str);
//
//		effects[i] = MediaSource::CreateFromUri(uri);
//	}
//
//	// Events
//	character_player->MediaEnded += ref new TypedEventHandler<MediaPlayer^, Object^>(&SFXEffectEnded);
//	for (int i = 0; i < sfx_player.size(); i++)
//	{
//		sfx_player[i]->MediaEnded += ref new TypedEventHandler<MediaPlayer^, Object^>(&SFXEffectEnded);
//	}
//
//	return true;
//}
//
//bool AudioManager::Shutdown()
//{
//	return true;
//}
//
//void AudioManager::PlayBackgroundMusic()
//{
//	music_loop->Source = bg_loop;
//	music_loop->Volume = main_loop_volume;
//	music_loop->Play();
//}
//
//int AudioManager::PlayEffect(UINT effect_id, float volume)
//{
//	// Validate source
//	assert(effect_id < effects.size());
//
//	int track = current_sfx_track;
//
//	sfx_player[current_sfx_track]->Source = effects[effect_id];
//	sfx_player[current_sfx_track]->Volume = volume;
//	sfx_player[current_sfx_track]->PlaybackRate = 3;
//	sfx_player[current_sfx_track]->Play();
//
//	// Increment track
//	current_sfx_track++;
//	if (current_sfx_track >= SFX_TRACK_COUNT)
//		current_sfx_track = 0;
//
//	return track;
//}
//
//void AudioManager::PauseEffect(UINT effect_id, int track)
//{
//	if (sfx_player[track]->Source == effects[effect_id])
//	{
//		sfx_player[track]->Pause();
//	}
//}
//
//void AudioManager::ResumeEffect(UINT effect_id, int track)
//{
//	if (sfx_player[track]->Source == effects[effect_id])
//	{
//		sfx_player[track]->Play();
//	}
//}
//
//void AudioManager::StopEffect(UINT effect_id, int track)
//{
//	if (sfx_player[track]->Source == effects[effect_id])
//	{
//		sfx_player[track]->Pause();
//		sfx_player[track]->Source = nullptr;
//	}
//}
//
//void AudioManager::PauseAllMusic()
//{
//	music_loop->Pause();
//	character_player->Pause();
//
//	for (int i = 0; i < SFX_TRACK_COUNT; i++)
//	{
//		if (sfx_player[i]->Source != nullptr)
//		{
//			sfx_player[i]->Pause();
//		}
//	}
//}
//
//void AudioManager::ResumeAllMusic()
//{
//	music_loop->Play();
//	if(character_player->Source != nullptr) character_player->Play();
//
//	for (int i = 0; i < SFX_TRACK_COUNT; i++)
//	{
//		if (sfx_player[i]->Source != nullptr)
//		{
//			sfx_player[i]->Play();
//		}
//	}
//}