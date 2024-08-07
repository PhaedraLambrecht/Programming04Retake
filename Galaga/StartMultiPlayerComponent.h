#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>


namespace dae
{
	class GameObject;

	class StartMultiPlayerComponent final
	{
	public:

		StartMultiPlayerComponent();
		~StartMultiPlayerComponent() = default;

		StartMultiPlayerComponent(const StartMultiPlayerComponent& other) = delete;
		StartMultiPlayerComponent(StartMultiPlayerComponent&& other) = delete;
		StartMultiPlayerComponent& operator=(const StartMultiPlayerComponent& other) = delete;
		StartMultiPlayerComponent& operator=(StartMultiPlayerComponent&& other) = delete;

		void LoadLevels();

	private:

		std::string m_levelFile;
		std::unordered_map<int, std::string> m_texturemaps;


		void LoadMultiPlayerScene(const std::string& levelName, const glm::vec2 offset, unsigned int controller1, unsigned int controller2);


	};
}

