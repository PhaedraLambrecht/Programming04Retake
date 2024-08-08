#include "NameComponent.h"
#include "GameObject.h"

#include <iostream>

dae::NameComponent::NameComponent(GameObject* owner)
	:BaseComponent(owner)
	, m_currentLetterIndex{ 0 }
    , m_letters { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
            'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
            'U', 'V', 'W', 'X', 'Y', 'Z' }
{
}

const std::string& dae::NameComponent::GetName() const
{
	return m_name;
}

void dae::NameComponent::AddLetterToName()
{
    if (!m_letters.empty() && m_currentLetterIndex >= 0 && m_currentLetterIndex < m_letters.size())
    {
        m_name += m_letters[m_currentLetterIndex];
    }

	std::cout << m_name << std::endl;
}

void dae::NameComponent::CycleLetters(int direction)
{
    m_currentLetterIndex = (m_currentLetterIndex + direction) % m_letters.size();
    if (m_currentLetterIndex < 0)
    {
        m_currentLetterIndex += int(m_letters.size());
    }
    std::cout << m_letters[m_currentLetterIndex] << std::endl;
}