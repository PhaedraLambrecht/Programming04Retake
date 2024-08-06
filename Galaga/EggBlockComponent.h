#ifndef ENGINE2D_EGGBLOCKCOMPONENT_H
#define ENGINE2D_EGGBLOCKCOMPONENT_H
#include "BlockComponent.h"
#include "glm/glm.hpp"
#include <vector>

namespace dae
{
	class GameObject;

	class EggBlockComponent final : public BlockComponent
	{
	public:

		EggBlockComponent(GameObject* owner);
		~EggBlockComponent() = default;

		EggBlockComponent(const EggBlockComponent& other) = delete;
		EggBlockComponent(EggBlockComponent&& other) = delete;
		EggBlockComponent& operator=(const EggBlockComponent& other) = delete;
		EggBlockComponent& operator=(EggBlockComponent&& other) = delete;

		void Update() override;
		void SetPlayer(std::vector<GameObject*> players);
		void SetBackground(GameObject* background);

	private:

		std::shared_ptr<dae::GameObject> CreateEnemyObject(const glm::vec2& position, std::vector<GameObject*> players, const dae::GameObject* backGroundImage);

		float m_hatchingTime;
		float m_currentTime;
		bool m_hasHatched;

		std::vector<GameObject*> m_pPlayers;
		GameObject* m_pBackground;
	};
}


#endif // !ENGINE2D_EGGBLOCKCOMPONENT_H