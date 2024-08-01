#include "Command.h"
#include <iostream>

dae::BaseCommand::~BaseCommand()
{
	std::cout << "BaseCommand\n";
}
