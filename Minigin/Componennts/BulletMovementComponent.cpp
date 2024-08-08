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


	 if (m_Direction == "Left") // Moving down
	 {
		 currentPos.x -=  distanceThisUpdate; // Subtract instead of add
	 }
	 else if (m_Direction == "Right") // Moving down
	 {
		 currentPos.x += distanceThisUpdate; // Add instead of subtract
	 }
	 else if (m_Direction == "Down")
	 {
		 currentPos.y += distanceThisUpdate; // Add instead of subtract
	 }
	 else if (m_Direction == "Up")
	 {
		 currentPos.y -= distanceThisUpdate; // Subtract instead of add
	 }

	m_DistanceTraveled += distanceThisUpdate;
	m_pTransform->SetLocalPosition(currentPos.x, currentPos.y);

	if (m_DistanceTraveled >= m_MaxDistance)
	{
		GetOwner()->MarkForDestruction();
	}

}

void dae::BulletMovementComponent::SetMaxDistance(float distance)
{
	m_MaxDistance = distance;
}

void dae::BulletMovementComponent::SetDirection(const std::string& direction)
{
	m_Direction = direction;
}
