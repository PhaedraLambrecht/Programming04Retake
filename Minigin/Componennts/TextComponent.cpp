#include "TextComponent.h"
#include "Base/Font.h"
#include "Base/Texture2D.h"
#include "Base/Renderer.h"
#include <SDL_ttf.h>
#include <stdexcept>
#include <iostream>
//#include <memory>
#include <sstream>

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


		// Split the text into lines
		std::istringstream iss{ m_Text };
		std::vector<std::string> lines;
		std::string line;


		// Render each line of text
		std::vector<SDL_Surface*> surfaces;
		int width = 0;
		int height = 0;
		while (std::getline(iss, line, '\n'))
		{
			const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), line.c_str(), m_Color);
			if (!surf)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			surfaces.push_back(surf);

			width = std::max(width, surf->w);
			height += surf->h;
		}


		// Create final surface and blit individual surfaces onto it
		SDL_Surface* finalSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
		if (!finalSurface)
		{
			throw std::runtime_error(std::string("Create final surface failed: ") + SDL_GetError());
		}
		int y = 0;
		for (const auto& surf : surfaces)
		{
			SDL_Rect rect{ 0, y, surf->w, surf->h };
			SDL_BlitSurface(surf, nullptr, finalSurface, &rect);
			y += surf->h;
			SDL_FreeSurface(surf);
		}


		// Create texture from final surface
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), finalSurface);
		if (!texture)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(finalSurface);

		// Assign texture to member variable
		auto test = std::make_shared<Texture2D>(texture);
		m_pTexture = test;


	}

}
