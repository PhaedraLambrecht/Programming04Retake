#include "MoveCommand.h"
#include "GameObject.h"
#include "Componennts/TransformComponent.h"
#include "Componennts/RotatorComponent.h"
#include "GameTime.h"
#include <iostream>

dae::MoveCommand::MoveCommand(GameObject* actor, glm::vec2 direction, float moveSpeed)
	:m_pActor{ actor }
	,m_Direction{direction}
	,m_MoveSpeed{moveSpeed}
{
	if (actor->HasComponent<TransformComponent>())
	{
		m_pTransform = actor->GetComponent<TransformComponent>();
	}
	else
	{
		m_pTransform = nullptr;
	}
}

dae::MoveCommand::~MoveCommand()
{
	std::cout << "MoveCommand\n";
}


void dae::MoveCommand::Execute()
{
	glm::vec2 position{ m_pTransform->GetLocalPosition() + m_Direction * m_MoveSpeed * GameTime::GetInstance().GetDeltaTime() };

	if (m_Direction.x > 0)
	{
		m_pActor->GetComponent<RotatorComponent>()->RotateToAngle(90.0f);
	}
	else if(m_Direction.x < 0)
	{
		m_pActor->GetComponent<RotatorComponent>()->RotateToAngle(-90.0f);
	}	
	if (m_Direction.y > 0)
	{
		m_pActor->GetComponent<RotatorComponent>()->RotateToAngle(90.0f);
	}
	else if (m_Direction.y < 0)
	{
		m_pActor->GetComponent<RotatorComponent>()->RotateToAngle(-90.0f);
	}



	m_pTransform->SetLocalPosition(position.x, position.y);
}
