#include "SoundManager.h"
#include "SDLSoundSystem.h"
#include "NullSoundSystem.h"


dae::SoundManager::SoundManager()
{
	m_SoundSystem = std::make_unique<NullSoundSystem>();
}

dae::SoundManager::~SoundManager()
{
	std::cout << "SoundManager\n";
}

void dae::SoundManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;
	m_SoundSystem->Init(dataPath);
}

void dae::SoundManager::Destroy()
{
	m_SoundSystem->Quit();
	
	if (m_SoundSystem)
	{
		m_SoundSystem = nullptr;
	}
}


void dae::SoundManager::SetSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
	m_SoundSystem->Quit();
	m_SoundSystem = std::move(soundSystem);

	m_SoundSystem->Init(m_DataPath);
}

dae::SoundSystem* dae::SoundManager::GetSoundSystem()
{
	return m_SoundSystem.get();
};
