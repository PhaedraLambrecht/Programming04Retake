#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>


void dae::SceneManager::Update()
{
	m_ActiveScene->Update();
}

void dae::SceneManager::FixedUpdate(const float fixedTimeStep)
{
	m_ActiveScene->FixedUpdate(fixedTimeStep);
}

void dae::SceneManager::Render()
{
	m_ActiveScene->Render();
}

void dae::SceneManager::Destroy()
{
	m_Scenes.clear();
	m_ActiveScene = nullptr;
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
	return *m_ActiveScene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (auto& scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			if (m_ActiveScene)
				m_ActiveScene->SetActive(false);

			m_ActiveScene = scene;
			m_ActiveScene->SetActive(true);
			return;
		}
	}
}

bool dae::SceneManager::IsSceneActive(const std::string& name)
{
	return m_ActiveScene->GetName() == name;
}

void dae::SceneManager::SwitchScene(const std::string& name)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(),
		[&name](const std::shared_ptr<dae::Scene>& scene) { return scene->GetName() == name; });

	if (it != m_Scenes.end())
	{
		if (m_ActiveScene) {
			m_ActiveScene->SetActive(false); // Optional: deactivate current scene
		}
		m_ActiveScene = *it;
		m_ActiveScene->SetActive(true); // Optional: activate new scene
	}
}

dae::Scene& dae::SceneManager::GetSceneByName(const std::string& name)
{
	// std::find_if to search for a scene vector
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&](const auto& scene)
	{
		return scene->GetName() == name;
	});

	if (it != m_Scenes.end()) {
		return **it;
	}

	return CreateScene(name);
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);

	if (m_ActiveScene)
		m_ActiveScene->SetActive(false);

	m_ActiveScene = scene;
	m_ActiveScene->SetActive(true);


	return *scene;
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	m_Scenes.erase(std::remove_if(m_Scenes.begin(), m_Scenes.end(), [&](std::shared_ptr<Scene> pScene) 
	{
		return pScene->GetName() == name;
	}));
}
