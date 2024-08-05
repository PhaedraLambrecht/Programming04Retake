#include "VolumeCommand.h"
#include "Sound/SoundManager.h"
#include "Sound/SDLSoundSystem.h"

#include <iostream>

void dae::DecreaseVolumeCommand::Execute()
{
	// get the current volume
	float volume = SoundManager::GetInstance().GetSoundSystem()->GetMasterVolume();

	// do - 0.1f to the volume
	if (volume > 0.0f)
	{
		volume -= 0.1f;
	}
	else
	{
		volume = 0.0f;
	}


	std::cout << "Volume: " << volume << std::endl;
	// set the new volume
	SoundManager::GetInstance().GetSoundSystem()->SetMasterVolume(volume);
}

void dae::IncreaseVolumeCommand::Execute()
{
	// get the current volume
	float volume = SoundManager::GetInstance().GetSoundSystem()->GetMasterVolume();

	// do + 0.1f to the volume
	if (volume < 1.0f)
	{
		volume += 0.1f;
	}
	else
	{
		volume = 1.0f;
	}

	std::cout << "Volume: " << volume << std::endl;

	// set the new volume
	SoundManager::GetInstance().GetSoundSystem()->SetMasterVolume(volume);

}
