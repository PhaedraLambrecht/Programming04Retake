#ifndef ENGINE2D_SOUNDSYSTEM_H
#define ENGINE2D_SOUNDSYSTEM_H
#include <iostream>


namespace dae
{
	using sound_id = unsigned short;

	struct SoundData
	{
		enum class SoundType
		{
			Music,
			SoundEffect,
		};

		sound_id id{};
		float volume{ 1 };
		SoundType soundType{ SoundType::SoundEffect };
		std::string filePath{ "" };
		bool loadFile{ false };
	};
	

	class SoundSystem
	{
	public:

		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		// Copy and move
		SoundSystem(const SoundSystem& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem& operator=(const SoundSystem& other) = delete;
		SoundSystem& operator=(SoundSystem&& other) = delete;


		virtual void Init(const std::string& m_DataPath) = 0;
		virtual void Quit() = 0;
		virtual void NotifySound(SoundData soundData) = 0;
	};
}


#endif // !ENGINE2D_SOUNDSYSTEM_H