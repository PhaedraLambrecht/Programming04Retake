#include "BaseEnemyComponent.h"
#include "Events/EventManager.h"
#include <iostream>

dae::BaseEnemyComponent::BaseEnemyComponent(GameObject* owner)
	:BaseComponent(owner)
{
	std::unique_ptr<Event> event = std::make_unique<Event>();
	event->eventType = "EnemySpawned";

	EventManager::GetInstance().QueueEvent(std::move(event));
}

dae::BaseEnemyComponent::~BaseEnemyComponent()
{
	std::cout << "BaseEnemyComponent \n";
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