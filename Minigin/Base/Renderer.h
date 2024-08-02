#ifndef ENGINE2D_RENDERRE_H
#define ENGINE2D_RENDERRE_H
#include <SDL.h>
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
		bool m_ShowDemo{ true };

	public:

		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderRect(float x, float y, float width, float height, SDL_Color color) const;
		void RenderRectEdges(float x, float y, float width, float height, SDL_Color color) const;


		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		SDL_Window* GetWindow() const; 

	};
}

#endif //ENGINE2D_RENDERRE_H