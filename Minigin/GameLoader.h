#ifndef ENGINE2D_GAMELOADER_H
#define ENGINE2D_GAMELOADER_H
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <memory>


namespace dae
{
	constexpr unsigned g_WindowWidth{ 640 }, g_WindowHeight{ 480 };

	class GameObject;
	
	class SoundSystem;
	struct SoundData;

	class Scene;


	class GameLoader final
	{
	public:

		void LoadSound(SoundSystem* soundSystem, const SoundData& soundData);
	
		// World
		std::shared_ptr<GameObject> LoadLevelBorder(Scene& scene);
		
		// Player and enemy
		std::shared_ptr<GameObject> LoadPlayer(Scene& scene, int playerIndex, unsigned int conntroller, const std::string texture, const glm::vec2 position, const glm::vec2 offset);
		std::shared_ptr<GameObject> LoadEnemy(dae::Scene& scene, const glm::vec2& position, const std::string& texture, std::vector<dae::GameObject*> players, const std::shared_ptr<dae::GameObject>& backGroundImage, const glm::vec2& offset);

		// Walls
		std::shared_ptr<GameObject> LoadWall(Scene& scene, const glm::vec2 position, const std::string& texture, const glm::vec2 offset, std::vector<dae::GameObject*> player);
		std::shared_ptr<GameObject> LoadEggWall(Scene& scene, const glm::vec2 position, const std::string& texture, const glm::vec2 offset, GameObject* background, std::vector<dae::GameObject*> player);
		std::shared_ptr<GameObject> LoadDiamondWall(Scene& scene, const glm::vec2 position, const std::string& texture, const glm::vec2 offset, std::vector<dae::GameObject*> player);

		void LoadHighScoreScene(const std::string& sceneName);


	private:

		// Helper functions
		void SetUpBaseCollision(GameObject* object, const std::string& tag, Scene& scene);
		void SetUpTransform(GameObject* object, const glm::vec2& position, const glm::vec2 offset);
		void SetUpUI(Scene& scene, int playerIndex, GameObject* player, glm::vec2 position);


		// Controls
		void SetupPlayerControls(GameObject* player, dae::Scene& scene);
		void AddControleler(Scene& scene, GameObject* player, unsigned controller);
		void AddKeyboard(Scene& scene, GameObject* player);
	};
}

#endif // ENGINE2D_GAMELOADER_H