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

		void LoadLevel(const std::string& file);
		std::vector<std::vector<int>> GetLevel() const;


	private:
		
		std::vector<std::vector<int>> m_Level;
	};
}

#endif // !ENGINE2D_LOADLEVELFROMFILECOMPONENT_H