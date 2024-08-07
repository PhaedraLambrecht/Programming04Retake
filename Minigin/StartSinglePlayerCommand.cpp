#include "StartSinglePlayerCommand.h"
#include "StartSinglePlayerComponent.h"
#include <iostream>


dae::StartSinglePlayerCommand::StartSinglePlayerCommand()
	:BaseCommand()
{
}

void dae::StartSinglePlayerCommand::Execute()
{
	StartSinglePlayerComponent startSinglePlayerComponent{ };
	startSinglePlayerComponent.LoadLevels();
}
