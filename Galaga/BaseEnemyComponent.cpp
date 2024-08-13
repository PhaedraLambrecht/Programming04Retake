#include "BaseEnemyComponent.h"
#include "Events/EventManager.h"
#include <iostream>

dae::BaseEnemyComponent::BaseEnemyComponent(GameObject* owner)
	:BaseComponent(owner)
	, m_Health{3}
	, m_EnemySpeed{40.0f}
	, m_ShootCannonCooldown{0.f}
	, position{}
	, m_Width{}
	, m_Height{}
	, m_BlockMoveLeft{ false }
	, m_BlockMoveRight{ false }
	, m_BlockMoveUp{ false }
	, m_BlockMoveDown{ false }
{
	std::unique_ptr<Event> event = std::make_unique<Event>();
	event->eventType = "EnemySpawned";

	EventManager::GetInstance().QueueEvent(std::move(event));
}

void dae::BaseEnemyComponent::Initialize(float , float , float , float , std::shared_ptr<GameObject> )
{
}

void dae::BaseEnemyComponent::Update()
{
}

void dae::BaseEnemyComponent::OnHitCallback(const CollisionData&, const CollisionData&)
{
}

void dae::BaseEnemyComponent::Attack()
{
}

void dae::BaseEnemyComponent::ShootCannon(float)
{
}

bool dae::BaseEnemyComponent::DoDamage()
{
	return false;
}