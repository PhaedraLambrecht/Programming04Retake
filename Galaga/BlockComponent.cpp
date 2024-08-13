#include "BlockComponent.h"
#include "GameObject.h"
#include "Componennts/CollisionComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include <iostream>


dae::BlockComponent::BlockComponent(GameObject* owner)
	:BaseComponent(owner)
	,m_isMoving(false)
	, m_direction("None")
	, m_currentState(nullptr)
{
	m_CollisionComponent = owner->GetComponent<dae::CollisionComponent>();
}

dae::BlockComponent::~BlockComponent()
{
	delete m_currentState;
	m_currentState = nullptr;
}

void dae::BlockComponent::Update()
{
    // Check collision to block movement
    CheckCollision(m_player);
    CheckCollision(SceneManager::GetInstance().GetActiveScene().GetEnemy());


    if (m_isMoving)
    {
		m_currentState->Update(GetOwner());
    }
}

void dae::BlockComponent::SetPlayer(std::vector<dae::GameObject*> player)
{
	std::vector<dae::GameObject*> tempPlayer;
	for (auto& p : player)
    {
        if (p != nullptr)
        {
			tempPlayer.push_back(p);
		}
	}

    m_player = tempPlayer;
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
	m_isMoving = true;
    m_direction = player->GetComponent<dae::TransformComponent>()->GetLastMovementDirection();

    delete m_currentState;
	m_currentState = new BlockMovingState();


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

bool dae::BlockComponent::GetIsMoving() const
{
    return m_isMoving;
}

void dae::BlockComponent::SetIsMoving(bool isMoving)
{
    m_isMoving = isMoving;
}

std::string dae::BlockComponent::GetDirection() const
{
    return m_direction;
}

void dae::BlockComponent::SetDirection(const std::string& direction)
{
    if (m_direction == "None")
    {
        return;
    }

    m_direction = direction;
}

dae::BlockState* dae::BlockComponent::GetCurrentState() const
{
	if (m_currentState == nullptr)
    {
		return new IdleState();
	}

    return m_currentState;
}

void dae::BlockComponent::SetCurrentState(BlockState* state)
{
    if (m_currentState != nullptr)
    {
        delete m_currentState;
    }

    m_currentState = state;
}

void dae::BlockComponent::CheckCollision(std::vector<GameObject*> otherObject)
{
    for (const auto& object : otherObject)
    {
        float enemyX = object->GetComponent<dae::TransformComponent>()->GetWorldPosition().x;
        float enemyY = object->GetComponent<dae::TransformComponent>()->GetWorldPosition().y;
        float enemyWidth = object->GetComponent<dae::CollisionComponent>()->GetBounds().x;
        float enemyHeight = object->GetComponent<dae::CollisionComponent>()->GetBounds().y;


        float thisX = GetOwner()->GetComponent<dae::TransformComponent>()->GetWorldPosition().x;
        float thisY = GetOwner()->GetComponent<dae::TransformComponent>()->GetWorldPosition().y;
        float thisWidth = GetOwner()->GetComponent<dae::CollisionComponent>()->GetBounds().x;
        float thisHeight = GetOwner()->GetComponent<dae::CollisionComponent>()->GetBounds().y;

        if (enemyX + enemyWidth > thisX && enemyX < thisX + thisWidth &&
            enemyY + enemyHeight > thisY && enemyY < thisY + thisHeight)
        {
            object->GetComponent<TransformComponent>()->BlockDirection(object->GetComponent<TransformComponent>()->GetLastMovementDirection());
        }
    }
}




void dae::BlockMovingState::Update(GameObject* block)
{
    glm::vec2 pos{ block->GetComponent<dae::TransformComponent>()->GetLocalPosition() };
	const auto blockComponent = block->GetComponent<BlockComponent>();
    if (blockComponent->IsPositionInsideWall(pos))
    {
		blockComponent->SetIsMoving(false);
		blockComponent->SetCurrentState(new IdleState());
        return;
    }


    if (blockComponent->GetDirection() == "Left")
    {
        pos.x -= 1.0f;
    }
    else if (blockComponent->GetDirection() == "Right")
    {
        pos.x += 1.0f;
    }
    else if (blockComponent->GetDirection() == "Up")
    {
        pos.y -= 1.0f;
    }
    else if (blockComponent->GetDirection() == "Down")
    {
        pos.y += 1.0f;
    }

    // Check if the new position is inside a wall
    if (blockComponent->IsPositionInsideWall(pos))
    {
        blockComponent->SetIsMoving(false);
        blockComponent->SetCurrentState(new IdleState());
        return;
    }

    block->GetComponent<dae::TransformComponent>()->SetLocalPosition(pos.x, pos.y);
}
