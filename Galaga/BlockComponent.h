#ifndef ENGINE2D_BLOCKCOMPONENT_H
#define ENGINE2D_BLOCKCOMPONENT_H
#include "Componennts/BaseComponent.h"
#include "Componennts/CollisionComponent.h"
#include <vector>


// State pattern
namespace dae
{
	class GameObject;
	class CollisionComponent;
	class BlockState;

	class BlockComponent : public BaseComponent
	{
	public:

		BlockComponent(GameObject* owner);
		virtual ~BlockComponent();

		BlockComponent(const BlockComponent& other) = delete;
		BlockComponent(BlockComponent&& other) = delete;
		BlockComponent& operator=(const BlockComponent& other) = delete;
		BlockComponent& operator=(BlockComponent&& other) = delete;



		void Update() override;
		void SetPlayer(std::vector<dae::GameObject*> player);
		std::vector<dae::GameObject*> GetPlayer();


		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);
		bool DoDamage(GameObject* player);
		bool IsPositionInsideWall(const glm::vec2& position);


		bool GetIsMoving() const;
		void SetIsMoving(bool isMoving);

		std::string GetDirection() const;
		void SetDirection(const std::string& direction);

		BlockState* GetCurrentState() const;
		void SetCurrentState(BlockState* state);


	private:

		BlockState* m_currentState;

		std::vector<dae::GameObject*> m_player;
		std::vector<dae::GameObject*> m_enemy;
		CollisionComponent* m_CollisionComponent;


		bool m_isMoving;
		std::string m_direction;


		void CheckCollision(std::vector<GameObject*> otherObject);
	};


	class BlockState
	{
	public:

		virtual ~BlockState() = default;

		virtual void Update(GameObject* block) = 0;
		virtual void OnHitCallback(BlockComponent* block, const CollisionData& collisionOwner, const CollisionData& hitObject) = 0;
	};

	// Define concrete states
	class IdleState : public BlockState
	{
	public:

		~IdleState() override = default;

		void Update(GameObject* /*block*/) override {};
		void OnHitCallback(BlockComponent* /*block*/, const CollisionData& /*collisionOwner*/, const CollisionData& /*hitObject*/) override {};
	};

	class BlockMovingState : public BlockState
	{
	public:
		
		~BlockMovingState() override = default;

		void Update(GameObject* block) override;
		void OnHitCallback(BlockComponent* /*block*/, const CollisionData& /*collisionOwner*/, const CollisionData& /*hitObject*/) override {};
	};

}

#endif // !ENGINE2D_BLOCKCOMPONENT_H
