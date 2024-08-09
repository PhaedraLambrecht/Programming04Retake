#ifndef ENGINE2D_EGGBLOCKCOMPONENT_H
#define ENGINE2D_EGGBLOCKCOMPONENT_H
#include "BlockComponent.h"
#include "Componennts/CollisionComponent.h"
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


		void Initialize(std::vector<GameObject*> players);
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);
		void OnDeath(const Event* e);
		bool DoDamage(GameObject* player);

		void Update() override;
		void SetBackground(GameObject* background);


	private:

		std::shared_ptr<dae::GameObject> CreateEnemyObject(const glm::vec2& position, std::vector<GameObject*> players, const dae::GameObject* backGroundImage);

		float m_hatchingTime;
		float m_currentTime;
		bool m_hasHatched;

		GameObject* m_pBackground;

		int m_Health;

		void AddPointsToPlayers(GameObject* player);
		void NotifyOnDeath();
	};
}


#endif // !ENGINE2D_EGGBLOCKCOMPONENT_H