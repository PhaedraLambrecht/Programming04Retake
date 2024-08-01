#include "AddPointsComponnent.h"
#include "GameObject.h"
#include "Events/EventManager.h"
#include <iostream> 


dae::AddPointsComponnent::AddPointsComponnent(GameObject* owner)
	:BaseComponent(owner)
	,m_PlayerIndex{}
{
}

dae::AddPointsComponnent::~AddPointsComponnent()
{
	std::cout << "AddPointsComponnent\n";
}


void dae::AddPointsComponnent::SetPlayerIndex(unsigned int playerIndex)
{
	m_PlayerIndex = playerIndex;
}

void dae::AddPointsComponnent::AddPoints()
{
	auto newEvent = std::make_unique<PlayerEvent>("EnemyDeath", m_PlayerIndex);
	EventManager::GetInstance().QueueEvent<PlayerEvent>(std::move(newEvent));
}