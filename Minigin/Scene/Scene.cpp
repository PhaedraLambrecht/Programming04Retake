#include "Scene.h"
#include "GameObject.h"
#include "Componennts/CollisionComponent.h"
#include "Events/EventManager.h"
#include <algorithm>


using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) 
	:m_Name(name)
	, m_Objects{}
	,m_WasGameObjectAdded{false}
	,m_WasObjectDestroyed{false}
	, m_DestructionEventString{"GameObject Destroyed"}
	, m_IsActive{ false }
{
	Event destroyed;
	destroyed.eventType = m_DestructionEventString.c_str();
	auto boundGameObjectDestroyed = std::bind(&Scene::HandleDestroyedEvent, this, std::placeholders::_1);
	EventManager::GetInstance().RegisterObserver(destroyed, boundGameObjectDestroyed);
}

Scene::~Scene()
{
	m_Objects.clear();
	m_ObjectCollisions.clear();
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_Objects.emplace_back(std::move(object));
	m_WasGameObjectAdded = true;
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}


void Scene::Update()
{
	DepthSortGameObjects();

	//Normal Update
	for (auto& object : m_Objects)
	{
		if (!object->IsReadyForDestruction())
		{
			object->Update();
		}
	}

	RemoveDestroyedObjects();


	//Collision Checks
	for (auto& objectCollision : m_ObjectCollisions)
	{
		if (objectCollision->IsActive())
			objectCollision->IsOverlappingOtherCollision(m_ObjectCollisions);
	}
}

void dae::Scene::FixedUpdate(const float fixedTimeStep)
{
	for (auto& object : m_Objects)
	{
		if (!object->IsReadyForDestruction())
		{
			object->FixedUpdate(fixedTimeStep);
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}


void dae::Scene::AddCollision(CollisionComponent* collision)
{
	m_ObjectCollisions.emplace_back(collision);
}

void dae::Scene::RemoveCollision(CollisionComponent* collision)
{
	auto it = std::find(m_ObjectCollisions.begin(), m_ObjectCollisions.end(), collision);

	if (it != m_ObjectCollisions.end())
		m_ObjectCollisions.erase(it);
}

bool dae::Scene::IsActive()
{
	return m_IsActive;
}

void dae::Scene::SetActive(bool isActive)
{
	m_IsActive = isActive;
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
	return m_Name;
}


void dae::Scene::DepthSortGameObjects()
{
	if (!m_WasGameObjectAdded)
		return;

	// Sort the objects based on their draw depth
	std::sort
	(
		m_Objects.begin(),
		m_Objects.end(),
		[](const auto& object1,
			const auto& object2) {return object1->GetDrawDepth() < object2->GetDrawDepth(); }
	);

	// Reset the flag indicating a game object was added
	m_WasGameObjectAdded = false;
}

void dae::Scene::RemoveDestroyedObjects()
{
	if (m_WasObjectDestroyed)
		return;

	m_Objects.erase
	(
		std::remove_if
		(
			m_Objects.begin(), 
			m_Objects.end(),
			[&](std::shared_ptr<GameObject>& object)
			{
				return object->IsReadyForDestruction();
			})
			, m_Objects.end()
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

	m_WasObjectDestroyed = false;
}

void dae::Scene::HandleDestroyedEvent(const Event* e)
{
	if (std::string(e->eventType) == m_DestructionEventString)
	{
		m_WasObjectDestroyed = true;
	}
}



