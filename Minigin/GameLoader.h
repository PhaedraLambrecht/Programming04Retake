#ifndef ENGINE2D_GAMELOADER_H
#define ENGINE2D_GAMELOADER_H
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <vector>


namespace dae
{
	constexpr unsigned g_WindowWidth{ 640 }, g_WindowHeight{ 480 };



	class GameObject;
	class InputManager;
	class MoveCommand;

	class TransformComponent;
	class Scene;

	class GameLoader
	{
	public:

		std::shared_ptr<GameObject> AddPlayer(Scene& scene, int playerIndex, InputManager& inputManager, const bool IsControllerUsed, const std::string texture);
		std::shared_ptr<GameObject> AddEnemy(Scene& scene, const std::string texture, std::vector<GameObject*> players);
	

		void AddControleler(Scene& scene, GameObject* player, unsigned controller);
		void AddKeyboard(Scene& scene, GameObject* player);
	

	private:


		void AddUI(Scene& scene, int playerIndex, GameObject* player);

		float getRandomFloat(float min, float max);
	};
}
#endif // ENGINE2D_GAMELOADER_H