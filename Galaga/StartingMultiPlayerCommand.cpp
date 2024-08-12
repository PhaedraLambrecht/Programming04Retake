#include "StartingMultiPlayerCommand.h"
#include "StartMultiPlayerComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

dae::StartingMultiPlayerCommand::StartingMultiPlayerCommand()
	:BaseCommand()
{
}

void dae::StartingMultiPlayerCommand::Execute()
{
	if (!SceneManager::GetInstance().GetActiveScene().GetPlayerName().empty())
	{
		StartMultiPlayerComponent startMultiPlayerComponent{ };
		startMultiPlayerComponent.LoadLevels();
	}
}

