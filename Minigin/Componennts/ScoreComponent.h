#ifndef ENGINE2D_SCORECOMPONENT_H
#define ENGINE2D_SCORECOMPONENT_H
#include "BaseComponent.h"
#include "Events/Event.h"


namespace dae
{
	class GameObject;
	class TextComponent;

	class ScoreComponent final : public BaseComponent
	{
	public:

		ScoreComponent(GameObject* owner);
		~ScoreComponent();


		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;




		void SetPlayerIndex(unsigned playerIndex);


	private:

		void UpdateText();
		void UpdateScore(const Event* e);


		const int m_RewardAmount;
		unsigned int m_PlayerIndex;
		int m_PointsEarned;

		TextComponent* m_pTextComponent;
	};
}

#endif // !ENGINE2D_SCORECOMPONENT_H
