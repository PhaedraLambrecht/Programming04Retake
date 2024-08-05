#pragma once
#include "BlockComponent.h"
#include "Componennts/CollisionComponent.h"



namespace dae
{
	class DiamondBlockComponent final : public BlockComponent
	{
	public:

		DiamondBlockComponent(GameObject* owner);
		virtual ~DiamondBlockComponent() = default;

		DiamondBlockComponent(const DiamondBlockComponent& other) = delete;
		DiamondBlockComponent(DiamondBlockComponent&& other) = delete;
		DiamondBlockComponent& operator=(const DiamondBlockComponent& other) = delete;
		DiamondBlockComponent& operator=(DiamondBlockComponent&& other) = delete;


		void Initialize(std::shared_ptr<GameObject> player);
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);
		void OnDeath(const Event* e);
		bool DoDamage();

	private:


		int m_Health;
		GameObject* m_pPlayer;


		void AddPointsAndNotifyDeath();
	};
}

