#include "LoadLevelFromFile.h"
#include <iostream>
#include <sstream>


void dae::LoadLevelFromFile::LoadLevel(const std::string& file)
{
	ClearLevel();  // Ensure the level data is cleared before loading a new level


	std::ifstream fileStream(file);
	if (!fileStream.is_open())
	{
		throw std::runtime_error("Error: Could not open file " + file);
	}

	std::string line;
	while (std::getline(fileStream, line))
	{
		if (line.empty())
		{
			continue;
		}

		auto row = ParseLine(line);
		if (!row.empty())
		{
			m_Level.push_back(std::move(row));
		}
	}
}

std::vector<std::vector<int>> dae::LoadLevelFromFile::GetLevel() const
{
	return m_Level;
}

void dae::LoadLevelFromFile::ClearLevel()
{
	m_Level.clear();
}

std::vector<int> dae::LoadLevelFromFile::ParseLine(const std::string& line) const
{
	std::vector<int> row;
	std::istringstream iss(line);
	int value;

	while (iss >> value)
	{
		if ( !(value < 0 || value > 5) )
		{
			row.push_back(value);
		}
	}

	return row;
}
