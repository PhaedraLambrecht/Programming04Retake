#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>

#include "SkipLevelCommand.h"
#include "GameLoader.h"

#include "Componennts/ScoreComponent.h"
#include "GameObject.h"


void dae::SceneManager::Update()
{
	m_pActiveScene->Update();

	if (m_pActiveScene->m_NoEnemies && m_pActiveScene->m_EnemyLoaded)
	{
		if (m_pActiveScene == m_pScenes.back())
		{
			GameLoader gameLoader{};
			std::string sceneName = "HighScore";
			gameLoader.LoadHighScoreScene(sceneName);
		}
		else
		{
			SkipLevelCommand skipLevelCommand = SkipLevelCommand(m_pActiveScene->GetPlayer(0));
			skipLevelCommand.Execute();
		}

	}
}

void dae::SceneManager::FixedUpdate(const float fixedTimeStep)
{
	m_pActiveScene->FixedUpdate(fixedTimeStep);
}

void dae::SceneManager::Render()
{
	m_pActiveScene->Render();
}

void dae::SceneManager::Destroy()
{
	m_pScenes.clear();
	m_pActiveScene = nullptr;
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
	return *m_pActiveScene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (auto& scene : m_pScenes)
	{
		if (scene->GetName() == name)
		{
			if (m_pActiveScene)
				m_pActiveScene->SetActive(false);

			m_pActiveScene = scene;
			m_pActiveScene->SetActive(true);
			return;
		}
	}
}

bool dae::SceneManager::IsSceneActive(const std::string& name)
{
	return m_pActiveScene->GetName() == name;
}

void dae::SceneManager::SwitchScene(const std::string& name)
{
	auto it = std::find_if(m_pScenes.begin(), m_pScenes.end(),
		[&name](const std::shared_ptr<dae::Scene>& scene) { return scene->GetName() == name; });

	if (it != m_pScenes.end())
	{
		if (m_pActiveScene) {
			m_pActiveScene->SetActive(false); // deactivate current scene
		}
		m_pActiveScene = *it;
		m_pActiveScene->SetActive(true); // activate new scene
	}
}

dae::Scene& dae::SceneManager::GetSceneByName(const std::string& name)
{
	// std::find_if to search for a scene vector
	auto it = std::find_if(m_pScenes.begin(), m_pScenes.end(), [&](const auto& scene)
	{
		return scene->GetName() == name;
	});

	if (it != m_pScenes.end()) {
		return **it;
	}

	return CreateScene(name);
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_pScenes.push_back(scene);

	if (m_pActiveScene)
		m_pActiveScene->SetActive(false);

	m_pActiveScene = scene;
	m_pActiveScene->SetActive(true);


	return *scene;
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	m_pScenes.erase(std::remove_if(m_pScenes.begin(), m_pScenes.end(), [&](std::shared_ptr<Scene> pScene) 
	{
		return pScene->GetName() == name;
	}));
}
