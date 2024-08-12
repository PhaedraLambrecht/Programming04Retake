#include "StartSinglePlayerCommand.h"
#include "StartSinglePlayerComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

dae::StartSinglePlayerCommand::StartSinglePlayerCommand()
	:BaseCommand()
{
}

void dae::StartSinglePlayerCommand::Execute()
{
	if (!SceneManager::GetInstance().GetActiveScene().GetPlayerName().empty())
	{
		StartSinglePlayerComponent startSinglePlayerComponent{ };
		startSinglePlayerComponent.LoadLevels();
	}
}
