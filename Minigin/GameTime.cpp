#include "GameTime.h"


namespace dae
{
	GameTime::GameTime()
		:m_DeltaTime{}
		, m_FrameRateCap{ 60 }
		, m_FixedTimeStep{ 0.01f }
		, m_PreviousTime{ std::chrono::high_resolution_clock::now() }
	{
		m_MSPerFrame = 1000 / m_FrameRateCap;
	}


	void GameTime::Update()
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		m_DeltaTime = std::chrono::duration<float>(currentTime - m_PreviousTime).count();


		m_PreviousTime = currentTime;
	}

	float GameTime::GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	float GameTime::GetFixedTimeStep() const
	{
		return m_FixedTimeStep;
	}

	int GameTime::GetMSPerFrame() const
	{
		return m_MSPerFrame;
	}

	std::chrono::time_point<std::chrono::steady_clock> GameTime::GetPreviousTime() const
	{
		return m_PreviousTime;
	}
}