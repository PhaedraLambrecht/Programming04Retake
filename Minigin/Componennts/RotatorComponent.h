#ifndef ENGINE2D_ROTATEORCOMPONENT_H
#define ENGINE2D_ROTATEORCOMPONENT_H
#include "BaseComponent.h"
#include <glm/glm.hpp>


namespace dae
{

	class TransformComponent;

	class RotatorComponent final : public BaseComponent
	{
	public:

		RotatorComponent(GameObject* Owner);
		~RotatorComponent() override;

		// Copy and move
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;



		void FixedUpdate(const float fixedTimeStep) override;

		void SetRotationpeed(float speed);
		void setRadius(float radius);

		void RotateToAngle(float targetAngle);


	private:

		float m_RotationSpeed;
		float m_Radius;
		float m_CurrAngle;

		TransformComponent* m_pTransform;
	};
}


#endif // !ENGINE2D_ROTATEORCOMPONENT_H