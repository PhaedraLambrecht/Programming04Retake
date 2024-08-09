#ifndef ENGINE2D_BLOCKCOMPONENT_H
#define ENGINE2D_BLOCKCOMPONENT_H
#include "Componennts/BaseComponent.h"
#include <vector>

namespace dae
{
	class GameObject;
	class CollisionComponent;

	class BlockComponent : public BaseComponent
	{
	public:

		BlockComponent(GameObject* owner);
		virtual ~BlockComponent() = default;

		BlockComponent(const BlockComponent& other) = delete;
		BlockComponent(BlockComponent&& other) = delete;
		BlockComponent& operator=(const BlockComponent& other) = delete;
		BlockComponent& operator=(BlockComponent&& other) = delete;


		void Update() override;
		void SetPlayer(std::vector<dae::GameObject*> player);

	private:

		std::vector<dae::GameObject*> m_player;
		CollisionComponent* m_CollisionComponent;
	};
}

#endif // !ENGINE2D_BLOCKCOMPONENT_H
