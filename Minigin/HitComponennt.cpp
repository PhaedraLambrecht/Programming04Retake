#include "HitComponennt.h"
#include "GameObject.h"
#include "Base/Texture2D.h"
#include "Componennts/TransformComponent.h"
#include "Resources/ResourceManager.h"
#include "Componennts/ImageComponent.h"
#include "Componennts/BulletMovementComponent.h"
#include "Componennts/ImageRenderComponent.h"
#include "Sound/SoundManager.h"
#include "Sound/SoundSystem.h"
#include "Componennts/CollisionComponent.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include <iostream>

dae::HitComponennt::HitComponennt(GameObject* Owner)
	:BaseComponent(Owner)
	, m_pScene{}
	, m_FiredBullets{}
	, m_HasDestroyedBullet{ false }
	, m_playerIndex{ 0 }
	, m_SceneName{ "Demo" }
{
	m_AttackTexture = ResourceManager::GetInstance().LoadTexture("Bullet.png");

	m_pPlayerTransform = GetOwner()->GetComponent<TransformComponent>();
	if (!m_pPlayerTransform)
	{
		throw std::invalid_argument("TextRendererComponent needs a TransformComponent");
	}


	glm::vec2 playerSpriteDimension = GetOwner()->GetComponent<dae::ImageComponent>()->GetTextureDimensions();
	glm::vec2 bulletSpriteDimensions = static_cast<glm::vec2>(m_AttackTexture->GetSize());

	m_pSoundsystem = SoundManager::GetInstance().GetSoundSystem();
}

dae::HitComponennt::~HitComponennt()
{
	std::cout << "HitComponennt\n";
}

void dae::HitComponennt::Attack()
{
	if (!m_pScene)
	{
		throw std::runtime_error("No scene set for ShootComponent. Use ShootComponent::SetScene to set the scene.");
	}

	if (m_FiredBullets.size() >= 1)
		return;

	const glm::vec2 ownPos = m_pPlayerTransform->GetWorldPosition() + GetOwner()->GetComponent<ImageComponent>()->GetTextureDimensions() / 2.f;
	auto bullet = std::make_shared<GameObject>();


	bullet->GetComponent<TransformComponent>()->SetLocalPosition(ownPos.x, ownPos.y);
	bullet->AddComponent<ImageComponent>()->SetTexture("Bullet.png");
	bullet->AddComponent<BulletMovementComponent>();
	bullet->GetComponent<BulletMovementComponent>()->SetMaxDistance(0.5f);



	auto collision = bullet->AddComponent<dae::CollisionComponent>();
	collision->SetCollisionData({ "DiamondAttack", bullet.get() });

	float collisionWidht{ bullet->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x }, collisionHeight{ bullet->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y };
	collision->SetBounds(collisionWidht, collisionHeight);


	auto boundHitCallback = std::bind(&HitComponennt::BulletHitCallback, this, std::placeholders::_1, std::placeholders::_2);
	collision->SetCallback(boundHitCallback);

	collision->SetScene(m_pScene);
	m_pScene->Add(bullet);
	m_pScene->AddCollision(collision);
	m_FiredBullets.push_back(bullet);



	std::unique_ptr<SceneEvent> hitEvent = std::make_unique<SceneEvent>();
	hitEvent->eventType = "BulletFired";
	hitEvent->sceneName = m_SceneName;
	EventManager::GetInstance().QueueEvent(std::move(hitEvent));



	// Play shooting sound
	m_pSoundsystem->NotifySound(SoundData{ 1, 0.1f, SoundData::SoundType::SoundEffect });
}

void dae::HitComponennt::Update()
{
	// std::remove_if to moves all elements in the vector that meet a certain condition 
	// to the end of the vector
	if (m_HasDestroyedBullet)
	{
		m_FiredBullets.erase
		(
			std::remove_if
			(
				std::begin(m_FiredBullets),
				std::end(m_FiredBullets),
				[&](const auto& bullet)
				{ return bullet->IsReadyForDestruction(); }
			),
			std::end(m_FiredBullets)
		);

		m_HasDestroyedBullet = false;
	}


	// Check if any bullets are marked for destruction after the erase operation
	for (auto& bullet : m_FiredBullets)
	{
		if (bullet->IsReadyForDestruction())
		{
			m_HasDestroyedBullet = true;
		}
	}
}

void dae::HitComponennt::SetScene(Scene* scene)
{
	m_pScene = scene;
}

void dae::HitComponennt::SetPlayerIndex(int index)
{
	m_playerIndex = index;
}

void dae::HitComponennt::BulletHitCallback(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject)
{
	if (!(strcmp(hitObject.ownerType.c_str(), "diamondBlock") == 0))
		return;


	std::unique_ptr<SceneEvent> hitEvent = std::make_unique<SceneEvent>();
	hitEvent->eventType = "BlockHit";
	hitEvent->sceneName = m_SceneName;
	EventManager::GetInstance().QueueEvent(std::move(hitEvent));


	collisionOwner.owningObject->MarkForDestruction();
}
