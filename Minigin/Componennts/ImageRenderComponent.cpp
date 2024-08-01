#include "ImageRenderComponent.h"
#include "ImageComponent.h"
#include "TransformComponent.h"
#include "Base/Renderer.h"
#include "GameObject.h"
#include <stdexcept>
#include <iostream>


namespace dae
{
	ImageRenderComponent::ImageRenderComponent(GameObject* Owner)
		:BaseComponent(Owner)
	{
		if (GetOwner()->HasComponent<ImageComponent>())
		{
			m_pImage = GetOwner()->GetComponent<ImageComponent>();
		}
		else
		{
			m_pImage = GetOwner()->AddComponent<ImageComponent>();
		}

		if (GetOwner()->HasComponent<TransformComponent>())
		{
			m_pTransform = GetOwner()->GetComponent<TransformComponent>();
		}
		else
		{
			m_pTransform = nullptr;
		}
	}

	ImageRenderComponent::~ImageRenderComponent()
	{
		std::cout << "ImageRenderComponent\n";
	}


	void ImageRenderComponent::Render() const
	{
		if (!m_pImage)
		{
			throw std::invalid_argument("ImageRenderComponent needs a ImageComponent");
		}

		if (!m_pTransform)
		{
			throw std::invalid_argument("TextRendererComponent needs a TransformComponent");
		}


		const auto& position = m_pTransform->GetLocalPosition();
		Renderer::GetInstance().RenderTexture(*m_pImage->GetTexture(), position.x, position.y);
	}
}