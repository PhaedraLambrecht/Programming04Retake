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
		virtual ~BaseEnemyComponent();

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

		int m_Health{ 3 };
		float m_EnemySpeed{ 40.f };
		float m_ShootCannonCooldown{ 0.f };
		glm::vec2 position{};
		float m_Width{};
		float m_Height{};
		bool m_BlockMoveLeft{ false };
		bool m_BlockMoveRight{ false };
		bool m_BlockMoveUp{ false };
		bool m_BlockMoveDown{ false };
		std::shared_ptr<ImageComponent> m_enemyHorizontalSprite;
		std::shared_ptr<ImageComponent> m_enemyVerticalSprite;
		std::shared_ptr<GameObject> m_PlayerTank;
	};
}


#endif // !ENGINE2D_BASEENEMYCOMPONENT_H