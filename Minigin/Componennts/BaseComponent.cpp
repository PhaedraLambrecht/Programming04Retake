#include "BaseComponent.h"
#include "GameObject.h"
#include <iostream>


namespace dae
{
	BaseComponent::BaseComponent(GameObject* Owner)
		:m_pOwner{ Owner }
	{
	}

	BaseComponent::~BaseComponent()
	{
		std::cout << "BaseComponent\n";
	}


	GameObject* BaseComponent::GetOwner() const
	{
		return m_pOwner;
	}

	void BaseComponent::Render() const
	{
	}

	void BaseComponent::RenderUI()
	{
	}

	void BaseComponent::FixedUpdate(const float)
	{

	}

	void BaseComponent::Update()
	{
	}
}