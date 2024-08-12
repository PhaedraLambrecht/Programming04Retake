#ifndef ENGINE2D_IMAGERENDERERCOMPONENT_H
#define ENGINE2D_IMAGERENDERERCOMPONENT_H
#include "BaseComponent.h"


namespace dae
{
	class ImageComponent;
	class TransformComponent;

	class ImageRenderComponent final : public BaseComponent
	{
	public:

		ImageRenderComponent(GameObject* Owner);
		~ImageRenderComponent() override = default;

		ImageRenderComponent(const ImageRenderComponent& other) = delete;
		ImageRenderComponent(ImageRenderComponent&& other) = delete;
		ImageRenderComponent& operator=(const ImageRenderComponent& other) = delete;
		ImageRenderComponent& operator=(ImageRenderComponent&& other) = delete;



		void Render() const override;


	private:

		ImageComponent* m_pImage;
		TransformComponent* m_pTransform;
	};

}

#endif // !ENGINE2D_IMAGERENDERERCOMPONENT_H