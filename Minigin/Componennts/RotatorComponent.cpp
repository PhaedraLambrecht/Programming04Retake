#include "RotatorComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Time.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>


namespace dae
{
	RotatorComponent::RotatorComponent(GameObject* Owner)
		:BaseComponent(Owner)
		,m_Radius{}
		,m_RotationSpeed{}
		,m_CurrAngle{}
	{
		if (GetOwner()->HasComponent<TransformComponent>())
		{
			m_pTransform = GetOwner()->GetComponent<TransformComponent>();
		}
		else
		{
			m_pTransform = nullptr;
		}

	}

	RotatorComponent::~RotatorComponent()
	{
		std::cout << "RotatorComponent\n";
	}


	void RotatorComponent::RotateToAngle(float targetAngle)
	{
		float pi{ float(M_PI) };

		// Ensure targetAngle is within [0, 2*pi) range
		while (targetAngle < 0)
			targetAngle += 2 * pi;
		while (targetAngle >= 2 * pi)
			targetAngle -= 2 * pi;

		// Update current angle
		m_CurrAngle = targetAngle;

		if (GetOwner()->GetParent())
		{
			// If you have a parent, take their worldposition add the angle multiplied by the radius
			const float x{ GetOwner()->GetParent()->GetComponent<TransformComponent>()->GetWorldPosition().x + cosf(m_CurrAngle) * m_Radius };
			const float y{ GetOwner()->GetParent()->GetComponent<TransformComponent>()->GetWorldPosition().y + sinf(m_CurrAngle) * m_Radius };
			m_pTransform->SetLocalPosition(x, y);
		}
		else
		{
			// Otherwise, get your position 
			const float x{ m_pTransform->GetLocalPosition().x + cosf(m_CurrAngle) * m_Radius };
			const float y{ m_pTransform->GetLocalPosition().y + sinf(m_CurrAngle) * m_Radius };
			m_pTransform->SetLocalPosition(x, y);
		}
	}



	void RotatorComponent::FixedUpdate(const float fixedTimeStep)
	{
		float pi{ float(M_PI) };

		auto nextAngle = m_CurrAngle + m_RotationSpeed * (fixedTimeStep /  2);
		if (nextAngle > 2 * pi)
		{
			nextAngle -= 2 * pi;
		}
		else if (nextAngle < 2 * pi)
		{
			nextAngle += 2 * pi;
		}


		if (nextAngle != m_CurrAngle)
		{
			m_CurrAngle = nextAngle;

			if (GetOwner()->GetParent())
			{
				// If you have a parent, take their worldposition add the angle multiplied by the radius
				const float x{ GetOwner()->GetParent()->GetComponent<TransformComponent>()->GetWorldPosition().x + cosf(m_CurrAngle) * m_Radius };
				const float y{ GetOwner()->GetParent()->GetComponent<TransformComponent>()->GetWorldPosition().y + sinf(m_CurrAngle) * m_Radius };
				m_pTransform->SetLocalPosition(x, y);
			}
			else
			{
				// otherwise, get your position 
				const float x{ m_pTransform->GetLocalPosition().x + cosf(m_CurrAngle) * m_Radius };
				const float y{ m_pTransform->GetLocalPosition().y + sinf(m_CurrAngle) * m_Radius };
				m_pTransform->SetLocalPosition(x, y);
			}

		}
	}

	void RotatorComponent::SetRotationpeed(float speed)
	{
		m_RotationSpeed = speed;
	}

	void RotatorComponent::setRadius(float radius)
	{
		m_Radius = radius;
	}
	
}