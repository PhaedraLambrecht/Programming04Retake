#include "Scene.h"
#include "Scene.h"
#include "GameObject.h"
#include "Componennts/CollisionComponent.h"
#include "Events/EventManager.h"
#include <algorithm>
#include <iostream>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) 
	:m_name(name)
	, m_pObjects{}
	,m_wasGameObjectAdded{false}
	,m_wasObjectDestroyed{false}
	, m_destructionEventString{"GameObject Destroyed"}
	, m_isActive{ false }
{
	Event destroyed;
	destroyed.eventType = m_destructionEventString.c_str();
	auto boundGameObjectDestroyed = std::bind(&Scene::HandleDestroyedEvent, this, std::placeholders::_1);
	EventManager::GetInstance().RegisterObserver(destroyed, boundGameObjectDestroyed);
}

Scene::~Scene()
{
	std::cout << "Scene Destructor" << std::endl;
	m_pObjectCollisions.clear();
	m_pObjects.clear();

	std::cout << "Scene Destructor Done" << std::endl;
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_pObjects.emplace_back(std::move(object));
	m_wasGameObjectAdded = true;
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	std::cout << "Remove Object" << std::endl;
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), object), m_pObjects.end());
}

void Scene::RemoveAll()
{
	std::cout << "Remove All Objects" << std::endl;
	m_pObjects.clear();
	std::cout << "Remove All Objects Done" << std::endl;
}


void Scene::Update()
{
	DepthSortGameObjects();

	//Normal Update
	for (auto& object : m_pObjects)
	{
		if (!object->IsReadyForDestruction())
		{
			object->Update();
		}
	}

	RemoveDestroyedObjects();


	//Collision Checks
	for (auto& objectCollision : m_pObjectCollisions)
	{
		if (objectCollision->IsActive())
			objectCollision->IsOverlappingOtherCollision(m_pObjectCollisions);
	}
}

void dae::Scene::FixedUpdate(const float fixedTimeStep)
{
	for (auto& object : m_pObjects)
	{
		if (!object->IsReadyForDestruction())
		{
			object->FixedUpdate(fixedTimeStep);
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}


void dae::Scene::AddCollision(CollisionComponent* collision)
{
	m_pObjectCollisions.emplace_back(collision);
}

void dae::Scene::RemoveCollision(CollisionComponent* collision)
{
	auto it = std::find(m_pObjectCollisions.begin(), m_pObjectCollisions.end(), collision);

	if (it != m_pObjectCollisions.end())
		m_pObjectCollisions.erase(it);
}

dae::CollisionComponent* dae::Scene::GetCollisionAt(const glm::vec2& position)
{


	for (const auto& colision : m_pObjectCollisions)
	{
		if (colision->GetCollisionData().ownerType != "World")
		{
			if (colision->GetCollisionData().ownerType != "Player")
			{
				// Check if the object's collision bounds contain the given position
				glm::vec2 objPosition = colision->GetCollisionData().owningObject->GetComponent<TransformComponent>()->GetWorldPosition();
				glm::vec2 objSize = colision->GetBounds();

				if (position.x >= objPosition.x && position.x <= objPosition.x + objSize.x &&
					position.y >= objPosition.y && position.y <= objPosition.y + objSize.y)
				{
					// Return the CollisionComponent if there's a collision
					return colision;
				}
			}
		}
	}


	// Return nullptr if no collision is found
	return nullptr;
}

bool dae::Scene::IsActive()
{
	return m_isActive;
}

void dae::Scene::SetActive(bool isActive)
{
	m_isActive = isActive;
}

void dae::Scene::AddEnemy(GameObject* enemy)
{
	m_pEnemies.push_back(enemy);
	m_EnemyLoaded = true;
}

std::vector<GameObject*> dae::Scene::GetEnemy()
{
	std::vector<GameObject*> enemies;
	for (const auto& enemy : m_pEnemies)
	{
		enemies.push_back(enemy);
	}

	return enemies;
}

bool dae::Scene::EnemyLoaded()
{
	return m_EnemyLoaded;
}

void dae::Scene::SetEnemyLoaded(bool loaded)
{
	m_EnemyLoaded = loaded;
}

bool dae::Scene::IsEnemiesEmpty()
{
	return m_NoEnemies;
}

void dae::Scene::SetIsEnemiesEmpty(bool noEnemies)
{
	m_NoEnemies = noEnemies;
}

void dae::Scene::AddPlayer(GameObject* player)
{
	m_pPlayer.push_back(player);
}

GameObject* dae::Scene::GetPlayer(int id)
{
	return m_pPlayer[id];
}

std::vector<GameObject*> dae::Scene::GetPlayers()
{
	std::vector<GameObject*> players;
	for (const auto& player : m_pPlayer)
	{
		players.push_back(player);
	}

	return players;
}

void dae::Scene::SetPlayerName(const std::string& name)
{
	m_playerName = name;
}

std::string dae::Scene::GetPlayerName()
{
	return m_playerName;
}

void dae::Scene::AddWalls(GameObject* walls)
{
	m_pWalls.push_back(walls);
}

std::vector<GameObject*> dae::Scene::GetWalls()
{
	std::vector<GameObject*> walls;
	for (const auto& wal : m_pWalls)
	{
		if (wal != nullptr)
		{
			walls.push_back(wal);
		}
	}

	return walls;
}

const std::string& dae::Scene::GetName() const
{
	return m_name;
}


void dae::Scene::DepthSortGameObjects()
{
	if (!m_wasGameObjectAdded)
		return;

	// Sort the objects based on their draw depth
	std::sort
	(
		m_pObjects.begin(),
		m_pObjects.end(),
		[](const auto& object1,
			const auto& object2) {return object1->GetDrawDepth() < object2->GetDrawDepth(); }
	);

	// Reset the flag indicating a game object was added
	m_wasGameObjectAdded = false;
}

void dae::Scene::RemoveDestroyedObjects()
{
	if (m_wasObjectDestroyed)
		return;

	m_pObjects.erase
	(
		std::remove_if
		(
			m_pObjects.begin(), 
			m_pObjects.end(),
			[&](std::shared_ptr<GameObject>& object)
			{
				return object->IsReadyForDestruction();
			})
			, m_pObjects.end()
	);


	m_pEnemies.erase
	(
		std::remove_if
		(
			m_pEnemies.begin(),
			m_pEnemies.end(),
			[&](GameObject* object)
			{
				return object->IsReadyForDestruction();
			})
		, m_pEnemies.end()
	);


	if (m_pEnemies.empty() && m_EnemyLoaded)
	{
		m_NoEnemies = true;
	}

	m_wasObjectDestroyed = false;
}

void dae::Scene::HandleDestroyedEvent(const Event* e)
{
	if (std::string(e->eventType) == m_destructionEventString)
	{
		m_wasObjectDestroyed = true;
	}
}



