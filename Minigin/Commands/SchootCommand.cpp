#include "SchootCommand.h"
#include "GameObject.h"
#include "Componennts/SchootComponent.h"


dae::SchootCommand::SchootCommand(GameObject* actor, int playerIndex)
	:BaseCommand()
{
	m_pSchootComponent = actor->GetComponent<dae::SchootComponent>();
	m_pSchootComponent->SetPlayerIndex(playerIndex);
}


void dae::SchootCommand::Execute()
{
	m_pSchootComponent->Attack();
}
