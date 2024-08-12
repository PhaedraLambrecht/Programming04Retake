#include "TextRenderComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "Base/Renderer.h"
#include "GameObject.h"
#include <stdexcept>
#include <iostream>


namespace dae
{
	TextRenderComponent::TextRenderComponent(GameObject* Owner)
		:BaseComponent(Owner)
		, m_IsActive{ true }
	{
		m_pText = GetOwner()->GetComponent<TextComponent>();
		if (!m_pText)
		{
			throw std::invalid_argument("TextRendererComponent needs a TextComponent");
		}
	
		m_pTransform = GetOwner()->GetComponent<TransformComponent>();
		if (!m_pTransform)
		{
			throw std::invalid_argument("TextRendererComponent needs a TransformComponent");
		}
	}


	void TextRenderComponent::Render() const
	{
		if (!m_IsActive || !m_pText || !m_pText->GetTexture())
		{
			return;
		}
	
		const auto& position = m_pTransform->GetLocalPosition();
		Renderer::GetInstance().RenderTexture(*m_pText->GetTexture(), position.x, position.y);
	}
	
	void TextRenderComponent::SetActive(bool isActive)
	{
		m_IsActive = isActive;
	}
}
