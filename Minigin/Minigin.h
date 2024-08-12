#ifndef ENGINE2D_MINIGIN_H
#define ENGINE2D_MINIGIN_H
#include <string>
#include <functional>


namespace dae
{
	class Minigin
	{
	public:

		explicit Minigin(const std::string& dataPath, int windowWidth = 640, int windowHeight = 480);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;



		SDL_Window* GetWindow();

	private:

		SDL_Window* m_window{};
	};
}

#endif // !ENGINE2D_MINIGIN_H