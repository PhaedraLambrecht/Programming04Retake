#pragma once
#include "Commands/Command.h"

namespace dae
{
	class GameObject;

	class SkipLevelCommand final : public BaseCommand
	{
	public:

		explicit SkipLevelCommand(GameObject* player);
		~SkipLevelCommand() override;

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


