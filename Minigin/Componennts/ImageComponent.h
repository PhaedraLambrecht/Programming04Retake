#ifndef ENGINE2D_IMAGECOMPONENT_H
#define ENGINE2D_IMAGECOMPONENT_H
#include "BaseComponent.h"
#include "glm/glm.hpp"
#include <string>
#include <memory>


namespace dae
{
	class Texture2D;

	class ImageComponent final : public BaseComponent
	{
	public:

		ImageComponent(GameObject* Owner);
		~ImageComponent() override;

		// Copy and move
		ImageComponent(const ImageComponent& other) = delete;
		ImageComponent(ImageComponent&& other) = delete;
		ImageComponent& operator=(const ImageComponent& other) = delete;
		ImageComponent& operator=(ImageComponent&& other) = delete;



		void SetTexture(const std::string& fileName);
	
		glm::vec2 GetTextureDimensions() const;
		std::shared_ptr<Texture2D> GetTexture() const;


	private:

		std::shared_ptr<Texture2D> m_pTexture;
	};
}

#endif // !ENGINE2D_IMAGECOMPONENT_H