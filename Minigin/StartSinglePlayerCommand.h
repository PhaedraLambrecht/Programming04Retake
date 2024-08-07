#pragma once
#include "Commands/Command.h"

namespace dae
{
	class GameObject;

	class StartSinglePlayerCommand final : public BaseCommand
	{
	public:

		 StartSinglePlayerCommand();
		~StartSinglePlayerCommand() override = default;

		StartSinglePlayerCommand(const StartSinglePlayerCommand& other) = delete;
		StartSinglePlayerCommand(StartSinglePlayerCommand&& other) = delete;
		StartSinglePlayerCommand& operator=(const StartSinglePlayerCommand& other) = delete;
		StartSinglePlayerCommand& operator=(StartSinglePlayerCommand&& other) = delete;

		void Execute() override;
	};
}

