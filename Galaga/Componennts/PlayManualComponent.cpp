#include "PlayManualComponent.h"
#include "imgui.h"

dae::PlayManualComponent::PlayManualComponent(GameObject* owner)
	:BaseComponent(owner)
	, m_manualText{}
	,m_isTextUpdated{true}
{

}

void dae::PlayManualComponent::RenderUI()
{
	ImGui::Begin("Play Manual");

	if(!m_isTextUpdated)
		SetText();


	ImGui::Text(m_manualText.c_str());
	ImGui::End();
}

void dae::PlayManualComponent::AddExplanation(std::string controls, std::string description)
{
	m_explanation[controls] =  description;
	m_explanationOrder.push_back(controls);
	m_isTextUpdated = false;
}


void dae::PlayManualComponent::SetText()
{
	m_manualText.clear();
	for (const auto& control : m_explanationOrder)
	{
		m_manualText += control + ": " + m_explanation[control] + '\n';
	}
	m_isTextUpdated = true;
}
