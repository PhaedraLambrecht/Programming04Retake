#include "StartButtonCommand.h"
#include "Scene/SceneManager.h"


dae::StartButtonCommand::StartButtonCommand()
{
//	m_pScene = &dae::SceneManager::GetInstance().GetActiveScene();
}

void dae::StartButtonCommand::Execute()
{
	SceneManager::GetInstance().SwitchScene("Demo");
}
