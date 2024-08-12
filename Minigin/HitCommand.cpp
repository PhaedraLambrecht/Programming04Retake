#include "HitCommand.h"
#include "GameObject.h"
#include "HitComponennt.h"


dae::HitCommand::HitCommand(GameObject* actor, int playerIndex)
	:BaseCommand()
{
	if (actor->HasComponent<HitComponennt>())
	{
		m_pSchootComponent = actor->GetComponent<HitComponennt>();
	}
	else
	{
		m_pSchootComponent = nullptr;
	}

	
	m_pSchootComponent->SetPlayerIndex(playerIndex);
}

void dae::HitCommand::Execute()
{
	m_pSchootComponent->Attack();
}
