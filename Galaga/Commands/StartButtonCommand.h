#ifndef ENGINE2D_STARTBUTTONCOMMAND_H
#define ENGINE2D_STARTBUTTONCOMMAND_H
#include "Commands/Command.h"

namespace dae
{
	class bMenuComponent;
	class GameObject;

	class StartButtonCommand : public BaseCommand
	{
	public:

		StartButtonCommand();
		~StartButtonCommand() override = default;

		// Copy and move
		StartButtonCommand(const StartButtonCommand& other) = delete;
		StartButtonCommand(StartButtonCommand&& other) = delete;
		StartButtonCommand& operator=(const StartButtonCommand& other) = delete;
		StartButtonCommand& operator=(StartButtonCommand&& other) = delete;

		void Execute() override;


	private:

		//Scene* m_pScene{ nullptr };
	};
}


#endif // ENGINE2D_STARTBUTTONCOMMAND