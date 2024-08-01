#include "EnemyController.h"
#include "BaseEnemyComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include <iostream>



dae::EnemyController::EnemyController(GameObject* owner)
	:BaseComponent(owner)
	, m_attackDelay{ 2.0f }
	, m_currentAttackDelay{ 0.0f }
{
	m_pEnemyController = GetOwner()->GetComponent<BaseEnemyComponent>();
	assert(m_pEnemyController && "An EnemyController must have a BaseEnemyComponent to control");
}

dae::EnemyController::~EnemyController()
{
	std::cout << "EnemyController \n";
}

void dae::EnemyController::Update()
{

	// Increment the attack delay timer
	m_currentAttackDelay += GameTime::GetInstance().GetDeltaTime();

	if (m_currentAttackDelay < m_attackDelay)
	{
		return;
	}

	// Attack
	m_pEnemyController->Attack();
	m_currentAttackDelay = 0;
}
