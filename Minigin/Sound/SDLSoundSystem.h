#ifndef ENGINE2D_SDLSOUNDSYSTEM_H
#define ENGINE2D_SDLSOUNDSYSTEM_H
#include "SoundSystem.h"
#include <iostream>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>


namespace dae
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:

		SDLSoundSystem();
		~SDLSoundSystem();

		SDLSoundSystem(const SDLSoundSystem& other) = delete;
		SDLSoundSystem(SDLSoundSystem&& other) = delete;
		SDLSoundSystem& operator=(const SDLSoundSystem& other) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&& other) = delete;

		void Init(const std::string& dataPath) override;
		void Quit() override;
		virtual void NotifySound(SoundData soundData);


	private:

		std::string m_DataPath;

		std::queue<SoundData> m_EventQueue;
		std::mutex m_QueueMutex;
		std::condition_variable m_QueueCondition;
		std::atomic<bool> m_ThreadRunning{ false };

		std::jthread m_SoundThread;
		void SoundThread();



		class SDLMixerImpl;
		std::unique_ptr<SDLMixerImpl> m_pImpl{ nullptr };



		bool IsSoundLoaded(unsigned short id);

		void PlaySound(const SoundData& soundData);
		void LoadSound(const SoundData& soundData);
	};
}

#endif // !ENGINE2D_SDLSOUNDSYSTEM_H
