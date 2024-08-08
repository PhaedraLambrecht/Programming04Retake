#pragma once
#include "Componennts/BaseComponent.h"
#include <string>
#include <vector>


namespace dae
{
	class TextComponent;

	class HighScoreComponent final : public BaseComponent
	{
	public:

		HighScoreComponent(GameObject* owner);
		~HighScoreComponent() = default;

		HighScoreComponent(const HighScoreComponent& other) = delete;
		HighScoreComponent(HighScoreComponent&& other) = delete;
		HighScoreComponent& operator=(const HighScoreComponent& other) = delete;
		HighScoreComponent& operator=(HighScoreComponent&& other) = delete;



		void LoadHighScores();
		void AddScore(int score, const std::string& playerName);


	private:

		void SaveHighScores();
	
		
		
		struct HighScore
		{
			int score;
			std::string name;
		};
		std::vector<HighScore> m_highScores;

		std::string m_filePath;
		TextComponent* m_ptextComponent;
	};
}
