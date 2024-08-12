#include "DamageCommand.h"
#include "GameObject.h"
#include "Componennts/HealthComponent.h"


dae::DamageCommand::DamageCommand(GameObject* actor, int damage)
	:BaseCommand()
	,m_Damage{damage}
{
	if (actor->HasComponent<HealthComponent>())
	{
		m_pHealthComponnent = actor->GetComponent<HealthComponent>();
	}
	else
	{
		m_pHealthComponnent = actor->AddComponent<HealthComponent>();
	}
}

void dae::DamageCommand::Execute()
{
	m_pHealthComponnent->Damaged(m_Damage);
}
