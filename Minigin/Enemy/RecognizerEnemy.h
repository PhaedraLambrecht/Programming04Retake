#ifndef ENGINE2D_RECOGNIZERENEMY_H
#define ENGINE2D_RECOGNIZERENEMY_H
#include "BaseEnemyComponent.h"

namespace dae
{
	class RecognizerEnemy final : public BaseEnemyComponent
	{
	public:

		explicit RecognizerEnemy(GameObject* gameObject);
		~RecognizerEnemy() override;

		RecognizerEnemy(const RecognizerEnemy& other) = delete;
		RecognizerEnemy(RecognizerEnemy&& other) = delete;
		RecognizerEnemy& operator=(const RecognizerEnemy& other) = delete;
		RecognizerEnemy& operator=(RecognizerEnemy&& other) = delete;


		void Initialize(float x, float y, float w, float h, std::shared_ptr<GameObject> playerTank) override;
		void Update() override;
		bool DoDamage() override;
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject) override;

		void OnBDeath(const Event* e);
		void SetWindowDimensions(float xPos, float yPos, float width, float height);


	private:

		struct PositionSize
		{
			glm::vec2 position;
			float width;
			float height;
		} m_PositionSize;

		struct MovementFlags
		{
			bool left;
			bool right;
			bool up;
			bool down;
		} m_MovementFlags;

		int m_Health;
		float m_EnemySpeed;
		std::shared_ptr<ImageComponent> m_enemyHorizontalSprite;
		GameObject* m_pPlayer;

		float m_ChangeDirectionInterval;
		float m_TimeSinceLastChange;

		float m_AttackCooldown;
		const float m_DamageInterval{ 5.0f };

		float m_windowWidth, m_windowHeight;
		float m_xPos, m_yPos;



		void AddPointsAndNotifyDeath();


		void ChangeDirection();

		void move(float deltaTime, int x, int y);
		void HandleBlockedMovement(float deltaTime);
		void ResetMovementBlocks();

		bool IsBlockingLeft(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds) const;
		bool IsBlockingRight(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds) const;
		bool IsBlockingUp(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds) const;
		bool IsBlockingDown(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds) const;
	
	};

}

#endif // !ENGINE2D_RECOGNIZERENjsonEnemy_H
