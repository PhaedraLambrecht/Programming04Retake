#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	class BlockStates final
	{
	public:

		BlockStates() = default;
		~BlockStates() = default;

		BlockStates(const BlockStates& other) = delete;
		BlockStates(BlockStates&& other) = delete;
		BlockStates& operator=(const BlockStates& other) = delete;
		BlockStates& operator=(BlockStates&& other) = delete;

		virtual BlockStates* Update();
		virtual BlockStates* HandleInput(glm::vec2 direction);
		void onEnter(GameObject* gameObject);
		void onExit();
	};
}

