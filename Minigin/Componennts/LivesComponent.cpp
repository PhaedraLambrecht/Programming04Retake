#include "LivesComponent.h"
#include "Events/EventManager.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include <memory>
#include <iostream>


dae::LivesComponent::LivesComponent(GameObject* owner)
	:BaseComponent(owner)
	,m_PlayerIndex{0}
	,m_LivesLeft{1}
{
	if (GetOwner()->HasComponent<TextComponent>())
	{
		m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
	}
	else
	{
		m_pTextComponent = nullptr;
	}


	UpdateText();


	// Define a lambda function to bind the LoseLife member function to an event observer
	auto boundLoseLife = [this](const Event* event)
		{
			// Call the LoseLife member function of the LivesComponent object with the event argument passed by the event manager
			this->Death(event);
		};
	PlayerEvent event{ "Death", m_PlayerIndex };
	EventManager::GetInstance().RegisterObserver(event, boundLoseLife);
}

dae::LivesComponent::~LivesComponent()
{
	std::cout << "LivesComponent\n";
}


void dae::LivesComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_PlayerIndex = playerIndex;
}

void dae::LivesComponent::SetLives(GameObject* Player)
{
	m_LivesLeft = Player->GetComponent<HealthComponent>()->GeTHealth();
	UpdateText();
}

void dae::LivesComponent::UpdateText()
{
	std::string text = "Player: " + std::to_string(m_PlayerIndex) + " - Lives: " + std::to_string(m_LivesLeft);
	m_pTextComponent->SetText(text);
}

void dae::LivesComponent::Death(const Event* e)
{
	if (m_LivesLeft <= 0)
	{
		std::cout << "Dead\n";
		return;
	}

	if (!(strcmp(e->eventType, "Death") == 0))
		return;


	if (const PlayerEvent* event = dynamic_cast<const PlayerEvent*>(e))
	{
		if (event->playerIndex == m_PlayerIndex)
		{
			m_LivesLeft -= 1;
			UpdateText();
		}
	}
}
