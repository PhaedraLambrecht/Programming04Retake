#include "TransformComponent.h"
#include "GameObject.h"
#include <iostream>


namespace dae
{
	TransformComponent::TransformComponent(GameObject* Owner)
		:BaseComponent(Owner)
		,m_LocalPosition{}
		,m_WorldPosition{}
		,m_IsDirty{false}
	{
	}

	TransformComponent::~TransformComponent()
	{

	}


	const glm::vec2& TransformComponent::GetLocalPosition() const
	{
		return m_LocalPosition;
	}
	
	const glm::vec2& TransformComponent::GetWorldPosition()
	{
		if (m_IsDirty)
		{
			SetWorldPosition();
		}
		
		return m_WorldPosition;
	}
	
	void TransformComponent::SetLocalPosition(float x, float y)
	{
		m_LocalPosition.x = x;
		m_LocalPosition.y = y;

		SwitchDirtyFlag(true);
	}

	void TransformComponent::SwitchDirtyFlag(bool IsFlagged)
	{
		m_IsDirty = IsFlagged;
	}

	void TransformComponent::SetLastMovementDirection(const std::string& direction)
	{
		m_LastMovementDirection = direction;
	}

	const std::string TransformComponent::GetLastMovementDirection() const
	{
		return m_LastMovementDirection;
	}



	void TransformComponent::BlockDirection(const std::string& direction)
	{
		m_BlockedDirections[direction] = true;
 	}

	bool TransformComponent::IsDirectionBlocked(const std::string& direction) const
	{
		auto it = m_BlockedDirections.find(direction);
		if (it != m_BlockedDirections.end())
		{
			return it->second;
		}
		return false;
	}

	void TransformComponent::ResetBlockedDirections()
	{
		m_BlockedDirections.clear();
	}




	void TransformComponent::SetWorldPosition()
	{
		const auto pParent{ GetOwner()->GetParent()};


		// If no parent exists, use the local position as world position
		if (!pParent)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			// Calculate the world position using the position of the parent
			const auto parentTransform{ pParent->GetComponent<TransformComponent>()->GetWorldPosition() + m_LocalPosition};
			m_WorldPosition = parentTransform;
		}

		SwitchDirtyFlag(false);
	}

}

