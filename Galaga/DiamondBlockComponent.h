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


		void Update() override;

	};
}

#endif // !ENGINE2D_DIAMONDBLOCKCOMPONENT_H