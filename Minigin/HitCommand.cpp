#include "HitCommand.h"
#include "GameObject.h"
#include "HitComponennt.h"
#include <iostream>

dae::HitCommand::HitCommand(GameObject* actor, int playerIndex)
{
	m_pSchootComponent = actor->GetComponent<dae::HitComponennt>();
	m_pSchootComponent->SetPlayerIndex(playerIndex);
}

dae::HitCommand::~HitCommand()
{
	std::cout << "SchootCommand\n";
}

void dae::HitCommand::Execute()
{
	m_pSchootComponent->Attack();
}
