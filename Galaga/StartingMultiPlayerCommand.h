#pragma once
#include "Commands/Command.h"

namespace dae
{
	class GameObject;

	class StartingMultiPlayerCommand final : public BaseCommand
	{
	public:

		StartingMultiPlayerCommand();
		~StartingMultiPlayerCommand() override = default;

		StartingMultiPlayerCommand(const StartingMultiPlayerCommand& other) = delete;
		StartingMultiPlayerCommand(StartingMultiPlayerCommand&& other) = delete;
		StartingMultiPlayerCommand& operator=(const StartingMultiPlayerCommand& other) = delete;
		StartingMultiPlayerCommand& operator=(StartingMultiPlayerCommand&& other) = delete;

		void Execute() override;
	};
}

