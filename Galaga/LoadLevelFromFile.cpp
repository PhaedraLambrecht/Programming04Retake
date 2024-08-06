#include "LoadLevelFromFile.h"
#include <iostream>
#include <sstream>


void dae::LoadLevelFromFile::LoadLevel(const std::string& filePath)
{
	ClearLevel();  // Ensure the level data is cleared before loading a new level


	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		throw std::runtime_error("Error: Could not open file " + filePath);
	}


	std::string rowString;
	while (std::getline(fileStream, rowString))
	{
		if (rowString.empty())
		{
			continue;
		}

		auto parsedRow = ParseLine(rowString);
		if (!parsedRow.empty())
		{
			m_Level.push_back(std::move(parsedRow));
		}
	}
}

std::vector<std::vector<int>> dae::LoadLevelFromFile::GetLevel() const
{
	return m_Level;
}

void dae::LoadLevelFromFile::ClearLevel()
{
	if (m_Level.empty())
	{
		return;
	}

	m_Level.clear();
}

std::vector<int> dae::LoadLevelFromFile::ParseLine(const std::string& line) const
{
	std::vector<int> row;
	std::istringstream iss(line);
	int tileValue;

	while (iss >> tileValue)
	{
		if ( !(tileValue < 0 || tileValue > 6) )
		{
			row.push_back(tileValue);
		}
	}

	return row;
}
