#pragma once

#include <vector>
#include <string>

using namespace Windows::Foundation::Collections;
using namespace Windows::Media::Playback;
using namespace Windows::Media::Core; 
using namespace Windows::Foundation;
using namespace Platform;

#define SFX_TRACK_COUNT 3

namespace
{
	void SFXEffectEnded(Windows::Media::Playback::MediaPlayer^ player, Platform::Object^ sender)
	{
		player->Source = nullptr;
	}
}

class AudioManager
{
public:
	AudioManager() = default;
	~AudioManager() = default;

	bool Initialize(std::string, std::vector<std::string> noises = std::vector<std::string>());
	bool Shutdown();

	void PlayBackgroundMusic();

	int PlayEffect(int effect_id, float volume = 1.0);
	void PauseEffect(int effect_id, int track);
	void ResumeEffect(int effect_id, int track);
	void StopEffect(int effect_id, int track);

	void PauseAllMusic();
	void ResumeAllMusic();

private:
	// Media players
	MediaPlayer^ music_loop;
	MediaPlayer^ character_player;
	std::vector<MediaPlayer^> sfx_player;

	MediaSource^ bg_loop;
	std::vector<MediaSource^> effects;

	// Volume
	float main_loop_volume = .2;
	int current_sfx_track = 0;
};

// NOTE: Needs significant refactoring, the point is there though
// Doesn't build on engine, missing some kind of app related dependency
// This is fine being on the client side if necessary - not ideal, but not worth the effort to fix dependencies