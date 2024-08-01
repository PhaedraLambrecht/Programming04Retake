#include "DamageCommand.h"
#include "GameObject.h"
#include "Componennts/HealthComponent.h"
#include <iostream>

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

dae::DamageCommand::~DamageCommand()
{
	std::cout << "DamageCommand\n";
}

void dae::DamageCommand::Execute()
{
	m_pHealthComponnent->Damaged(m_Damage);
}
