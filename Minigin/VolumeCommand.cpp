#include "VolumeCommand.h"
#include "Sound/SoundManager.h"
#include "Sound/SDLSoundSystem.h"

#include <iostream>

void dae::DecreaseVolumeCommand::Execute()
{
	adjust_volume(-0.1f);
}

void dae::IncreaseVolumeCommand::Execute()
{
	adjust_volume(0.1f);
}

void dae::adjust_volume(float delta)
{
	float volume = SoundManager::GetInstance().GetSoundSystem()->GetMasterVolume();

	if (volume > 0.0f && delta < 0.0f)
	{
		volume = std::max(0.0f, volume + delta);
	}
	else if (volume < 1.0f && delta > 0.0f)
	{
		volume = std::min(1.0f, volume + delta);
	}

	SoundManager::GetInstance().GetSoundSystem()->SetMasterVolume(volume);
}

void dae::MuteVolumeCommand::Execute()
{
	float volume = SoundManager::GetInstance().GetSoundSystem()->GetMasterVolume();

	if (volume > 0.0f)
	{
		SoundManager::GetInstance().GetSoundSystem()->IsSoundMuterd(true);
	}
	else
	{
		SoundManager::GetInstance().GetSoundSystem()->IsSoundMuterd(false);
	}
}
