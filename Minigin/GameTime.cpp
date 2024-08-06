#include "GameTime.h"


namespace dae
{
	GameTime::GameTime()
		:m_deltaTime{}
		, m_frameRateCap{ 60 }
		, m_fixedTimeStep{ 0.01f }
		, m_previousTime{ std::chrono::high_resolution_clock::now() }
	{
		m_MSPerFrame = 1000 / m_frameRateCap;
	}


	void GameTime::Update()
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		m_deltaTime = std::chrono::duration<float>(currentTime - m_previousTime).count();


		m_previousTime = currentTime;
	}

	float GameTime::GetDeltaTime() const
	{
		return m_deltaTime;
	}

	float GameTime::GetFixedTimeStep() const
	{
		return m_fixedTimeStep;
	}

	int GameTime::GetMSPerFrame() const
	{
		return m_MSPerFrame;
	}

	std::chrono::time_point<std::chrono::steady_clock> GameTime::GetPreviousTime() const
	{
		return m_previousTime;
	}
}