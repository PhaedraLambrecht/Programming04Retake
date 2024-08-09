#pragma once
#include "Componennts/BaseComponent.h"
#include <string>
#include <vector>

namespace dae
{
	class TextComponent;

	class NameComponent final : public BaseComponent
	{
	public:

		NameComponent(GameObject* owner);
		~NameComponent() = default;

		NameComponent(const NameComponent& other) = delete;
		NameComponent(NameComponent&& other) = delete;
		NameComponent& operator=(const NameComponent& other) = delete;
		NameComponent& operator=(NameComponent&& other) = delete;


		const std::string& GetName() const;


		void AddLetterToName();
		void CycleLetters(int direction); // direction: 1 for forward, -1 for backward


	private:

		std::string m_name;
		std::vector<char> m_letters;
		int m_currentLetterIndex;




		TextComponent* m_pTextComponent;
	};
}