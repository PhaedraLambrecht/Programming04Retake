#ifndef ENGINE2D_SCENE_H
#define ENGINE2D_SCENE_H
#include "SceneManager.h"
#include "glm/glm.hpp"
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
		CollisionComponent* GetCollisionAt(const glm::vec2& position);

		bool IsActive();
		void SetActive(bool isActive);

		void AddEnemy(std::shared_ptr<GameObject> enemy);
		void AddPlayer(GameObject* player);
		GameObject* GetPlayer();
		bool m_NoEnemies = false;
		bool m_EnemyLoaded = false;


	private: 
	
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_pObjects;
		std::vector <CollisionComponent*> m_pObjectCollisions;

	
		void DepthSortGameObjects();
		void RemoveDestroyedObjects();

		void HandleDestroyedEvent(const Event* e);

		bool m_isActive;

		static unsigned int m_idCounter;
		bool m_wasGameObjectAdded;
		bool m_wasObjectDestroyed;

		const std::string m_destructionEventString;

		std::vector<std::shared_ptr<GameObject>> m_pEnemies;
		GameObject* m_pPlayer;
	};

}
#endif // ENGINE2D_SCENE_H