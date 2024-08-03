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


	ImageComponent::~ImageComponent()
	{
		std::cout << "ImageComponent\n";
	}

	void ImageComponent::SetTexture(const std::string& fileName)
	{
		m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
	}


	glm::vec2 ImageComponent::GetTextureDimensions() const
	{
		return m_pTexture->GetSize();
	}

	std::shared_ptr<Texture2D> ImageComponent::GetTexture() const
	{
		return m_pTexture;
	}
}