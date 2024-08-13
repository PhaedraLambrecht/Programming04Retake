#ifndef ENGINE2D_BASEENEMYCOMPONENT_H
#define ENGINE2D_BASEENEMYCOMPONENT_H
#include "Componennts/BaseComponent.h"
#include "Componennts/CollisionComponent.h"
#include <glm/glm.hpp>


namespace dae
{
	class ImageComponent;


	class BaseEnemyComponent : public BaseComponent
	{
	public:

		BaseEnemyComponent(GameObject* owner);
		virtual ~BaseEnemyComponent() = default;

		BaseEnemyComponent(const BaseEnemyComponent& other) = delete;
		BaseEnemyComponent(BaseEnemyComponent&& other) = delete;
		BaseEnemyComponent& operator=(const BaseEnemyComponent& other) = delete;
		BaseEnemyComponent& operator=(BaseEnemyComponent&& other) = delete;



		virtual void Initialize(float x, float y, float w, float h, std::shared_ptr<GameObject> playerTank);
		void Update() override;
		virtual void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);

		virtual void Attack();
		virtual void ShootCannon(float degreesAngle);
		virtual bool DoDamage();

	private:

		int m_Health;
		float m_EnemySpeed;
		float m_ShootCannonCooldown;
		glm::vec2 position;
		float m_Width;
		float m_Height;
		bool m_BlockMoveLeft;
		bool m_BlockMoveRight;
		bool m_BlockMoveUp;
		bool m_BlockMoveDown;
		std::shared_ptr<ImageComponent> m_enemyHorizontalSprite;
		std::shared_ptr<ImageComponent> m_enemyVerticalSprite;
		std::shared_ptr<GameObject> m_PlayerTank;
	};
}


#endif // !ENGINE2D_BASEENEMYCOMPONENT_H