#ifndef ENGINE2D_NULLSOUNDSYSTEM_H
#define ENGINE2D_NULLSOUNDSYSTEM_H
#include "SoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public SoundSystem
	{
	public:

		NullSoundSystem() = default;
		~NullSoundSystem() override = default;

		// Copy and move
		NullSoundSystem(const NullSoundSystem& other) = delete;
		NullSoundSystem(NullSoundSystem&& other) = delete;
		NullSoundSystem& operator=(const NullSoundSystem& other) = delete;
		NullSoundSystem& operator=(NullSoundSystem&& other) = delete;


		void Init(const std::string&) override {};
		virtual void Quit() override {};

		virtual void NotifySound(SoundData) {};



		void SetMasterVolume(float /*volume*/) override {};
		float GetMasterVolume() const override { return 0.0f; };

		void IsSoundMuterd(bool /*isMuted*/) override {};
	};
}

#endif // !ENGINE2D_NULLSOUNDSYSTEM_H