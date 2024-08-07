#ifndef ENGINE2D_STARTINGMULTIPLAYERCOMMAND_H
#define ENGINE2D_STARTINGMULTIPLAYERCOMMAND_H
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

#endif //ENGINE2D_STARTINGMULTIPLAYERCOMMAND_H