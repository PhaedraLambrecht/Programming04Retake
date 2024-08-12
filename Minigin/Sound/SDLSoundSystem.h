#ifndef ENGINE2D_SDLSOUNDSYSTEM_H
#define ENGINE2D_SDLSOUNDSYSTEM_H
#include "SoundSystem.h"
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

		void SetMasterVolume(float volume) override;
		float GetMasterVolume() const override;

		void IsSoundMuterd(bool isMuted) override;


	private:


		float m_masterVolume{ 1.0f };// 0.0f - 1.0f
		float m_savedVolume;// 0.0f - 1.0f

		std::string m_dataPath;

		std::queue<SoundData> m_eventQueue;
		std::mutex m_queueMutex;
		std::condition_variable m_queueCondition;
		std::atomic<bool> m_threadRunning{ false };

		std::jthread m_soundThread;
		void SoundThread();



		class SDLMixerImpl;
		std::unique_ptr<SDLMixerImpl> m_pImpl{ nullptr };



		bool IsSoundLoaded(unsigned short id);

		void PlaySound(const SoundData& soundData);
		void LoadSound(const SoundData& soundData);
	};
}

#endif // !ENGINE2D_SDLSOUNDSYSTEM_H
