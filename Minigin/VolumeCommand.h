#pragma once
#include "Commands/Command.h"

namespace dae
{
	class DecreaseVolumeCommand final : public BaseCommand
	{
	public:

		DecreaseVolumeCommand() = default;
		virtual ~DecreaseVolumeCommand() = default;

		DecreaseVolumeCommand(const DecreaseVolumeCommand& other) = delete;
		DecreaseVolumeCommand(DecreaseVolumeCommand&& other) = delete;
		DecreaseVolumeCommand& operator=(const DecreaseVolumeCommand& other) = delete;
		DecreaseVolumeCommand& operator=(DecreaseVolumeCommand&& other) = delete;


		virtual void Execute() override;
	};


	class IncreaseVolumeCommand final : public BaseCommand
	{
	public:

		IncreaseVolumeCommand() = default;
		virtual ~IncreaseVolumeCommand() = default;

		IncreaseVolumeCommand(const IncreaseVolumeCommand& other) = delete;
		IncreaseVolumeCommand(IncreaseVolumeCommand&& other) = delete;
		IncreaseVolumeCommand& operator=(const IncreaseVolumeCommand& other) = delete;
		IncreaseVolumeCommand& operator=(IncreaseVolumeCommand&& other) = delete;


		virtual void Execute() override;
	};

}


