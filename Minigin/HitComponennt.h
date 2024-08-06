#ifndef ENGINE2D_HITCOMPONENNT_H
#define ENGINE2D_HITCOMPONENNT_H
#include "Componennts/BaseComponent.h"
#include "Scene/Scene.h"
#include "glm/glm.hpp"
#include "Componennts/CollisionComponent.h"
#include <vector>

namespace dae
{
	class SoundSystem;
	class Texture2D;
	class Scene;
	class TransformComponent;

	class HitComponennt final : public BaseComponent
	{
	public:

		HitComponennt(GameObject* Owner);
		~HitComponennt() override = default;

		// Copy and move
		HitComponennt(const HitComponennt& other) = delete;
		HitComponennt(HitComponennt&& other) = delete;
		HitComponennt& operator=(const HitComponennt& other) = delete;
		HitComponennt& operator=(HitComponennt&& other) = delete;


		void Attack();
		void Update() override;
		void SetScene(Scene* scene);
		void SetPlayerIndex(int index);
	

	private:

		SoundSystem* m_pSoundsystem;

		std::vector<std::shared_ptr<GameObject>> m_pFiredBullets;
		std::shared_ptr<Texture2D> m_pAttackTexture;

		std::string m_sceneName;

		glm::vec2 m_bulletStartOffset;
		TransformComponent* m_pPlayerTransform;

		Scene* m_pScene;
		bool m_hasDestroyedBullet;

		int m_playerIndex;

	
		void CreateBullet(glm::vec2 position);
		void BulletHitCallback(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject);
	};
}

#endif // !ENGINE2D_HITCOMPONENNT_H