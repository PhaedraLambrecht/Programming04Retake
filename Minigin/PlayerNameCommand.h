#pragma once
#include "Commands/Command.h"
#include <string>


namespace dae
{
	class NameComponent;
	class GameObject;

	class PlayerNameDownCommand final : public BaseCommand
	{
	public:

		PlayerNameDownCommand(GameObject* player);
		~PlayerNameDownCommand() = default;

		PlayerNameDownCommand(const PlayerNameDownCommand& other) = delete;
		PlayerNameDownCommand(PlayerNameDownCommand&& other) = delete;
		PlayerNameDownCommand& operator=(const PlayerNameDownCommand& other) = delete;
		PlayerNameDownCommand& operator=(PlayerNameDownCommand&& other) = delete;

		void Execute() override;

	private:

		NameComponent* m_pNameComponent;

	};

	class PlayerNameUpCommand final : public BaseCommand
	{
	public:

		PlayerNameUpCommand(GameObject* player);
		~PlayerNameUpCommand() = default;

		PlayerNameUpCommand(const PlayerNameUpCommand& other) = delete;
		PlayerNameUpCommand(PlayerNameUpCommand&& other) = delete;
		PlayerNameUpCommand& operator=(const PlayerNameUpCommand& other) = delete;
		PlayerNameUpCommand& operator=(PlayerNameUpCommand&& other) = delete;

		void Execute() override;

	private:

		NameComponent* m_pNameComponent;

	};



	class PlayerNameEnterCommand final : public BaseCommand
	{
	public:

		PlayerNameEnterCommand(GameObject* player);
		~PlayerNameEnterCommand() = default;

		PlayerNameEnterCommand(const PlayerNameEnterCommand& other) = delete;
		PlayerNameEnterCommand(PlayerNameEnterCommand&& other) = delete;
		PlayerNameEnterCommand& operator=(const PlayerNameEnterCommand& other) = delete;
		PlayerNameEnterCommand& operator=(PlayerNameEnterCommand&& other) = delete;

		void Execute() override;

	private:

		NameComponent* m_pNameComponent;

	};
}

