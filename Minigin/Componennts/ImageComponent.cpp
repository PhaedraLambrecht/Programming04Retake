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
		// We don't want to load an epty file
		if (fileName.empty())
		{
			return;
		}
		
		// Load the texture
		m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
	}


	glm::vec2 ImageComponent::GetTextureDimensions() const
	{
		if (m_pTexture->GetSize() == glm::ivec2{0,0})
		{
			throw std::invalid_argument("Texture size is 0,0");
		}

		return m_pTexture->GetSize();
	}

	std::shared_ptr<Texture2D> ImageComponent::GetTexture() const
	{
		return m_pTexture;
	}
}