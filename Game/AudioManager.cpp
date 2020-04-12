#include "pchgame.h"
#include "AudioManager.h"
#include "Utils.h"

bool AudioManager::Initialize(std::string thing)
{
	// Initialize the music loops
	music_loop = ref new MediaPlayer();

	Platform::String^ appInstallFolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
	std::string READ_PATH = std::string(appInstallFolder->Begin(), appInstallFolder->End()).append("\\");

	thing = READ_PATH.append(thing);
	std::wstring w;
	const wchar_t* wchar;
	String^ str = nullptr;
	Uri^ uri = nullptr;

	w = std::wstring(thing.begin(), thing.end());
	wchar = w.c_str();
	str = ref new String(wchar);
	uri = ref new Uri(str);
	uri = ref new Uri("ms-appx:///files//audio/main_theme.wav");
	
	bg_loop = MediaSource::CreateFromUri(uri);

	return true;
}

bool AudioManager::Shutdown()
{
	return true;
}

void AudioManager::PlayBackgroundMusic()
{
	music_loop->Source = bg_loop;
	music_loop->Volume = main_loop_volume;
	music_loop->Play();
}
