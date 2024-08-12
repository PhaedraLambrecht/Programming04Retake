#include "HealthComponent.h"
#include "Events/EventManager.h"
#include "GameLoader.h"
#include <iostream>


dae::HealthComponent::HealthComponent(GameObject* owner)
	:BaseComponent(owner)
	,m_PlayerIndex{0}
{
	 SetHealth(0);
}

void dae::HealthComponent::Damaged(int damage)
{
	m_Health -= damage;
	std::cout << m_Health << std::endl;

	std::unique_ptr<PlayerEvent> event = std::make_unique<PlayerEvent>("Damage", m_PlayerIndex);
	EventManager::GetInstance().QueueEvent<PlayerEvent>(std::move(event));



	if (m_Health <= 0)
	{
		std::cout << m_Health << std::endl;
		Death();
		
	}
}

void dae::HealthComponent::SetHealth(int health = 1)
{
	m_Health = health;
}

int dae::HealthComponent::GeTHealth() const
{
	return m_Health;
}


void dae::HealthComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_PlayerIndex = playerIndex;
}


void dae::HealthComponent::Death()
{
	std::unique_ptr<PlayerEvent> event = std::make_unique<PlayerEvent>("Death", m_PlayerIndex);
	EventManager::GetInstance().QueueEvent<PlayerEvent>(std::move(event));

	GameLoader gameLoader{};
	std::string sceneName = "HighScore";
	gameLoader.LoadHighScoreScene(sceneName);
}