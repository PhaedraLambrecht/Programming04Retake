#include "BlockComponent.h"
#include "GameObject.h"
#include "Componennts/CollisionComponent.h"

#include <iostream>


#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

dae::BlockComponent::BlockComponent(GameObject* owner)
	:BaseComponent(owner)
{
	m_CollisionComponent = owner->GetComponent<dae::CollisionComponent>();
}

void dae::BlockComponent::Update()
{
    // Check for overlap with the player
    for (const auto& player : m_player)
    {
        float playerX = player->GetComponent<dae::TransformComponent>()->GetWorldPosition().x;
        float playerY = player->GetComponent<dae::TransformComponent>()->GetWorldPosition().y;
        float playerWidth = player->GetComponent<dae::CollisionComponent>()->GetBounds().x;
        float playerHeight = player->GetComponent<dae::CollisionComponent>()->GetBounds().y;


        float thisX = GetOwner()->GetComponent<dae::TransformComponent>()->GetWorldPosition().x;
        float thisY = GetOwner()->GetComponent<dae::TransformComponent>()->GetWorldPosition().y;
        float thisWidth = GetOwner()->GetComponent<dae::CollisionComponent>()->GetBounds().x;
        float thisHeight = GetOwner()->GetComponent<dae::CollisionComponent>()->GetBounds().y;

        if (playerX + playerWidth > thisX && playerX < thisX + thisWidth &&
            playerY + playerHeight > thisY && playerY < thisY + thisHeight)
        {
            player->GetComponent<TransformComponent>()->BlockDirection(player->GetComponent<TransformComponent>()->GetLastMovementDirection());
        }
    }

  
    for (const auto& enemy : SceneManager::GetInstance().GetActiveScene().GetEnemy())
    {
        float enemyX = enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().x;
        float enemyY = enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().y;
        float enemyWidth = enemy->GetComponent<dae::CollisionComponent>()->GetBounds().x;
        float enemyHeight = enemy->GetComponent<dae::CollisionComponent>()->GetBounds().y;


        float thisX = GetOwner()->GetComponent<dae::TransformComponent>()->GetWorldPosition().x;
        float thisY = GetOwner()->GetComponent<dae::TransformComponent>()->GetWorldPosition().y;
        float thisWidth = GetOwner()->GetComponent<dae::CollisionComponent>()->GetBounds().x;
        float thisHeight = GetOwner()->GetComponent<dae::CollisionComponent>()->GetBounds().y;

        if (enemyX + enemyWidth > thisX && enemyX < thisX + thisWidth &&
            enemyY + enemyHeight > thisY && enemyY < thisY + thisHeight)
        {
            std::cout << "stop moving in that direction" << std::endl;
            enemy->GetComponent<TransformComponent>()->BlockDirection(enemy->GetComponent<TransformComponent>()->GetLastMovementDirection());
        }
    }

}

void dae::BlockComponent::SetPlayer(std::vector<dae::GameObject*> player)
{
	m_player = player;
}

std::vector<dae::GameObject*> dae::BlockComponent::GetPlayer()
{
    return m_player;
}
