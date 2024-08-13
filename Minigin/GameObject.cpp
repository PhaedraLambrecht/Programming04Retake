#include "GameObject.h"
#include "Resources/ResourceManager.h"
#include "Base/Renderer.h"
#include "Events/EventManager.h"
#include "Componennts/CollisionComponent.h"
#include <iostream>


namespace dae
{
	GameObject::GameObject(int depthvalue)
		:m_pChildren{}
		,m_pParent{}
		,m_depthValue{depthvalue}
	{
		m_pTransform = AddComponent<TransformComponent>();
	}



	void GameObject::Update()
	{
		for (const auto& component : m_pComponents)
		{
			component->Update();
		}
	}

	void GameObject::FixedUpdate(const float fixedTimeStep)
	{		
		for (const auto& component : m_pComponents)
		{
			component->FixedUpdate(fixedTimeStep);
		}
	}

	void GameObject::Render() const
	{
		for (const auto& component : m_pComponents)
		{
			component->Render();
			component->RenderUI();
		}
	}

	void GameObject::SetParent(GameObject* parent, bool keepWorldPos)
	{
		if (!parent)
		{
			if (m_pTransform)
			{
				m_pTransform->SetLocalPosition(m_pTransform->GetWorldPosition().x, m_pTransform->GetWorldPosition().y);
			}
		}
		else
		{
			// Is the parent the same as this object?
			if (parent == this)
			{
				throw std::invalid_argument("Parent cant be the same as the object it is set to");

			}
			
			// Is the new parent an alrady existing child to this object?
			for (int i{ 0 }; i < GetChildCount(); ++i)
			{
				if (GetChildAt(i) == parent) 
				{
					throw std::invalid_argument("Parent cant be a previous child");
				}
			}


			// If neither of the above 2, set the transform
			if (keepWorldPos)
			{
				m_pTransform->SetLocalPosition(m_pTransform->GetLocalPosition().x - parent->GetComponent<dae::TransformComponent>()->GetWorldPosition().x,
											   m_pTransform->GetLocalPosition().y - parent->GetComponent<dae::TransformComponent>()->GetWorldPosition().y);
			}

			m_pTransform->SwitchDirtyFlag(true);
		}



		if (m_pParent != nullptr)
		{
			m_pParent->RemoveChild( this );
		}

		m_pParent = parent;

		if (m_pParent)
		{
			m_pParent->AddChild( this );
		}
	}

	GameObject* GameObject::GetParent() const
	{
		return m_pParent;
	}

	int GameObject::GetChildCount() const
	{
		return int(m_pChildren.size());
	}

	GameObject* GameObject::GetChildAt(int index) const
	{
		return m_pChildren[index];
	}

	std::vector<GameObject*> GameObject::GetChildren() const
	{
		return m_pChildren;
	}

	void GameObject::RemoveAllChildren()
	{
		m_pChildren.clear();
	}

	bool GameObject::IsReadyForDestruction()
	{
		return m_isMarkedForDestruction;
	}

	int GameObject::GetDrawDepth()
	{
		return m_depthValue;
	}

	void GameObject::MarkForDestruction()
	{
		std::unique_ptr<Event> event = std::make_unique<Event>();
		event->eventType = "GameObject Destroyed";

		EventManager::GetInstance().QueueEvent<dae::Event>(std::move(event));

		m_isMarkedForDestruction = true;
	}

	void GameObject::AddChild(GameObject* pChild)
	{
		const auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);

		if (it == m_pChildren.end())
		{
			m_pChildren.push_back( pChild);
		}
	}

	void GameObject::RemoveChild(GameObject* pChild)
	{
		m_pChildren.erase(std::remove(std::begin(m_pChildren), std::end(m_pChildren), pChild));
	}


}