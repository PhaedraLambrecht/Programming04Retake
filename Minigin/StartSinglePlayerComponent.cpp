#include "StartSinglePlayerComponent.h"

// Input includes
#include "Input/InputManager.h"

// Scene includes
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

// Sound includes
#include "Sound/SoundManager.h"
#include "Sound/SDLSoundSystem.h"

// GameLoader includes
#include "GameLoader.h"


// General includes
#include "GameObject.h"
#include "LoadLevelFromFile.h"

// Components includes
#include "Componennts/ImageComponent.h"
#include "Componennts/ImageRenderComponent.h"



dae::StartSinglePlayerComponent::StartSinglePlayerComponent()
	:m_levelFile{}
{
	m_texturemaps = 
	{
		{1, "iceBlock.png" },
		{3,  "snoBee1.png" },
		{4,  "diamondBlock.png" },
		{5,  "penguin2Down.png" },
	};
}


void dae::StartSinglePlayerComponent::LoadLevels()
{
	auto& inputManager = dae::InputManager::GetInstance();
	unsigned controller = inputManager.AddController();


	LoadSinglePlayerScene("level1", { 250,100 }, controller);
	LoadSinglePlayerScene("level2", { 250,125 }, controller);
	LoadSinglePlayerScene("level3", { 250,150 }, controller);

	dae::SceneManager::GetInstance().SetActiveScene("level1");

}

void dae::StartSinglePlayerComponent::LoadSinglePlayerScene(const std::string& levelName, const glm::vec2 offset, unsigned int controller)
{
	GameLoader loader;
	m_levelFile = "../Data/" + levelName + ".txt";

	// Create scene
	auto& scene = dae::SceneManager::GetInstance().CreateScene(levelName);
	scene.SetActive(true);

	// Setup input and sound
	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();

	{
		dae::SoundData backgroundMusic = { 1, 0.1f,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true };
		dae::SoundData schootEffect = { 1, 1,  dae::SoundData::SoundType::SoundEffect, "Sound/Shoot.mp3", true };
		loader.LoadSound(sounds, backgroundMusic);
		loader.LoadSound(sounds, schootEffect);
	}



	// Load Level boarder
	const auto& background = loader.LoadLevelBorder(scene);
	

	// Load level
	dae::LoadLevelFromFile levelLoader{};
	levelLoader.LoadLevel(m_levelFile);
	std::vector<std::vector<int>> leveldata = levelLoader.GetLevel();


	std::vector<dae::GameObject*> players{};
	for (int i{}; i < int(leveldata.size()); ++i)
	{
		for (int x{}; x < int(leveldata[i].size()); ++x)
		{
			int objectType = leveldata[i][x];
			if (objectType == 0)
			{
				continue;
			}


			if (objectType == 5)
			{
				const auto& player = loader.LoadPlayer(scene, 0, controller, m_texturemaps.at(objectType), { x, i }, offset);

				players.push_back(player.get());
				scene.AddPlayer(player.get());
			}
		}
	}


	for (int i{}; i < int(leveldata.size()); ++i)
	{
		for (int x{}; x < int(leveldata[i].size()); ++x)
		{
			int objectType = leveldata[i][x];
			if (objectType == 0)
			{
				continue;
			}

			if (objectType == 1)
			{
				const auto& wall = loader.LoadWall(scene, { x, i }, m_texturemaps.at(objectType), offset, players);
				scene.AddWalls(wall.get());
			}
			if (objectType == 2)
			{
				loader.LoadEggWall(scene, { x, i }, m_texturemaps.at(objectType - 1), offset, background.get(), players);
			}	
			if (objectType == 3)
			{
				const auto& enemy = loader.LoadEnemy(scene, { x, i }, m_texturemaps.at(objectType), players, background, offset);
				scene.AddEnemy(enemy.get());
			}
			if (objectType == 4)
			{
				const auto& wall = loader.LoadDiamondWall(scene, { x, i }, m_texturemaps.at(objectType), offset, players);
				scene.AddWalls(wall.get());
			}
		}
	}

}
