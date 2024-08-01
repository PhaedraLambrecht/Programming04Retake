#ifndef ENGINE2D_SCENE_H
#define ENGINE2D_SCENE_H
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class CollisionComponent;
	struct Event;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	
	public:

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;




		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		const std::string& GetName() const;


		void Update();
		void FixedUpdate(const float fixedTimeStep);
		void Render() const;

		void AddCollision(CollisionComponent* collision);
		void RemoveCollision(CollisionComponent* collision);

		bool IsActive();
		void SetActive(bool isActive);


	private: 
	
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < std::shared_ptr<GameObject>> m_Objects;
		std::vector <CollisionComponent*> m_ObjectCollisions;

	
		void DepthSortGameObjects();
		void RemoveDestroyedObjects();

		void HandleDestroyedEvent(const Event* e);

		bool m_IsActive;

		static unsigned int m_IdCounter;
		bool m_WasGameObjectAdded;
		bool m_WasObjectDestroyed;

		const std::string m_DestructionEventString;
	};

}
#endif // ENGINE2D_SCENE_H