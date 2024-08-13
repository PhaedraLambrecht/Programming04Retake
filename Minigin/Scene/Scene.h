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



		// Adding/Removing objects
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();


		// Updates/Rendering
		void Update();
		void FixedUpdate(const float fixedTimeStep);
		void Render() const;


		// Collision
		void AddCollision(CollisionComponent* collision);
		void RemoveCollision(CollisionComponent* collision);
		CollisionComponent* GetCollisionAt(const glm::vec2& position);

		// Scene data
		bool IsActive();
		void SetActive(bool isActive);
		const std::string& GetName() const;


		// Enemies/Players/Walls
		void AddEnemy(GameObject* enemy);
		std::vector<GameObject*> GetEnemy();
	
		bool EnemyLoaded();
		void SetEnemyLoaded(bool loaded);
		bool IsEnemiesEmpty();
		void SetIsEnemiesEmpty(bool noEnemies);
		


		void AddPlayer(GameObject* player);
		GameObject* GetPlayer(int id);
		std::vector<GameObject*> GetPlayers();
		void SetPlayerName(const std::string& name);
		std::string GetPlayerName();


		void AddWalls(GameObject* walls);
		std::vector<GameObject*> GetWalls();


	private: 
	
		explicit Scene(const std::string& name);

		// Scene data
		std::string m_name;
		bool m_isActive;

		// Objects
		std::vector < std::shared_ptr<GameObject>> m_pObjects;
		std::vector <CollisionComponent*> m_pObjectCollisions;


		static unsigned int m_idCounter;
		bool m_wasGameObjectAdded;
		bool m_wasObjectDestroyed;

		const std::string m_destructionEventString;
	
		// Enemies/Players/Walls
		std::vector<GameObject*> m_pEnemies;
		std::vector<GameObject*> m_pWalls;
		std::vector<GameObject*> m_pPlayer;

		bool m_NoEnemies{false};
		bool m_EnemyLoaded{false};
		std::string m_playerName{};



		void DepthSortGameObjects();
		void RemoveDestroyedObjects();

		void HandleDestroyedEvent(const Event* e);
	};

}
#endif // ENGINE2D_SCENE_H