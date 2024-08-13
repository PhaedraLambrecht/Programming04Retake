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
#include "events/Event.h"
#include "Commands/AddPointsCommand.h"
#include "Resources/ResourceManager.h"

#include <iostream>


dae::EggBlockComponent::EggBlockComponent(GameObject* owner)
	:BlockComponent(owner)
	, m_currentTime{ 0.0f }
	, m_hasHatched{ false }
	, m_Health{ 1 }
	, m_isFlashing{ false }
	, m_flashTimer{ 0.0f }
	, m_texture1{ "" }
	, m_texture2{ "" }
	, m_pBackground{ nullptr }
{
	// Random time between 10 and 20 seconds
	m_hatchingTime = float(rand() % 20 + 10);


	// Make the blocks flash
	StartFlashing();
}

void dae::EggBlockComponent::Update()
{
	if (m_isFlashing)
	{
		m_flashTimer += GameTime::GetInstance().GetDeltaTime();
		if (m_flashTimer >= 2.0f)
		{
			StopFlashing();
		}
		else
		{
			// Flash between textures
			if (fmod(m_flashTimer, 0.5f) < 0.25f) // Flash every 0.5 seconds
			{
				// original
				GetOwner()->GetComponent<ImageComponent>()->SetTexture(m_texture1);
			}
			else
			{
				// flash
				GetOwner()->GetComponent<ImageComponent>()->SetTexture(m_texture2);
			}
		}
	}


    m_currentTime += GameTime::GetInstance().GetDeltaTime();
    if (m_currentTime >= m_hatchingTime && !m_hasHatched)
    {
		m_hasHatched = true;

		const auto enemy = CreateEnemyObject(GetOwner()->GetComponent<dae::TransformComponent>()->GetWorldPosition(), GetPlayer(), m_pBackground);
		SceneManager::GetInstance().GetActiveScene().Add(enemy);
		SceneManager::GetInstance().GetActiveScene().AddEnemy(enemy.get());


		GetOwner()->MarkForDestruction();
    }
}

void dae::EggBlockComponent::SetBackground(GameObject* background)
{
	m_pBackground = background;
}

std::shared_ptr<dae::GameObject> dae::EggBlockComponent::CreateEnemyObject(const glm::vec2& position, std::vector<dae::GameObject*> players, const dae::GameObject* backGroundImage)
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
	enemy->AddComponent<dae::RecognizerEnemy>()->Initialize(enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().x, enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().y, 10, 10, players);

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

void dae::EggBlockComponent::AddPointsToPlayers(GameObject* player)
{
	dae::AddPointsCommand addPointsCommand(player);
	addPointsCommand.Execute();
}

void dae::EggBlockComponent::NotifyOnDeath()
{
	Event event;
	event.eventType = "GainDiamond";

	GetOwner()->MarkForDestruction();
}

void dae::EggBlockComponent::StartFlashing()
{
	m_isFlashing = true;
	m_flashTimer = 0.0f;

	m_texture1 = "iceBlock.png";
	m_texture2 = "iceBlockGreen.png";		
}

void dae::EggBlockComponent::StopFlashing()
{
	m_isFlashing = false;
	GetOwner()->GetComponent<ImageComponent>()->SetTexture(m_texture1);
}

void dae::EggBlockComponent::OnHitCallback(const CollisionData& /*collisionOwner*/, const CollisionData& hitObject)
{
	if (hitObject.ownerType != "DiamondAttack")
		return;

	DoDamage(hitObject.owningObject);
}

void dae::EggBlockComponent::OnDeath(const Event* e)
{
	if (strcmp(e->eventType, "DiamondDeath") != 0)
		return;

	NotifyOnDeath();
}

bool dae::EggBlockComponent::DoDamage(GameObject* player)
{
	std::cout << m_Health << std::endl;
	--m_Health;

	if (m_Health > 0)
		return false;

	AddPointsToPlayers(player);
	NotifyOnDeath();
	return true;
}
