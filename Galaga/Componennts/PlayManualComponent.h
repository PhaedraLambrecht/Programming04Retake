#ifndef ENGINE2D_PLAYERMANUALCOMPONENT_H
#define ENGINE2D_PLAYERMANUALCOMPONENT_H
#include "Componennts/BaseComponent.h"
#include <iostream>
#include <map>
#include <vector>


namespace dae
{
	struct PlayerManual
	{
		std::string controls;
		std::string description;
	};

	class PlayManualComponent final : public BaseComponent
	{
	public:

		PlayManualComponent(GameObject* owner);
		~PlayManualComponent() override;

		PlayManualComponent(const PlayManualComponent& other) = delete;
		PlayManualComponent(PlayManualComponent&& other) = delete;
		PlayManualComponent& operator=(const PlayManualComponent& other) = delete;
		PlayManualComponent& operator=(PlayManualComponent&& other) = delete;




		void RenderUI() override;
		void AddExplanation(PlayerManual playermanual);


	private:

		void SetText();

		bool m_IstextUpdated;
		std::string m_Text;

		PlayerManual m_constolsExplanation;
		std::vector<PlayerManual> m_InsertionOrder;
	};
}


#endif // !ENGINE2D_PLAYERMANUALCOMPONENT_H