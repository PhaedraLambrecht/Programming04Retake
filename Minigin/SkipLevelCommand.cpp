#include "SkipLevelCommand.h" 
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "GameObject.h"
#include "Componennts/HealthComponent.h"
#include <string>


dae::SkipLevelCommand::SkipLevelCommand(GameObject* player) 
    : BaseCommand()
    , m_pPlayer(player)
{
}


void dae::SkipLevelCommand::Execute() 
{
    SwitchtoNextScene();
}

void dae::SkipLevelCommand::SwitchtoNextScene() 
{
	// Get the active scene's name 
    const std::string activeSceneName = dae::SceneManager::GetInstance().GetActiveScene().GetName();
   
    // Find the current scene in the list of scenes
    auto& scenes = SceneManager::GetInstance().m_pScenes;
    auto it = std::find_if(scenes.begin(), scenes.end(),
        [&activeSceneName](const std::shared_ptr<Scene>& scene)
        {
            return scene->GetName() == activeSceneName;
        });

    // Move to the next scene
    if (it != scenes.end())
    {
        ++it; // Move to the next scene in the vector
        if (it == scenes.end())
        {
            // If we reach the end, wrap around to the first scene
            it = scenes.begin();
        }


        // Check if the next scene is "StartScreen" and skip it if so
        if ((*it)->GetName() == "StartScreen")
        {
            ++it; // Move to the next scene
            if (it == scenes.end())
            {
                // If we reach the end again, wrap around to the first scene
                it = scenes.begin();
            }
        }


        // Switch to the new scene
        SceneManager::GetInstance().SwitchScene((*it)->GetName());
    }
}