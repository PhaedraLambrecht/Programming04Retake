#ifndef ENGINE2D_LOADLEVELFROMFILECOMPONENT_H
#include <fstream>
#include <vector>

namespace dae
{
	class LoadLevelFromFile final
	{
	public:

		LoadLevelFromFile() = default;
		~LoadLevelFromFile() = default;

		LoadLevelFromFile(const LoadLevelFromFile& other) = delete;
		LoadLevelFromFile(LoadLevelFromFile&& other) = delete;
		LoadLevelFromFile& operator=(const LoadLevelFromFile& other) = delete;
		LoadLevelFromFile& operator=(LoadLevelFromFile&& other) = delete;


		// Load level data from file
		void LoadLevel(const std::string& file);

		// Retrieve the parsed level data
		std::vector<std::vector<int>> GetLevel() const;


	private:
		
		std::vector<std::vector<int>> m_Level;


		// Helper functions for processing
		void ClearLevel();
		std::vector<int> ParseLine(const std::string& line) const;
		bool ValidateValue(int value) const;
	};
}

#endif // !ENGINE2D_LOADLEVELFROMFILECOMPONENT_H