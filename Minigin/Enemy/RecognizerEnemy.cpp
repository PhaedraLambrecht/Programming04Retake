#include "Enemy/RecognizerEnemy.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Events/EventManager.h"
#include "Commands/AddPointsCommand.h"
#include "Componennts/ImageComponent.h"
#include "Componennts/HealthComponent.h"
#include <iostream>
#include <algorithm>

#include "Commands/DamageCommand.h"

dae::RecognizerEnemy::RecognizerEnemy(GameObject* gameObject)
	: BaseEnemyComponent(gameObject)
	, m_Health{3}
	, m_EnemySpeed{ 25.f }
	, m_ChangeDirectionInterval{ 2.f }
	, m_TimeSinceLastChange{ 0.f }
{
	std::unique_ptr<Event> event = std::make_unique<Event>();
	event->eventType = "EnemySpawned";

	EventManager::GetInstance().QueueEvent(std::move(event));
}

dae::RecognizerEnemy::~RecognizerEnemy()
{
	std::cout << "RecognizerEnemy \n";
}

void dae::RecognizerEnemy::Initialize(float x, float y, float w, float h, std::vector<GameObject*> pPlayers)
{
	m_PositionSize.position = glm::vec2(x, y);
	m_PositionSize.width = w;
	m_PositionSize.height = h;

	m_pPlayer = pPlayers;

	ChangeDirection();
}



void dae::RecognizerEnemy::Update()
{
	if (m_pPlayer.empty())
	{
		return;
	}

	for (auto& player : m_pPlayer)
	{

		glm::vec2 playerPos = player->GetComponent<TransformComponent>()->GetWorldPosition();
		glm::vec2 enemyPos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
		glm::vec2 playerSize = player->GetComponent<ImageComponent>()->GetTextureDimensions();
		glm::vec2 enemySize = GetOwner()->GetComponent<ImageComponent>()->GetTextureDimensions();

	
		// Detect overlap
		if (enemyPos.x < playerPos.x + playerSize.x &&
			enemyPos.x + enemySize.x > playerPos.x &&
			enemyPos.y < playerPos.y + playerSize.y &&
			enemyPos.y + enemySize.y > playerPos.y)
		{
			if (m_AttackCooldown <= 0.0f)
			{
				dae::DamageCommand damageCommand(player, 1);
				damageCommand.Execute();
				m_AttackCooldown = m_DamageInterval; // Reset cooldown timer to 10 seconds
			}
		}
	

	
	
	}

	m_AttackCooldown -= GameTime::GetInstance().GetDeltaTime();
	
	float deltaTime = GameTime::GetInstance().GetDeltaTime();
	m_TimeSinceLastChange += deltaTime;
	if (m_TimeSinceLastChange >= m_ChangeDirectionInterval)
	{
		ChangeDirection();
		m_TimeSinceLastChange = 0.0f;
	}

	// Normalize the direction vector
	//glm::vec2 direction = glm::normalize(playerPos - enemyPos);
	//glm::vec2 movement = direction * m_EnemySpeed * deltaTime;
	//move(deltaTime, (int)movement.x, (int)movement.y);


	// Handle blocked movements
	HandleBlockedMovement(deltaTime);


}

bool dae::RecognizerEnemy::DoDamage()
{
	--m_Health;
	if (m_Health > 0)
	{
		return false;
	}


	AddPointsAndNotifyDeath();
	return true;
}

void dae::RecognizerEnemy::move(float deltaTime, int x, int y)
{
	auto transform = GetOwner()->GetComponent<TransformComponent>();
	glm::vec2 newPos = transform->GetWorldPosition() + glm::vec2(x, y) * (deltaTime * m_EnemySpeed);

	newPos.x = std::clamp( newPos.x, m_xPos, m_windowWidth - (GetOwner()->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x * 2) );
	newPos.y = std::clamp( newPos.y, m_yPos, m_windowHeight - (GetOwner()->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y * 10) );
	transform->SetLocalPosition(newPos.x, newPos.y);
}

void dae::RecognizerEnemy::HandleBlockedMovement(float deltaTime)
{
	bool blockedDirection{ false };
	if (m_MovementFlags.left && !m_MovementFlags.right)
	{
		GetOwner()->GetComponent<TransformComponent>()->SetLastMovementDirection("Left");

		blockedDirection = !GetOwner()->GetComponent<TransformComponent>()->IsDirectionBlocked(GetOwner()->GetComponent<TransformComponent>()->GetLastMovementDirection());
		if (blockedDirection)
		{
			move(deltaTime, -1, 0);
		}
		else
		{
			GetOwner()->GetComponent<TransformComponent>()->ResetBlockedDirections();
		}
	}
	else if (m_MovementFlags.right && !m_MovementFlags.left)
	{
		GetOwner()->GetComponent<TransformComponent>()->SetLastMovementDirection("Right");
		
		blockedDirection = !GetOwner()->GetComponent<TransformComponent>()->IsDirectionBlocked(GetOwner()->GetComponent<TransformComponent>()->GetLastMovementDirection());
		if (blockedDirection)
		{
			move(deltaTime, 1, 0);
		}
		else
		{
			GetOwner()->GetComponent<TransformComponent>()->ResetBlockedDirections();
		}

	}
	else if (m_MovementFlags.up && !m_MovementFlags.down)
	{
		GetOwner()->GetComponent<TransformComponent>()->SetLastMovementDirection("Up");

		blockedDirection = !GetOwner()->GetComponent<TransformComponent>()->IsDirectionBlocked(GetOwner()->GetComponent<TransformComponent>()->GetLastMovementDirection());
		if (blockedDirection)
		{
			move(deltaTime, 0, -1);
		}
		else
		{
			GetOwner()->GetComponent<TransformComponent>()->ResetBlockedDirections();
		}
	}
	else if (m_MovementFlags.down && !m_MovementFlags.up)
	{
		GetOwner()->GetComponent<TransformComponent>()->SetLastMovementDirection("Down");

		blockedDirection = !GetOwner()->GetComponent<TransformComponent>()->IsDirectionBlocked(GetOwner()->GetComponent<TransformComponent>()->GetLastMovementDirection());
		if (blockedDirection)
		{
			move(deltaTime, 0, 1);
		}
		else
		{
			GetOwner()->GetComponent<TransformComponent>()->ResetBlockedDirections();
		}
	}
}


void dae::RecognizerEnemy::ChangeDirection()
{
	int random = rand() % 4;

	// Reset all movement flags
	m_MovementFlags.left = false;
	m_MovementFlags.right = false;
	m_MovementFlags.up = false;
	m_MovementFlags.down = false;

	switch (random)
	{
	case 0:
		m_MovementFlags.left = true;
		break;
	case 1:
		m_MovementFlags.right = true;
		break;
	case 2:
		m_MovementFlags.up = true;
		break;
	case 3:
		m_MovementFlags.down = true;
		break;
	}
}


void dae::RecognizerEnemy::AddPointsAndNotifyDeath()
{
	dae::AddPointsCommand addPointsCommand(m_pPlayer[0]);
	addPointsCommand.Execute();


	Event event;
	event.eventType = "EnemyDeath";

	GetOwner()->MarkForDestruction();
}


void dae::RecognizerEnemy::OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject)
{
	if (hitObject.ownerType != "PlayerAttack")
		return;

	DoDamage();



	glm::vec2 enemyPos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
	glm::vec2 collsionBounds = collisionOwner.owningObject->GetComponent<CollisionComponent>()->GetBounds();
	glm::vec2 collisionPos = collisionOwner.owningObject->GetComponent<TransformComponent>()->GetWorldPosition();


	if (IsBlockingLeft(enemyPos, collisionPos, collsionBounds))
	{
		m_MovementFlags.left = true;
	}
	if (IsBlockingRight(enemyPos, collisionPos, collsionBounds))
	{
		m_MovementFlags.right = true;
	}
	if (IsBlockingDown(enemyPos, collisionPos, collsionBounds))
	{
		m_MovementFlags.down = true;
	}

	if (IsBlockingUp(enemyPos, collisionPos, collsionBounds))
	{
		m_MovementFlags.up = true;
	}
}

bool dae::RecognizerEnemy::IsBlockingLeft(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds) const
{
	return collisionPos.x + collisionBounds.y > enemyPos.x && // right edge of collision
		collisionPos.x < enemyPos.x && // left edge of collision
		collisionPos.y < enemyPos.y + m_PositionSize.height && // bottom edge of collision
		collisionPos.y + collisionBounds.x > enemyPos.y; // top edge of collision

}

bool dae::RecognizerEnemy::IsBlockingRight(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds) const
{
	return collisionPos.x < enemyPos.x + m_PositionSize.width && // left edge of collision
		collisionPos.x + collisionBounds.y > enemyPos.x + m_PositionSize.width && // right edge of collision
		collisionPos.y < enemyPos.y + m_PositionSize.height && // bottom edge of collision
		collisionPos.y + collisionBounds.x > enemyPos.y; // top edge of collision
}

bool dae::RecognizerEnemy::IsBlockingDown(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds) const
{
	return collisionPos.y + collisionBounds.x > enemyPos.y && // top edge of collision
		collisionPos.y < enemyPos.y && // bottom edge of collision
		collisionPos.x < enemyPos.x + m_PositionSize.width && // left edge of collision
		collisionPos.x + collisionBounds.y > enemyPos.x; // right edge of collision
}

bool dae::RecognizerEnemy::IsBlockingUp(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds) const
{
	return collisionPos.y < enemyPos.y + m_PositionSize.height && // bottom edge of collision
		collisionPos.y + collisionBounds.y > enemyPos.y + m_PositionSize.height && // top edge of collision
		collisionPos.x < enemyPos.x + m_PositionSize.width && // left edge of collision
		collisionPos.x + collisionBounds.x > enemyPos.x; // right edge of collision
}



void dae::RecognizerEnemy::OnDeath(const Event* e)
{
	if (strcmp(e->eventType, "EnemyDeath") != 0)
		return;
}

void dae::RecognizerEnemy::SetWindowDimensions(float xPos, float yPos,  float width, float height)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_windowWidth = width;
	m_windowHeight = height;
}

