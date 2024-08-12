#ifndef ENGINE2D_SKIPLEVELCOMMAND_H
#define ENGINE2D_SKIPLEVELCOMMAND_H
#include "Commands/Command.h"


namespace dae
{
	class GameObject;
	class Scene;

	class SkipLevelCommand final : public BaseCommand
	{
	public:

		explicit SkipLevelCommand(GameObject* player);
		~SkipLevelCommand() override = default;

		SkipLevelCommand(const SkipLevelCommand& other) = delete;
		SkipLevelCommand(SkipLevelCommand&& other) = delete;
		SkipLevelCommand& operator=(const SkipLevelCommand& other) = delete;
		SkipLevelCommand& operator=(SkipLevelCommand&& other) = delete;



		void Execute() override;


	private:

		GameObject* m_pPlayer;

		void SwitchtoNextScene();
	};
}


#endif // !ENGINE2D_SKIPLEVELCOMMAND_H