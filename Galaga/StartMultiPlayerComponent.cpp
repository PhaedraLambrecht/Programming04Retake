#include "StartMultiPlayerComponent.h"

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


dae::StartMultiPlayerComponent::StartMultiPlayerComponent()
	:m_levelFile{}
{
	m_texturemaps =
	{
		{1, "iceBlock.png" },
		{3,  "snoBee1.png" },
		{4,  "diamondBlock.png" },
		{5,  "penguin2Down.png" },
		{6,  "penguinWalkDown.png" }
	};
}


void dae::StartMultiPlayerComponent::LoadLevels()
{
	auto& inputManager = dae::InputManager::GetInstance();
	unsigned controller1 = inputManager.AddController();
	unsigned controller2 = inputManager.AddController();


	LoadMultiPlayerScene("level4", { 250,100 }, controller1, controller2);
	LoadMultiPlayerScene("level5", { 250,125 }, controller1, controller2);
	LoadMultiPlayerScene("level6", { 250,150 }, controller1, controller2);

	dae::SceneManager::GetInstance().SetActiveScene("level4");
}

void dae::StartMultiPlayerComponent::LoadMultiPlayerScene(const std::string& levelName, const glm::vec2 offset, unsigned int controller1, unsigned int controller2)
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

				const auto& player = loader.LoadPlayer(scene, 0, controller2, m_texturemaps.at(objectType), { x, i }, offset);
				players.push_back(player.get());
				scene.AddPlayer(player.get());
			}

			if (objectType == 6)
			{
				const auto& player = loader.LoadPlayer(scene, 1, controller1, m_texturemaps.at(objectType), { x, i }, offset);
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
