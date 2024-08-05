#include "SDLSoundSystem.h"
#include <stdexcept>
#include <unordered_map>

#include "SDL.h"
#include "SDL_mixer.h"

#define MIX_CHANNEL_GROUP_MUSIC 0
#define MIX_CHANNEL_GROUP_EFFECTS 1


class dae::SDLSoundSystem::SDLMixerImpl final
{
public:

	SDLMixerImpl();
	~SDLMixerImpl();

	SDLMixerImpl(const SDLMixerImpl& other) = delete;
	SDLMixerImpl(SDLMixerImpl&& other) = delete;
	SDLMixerImpl& operator=(const SDLMixerImpl& other) = delete;
	SDLMixerImpl& operator=(SDLMixerImpl&& other) = delete;


	void LoadSound(dae::SoundData soundData);
	void PlaySound(unsigned short id, dae::SoundData::SoundType soundType, float volume);
	bool IsSoundLoaded(unsigned short id);
	bool IsMusicLoaded(unsigned short id);

	void Init();
	void Quit();

	void SetMasterVolume(float volume);
	float GetMasterVolume() const;


private:

	float m_MasterVolume{ 0.5f };// 0.0f - 1.0f
	std::unordered_map<unsigned short, Mix_Chunk*> m_LoadedSounds;
	std::unordered_map<unsigned short, Mix_Music*> m_Loadedmusic;
};

dae::SDLSoundSystem::SDLMixerImpl::SDLMixerImpl()
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error("Failed to initialize SDL audio: " + std::string(SDL_GetError()));
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		throw std::runtime_error("Failed to open audio: " + std::string(Mix_GetError()));
	}

	Mix_GroupChannels(0, 0, MIX_CHANNEL_GROUP_MUSIC);
	Mix_GroupChannels(1, 31, MIX_CHANNEL_GROUP_EFFECTS);
}

dae::SDLSoundSystem::SDLMixerImpl::~SDLMixerImpl()
{
	for (auto& sound : m_LoadedSounds)
	{
		Mix_FreeChunk(sound.second);
	}

	Mix_CloseAudio();
	SDL_Quit();
}


void dae::SDLSoundSystem::SDLMixerImpl::LoadSound(dae::SoundData soundData)
{
	// Sound is already loaded
	if (IsSoundLoaded(soundData.id))
		return;

	Mix_Music* music = Mix_LoadMUS(soundData.filePath.c_str());
	Mix_Chunk* chunk = Mix_LoadWAV(soundData.filePath.c_str());

	switch (soundData.soundType)
	{
	case SoundData::SoundType::Music:

		if (music == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load music: ") + Mix_GetError());
		}
		m_Loadedmusic[soundData.id] = music;
		PlaySound(soundData.id, soundData.soundType, soundData.volume);
		break;

	case SoundData::SoundType::SoundEffect:

		if (chunk == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load sound: ") + Mix_GetError());
		}
		m_LoadedSounds[soundData.id] = chunk;
		break;

	}
}

void dae::SDLSoundSystem::SDLMixerImpl::PlaySound(unsigned short id, SoundData::SoundType soundType, float /*volume*/)
{
	bool soundLoaded = IsSoundLoaded(id);
	bool musicLoaded = IsMusicLoaded(id);


	
	int mixVolume = static_cast<int>(m_MasterVolume * MIX_MAX_VOLUME);// Convert float volume to SDL_mixer scale


	Mix_Chunk* chunk{};
	int channel{ Mix_GroupAvailable(MIX_CHANNEL_GROUP_EFFECTS) };
	Mix_Music* music{};

	switch (soundType)
	{
	case SoundData::SoundType::SoundEffect:

		if (!soundLoaded)
		{
			throw std::runtime_error("Sound with ID " + std::to_string(id) + " is not loaded.");
		}

		chunk = m_LoadedSounds[id];
		Mix_VolumeChunk(chunk, mixVolume);

		// Play sound effect
		if (channel == -1)
		{
			channel = 1; // Use a default channel if none is available
		}

		Mix_PlayChannel(channel, chunk, 0); // 0 means play once, use -1 for looping
		break;

	case SoundData::SoundType::Music:

		if (!musicLoaded)
		{
			throw std::runtime_error("Music with ID " + std::to_string(id) + " is not loaded.");
		}

		music = m_Loadedmusic[id];
		Mix_VolumeMusic(mixVolume);

		// Play background music
		if (Mix_PlayingMusic() == 0) // Only play music if none is currently playing
		{
			Mix_PlayMusic(music, -1); // -1 means loop indefinitely
		}
		break;
	}
}

bool dae::SDLSoundSystem::SDLMixerImpl::IsSoundLoaded(unsigned short id)
{
	return m_LoadedSounds.find(id) != m_LoadedSounds.end();
}

bool dae::SDLSoundSystem::SDLMixerImpl::IsMusicLoaded(unsigned short id)
{
	return m_Loadedmusic.find(id) != m_Loadedmusic.end();
}

void dae::SDLSoundSystem::SDLMixerImpl::Init()
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error("Failed to initialize SDL audio: " + std::string(SDL_GetError()));
	}

	SDL_version compiled;
	SDL_version linked;

	SDL_MIXER_VERSION(&compiled);
	SDL_MIXER_VERSION(&linked);


	printf("We compiled against SDL_mixer version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);

	printf("We are linking against SDL_mixer version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);


	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 5, 0) < 0) 
	{
		throw std::runtime_error("Failed to open audio: " + std::string(Mix_GetError()));
	}

	Mix_GroupChannels(0, 0, MIX_CHANNEL_GROUP_MUSIC);       // Background music group (1 channel)
	Mix_GroupChannels(1, 4, MIX_CHANNEL_GROUP_EFFECTS);     // SoundEffects music group (4 channels)
}

void dae::SDLSoundSystem::SDLMixerImpl::Quit()
{
	for (auto& sound : m_LoadedSounds)
	{
		Mix_FreeChunk(sound.second);
	}

	m_LoadedSounds.clear();

	Mix_CloseAudio();
	SDL_Quit();
}

void dae::SDLSoundSystem::SDLMixerImpl::SetMasterVolume(float volume)
{
	m_MasterVolume = std::clamp(volume, 0.0f, 1.0f);

	Mix_Volume(-1, static_cast<int>(m_MasterVolume * MIX_MAX_VOLUME));
	Mix_VolumeMusic(static_cast<int>(m_MasterVolume * MIX_MAX_VOLUME));
}

float dae::SDLSoundSystem::SDLMixerImpl::GetMasterVolume() const
{
	return m_MasterVolume;
}




dae::SDLSoundSystem::SDLSoundSystem()
	:SoundSystem()
{
}

dae::SDLSoundSystem::~SDLSoundSystem()
{
	Quit();
}

void dae::SDLSoundSystem::Init(const std::string& dataPath)
{
	m_pImpl = std::make_unique<SDLMixerImpl>();
	m_DataPath = dataPath;
	m_pImpl->Init();


	m_ThreadRunning = true;
	m_SoundThread = std::jthread(&SDLSoundSystem::SoundThread, this);
}

void dae::SDLSoundSystem::Quit()
{
	m_ThreadRunning = false;
	m_QueueCondition.notify_all();

	if (m_pImpl)
	{
		m_pImpl->Quit();
	}
}

void dae::SDLSoundSystem::NotifySound(SoundData soundData)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	soundData.filePath = m_DataPath + soundData.filePath;
	m_EventQueue.push(soundData);


	m_QueueCondition.notify_all();
}

void dae::SDLSoundSystem::SetMasterVolume(float volume)
{
	m_pImpl->SetMasterVolume(volume);
}

float dae::SDLSoundSystem::GetMasterVolume() const
{
	return m_pImpl->GetMasterVolume();
}

void dae::SDLSoundSystem::PlaySound(const SoundData& soundData)
{
	if (!IsSoundLoaded(soundData.id))
	{
		LoadSound(soundData);
	}


	m_pImpl->PlaySound(soundData.id, soundData.soundType, soundData.volume);
}

void dae::SDLSoundSystem::LoadSound(const SoundData& soundData)
{
	if (!IsSoundLoaded(soundData.id))
	{
		m_pImpl->LoadSound(soundData);
	}
}

bool dae::SDLSoundSystem::IsSoundLoaded(unsigned short id)
{
	return m_pImpl->IsSoundLoaded(id);
}

void dae::SDLSoundSystem::SoundThread()
{
	while (m_ThreadRunning)
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_QueueCondition.wait(lock, [&] {

			if (!m_ThreadRunning)
				return true;

			return !m_EventQueue.empty();
			});


		if (m_EventQueue.empty())
			return;

		SoundData data = m_EventQueue.front();


		m_EventQueue.pop();
		lock.unlock();

		if (data.loadFile)
			LoadSound(data);
		else
			PlaySound(data);

	}
}