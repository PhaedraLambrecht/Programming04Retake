#include "EggBlockComponent.h"
#include "GameTime.h"
#include "GameObject.h"

#include "Componennts/ImageComponent.h"
#include "Componennts/ImageRenderComponent.h"
#include "Componennts/HealthComponent.h"
#include "Enemy/RecognizerEnemy.h"
#include "Enemy/EnemyController.h"
#include "Componennts/CollisionComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include <iostream>

dae::EggBlockComponent::EggBlockComponent(GameObject* owner)
	:BlockComponent(owner)
	, m_HatchingTime{ 3.0f }
	, m_CurrentTime{ 0.0f }
	, m_HasHatched{ false }
{
}

void dae::EggBlockComponent::Update()
{
    m_CurrentTime += GameTime::GetInstance().GetDeltaTime();
    if (m_CurrentTime >= m_HatchingTime && !m_HasHatched)
    {
		std::cout << "Hatched" << std::endl;
		m_HasHatched = true;
		const auto enemy = CreateEnemyObject(GetOwner()->GetComponent<dae::TransformComponent>()->GetWorldPosition(), m_Player, m_Background);
		SceneManager::GetInstance().GetActiveScene().Add(enemy);


		GetOwner()->MarkForDestruction();
    }
}

void dae::EggBlockComponent::SetPlayer(std::shared_ptr<dae::GameObject> player)
{
	m_Player = player;
}

void dae::EggBlockComponent::SetBackground(GameObject* background)
{
	m_Background = background;
}

std::shared_ptr<dae::GameObject> dae::EggBlockComponent::CreateEnemyObject(const glm::vec2& position, std::shared_ptr<dae::GameObject> player, const dae::GameObject* backGroundImage)
{
	const auto enemy = std::make_shared<dae::GameObject>();


	// Image
	enemy->AddComponent<dae::ImageComponent>();
	enemy->AddComponent<dae::ImageRenderComponent>();
	enemy->GetComponent<dae::ImageComponent>()->SetTexture("snoBee1.png");

	// Transform
	enemy->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y);

	// Health
	enemy->AddComponent<dae::HealthComponent>()->SetHealth(1);


	// Enemy components
	enemy->AddComponent<dae::RecognizerEnemy>()->Initialize(enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().x, enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().y, 10, 10, player);

	const auto baseEnemy = enemy->GetComponent<dae::BaseEnemyComponent>();
	enemy->GetComponent<dae::RecognizerEnemy>()->SetWindowDimensions(backGroundImage->GetComponent<dae::TransformComponent>()->GetWorldPosition().x,
		backGroundImage->GetComponent<dae::TransformComponent>()->GetWorldPosition().y,
		backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x * 2,
		backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y * 2
	);

	enemy->AddComponent<dae::EnemyController>();


	dae::CollisionComponent* collision = enemy->AddComponent<dae::CollisionComponent>();
	SceneManager::GetInstance().GetActiveScene().AddCollision(collision);
	collision->SetScene(&SceneManager::GetInstance().GetActiveScene());

	collision->SetCollisionData({ "Enemy", enemy.get() });
	collision->SetBounds(
		enemy->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x,
		enemy->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y
	);

	auto boundHitCallback = std::bind(&dae::BaseEnemyComponent::OnHitCallback, baseEnemy, std::placeholders::_1, std::placeholders::_2);
	enemy->GetComponent<dae::CollisionComponent>()->SetCallback(boundHitCallback);


	return enemy;
}
