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
	if (!SceneManager::GetInstance().GetActiveScene().m_playerName.empty())
	{
		StartMultiPlayerComponent startMultiPlayerComponent{ };
		startMultiPlayerComponent.LoadLevels();
	}
}

