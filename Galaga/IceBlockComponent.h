#pragma once
#include "Componennts/BaseComponent.h"
#include <glm/glm.hpp>

namespace dae
{
	class BlockStates;

	class IceBlockComponent final : public BaseComponent
	{
	public:

		IceBlockComponent(GameObject* owner);
		~IceBlockComponent() override;

		IceBlockComponent(const IceBlockComponent& other) = delete;
		IceBlockComponent(IceBlockComponent&& other) = delete;
		IceBlockComponent& operator=(const IceBlockComponent& other) = delete;
		IceBlockComponent& operator=(IceBlockComponent&& other) = delete;


		void Update() override;
		void SetDirection(glm::vec2 direction);

	private:

		glm::vec2 m_Direction;
		std::unique_ptr<BlockStates> m_pBlockState;
	};
}


