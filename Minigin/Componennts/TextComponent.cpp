#include "TextComponent.h"
#include "Base/Font.h"
#include "Base/Texture2D.h"
#include "Base/Renderer.h"
#include <SDL_ttf.h>
#include <stdexcept>
#include <iostream>


namespace dae
{
	TextComponent::TextComponent(GameObject* Owner)
		:BaseComponent(Owner)
		,m_Text{"Default"}
		,m_Color{}
		,m_pFont{ nullptr }
		,m_pTexture{nullptr}
	{
	}

	TextComponent::~TextComponent()
	{
		std::cout << "TextComponent\n";
	}


	void TextComponent::SetText(std::string& text)
	{
		m_Text = text;
		SetTexture();
	}

	void TextComponent::SetFont(std::shared_ptr<Font> font)
	{
		if (font)
		{
			m_pFont = std::move(font);
			SetTexture();
		}
		else
		{
			// It reports errors that arise because an argument value has not been accepted.
			throw std::invalid_argument("Font pointer is null");
		}
	}

	void TextComponent::SetColor(const SDL_Color color)
	{
		m_Color = color;
		SetTexture();
	}

	std::shared_ptr<Texture2D> TextComponent::GetTexture() const
	{
		return m_pTexture;
	}

	void TextComponent::SetTexture()
	{
		if (!m_pFont)
		{
			throw std::invalid_argument("Font pointer is null");
		}

		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (!surf)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}


		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (!texture)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}


		SDL_FreeSurface(surf);
		auto test = std::make_shared<Texture2D>(texture);
		m_pTexture = test;
	}
}
