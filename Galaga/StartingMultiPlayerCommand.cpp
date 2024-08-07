#include "StartingMultiPlayerCommand.h"
#include "StartMultiPlayerComponent.h"

dae::StartingMultiPlayerCommand::StartingMultiPlayerCommand()
	:BaseCommand()
{
}

void dae::StartingMultiPlayerCommand::Execute()
{
	StartMultiPlayerComponent startMultiPlayerComponent{ };
	startMultiPlayerComponent.LoadLevels();
}

