#ifndef ENGINE2D_TEXTURE2D_H
#define ENGINE2D_TEXTURE2D_H
#include <glm/vec2.hpp>


struct SDL_Texture;

namespace dae
{
	class Texture2D final
	{
	public:

		explicit Texture2D(SDL_Texture* texture);
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;
		


		SDL_Texture* GetSDLTexture() const;
		glm::ivec2 GetSize() const;


	private:

		SDL_Texture* m_texture;
	};
}

#endif //ENGINE2D_TEXTURE2D_H