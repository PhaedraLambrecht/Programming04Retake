#include "Componennts/BulletMovementComponent.h"
#include "GameObject.h"
#include "Componennts/TransformComponent.h"
#include "GameTime.h"
#include <iostream>



dae::BulletMovementComponent::BulletMovementComponent(GameObject* Owner)
	:BaseComponent(Owner)
{
	if (GetOwner()->HasComponent<TransformComponent>())
	{
		m_pTransform = GetOwner()->GetComponent<TransformComponent>();
	}
	else
	{
		m_pTransform = nullptr;
	}
}

dae::BulletMovementComponent::~BulletMovementComponent()
{
	std::cout << "BulletMovementComponent\n";
}

void dae::BulletMovementComponent::Update()
{
	auto currentPos = m_pTransform->GetWorldPosition();
	const float distanceThisUpdate{ m_MoveSpeed * GameTime::GetInstance().GetDeltaTime() };
	currentPos.y -= distanceThisUpdate;
	m_DistanceTraveled += distanceThisUpdate;
	m_pTransform->SetLocalPosition(currentPos.x, currentPos.y);

	if (m_DistanceTraveled >= m_MaxDistance)
	{
		GetOwner()->MarkForDestruction();
	}

}