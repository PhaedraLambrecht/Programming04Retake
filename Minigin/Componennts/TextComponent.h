#ifndef ENGINE2D_TEXTCOMPONENT_H
#define ENGINE2D_TEXTCOMPONENT_H
#include "BaseComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include <SDL_pixels.h>
#include <iostream>
#include <memory>


namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:

		TextComponent(GameObject* Owner);
		~TextComponent() override = default;

		// Copy and move
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;



		void SetText(std::string& text);
		void SetFont(std::shared_ptr<Font> font);
		void SetColor(const SDL_Color color);

		std::shared_ptr<Texture2D> GetTexture() const;


	private:

		void SetTexture();

		std::string m_Text;
		SDL_Color m_Color;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTexture;
	};
}
#endif // ENGINE2D_TEXTCOMPONENT_H