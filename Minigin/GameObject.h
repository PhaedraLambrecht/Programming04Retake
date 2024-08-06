#ifndef ENGINE2D_GAMEOBJECT_H
#define ENGINE2D_GAMEOBJECT_H
#include "Componennts/TransformComponent.h"
#include <memory>
#include <vector>
#include <algorithm>


namespace dae
{
	class BaseComponent;
	class TransformComponent;
	class CollisionComponent;

	class GameObject final
	{
	public:

		GameObject(int depthvalue = 0);
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;



		void Update();
		void FixedUpdate(const float fixedTimeStep);
		void Render() const;

		template <typename T> T* AddComponent();
		template <typename T> T* GetComponent() const;
		template <typename T> bool HasComponent() const;
		template <typename T> void RemoveComponent();


		// Scenegraph
		void SetParent(GameObject* parent, bool keepWorldPos);
		GameObject* GetParent() const;

		int GetChildCount() const;
		GameObject* GetChildAt(int index) const;
		std::vector<GameObject*> GetChildren() const;
		void RemoveAllChildren();

		bool IsReadyForDestruction();
		int GetDrawDepth();
		void MarkForDestruction();


	private:

		std::vector<std::unique_ptr<BaseComponent>> m_pComponents;
		TransformComponent* m_pTransform;

		// Scenegraph
		std::vector<GameObject*> m_pChildren;
		GameObject* m_pParent;
	

		bool m_isMarkedForDestruction{ false };
		int m_depthValue{ 0 };


		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* child);

	};




	template<typename T>
	inline T* GameObject::AddComponent()
	{
		// Ensure that the template argument is derived from BaseComponent
		static_assert(std::is_base_of<BaseComponent, T>(), "Provided template argument is not a component");


		auto newComponent = std::make_unique<T>(this); // Create a new component of the specified type
		T* rawPointer = newComponent.get();	// Get a raw pointer to the newly created component


		m_pComponents.push_back(std::move(newComponent));
		return rawPointer;
	}

	template<typename T>
	inline T* GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<BaseComponent, T>::value, "Provided template argument is not a component");


		// Use std::find_if with a lambda function to search for the component
		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const auto& component) 
			{
				// Check if the component is of the specified type & return this awnser
				return dynamic_cast<T*>(component.get()) != nullptr;
			});


		if (it != m_pComponents.end()) 
		{
			return dynamic_cast<T*>(it->get());
		}


		return nullptr;
	}

	template<typename T>
	inline bool GameObject::HasComponent() const
	{
		const auto components = GetComponent<T>();

		if (components != nullptr)
		{
			return true;
		}
		return false;

	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<BaseComponent, T>::value, "Provided template argument is not a component");


		// Use iterator to efficiently find and erase the component
		auto it = std::remove_if(m_pComponents.begin(), m_pComponents.end(), [](const auto& component) 
			{
				// Check if the component is of the specified type & return this awnser
				return dynamic_cast<T*>(component.get()) != nullptr;
			});


		// Erase the removed components from the vector
		m_pComponents.erase(it, m_pComponents.end());
	}

}

#endif // !ENGINE2D_GAMEOBJECT_H