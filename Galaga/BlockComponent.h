#pragma once
#include "Componennts/BaseComponent.h"

namespace dae
{
	class GameObject;

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
	};
}

