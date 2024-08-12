#include "NameComponent.h"
#include "NameComponent.h"
#include "GameObject.h"
#include "Componennts/TextComponent.h"

#include <iostream>


dae::NameComponent::NameComponent(GameObject* owner)
    : BaseComponent(owner)
    , m_currentLetterIndex{ 0 }
    , m_letters{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
            'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
            'U', 'V', 'W', 'X', 'Y', 'Z' }
{
    m_pTextComponent = owner->GetComponent<dae::TextComponent>();
    if (m_pTextComponent == nullptr)
    {
        // Handle the error, for example by throwing an exception
        throw std::runtime_error("TextComponent not found");
    }



    // Set the default to "---"
    std::string displayName = m_name;
    for (int i = int(m_name.size()); i < 3; i++)
    {
        displayName += "-";
    }
    if (m_pTextComponent != nullptr)
    {
        m_pTextComponent->SetText(displayName);
    }
}

const std::string& dae::NameComponent::GetName() const
{
	return m_name;
}

void dae::NameComponent::AddLetterToName()
{
    bool isFull = m_name.size() < 3;
    bool isLettersEmpty = m_letters.empty();
    bool isCurrentLetterIndexValid = m_currentLetterIndex >= 0 && m_currentLetterIndex < static_cast<int>(m_letters.size());


    if (isFull && !isLettersEmpty && isCurrentLetterIndexValid)
    {
        m_name += m_letters[m_currentLetterIndex];
        ++m_currentLetterIndex; // Move to the next letter


        if (m_currentLetterIndex >= static_cast<int>(m_letters.size()))
        {
            m_currentLetterIndex = 0;
        }

        if (m_name.size() == 3)
        {
            SceneManager::GetInstance().GetActiveScene().m_playerName = GetName();
        }

    }


    // Set the default to "---"
    std::string displayName = m_name;
    for (int i = int(m_name.size()); i < 3; i++)
    {
        displayName += "-";
    }
    if (m_pTextComponent != nullptr)
    {
        m_pTextComponent->SetText(displayName);
    }
}

void dae::NameComponent::CycleLetters(int direction)
{
    m_currentLetterIndex = (m_currentLetterIndex + direction) % m_letters.size();
    if (m_currentLetterIndex < 0)
    {
        m_currentLetterIndex += int(m_letters.size());
    }


    // Set the default to "---"
    std::string displayName = m_name;
    for (int i = int(m_name.size()); i < 3; i++)
    {
        displayName += "-";
    }

    displayName[m_name.size()] = m_letters[m_currentLetterIndex];
    if (m_pTextComponent != nullptr)
    {
        m_pTextComponent->SetText(displayName);
    }
}

