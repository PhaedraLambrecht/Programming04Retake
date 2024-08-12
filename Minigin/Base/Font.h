#ifndef ENGINE2D_FONT_H
#define ENGINE2D_FONT_H
#include <string>


struct _TTF_Font;
namespace dae
{
	class Font final
	{
	public:

		explicit Font(const std::string& fullPath, unsigned int size);
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;



		_TTF_Font* GetFont() const;


	private:

		_TTF_Font* m_font;
	};
}

#endif //ENGINE2D_FONT_H