#include "PlayerNameCommand.h"
#include "GameObject.h"
#include "NameComponent.h"


dae::PlayerNameDownCommand::PlayerNameDownCommand(GameObject* player)
	:BaseCommand()
{
	m_pNameComponent = player->GetComponent<NameComponent>();
}

void dae::PlayerNameDownCommand::Execute()
{
	m_pNameComponent->CycleLetters(1);
}


dae::PlayerNameUpCommand::PlayerNameUpCommand(GameObject* player)
	:BaseCommand()
{
	m_pNameComponent = player->GetComponent<NameComponent>();
}

void dae::PlayerNameUpCommand::Execute()
{
	m_pNameComponent->CycleLetters(-1);
}




dae::PlayerNameEnterCommand::PlayerNameEnterCommand(GameObject* player)
	:BaseCommand()
{
	m_pNameComponent = player->GetComponent<NameComponent>();
}

void dae::PlayerNameEnterCommand::Execute()
{
	m_pNameComponent->AddLetterToName();
}

