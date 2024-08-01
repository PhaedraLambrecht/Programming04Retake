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


	void PlaySound(unsigned short id, dae::SoundData::SoundType soundType, float volume);
	void LoadSound(unsigned short id, const std::string& filepath);
	bool IsSoundLoaded(unsigned short id);

	void Init();
	void Quit();


private:

	std::unordered_map<unsigned short, Mix_Chunk*> m_LoadedSounds;
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


void dae::SDLSoundSystem::SDLMixerImpl::PlaySound(unsigned short id, SoundData::SoundType soundType, float volume)
{
	if (!IsSoundLoaded(id))
		throw std::runtime_error("Sound with ID " + std::to_string(id) + " is not loaded.");

	Mix_Chunk* chunk = m_LoadedSounds[id];

	int mixVolume = static_cast<int>(volume * MIX_MAX_VOLUME); // Convert float volume to SDL_mixer scale
	Mix_VolumeChunk(chunk, mixVolume);


	//int channel{ -1 };
	//int loops{ 0 };
	//// This only cares about effects atm
	//switch (soundType)
	//{
	//case dae::SoundData::SoundType::SoundEffect:
	//	channel = Mix_GroupAvailable(MIX_CHANNEL_GROUP_EFFECTS);
	//	loops = 0;
	//	// if no channel available use channel 1
	//	if (channel == -1)
	//	{
	//		channel = 1;
	//	}
	//	break;
	//case dae::SoundData::SoundType::Music:
	//	channel = Mix_GroupAvailable(MIX_CHANNEL_GROUP_MUSIC);
	//	loops = -1; // Loop indefinitely
	//	// If no channel available, use channel 0 (reserved for background music)
	//	if (channel == -1) {
	//		channel = 0;
	//	}
	//	break;
	//}

	//Mix_VolumeChunk(chunk, static_cast<int>(volume * MIX_MAX_VOLUME));
	//Mix_PlayChannel(channel, chunk, loops);
	
	int channel{ 0 };
	int repeat{ 1 };
	switch (soundType)
	{
	case SoundData::SoundType::SoundEffect:
		channel = Mix_GroupAvailable(MIX_CHANNEL_GROUP_EFFECTS);
		repeat = 0;
		// if no channel available use channel 1
		if (channel == -1)
		{
			channel = 1;
		}
		break;
	}


	Mix_PlayChannel(channel, chunk, repeat);
}

void dae::SDLSoundSystem::SDLMixerImpl::LoadSound(unsigned short id, const std::string& filepath)
{
	// Sound is already loaded
	if (IsSoundLoaded(id)) 
		return; 
	

	Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());
	
	//auto* song = Mix_LoadMUS(filepath.c_str());		<--- For music
	
	if (chunk == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load sound: ") + Mix_GetError());
	}
	m_LoadedSounds[id] = chunk;
	
}

bool dae::SDLSoundSystem::SDLMixerImpl::IsSoundLoaded(unsigned short id)
{
	return m_LoadedSounds.find(id) != m_LoadedSounds.end();
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


	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 5, 0) < 0) {
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
		m_pImpl->LoadSound(soundData.id, soundData.filePath);
	}
}

bool dae::SDLSoundSystem::IsSoundLoaded(unsigned short id)
{
	return m_pImpl->IsSoundLoaded(id);
}

void dae::SDLSoundSystem::SoundThread()
{
	//while (m_ThreadRunning)
	//{
	//	std::unique_lock<std::mutex> lock(m_QueueMutex);
	//	m_QueueCondition.wait(lock, [&] {
	//
	//		if (!m_ThreadRunning)
	//			return true;
	//
	//		return !m_EventQueue.empty();
	//		});
	//
	//	if (m_EventQueue.empty())
	//		return;
	//
	//	SoundData data = m_EventQueue.front();
	//	m_EventQueue.pop();
	//	lock.unlock();
	//
	//
	//	if (!data.loadFile)
	//	{
	//		LoadSound(data);
	//	}
	//	else
	//	{
	//		PlaySound(data);
	//	}
	//
	//}

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