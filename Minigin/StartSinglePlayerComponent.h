#ifndef ENGINE2D_STARTSINGLEPLAYERCOMPONENT_H
#define ENGINE2D_STARTSINGLEPLAYERCOMPONENT_H
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace dae
{
	class GameObject;

	class StartSinglePlayerComponent final
	{
	public:

		StartSinglePlayerComponent();
		~StartSinglePlayerComponent() = default;

		StartSinglePlayerComponent(const StartSinglePlayerComponent& other) = delete;
		StartSinglePlayerComponent(StartSinglePlayerComponent&& other) = delete;
		StartSinglePlayerComponent& operator=(const StartSinglePlayerComponent& other) = delete;
		StartSinglePlayerComponent& operator=(StartSinglePlayerComponent&& other) = delete;

		void LoadLevels();

	private:

		std::string m_levelFile;
		std::unordered_map<int, std::string> m_texturemaps;


		void LoadSinglePlayerScene(const std::string& levelName, const glm::vec2 offset, unsigned int controller);


	};
}

#endif // !ENGINE2D_STARTSINGLEPLAYERCOMPONENT_H