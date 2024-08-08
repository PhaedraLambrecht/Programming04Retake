#ifndef ENGINE2D_BULLETMOVEMENTCOMPONENT_H
#define ENGINE2D_BULLETMOVEMENTCOMPONENT_H
#include "Componennts/BaseComponent.h"
#include "glm/glm.hpp"
#include <string>


namespace dae
{
	class TransformComponent;

	class BulletMovementComponent final : public BaseComponent
	{
	public:

		BulletMovementComponent(GameObject* Owner);
		~BulletMovementComponent() override;

		// Copy and move
		BulletMovementComponent(const BulletMovementComponent& other) = delete;
		BulletMovementComponent(BulletMovementComponent&& other) = delete;
		BulletMovementComponent& operator=(const BulletMovementComponent& other) = delete;
		BulletMovementComponent& operator=(BulletMovementComponent&& other) = delete;

		void Update() override;
		void SetMaxDistance(float distance);

		void SetDirection(const std::string& direction);


	private:

		float m_MoveSpeed{ 600 };

		float m_MaxDistance{ 300 };
		float m_DistanceTraveled{ 0 };

		TransformComponent* m_pTransform{};

		std::string m_Direction;
	};

}

#endif // !ENGINE2D_BULLETMOVEMENTCOMPONENT_H