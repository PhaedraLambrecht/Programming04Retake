#include "PlayManualComponent.h"
#include "imgui.h"

dae::PlayManualComponent::PlayManualComponent(GameObject* owner)
	:BaseComponent(owner)
	, m_Text{}
	,m_IstextUpdated{true}
{

}

dae::PlayManualComponent::~PlayManualComponent()
{
	std::cout << "PlayManualComponent\n";
}

void dae::PlayManualComponent::RenderUI()
{
	ImGui::Begin("Play Manual");

	if(!m_IstextUpdated)
		SetText();

	ImGui::Text(m_Text.c_str());

	ImGui::End();
}

void dae::PlayManualComponent::AddExplanation(std::string controls, std::string description)
{
	m_constolsExplanation[controls] =  description;
	insertionOrder.push_back(controls);
	m_IstextUpdated = false;
}


void dae::PlayManualComponent::SetText()
{
	for (const std::string& key : insertionOrder)
	{
		const std::string& value = m_constolsExplanation[key];
		m_Text += key + ": " + value + '\n';
	}
	m_IstextUpdated = true;
}
