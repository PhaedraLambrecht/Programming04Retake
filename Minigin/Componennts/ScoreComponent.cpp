#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Events/EventManager.h"
#include "Events/Event.h"
#include "GameObject.h"
#include <memory>
#include <stdexcept>
#include <iostream>

dae::ScoreComponent::ScoreComponent(GameObject* owner)
	:BaseComponent(owner)
	,m_rewardAmount{50}
	,m_pointsEarned{0}
	,m_playerIndex{0}
{

	if (GetOwner()->HasComponent<TextComponent>())
	{
		m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
	}
	else
	{
		m_pTextComponent = GetOwner()->AddComponent<TextComponent>();
	}


	UpdateText();


	// Define a lambda function to bind the EarnPoints member function to an event observer
	auto boundsGainScore = [this](const Event* event)
	{
		// Call the EarnPoints member function of the ScoreComponent object with the event argument passed by the event manager
		this->UpdateScore(event);
	};

	PlayerEvent event{ "EnemyDeath", m_playerIndex };
	EventManager::GetInstance().RegisterObserver(event, boundsGainScore);

	//PlayerEvent event2{ "DiamondAttack", m_playerIndex };
	//EventManager::GetInstance().RegisterObserver(event2, boundsGainScore);
}

void dae::ScoreComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_playerIndex = playerIndex;
}

void dae::ScoreComponent::UpdateText()
{
	SceneManager::GetInstance().m_Score = m_pointsEarned;
	std::string text = "Player: " + std::to_string(m_playerIndex) + " - score: " + std::to_string(m_pointsEarned);
	m_pTextComponent->SetText(text);

}

void dae::ScoreComponent::UpdateScore(const Event* e)
{
	if (strcmp(e->eventType, "EnemyDeath") == 0)
	{
		const PlayerEvent* playerEvent = dynamic_cast<const PlayerEvent*>(e);
		if (playerEvent && playerEvent->playerIndex == m_playerIndex)
		{
			m_pointsEarned += m_rewardAmount;
			UpdateText();

		}
	}
}