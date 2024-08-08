#include "PlayerNameCommand.h"
#include "GameObject.h"
#include "NameComponent.h"

#include <iostream>

dae::PlayerNameCommand::PlayerNameCommand(GameObject* player)
	:BaseCommand()
{
	m_pNameComponent = player->GetComponent<NameComponent>();
}

void dae::PlayerNameCommand::Execute()
{
	m_pNameComponent->CycleLetters(1);
}



dae::PlayerNameEnterCommand::PlayerNameEnterCommand(GameObject* player)
	:BaseCommand()
{
	m_pNameComponent = player->GetComponent<NameComponent>();
}

void dae::PlayerNameEnterCommand::Execute()
{
	m_pNameComponent->AddLetterToName();
	std::cout << m_pNameComponent->GetName() << std::endl;
}
