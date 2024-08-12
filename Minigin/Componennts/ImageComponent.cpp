#include "ImageComponent.h"
#include "Base/Texture2D.h"
#include "Resources/ResourceManager.h"
#include <iostream>


namespace dae
{
	ImageComponent::ImageComponent(GameObject* Owner)
		:BaseComponent(Owner)
		,m_pTexture{nullptr}
	{
	}


	void ImageComponent::SetTexture(const std::string& fileName)
	{
		if (fileName.empty())
		{
			std::cerr << "ImageComponent::SetTexture() - fileName is empty\n";
			return;
		}
		m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
	}


	glm::vec2 ImageComponent::GetTextureDimensions() const
	{
		if (m_pTexture == nullptr)
		{
			std::cerr << "ImageComponent::GetTextureDimensions() - m_pTexture is nullptr\n";
			return glm::vec2{0,0};
		}
		return m_pTexture->GetSize();
	}

	std::shared_ptr<Texture2D> ImageComponent::GetTexture() const
	{
		if(m_pTexture == nullptr)
		{
			std::cerr << "ImageComponent::GetTexture() - m_pTexture is nullptr\n";
			return nullptr;
		}

		return m_pTexture;
	}
}