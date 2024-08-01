#include "LoadLevelFromFile.h"
#include <iostream>
#include <sstream>


void dae::LoadLevelFromFile::LoadLevel(const std::string& file)
{
	std::ifstream fileStream(file);
	if (!fileStream.is_open()) 
	{
		throw std::runtime_error("Error: Could not open file " + file);
	}

	std::string line;
	std::vector<int> row;

	while (std::getline(fileStream, line))
	{
		if (line.empty())
		{
			continue;
		}

		row.clear();
		std::istringstream iss(line);
		int value;

		while (iss >> value)
		{
			if (value >= 0 && value <= 5)
			{
				row.push_back(value);
			}
		}
		
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
