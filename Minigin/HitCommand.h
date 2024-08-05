#pragma once
#include "commands/Command.h"


namespace dae
{
	class HitComponennt;
	class GameObject;

	class HitCommand final : public BaseCommand
	{
	public:

		HitCommand(GameObject* actor, int playerIndex);
		~HitCommand() override;

		HitCommand(const HitCommand& other) = delete;
		HitCommand(HitCommand&& other) = delete;
		HitCommand& operator=(const HitCommand& other) = delete;
		HitCommand& operator=(HitCommand&& other) = delete;


		void Execute() override;

	private:

		HitComponennt* m_pSchootComponent{ nullptr };
	};
}


