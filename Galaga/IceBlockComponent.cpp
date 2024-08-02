#include "IceBlockComponent.h"
#include "BlockStates.h"
#include "GameObject.h"
#include <memory>

dae::IceBlockComponent::IceBlockComponent(GameObject* owner)
	:BaseComponent(owner)
	,m_Direction{ 0,0 }
{
	m_pBlockState = std::make_unique<BlockStates>();
	m_pBlockState->onEnter(owner);
}

dae::IceBlockComponent::~IceBlockComponent()
{
}

void dae::IceBlockComponent::Update()
{
	auto state = m_pBlockState->HandleInput(m_Direction);
	auto state2 = m_pBlockState->Update();
	if (state != m_pBlockState.get() && state != nullptr)
	{
		m_pBlockState->onExit();
		m_pBlockState = std::unique_ptr<BlockStates>(state);
		m_pBlockState->onEnter(GetOwner());
	}
	else if (state2 != m_pBlockState.get() && state != nullptr)
	{
		m_pBlockState->onExit();
		m_pBlockState = std::unique_ptr<BlockStates>(state2);
		m_pBlockState->onEnter(GetOwner());
	}
}

void dae::IceBlockComponent::SetDirection(glm::vec2 direction)
{
	m_Direction = direction;
}
