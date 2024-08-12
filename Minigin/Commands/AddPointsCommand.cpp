#include "AddPointsCommand.h"
#include "GameObject.h"
#include "Componennts/AddPointsComponnent.h"
#include <iostream>


dae::AddPointsCommand::AddPointsCommand(GameObject* actor)
	:BaseCommand()
{
	if (actor->HasComponent<AddPointsComponnent>())
	{
		m_pAddPointsComponnent = actor->GetComponent<AddPointsComponnent>();
	}
	else
	{
		m_pAddPointsComponnent = actor->AddComponent<AddPointsComponnent>();
	}
}

void dae::AddPointsCommand::Execute()
{
	m_pAddPointsComponnent->AddPoints();
}