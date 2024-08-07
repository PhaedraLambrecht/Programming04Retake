#ifndef ENGINE2D_DIAMONDBLOCKCOMPONENT_H
#define ENGINE2D_DIAMONDBLOCKCOMPONENT_H
#include "BlockComponent.h"
#include "Componennts/CollisionComponent.h"
#include <vector>


namespace dae
{
	class DiamondBlockComponent final : public BlockComponent
	{
	public:

		DiamondBlockComponent(GameObject* owner);
		~DiamondBlockComponent() override = default;

		DiamondBlockComponent(const DiamondBlockComponent& other) = delete;
		DiamondBlockComponent(DiamondBlockComponent&& other) = delete;
		DiamondBlockComponent& operator=(const DiamondBlockComponent& other) = delete;
		DiamondBlockComponent& operator=(DiamondBlockComponent&& other) = delete;



		void Initialize(std::vector<GameObject*> players);
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);
		void OnDeath(const Event* e);
		bool DoDamage(GameObject* player);


	private:


		int m_Health;

		std::vector<dae::GameObject*> m_pPlayer;

		void AddPointsToPlayers(GameObject* player);
		void NotifyOnDeath();
	};
}

#endif // !ENGINE2D_DIAMONDBLOCKCOMPONENT_H