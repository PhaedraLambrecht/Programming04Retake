#include "Scene.h"
#include "GameObject.h"
#include "Componennts/CollisionComponent.h"
#include "Events/EventManager.h"
#include <algorithm>


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
	m_pObjects.clear();
	m_pObjectCollisions.clear();
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_pObjects.emplace_back(std::move(object));
	m_wasGameObjectAdded = true;
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), object), m_pObjects.end());
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
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

bool dae::Scene::IsActive()
{
	return m_isActive;
}

void dae::Scene::SetActive(bool isActive)
{
	m_isActive = isActive;
}

void dae::Scene::AddEnemy(std::shared_ptr<GameObject> enemy)
{
	m_pEnemies.emplace_back(std::move(enemy));
	m_EnemyLoaded = true;
}

void dae::Scene::AddPlayer(GameObject* player)
{
	m_pPlayer = player;
}

GameObject* dae::Scene::GetPlayer()
{
	return m_pPlayer;
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
			[&](std::shared_ptr<GameObject>& object)
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



