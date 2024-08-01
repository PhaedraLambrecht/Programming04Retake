#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include <stdexcept>
#include <iostream>


namespace dae
{
	FPSComponent::FPSComponent(GameObject* Owner)
		:BaseComponent(Owner)
		,m_fps{}
	{
		if (GetOwner()->HasComponent<TextComponent>())
		{
			m_pText = GetOwner()->GetComponent<TextComponent>();
		}
		else
		{
			m_pText = GetOwner()->AddComponent<TextComponent>();
		}
	}

	FPSComponent::~FPSComponent()
	{
		std::cout << "FPSComponent\n";
	}


	void FPSComponent::Update()
	{
		if (!m_pText)
		{
			throw std::invalid_argument("FPSComponent needs a TextComponent");
		}

		float newFPS =  1.0f / GameTime::GetInstance().GetDeltaTime();


		if (newFPS != m_fps)
		{
			m_fps = newFPS;

			std::string fpsText = std::to_string((int)m_fps) + "FPS";
			m_pText->SetText(fpsText);
		}	
	}
}