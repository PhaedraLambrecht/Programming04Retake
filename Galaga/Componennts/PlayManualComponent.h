#ifndef ENGINE2D_PLAYERMANUALCOMPONENT_H
#define ENGINE2D_PLAYERMANUALCOMPONENT_H
#include "Componennts/BaseComponent.h"
#include <iostream>
#include <map>
#include <vector>


namespace dae
{
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
		// todo: make this accept a multi map  to make life easier
		void AddExplanation(std::string controls, std::string description);


	private:

		void SetText();

		bool m_IstextUpdated;
		std::string m_Text;

		std::map<std::string, std::string> m_constolsExplanation;
		std::vector<std::string> insertionOrder;
	};
}


#endif // !ENGINE2D_PLAYERMANUALCOMPONENT_H