#ifndef ENGINE2D_SOUNDMANAGER_H
#define ENGINE2D_SOUNDMANAGER_H
#include "Singleton.h"
#include <iostream>
#include <memory>


namespace dae
{
	class SoundSystem;


	class SoundManager final : public Singleton<SoundManager>
	{
	public:

		void Init(const std::string& dataPath);
		void Destroy();

		void SetSoundSystem(std::unique_ptr<SoundSystem> soundSystem);
		SoundSystem* GetSoundSystem();


	private:

		friend class Singleton<SoundManager>;
		
		std::string m_DataPath;
		std::unique_ptr<SoundSystem> m_SoundSystem{ nullptr };


		SoundManager();
		~SoundManager() override;

		// Copy and move
		SoundManager(const SoundManager& other) = delete;
		SoundManager(SoundManager&& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
		SoundManager& operator=(SoundManager&& other) = delete;

	};
}

#endif // !ENGINE2D_SOUNDMANAGER_H
