#ifndef ENGINE2D_RECOGNIZERENEMY_H
#define ENGINE2D_RECOGNIZERENEMY_H
#include "Enemy/BaseEnemyComponent.h"


namespace dae
{
	class State;


	class RecognizerEnemy final : public BaseEnemyComponent
	{
		struct MovementFlags
		{
			bool left;
			bool right;
			bool up;
			bool down;
		};

	public:

		explicit RecognizerEnemy(GameObject* gameObject);
		~RecognizerEnemy() override;

		RecognizerEnemy(const RecognizerEnemy& other) = delete;
		RecognizerEnemy(RecognizerEnemy&& other) = delete;
		RecognizerEnemy& operator=(const RecognizerEnemy& other) = delete;
		RecognizerEnemy& operator=(RecognizerEnemy&& other) = delete;



		void Initialize(float x, float y, float w, float h, std::vector<GameObject*> pPlayers);
		void Update() override;
		bool DoDamage() override;
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject) override;

		void OnDeath(const Event* e);
		void SetWindowDimensions(float xPos, float yPos, float width, float height);


		// For State
		float GetTimeSinceLastChange() const;
		void SetTimeSinceLastChange(float time);
		float GetDirectionInterval() const;
		float GetEnemySpeed() const;
		float GetXPos() const;
		float GetYPos() const;

		MovementFlags m_MovementFlags;

		float m_windowWidth, m_windowHeight;


	private:

		State* currentState;


		struct PositionSize
		{
			glm::vec2 position;
			float width;
			float height;
		} m_PositionSize;


		int m_Health;
		float m_EnemySpeed;
		std::shared_ptr<ImageComponent> m_enemyHorizontalSprite;
		std::vector<GameObject*> m_pPlayer;

		float m_ChangeDirectionInterval;
		float m_TimeSinceLastChange;

		float m_AttackCooldown;
		const float m_DamageInterval{ 5.0f };

		float m_xPos, m_yPos;



		void AddPointsAndNotifyDeath();
		void IsBlocking(const glm::vec2& enemyPos, const glm::vec2& collisionPos, const glm::vec2& collisionBounds);

		void OverlappingWall();
	};



	class State
	{
	public:

		State(GameObject* enemy);
		virtual ~State();

		virtual void Update() = 0;


		GameObject* m_enemy;
	};

	class MovingState : public State
	{
	public:

		MovingState(GameObject* enemy)
			:State(enemy)
		{};
		void Update() override;

	private:

		void move(float deltaTime, int x, int y);
		void HandleBlockedMovement(float deltaTime);
		void ChangeDirection();
	};

}

#endif // !ENGINE2D_RECOGNIZERENjsonEnemy_H
