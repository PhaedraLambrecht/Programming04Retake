#include "BlockComponent.h"
#include "GameObject.h"
#include "Componennts/CollisionComponent.h"

#include <iostream>


#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

dae::BlockComponent::BlockComponent(GameObject* owner)
	:BaseComponent(owner)
	,m_isMoving(false)
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
            enemy->GetComponent<TransformComponent>()->BlockDirection(enemy->GetComponent<TransformComponent>()->GetLastMovementDirection());
        }
    }



    if (m_isMoving)
    {
        glm::vec2 pos{ GetOwner()->GetComponent<dae::TransformComponent>()->GetLocalPosition() };
        if (IsPositionInsideWall(pos))
        {
			m_isMoving = false;
			return;
        }

        if (m_direction == "Left")
        {
            pos.x -= 1.0f;
        }
        else if (m_direction == "Right")
        {
            pos.x += 1.0f;
        }
        else if (m_direction == "Up")
        {
            pos.y -= 1.0f;
        }
        else if (m_direction == "Down")
        {
            pos.y += 1.0f;
        }

        // Check if the new position is inside a wall
        if (IsPositionInsideWall(pos))
        {
            m_isMoving = false;
            return;
        }

        GetOwner()->GetComponent<dae::TransformComponent>()->SetLocalPosition(pos.x, pos.y);
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

void dae::BlockComponent::OnHitCallback(const CollisionData& /*collisionOwner*/, const CollisionData& hitObject)
{
    if (hitObject.ownerType != "DiamondAttack")
        return;

    DoDamage(hitObject.owningObject);
}

bool dae::BlockComponent::DoDamage(GameObject* player)
{
    // If it's not, move to the new position
	m_isMoving = true;
    

	m_direction = player->GetComponent<dae::TransformComponent>()->GetLastMovementDirection();


    return true;
}


bool dae::BlockComponent::IsPositionInsideWall(const glm::vec2& /*position*/)
{
    auto ownerTransform = GetOwner()->GetComponent<dae::TransformComponent>();
    auto ownerCollision = GetOwner()->GetComponent<dae::CollisionComponent>();


    for (const auto& wall : SceneManager::GetInstance().GetActiveScene().GetWalls())
    {
        if (wall == GetOwner())
            continue;

        auto wallTransform = wall->GetComponent<dae::TransformComponent>();
        auto wallCollision = wall->GetComponent<dae::CollisionComponent>();
        float wallX = wallTransform->GetWorldPosition().x;
        float wallY = wallTransform->GetWorldPosition().y;
        float wallWidth = wallCollision->GetBounds().x;
        float wallHeight = wallCollision->GetBounds().y;

        float thisX =  ownerTransform->GetWorldPosition().x;
        float thisY = ownerTransform->GetWorldPosition().y;
        float thisWidth = ownerCollision->GetBounds().x;
        float thisHeight = ownerCollision->GetBounds().y;

        if (thisX + thisWidth < wallX || thisX > wallX + wallWidth ||
            thisY + thisHeight < wallY || thisY > wallY + wallHeight)
        {
            // No collision
        }
        else
        {
            return true; // Collision detected
        }
    }

    return false; // No collision detected
}