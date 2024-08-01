#include "AddPointsCommand.h"
#include "GameObject.h"
#include "Componennts/AddPointsComponnent.h"
#include <iostream>


dae::AddPointsCommand::AddPointsCommand(GameObject* actor)
	:BaseCommand()
	,m_Damage{}
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

dae::AddPointsCommand::~AddPointsCommand()
{
	std::cout << "AddPointsCommand\n";
}

void dae::AddPointsCommand::Execute()
{
	m_pAddPointsComponnent->AddPoints();
}