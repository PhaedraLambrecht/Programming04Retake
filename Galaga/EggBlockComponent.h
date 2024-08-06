#pragma once
#include "BlockComponent.h"
#include "glm/glm.hpp"
#include <vector>

namespace dae
{
	class GameObject;

	class EggBlockComponent : public BlockComponent
	{
	public:

		EggBlockComponent(GameObject* owner);
		virtual ~EggBlockComponent() = default;

		EggBlockComponent(const EggBlockComponent& other) = delete;
		EggBlockComponent(EggBlockComponent&& other) = delete;
		EggBlockComponent& operator=(const EggBlockComponent& other) = delete;
		EggBlockComponent& operator=(EggBlockComponent&& other) = delete;

		void Update() override;
		void SetPlayer(std::vector<GameObject*> players);
		void SetBackground(GameObject* background);

	private:

		std::shared_ptr<dae::GameObject> CreateEnemyObject(const glm::vec2& position, std::vector<GameObject*> players, const dae::GameObject* backGroundImage);

		float m_HatchingTime;
		float m_CurrentTime;
		bool m_HasHatched;

		std::vector<GameObject*> m_Players;
		GameObject* m_Background;
	};
}

