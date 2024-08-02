#include "BlockStates.h"


dae::BlockStates* dae::BlockStates::Update()
{
	const auto state = this;
	return state;
}

dae::BlockStates* dae::BlockStates::HandleInput(glm::vec2)
{
	const auto state = this;
	return state;
}

void dae::BlockStates::onEnter(GameObject*)
{
}

void dae::BlockStates::onExit()
{
}
