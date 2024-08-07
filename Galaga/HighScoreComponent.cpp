#include "HighScoreComponent.h"
#include "GameObject.h"
#include "Componennts/TextComponent.h"

#include <fstream>
#include <sstream>


dae::HighScoreComponent::HighScoreComponent(GameObject* owner)
	:BaseComponent(owner)
    , m_filePath{ "../Data/HighScore.txt" }
{
	m_ptextComponent = owner->GetComponent<TextComponent>();

	LoadHighScores();
}


void dae::HighScoreComponent::LoadHighScores()
{
    m_highScores.clear();

    std::ifstream file(m_filePath);
	if (!file.is_open())
    {
		throw std::runtime_error("Error: Could not open file " + m_filePath);
	}

	std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        int score;
        std::string playerName;
        iss >> score >> playerName;

      
        HighScore highScore{ score, playerName };
        m_highScores.push_back(highScore);
    }
	file.close();


	std::sort(m_highScores.begin(), m_highScores.end(), [](const HighScore& a, const HighScore& b)
	{
			return a.score > b.score;
	});

	std::string highScoreText;
	for (int i = 0; i < m_highScores.size(); ++i)
    {
		highScoreText += std::to_string(i + 1) + ": " + m_highScores[i].name + " " + std::to_string(m_highScores[i].score) + '\n';
    }
	m_ptextComponent->SetText(highScoreText);
}

void dae::HighScoreComponent::SaveHighScores()
{
    std::ofstream file(m_filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: Could not open file " + m_filePath);
    }


    for (const auto& highScore : m_highScores)
    {
        file << highScore.score << " " << highScore.name << std::endl;
    }
    file.close();
}

void dae::HighScoreComponent::AddScore(int score, const std::string& playerName)
{
    HighScore highScore{ score, playerName };

    if (m_highScores.size() < 5)
    {
		m_highScores.push_back(highScore);
    }
    else
    {
        auto it = m_highScores.begin();
        while (it != m_highScores.end() && it->score > score)
        {
            ++it;
        }

        if (it != m_highScores.end() && it->score <= score)
        {
            m_highScores.insert(it, highScore);
            m_highScores.pop_back();
        }
    }

   
    SaveHighScores();
}
