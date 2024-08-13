#include "RecognizerEnemy.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Events/EventManager.h"
#include "Commands/AddPointsCommand.h"
#include "Componennts/ImageComponent.h"
#include "Componennts/HealthComponent.h"
#include "Commands/DamageCommand.h"
#include "BlockComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include <iostream>
#include <algorithm>


dae::RecognizerEnemy::RecognizerEnemy(GameObject* gameObject)
	: BaseEnemyComponent(gameObject)
	, m_Health{ 3 }
	, m_EnemySpeed{ 25.f }
	, m_ChangeDirectionInterval{ 2.f }
	, m_TimeSinceLastChange{ 0.f }
{
	currentState = new MovingState(GetOwner());

	std::unique_ptr<Event> event = std::make_unique<Event>();
	event->eventType = "EnemySpawned";

	EventManager::GetInstance().QueueEvent(std::move(event));
}

dae::RecognizerEnemy::~RecognizerEnemy()
{
	delete currentState;
	currentState = nullptr;
}


void dae::RecognizerEnemy::Initialize(float x, float y, float w, float h, std::vector<GameObject*> pPlayers)
{
	m_PositionSize.position = glm::vec2(x, y);
	m_PositionSize.width = w;
	m_PositionSize.height = h;

	m_pPlayer = pPlayers;
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
	currentState->Update();


	OverlappingWall();
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

	IsBlocking(enemyPos, collisionPos, collsionBounds);
}

void dae::RecognizerEnemy::IsBlocking(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds)
{
	// LeftCHeck
	if (
		collisionPos.x + collisionBounds.y > enemyPos.x && // right edge of collision
		collisionPos.x < enemyPos.x && // left edge of collision
		collisionPos.y < enemyPos.y + m_PositionSize.height && // bottom edge of collision
		collisionPos.y + collisionBounds.x > enemyPos.y // top edge of collision
		)
	{
		m_MovementFlags.left = true;
	}


	// RightCheck
	if (
		collisionPos.x < enemyPos.x + m_PositionSize.width && // left edge of collision
		collisionPos.x + collisionBounds.y > enemyPos.x + m_PositionSize.width && // right edge of collision
		collisionPos.y < enemyPos.y + m_PositionSize.height && // bottom edge of collision
		collisionPos.y + collisionBounds.x > enemyPos.y // top edge of collision
		)
	{
		m_MovementFlags.right = true;
	}


	// UpCheck
	if (
		collisionPos.y + collisionBounds.x > enemyPos.y && // top edge of collision
		collisionPos.y < enemyPos.y && // bottom edge of collision
		collisionPos.x < enemyPos.x + m_PositionSize.width && // left edge of collision
		collisionPos.x + collisionBounds.y > enemyPos.x // right edge of collision
		)
	{
		m_MovementFlags.down = true;
	}


	// DownCheck
	if (
		collisionPos.y < enemyPos.y + m_PositionSize.height && // bottom edge of collision
		collisionPos.y + collisionBounds.y > enemyPos.y + m_PositionSize.height && // top edge of collision
		collisionPos.x < enemyPos.x + m_PositionSize.width && // left edge of collision
		collisionPos.x + collisionBounds.x > enemyPos.x // right edge of collision
		)
	{
		m_MovementFlags.up = true;
	}
}

void dae::RecognizerEnemy::OverlappingWall()
{
	std::vector<GameObject*> blocks = SceneManager::GetInstance().GetActiveScene().GetWalls();
	for (auto& block : blocks)
	{
		if (block->GetComponent<dae::BlockComponent>()->GetIsMoving())
		{
			glm::vec2 blockPos = block->GetComponent<TransformComponent>()->GetWorldPosition();
			glm::vec2 blockSize = block->GetComponent<ImageComponent>()->GetTextureDimensions();
			glm::vec2 enemyPos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
			glm::vec2 enemySize = GetOwner()->GetComponent<ImageComponent>()->GetTextureDimensions();

			if (enemyPos.x < blockPos.x + blockSize.x &&
				enemyPos.x + enemySize.x > blockPos.x &&
				enemyPos.y < blockPos.y + blockSize.y &&
				enemyPos.y + enemySize.y > blockPos.y)
			{
				DoDamage(); // kill the enemy
			}
		}
	}
}


void dae::RecognizerEnemy::OnDeath(const Event* e)
{
	if (strcmp(e->eventType, "EnemyDeath") != 0)
		return;
}

void dae::RecognizerEnemy::SetWindowDimensions(float xPos, float yPos, float width, float height)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_windowWidth = width;
	m_windowHeight = height;
}

float dae::RecognizerEnemy::GetTimeSinceLastChange() const
{
	return m_TimeSinceLastChange;
}

void dae::RecognizerEnemy::SetTimeSinceLastChange(float time)
{
	m_TimeSinceLastChange = time;
}

float dae::RecognizerEnemy::GetDirectionInterval() const
{
	return m_ChangeDirectionInterval;
}

float dae::RecognizerEnemy::GetEnemySpeed() const
{
	return m_EnemySpeed;
}

float dae::RecognizerEnemy::GetXPos() const
{
	return m_xPos;
}

float dae::RecognizerEnemy::GetYPos() const
{
	return m_yPos;
}






// base
dae::State::State(GameObject* enemy)
	: m_enemy{ enemy }
{
}

dae::State::~State()
{
}


// MovingState
void dae::MovingState::Update()
{
	RecognizerEnemy* enemy = m_enemy->GetComponent<RecognizerEnemy>();
	float deltaTime = GameTime::GetInstance().GetDeltaTime();
	enemy->SetTimeSinceLastChange(enemy->GetTimeSinceLastChange() + deltaTime);


	if (enemy->GetTimeSinceLastChange() >= enemy->GetDirectionInterval())
	{
		ChangeDirection();
		enemy->SetTimeSinceLastChange(0.0f);
	}


	// Handle blocked movements
	HandleBlockedMovement(deltaTime);

}

void dae::MovingState::move(float deltaTime, int x, int y)
{
	RecognizerEnemy* enemy = m_enemy->GetComponent<RecognizerEnemy>();

	auto transform = m_enemy->GetComponent<TransformComponent>();
	glm::vec2 newPos = transform->GetWorldPosition() + glm::vec2(x, y) * (deltaTime * enemy->GetEnemySpeed());

	newPos.x = std::clamp(newPos.x, enemy->GetXPos(), enemy->m_windowHeight - (m_enemy->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x * 2));
	newPos.y = std::clamp(newPos.y, enemy->GetYPos(), enemy->m_windowHeight - (m_enemy->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y * 10));
	transform->SetLocalPosition(newPos.x, newPos.y);
}

void dae::MovingState::HandleBlockedMovement(float deltaTime)
{
	RecognizerEnemy* enemy = m_enemy->GetComponent<RecognizerEnemy>();
	bool blockedDirection{ false };
	if (enemy->m_MovementFlags.left && !enemy->m_MovementFlags.right)
	{

		m_enemy->GetComponent<TransformComponent>()->SetLastMovementDirection("Left");

		blockedDirection = !m_enemy->GetComponent<TransformComponent>()->IsDirectionBlocked(m_enemy->GetComponent<TransformComponent>()->GetLastMovementDirection());
		if (blockedDirection)
		{
			move(deltaTime, -1, 0);
		}
		else
		{
			m_enemy->GetComponent<TransformComponent>()->ResetBlockedDirections();
		}
	}
	else if (enemy->m_MovementFlags.right && !enemy->m_MovementFlags.left)
	{
		m_enemy->GetComponent<TransformComponent>()->SetLastMovementDirection("Right");

		blockedDirection = !m_enemy->GetComponent<TransformComponent>()->IsDirectionBlocked(m_enemy->GetComponent<TransformComponent>()->GetLastMovementDirection());
		if (blockedDirection)
		{
			move(deltaTime, 1, 0);
		}
		else
		{
			m_enemy->GetComponent<TransformComponent>()->ResetBlockedDirections();
		}

	}
	else if (enemy->m_MovementFlags.up && !enemy->m_MovementFlags.down)
	{
		m_enemy->GetComponent<TransformComponent>()->SetLastMovementDirection("Up");

		blockedDirection = !m_enemy->GetComponent<TransformComponent>()->IsDirectionBlocked(m_enemy->GetComponent<TransformComponent>()->GetLastMovementDirection());
		if (blockedDirection)
		{
			move(deltaTime, 0, -1);
		}
		else
		{
			m_enemy->GetComponent<TransformComponent>()->ResetBlockedDirections();
		}
	}
	else if (enemy->m_MovementFlags.down && !enemy->m_MovementFlags.up)
	{
		m_enemy->GetComponent<TransformComponent>()->SetLastMovementDirection("Down");

		blockedDirection = !m_enemy->GetComponent<TransformComponent>()->IsDirectionBlocked(m_enemy->GetComponent<TransformComponent>()->GetLastMovementDirection());
		if (blockedDirection)
		{
			move(deltaTime, 0, 1);
		}
		else
		{
			m_enemy->GetComponent<TransformComponent>()->ResetBlockedDirections();
		}
	}
}

void dae::MovingState::ChangeDirection()
{
	RecognizerEnemy* enemy = m_enemy->GetComponent<RecognizerEnemy>();

	int random = rand() % 4;

	// Reset all movement flags
	enemy->m_MovementFlags.left = false;
	enemy->m_MovementFlags.right = false;
	enemy->m_MovementFlags.up = false;
	enemy->m_MovementFlags.down = false;

	switch (random)
	{
	case 0:
		enemy->m_MovementFlags.left = true;
		break;
	case 1:
		enemy->m_MovementFlags.right = true;
		break;
	case 2:
		enemy->m_MovementFlags.up = true;
		break;
	case 3:
		enemy->m_MovementFlags.down = true;
		break;
	}
}