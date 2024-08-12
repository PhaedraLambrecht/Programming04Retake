#ifndef ENGINE2D_SCHOOTCOMMAND_H
#define ENGINE2D_SCHOOTCOMMAND_H
#include "commands/Command.h"


namespace dae
{
	class SchootComponent;
	class GameObject;

	class SchootCommand final : public BaseCommand
	{
	public:

		SchootCommand(GameObject* actor, int playerIndex);
		~SchootCommand() override = default;

		SchootCommand(const SchootCommand& other) = delete;
		SchootCommand(SchootCommand&& other) = delete;
		SchootCommand& operator=(const SchootCommand& other) = delete;
		SchootCommand& operator=(SchootCommand&& other) = delete;



		void Execute() override;


	private:

		SchootComponent* m_pSchootComponent;
	};
}

#endif // !ENGINE2D_SCHOOTCOMMAND_H