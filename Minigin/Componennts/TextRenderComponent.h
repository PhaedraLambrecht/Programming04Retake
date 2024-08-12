#ifndef ENGINE2D_TEXTRENDERCOMPONENT_H
#define ENGINE2D_TEXTRENDERCOMPONENT_H
#include "BaseComponent.h"


namespace dae
{
	class TextComponent;
	class TransformComponent;

	class TextRenderComponent final : public BaseComponent
	{
	public:

		TextRenderComponent(GameObject* Owner);
		~TextRenderComponent() override = default;

		// Copy and move
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;



		void Render() const override;
		void SetActive(bool isActive);


	private:

		bool m_IsActive;

		TextComponent* m_pText;
		TransformComponent* m_pTransform;
	};
}
#endif // ENGINE2D_TEXTRENDERCOMPONENT_H