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
		~PlayManualComponent() override = default;

		PlayManualComponent(const PlayManualComponent& other) = delete;
		PlayManualComponent(PlayManualComponent&& other) = delete;
		PlayManualComponent& operator=(const PlayManualComponent& other) = delete;
		PlayManualComponent& operator=(PlayManualComponent&& other) = delete;




		void RenderUI() override;
		void AddExplanation(std::string controls, std::string description);


	private:

		void SetText();

		bool m_isTextUpdated;
		std::string m_manualText;

		std::map<std::string, std::string> m_explanation;
		std::vector<std::string> m_explanationOrder;
	};
}


#endif // !ENGINE2D_PLAYERMANUALCOMPONENT_H