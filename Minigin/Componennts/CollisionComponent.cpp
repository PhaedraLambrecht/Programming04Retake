#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "Base/Renderer.h"
#include "GameObject.h"

#include <iostream>
#include <cassert>

dae::CollisionComponent::CollisionComponent(GameObject* Owner)
	:BaseComponent(Owner)
	, m_width{}
	, m_height{}
	, m_CollisionData{}
	, m_OnHitCallback{}
	, m_pScene{}
	, m_IsActive{ true }
{
	m_pTransform = GetOwner()->GetComponent<TransformComponent>();
	if (!m_pTransform)
	{
		throw std::invalid_argument("TextRendererComponent needs a TransformComponent");
	}
}

dae::CollisionComponent::~CollisionComponent()
{
	assert(m_pScene && "m_pScene must be set in collision Component");
	m_pScene->RemoveCollision(this);


	std::cout << "CollisionComponent\n";
}


dae::CollisionData dae::CollisionComponent::GetCollisionData() const
{
	return m_CollisionData;
}

void dae::CollisionComponent::SetCollisionData(dae::CollisionData data)
{
	m_CollisionData = data;
}

void dae::CollisionComponent::SetCallback(std::function<void(const CollisionData& owner, const CollisionData& hitObject)> callbackFunc)
{
	m_OnHitCallback = callbackFunc;
}

void dae::CollisionComponent::SetScene(Scene* scene)
{
	m_pScene = scene;
}

void dae::CollisionComponent::IsOverlappingOtherCollision(const std::vector<CollisionComponent*>& collisionsToCheck) const
{
	GameObject* selfCollisionComponent = GetOwner();
	for (const auto collisionToCheck : collisionsToCheck)
	{
		if (selfCollisionComponent == collisionToCheck->GetOwner())
			continue;

		if (!CheckOverlapp(collisionToCheck) || !m_OnHitCallback)
			continue;

		m_OnHitCallback(m_CollisionData, collisionToCheck->m_CollisionData);
	}
}

void dae::CollisionComponent::SetBounds(float width, float height)
{
	m_width = width;
	m_height = height;
}

void dae::CollisionComponent::Render() const
{
	TransformComponent* pTransform = GetOwner()->GetComponent<TransformComponent>();


	Renderer::GetInstance().RenderRectEdges(pTransform->GetWorldPosition().x, pTransform->GetWorldPosition().y, m_width, m_height, { 255, 0, 0, 255 });
}


bool dae::CollisionComponent::CheckOverlapp(CollisionComponent* pOther) const
{
	if (!m_pTransform || !pOther->m_pTransform)
		return false;


	const glm::vec2 worldPos = m_pTransform->GetWorldPosition();
	const  glm::vec2 otherWorldPos = pOther->m_pTransform->GetWorldPosition();


	// Get world positions and dimensions
	const glm::vec2& pos = m_pTransform->GetWorldPosition();
	const glm::vec2& otherPos = pOther->m_pTransform->GetWorldPosition();
	const glm::vec2 size(m_width, m_height);
	const glm::vec2 otherSize(pOther->m_width, pOther->m_height);

	// Calculate bounding box corners
	const glm::vec2& bottomRight = pos + size;
	const glm::vec2& otherBottomRight = otherPos + otherSize;

	// Check for overlap in both X and Y axes
	bool overlapX = pos.x < otherBottomRight.x && bottomRight.x > otherPos.x;
	bool overlapY = pos.y < otherBottomRight.y && bottomRight.y > otherPos.y;

	// Return true if overlaps in both axes
	return overlapX && overlapY;
}
