#pragma once
#include "Commands/Command.h"
#include <string>


namespace dae
{
	class NameComponent;
	class GameObject;

	class PlayerNameCommand final : public BaseCommand
	{
	public:

		PlayerNameCommand(GameObject* player);
		~PlayerNameCommand() = default;

		PlayerNameCommand(const PlayerNameCommand& other) = delete;
		PlayerNameCommand(PlayerNameCommand&& other) = delete;
		PlayerNameCommand& operator=(const PlayerNameCommand& other) = delete;
		PlayerNameCommand& operator=(PlayerNameCommand&& other) = delete;

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

	class FinalPlayerNmaeCommand final : public BaseCommand
	{
	public:

		FinalPlayerNmaeCommand(GameObject* player);
		~FinalPlayerNmaeCommand() = default;

		FinalPlayerNmaeCommand(const FinalPlayerNmaeCommand& other) = delete;
		FinalPlayerNmaeCommand(FinalPlayerNmaeCommand&& other) = delete;
		FinalPlayerNmaeCommand& operator=(const FinalPlayerNmaeCommand& other) = delete;
		FinalPlayerNmaeCommand& operator=(FinalPlayerNmaeCommand&& other) = delete;

		void Execute() override;

	private:

		NameComponent* m_pNameComponent;
	};

}

