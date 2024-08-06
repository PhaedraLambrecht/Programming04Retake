#ifndef ENGINE2D_SCENEMANAGER_H
#define ENGINE2D_SCENEMANAGER_H
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"


namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:

		Scene& CreateScene(const std::string& name);
		void RemoveScene(const std::string& name);
		Scene& GetSceneByName(const std::string& name);

		void Update();
		void FixedUpdate(const float fixedTimeStep);
		void Render();

		void Destroy();

		Scene& GetActiveScene();
		void SetActiveScene(const std::string& name);
		bool IsSceneActive(const std::string& name);


		void SwitchScene(const std::string& name);
		std::vector<std::shared_ptr<Scene>> m_pScenes;


	private:

		friend class Singleton<SceneManager>;
		SceneManager() = default;


		std::shared_ptr<Scene> m_pActiveScene;
	};
}
#endif // ENGINE2D_SCENEMANAGER_H