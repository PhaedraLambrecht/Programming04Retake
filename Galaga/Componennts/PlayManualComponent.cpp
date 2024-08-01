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

void dae::PlayManualComponent::AddExplanation(PlayerManual playermanual)
{
	m_constolsExplanation.controls =  playermanual.controls;
	m_constolsExplanation.description = playermanual.description;
	m_InsertionOrder.push_back(playermanual);
	m_IstextUpdated = false;
}


void dae::PlayManualComponent::SetText()
{
	for (const PlayerManual& keybind : m_InsertionOrder)
	{
		m_Text += keybind.controls + ": " + keybind.description + '\n';
	}
	m_IstextUpdated = true;
}
