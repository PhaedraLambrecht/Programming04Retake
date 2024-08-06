#ifndef ENGINE2D_GAMETIME_H
#define ENGINE2D_GAMETIME_H
#include "Singleton.h"
#include <chrono>


namespace dae
{
	class GameTime final: public Singleton<GameTime>
	{
	public:

		GameTime();
		virtual ~GameTime() = default;

		// Copy and move
		GameTime(const GameTime& other) = delete;
		GameTime(GameTime&& other) = delete;
		GameTime& operator=(const GameTime& other) = delete;	
		GameTime& operator=(GameTime&& other) = delete;



		void Update();
		float GetDeltaTime() const;
		float GetFixedTimeStep() const;
		int GetMSPerFrame() const;
		std::chrono::time_point<std::chrono::steady_clock> GetPreviousTime() const;


	private:

		float m_deltaTime;
		const int m_frameRateCap;
		const float m_fixedTimeStep;
		int m_MSPerFrame;
		std::chrono::time_point<std::chrono::steady_clock> m_previousTime;

	};
}

#endif // !ENGINE2D_GAMETIME_H