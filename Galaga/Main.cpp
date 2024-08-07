#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "Scene/SceneManager.h"
#include "Resources/ResourceManager.h"
#include "Scene/Scene.h"

// FPS counter
#include "GameObject.h"
#include "Componennts/ImageComponent.h"
#include "Componennts/TextComponent.h"
#include "Componennts/TransformComponent.h"
#include "Componennts/ImageRenderComponent.h"
#include "Componennts/TextRenderComponent.h"
#include "Componennts/FPSComponent.h"

// Input 
#include "Input/InputManager.h"
#include "Input/Controller.h"
#include "Commands/MoveCommand.h"

// Event queue
#include "Componennts/LivesComponent.h"
#include "Componennts/AddPointsComponnent.h"
#include "Commands/AddPointsCommand.h"
#include "Componennts/ScoreComponent.h"
#include "Commands/DamageCommand.h"

#include "GameLoader.h"

// Sound
#include "Sound/SoundManager.h"
#include "Sound/SDLSoundSystem.h"

// Components
#include "Componennts/PlayManualComponent.h"

// Commands
#include "Commands/SchootCommand.h"
#include "SkipLevelCommand.h"

#include <iostream>

#include "LoadLevelFromFile.h"
#include "Componennts/CollisionComponent.h"
#include "Enemy/BaseEnemyComponent.h"
#include "Enemy/RecognizerEnemy.h"
#include "Enemy/EnemyController.h"
#include "Componennts/HealthComponent.h"
#include "EggBlockComponent.h"
#include "VolumeCommand.h"
#include "DiamondBlockComponent.h"
#include "StartSinglePlayerCommand.h"


//temp, look into changing this
constexpr unsigned int g_WindowWidth{ 640 }, g_WindowHeight{ 480 };
dae::Minigin m_Engine("../Data/", g_WindowWidth, g_WindowHeight);
const enum objectTypes
{
	iceBlock = 1,
	eggBlock = 2,
	enemy = 3,
	diamondBlock = 4,
	player = 5
};



void loadSound(dae::SoundSystem* sounds, const dae::SoundData& soundData)
{
	sounds->NotifySound(soundData);
}

void SetupCollision(std::shared_ptr<dae::GameObject> object, const std::string& tag, dae::Scene& scene)
{
	dae::CollisionComponent* collision = object->AddComponent<dae::CollisionComponent>();
	scene.AddCollision(collision);
	collision->SetScene(&scene);

	collision->SetCollisionData({ tag, object.get() });
	collision->SetBounds(
						 object->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x,
						 object->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y
						);
}

void SetUpTransform(std::shared_ptr<dae::GameObject> object, const glm::vec2& position, glm::vec2 offset)
{
	object->GetComponent<dae::TransformComponent>()->SetLocalPosition(
													position.x * 25 + offset.x - g_WindowWidth / 8,
													position.y * 25 + offset.y - g_WindowHeight / 8
													);
}

void SetupPlayerControls(dae::InputManager& inputManager, std::shared_ptr<dae::GameObject> player, dae::Scene& scene)
{
	{
		// Game commnads

		// Skipping level
		inputManager.AddKeyboardCommand<dae::SkipLevelCommand>(
			std::make_unique<dae::SkipLevelCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_F1, dae::ButtonState::Up, scene.GetName() });

		// Increase volume
		inputManager.AddKeyboardCommand<dae::IncreaseVolumeCommand>(
			std::make_unique<dae::IncreaseVolumeCommand>(),
			dae::KeyboardInput{ SDL_SCANCODE_F7, dae::ButtonState::Up, scene.GetName() });

		// Decrease volume
		inputManager.AddKeyboardCommand<dae::DecreaseVolumeCommand>(
			std::make_unique<dae::DecreaseVolumeCommand>(),
			dae::KeyboardInput{ SDL_SCANCODE_F6, dae::ButtonState::Up, scene.GetName() });

		// Decrease volume
		inputManager.AddKeyboardCommand<dae::StartSinglePlayerCommand>(
			std::make_unique<dae::StartSinglePlayerCommand>(),
			dae::KeyboardInput{ SDL_SCANCODE_F2, dae::ButtonState::Up, scene.GetName() });
	}
}


void CreateBlockObject(dae::Scene& scene, const glm::vec2& position, const std::string& texture, const std::string& collisionTag, const glm::vec2& offset)
{
	const auto block = std::make_shared<dae::GameObject>();

	// Image
	block->AddComponent<dae::ImageComponent>();
	block->AddComponent<dae::ImageRenderComponent>();
	block->GetComponent<dae::ImageComponent>()->SetTexture(texture);

	// Transform
	SetUpTransform(block, position, offset);

	// Collision
	SetupCollision(block, collisionTag, scene);
	
	scene.Add(block);
}

void CreateEnemyObject(dae::Scene& scene, const glm::vec2& position, const std::string& texture, std::vector<dae::GameObject*> players, const std::shared_ptr<dae::GameObject>& backGroundImage, const glm::vec2& offset)
{
	const auto enemy = std::make_shared<dae::GameObject>();

	// Image
	enemy->AddComponent<dae::ImageComponent>();
	enemy->AddComponent<dae::ImageRenderComponent>();
	enemy->GetComponent<dae::ImageComponent>()->SetTexture(texture);

	// Transform
	SetUpTransform(enemy, position, offset);

	// Health
	enemy->AddComponent<dae::HealthComponent>()->SetHealth(1);


	// Enemy components
	enemy->AddComponent<dae::RecognizerEnemy>()->Initialize( enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().x, enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().y, 10, 10, players);

	const auto baseEnemy = enemy->GetComponent<dae::BaseEnemyComponent>();
	enemy->GetComponent<dae::RecognizerEnemy>()->SetWindowDimensions(backGroundImage->GetComponent<dae::TransformComponent>()->GetWorldPosition().x,
																	 backGroundImage->GetComponent<dae::TransformComponent>()->GetWorldPosition().y,
																	 backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x * 2,
																	 backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y * 2
																	);

	enemy->AddComponent<dae::EnemyController>();


	// Collision
	SetupCollision(enemy, "Enemy", scene);


	auto boundHitCallback = std::bind(&dae::BaseEnemyComponent::OnHitCallback, baseEnemy, std::placeholders::_1, std::placeholders::_2);
	enemy->GetComponent<dae::CollisionComponent>()->SetCallback(boundHitCallback);


	scene.AddEnemy(enemy);
	scene.Add(enemy);
}



//void LoadMultiPlayer(const std::string& levelName, const glm::vec2 offset, unsigned controller1, unsigned controller2)
//{
//	// Base variables
//	std::string levelFile{ "../Data/" + levelName + ".txt" };
//	const std::unordered_map<int, std::string> textureMap
//	{
//		{1, "iceBlock.png" },
//		{ 2, "eggBlock.png" },
//		{ 3, "snoBee1.png" },
//		{ 4, "diamondBlock.png" },
//		{ 5, "penguin2Down.png" },
//		{6, "GreenTank.png"}
//	};
//
//	// Create scene
//	auto& scene = dae::SceneManager::GetInstance().CreateScene(levelName);
//	scene.SetActive(true);
//
//	// Setup input and sound
//	auto& inputManager = dae::InputManager::GetInstance();
//	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();
//
//	// Load sounds
//	dae::SoundData backgroundMusic = { 1, 0.1f,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true };
//	dae::SoundData schootEffect = { 1, 0.f,  dae::SoundData::SoundType::SoundEffect, "Sound/Shoot.mp3", true };
//	loadSound(sounds, backgroundMusic);
//	loadSound(sounds, schootEffect);
//
//	// Load Level boarder
//	const auto backGroundImage = std::make_shared<dae::GameObject>();
//
//	// Image
//	const auto image = backGroundImage->AddComponent<dae::ImageComponent>();
//	backGroundImage->AddComponent<dae::ImageRenderComponent>();
//	backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("level.png");
//
//	// Transform
//	backGroundImage->GetComponent<dae::TransformComponent>()->SetLocalPosition(g_WindowWidth / 2 - image->GetTextureDimensions().x / 2, g_WindowHeight / 2 - image->GetTextureDimensions().y / 2);
//
//	// Collision
//	SetupCollision(backGroundImage, "world", scene);
//
//	scene.Add(backGroundImage);
//
//
//
//
//	// Load level
//	dae::LoadLevelFromFile levelLoader{};
//	levelLoader.LoadLevel(levelFile);
//	std::vector<std::vector<int>> leveldata = levelLoader.GetLevel();
//
//
//	std::vector<dae::GameObject*> players{};
//	for (int i{}; i < int(leveldata.size()); ++i)
//	{
//		for (int x{}; x < int(leveldata[i].size()); ++x)
//		{
//			int objectType = leveldata[i][x];
//			if (objectType == 0)
//			{
//				continue;
//			}
//
//			// I'm scared of my own code...... that is 2 players on 1 variable.....
//			if (objectType == 5)
//			{
//
//				const auto& player = dae::GameLoader().AddPlayer(scene, 0, inputManager, false, textureMap.at(objectType));
//				SetUpTransform(player, {x, i}, offset);
//
//
//				dae::GameLoader().AddKeyboard(scene, player.get());
//				dae::GameLoader().AddControleler(scene, player.get(), controller1);
//
//				SetupPlayerControls(inputManager, player, scene);
//
//				players.push_back(player.get());
//			}
//			if (objectType == 6)
//			{
//				const auto& player = dae::GameLoader().AddPlayer(scene, 1, inputManager, false, textureMap.at(objectType));
//				SetUpTransform(player, { x, i }, offset);
//
//
//				dae::GameLoader().AddKeyboard(scene, player.get());
//				dae::GameLoader().AddControleler(scene, player.get(), controller2);
//
//				SetupPlayerControls(inputManager, player, scene);
//
//				players.push_back(player.get());
//			}
//		}
//	}
//
//
//	for (int i{}; i < int(leveldata.size()); ++i)
//	{
//		for (int x{}; x < int(leveldata[i].size()); ++x)
//		{
//			int objectType = leveldata[i][x];
//			if (objectType == 0)
//			{
//				continue;
//			}
//
//			if (objectType == 1)
//			{
//				CreateBlockObject(scene, { x, i }, textureMap.at(objectType), "iceBlock", offset);
//			}
//			if (objectType == 2)
//			{
//				//	CreateBlockObject(scene, { x, i }, textureMap.at(objectType), "eggBlock", offset);
//
//				const auto block = std::make_shared<dae::GameObject>();
//
//				// Image
//				block->AddComponent<dae::ImageComponent>();
//				block->AddComponent<dae::ImageRenderComponent>();
//				block->GetComponent<dae::ImageComponent>()->SetTexture(textureMap.at(objectType));
//
//				// Transform
//				SetUpTransform(block, { x, i }, offset);
//
//				// Collision
//				SetupCollision(block, "eggBlock", scene);
//
//				block->AddComponent<dae::EggBlockComponent>();
//				block->GetComponent<dae::EggBlockComponent>()->SetPlayer(players);
//				block->GetComponent<dae::EggBlockComponent>()->SetBackground(backGroundImage.get());
//
//				scene.Add(block);
//			}
//			if (objectType == 3)
//			{
//				CreateEnemyObject(scene, { x, i }, textureMap.at(objectType), players, backGroundImage, offset);
//			}
//			if (objectType == 4)
//			{
//				//	CreateBlockObject(scene, { x, i }, textureMap.at(objectType), "diamondBlock", offset);
//
//				const auto diamondBlock = std::make_shared<dae::GameObject>();
//
//				// Image
//				diamondBlock->AddComponent<dae::ImageComponent>();
//				diamondBlock->AddComponent<dae::ImageRenderComponent>();
//				diamondBlock->GetComponent<dae::ImageComponent>()->SetTexture(textureMap.at(objectType));
//
//				// Transform
//				SetUpTransform(diamondBlock, { x, i }, offset);
//
//				diamondBlock->AddComponent<dae::DiamondBlockComponent>();
//				diamondBlock->GetComponent<dae::DiamondBlockComponent>()->Initialize(players);
//
//
//				// Collision
//				SetupCollision(diamondBlock, "diamondBlock", scene);
//				auto boundHitCallback = std::bind(&dae::DiamondBlockComponent::OnHitCallback, diamondBlock->GetComponent<dae::DiamondBlockComponent>(), std::placeholders::_1, std::placeholders::_2);
//				diamondBlock->GetComponent<dae::CollisionComponent>()->SetCallback(boundHitCallback);
//
//
//
//				scene.Add(diamondBlock);
//			}
//
//		}
//	}
//}


void LoadStartScreen(const std::string& levelName)
{
	const SDL_Color color{ 0, 255, 0 };
	const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };
	std::string text{};

	glm::vec2 position{ g_WindowWidth / 2 - 100.f, g_WindowHeight / 2 };


	auto& scene = dae::SceneManager::GetInstance().CreateScene(levelName);
	scene.SetActive(true);


	// Setup input and sound
	auto& inputManager = dae::InputManager::GetInstance();
	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();

	// Load sounds
	dae::SoundData backgroundMusic = { 1, 0.1f,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true };
	loadSound(sounds, backgroundMusic);
	
	
	
	
	// Load Level boarder
	const auto backGroundImage = std::make_shared<dae::GameObject>();

	// Image
	backGroundImage->AddComponent<dae::ImageComponent>();
	backGroundImage->AddComponent<dae::ImageRenderComponent>();
	backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("background.tga");


	scene.Add(backGroundImage);

	

	// Singleplayer
	const auto& singlePlayerText = std::make_shared<dae::GameObject>();

	// Text
	singlePlayerText->AddComponent<dae::TextComponent>();
	singlePlayerText->AddComponent<dae::TextRenderComponent>();


	text = "SinglePlayer F2";
	singlePlayerText->GetComponent<dae::TextComponent>()->SetFont(font);
	singlePlayerText->GetComponent<dae::TextComponent>()->SetColor(color);
	singlePlayerText->GetComponent<dae::TextComponent>()->SetText(text);

	// Transform
	singlePlayerText->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y- 50.f);

	scene.Add(singlePlayerText);



	// Multiplayer
	const auto& multiPlayerText = std::make_shared<dae::GameObject>();

	// Text
	multiPlayerText->AddComponent<dae::TextComponent>();
	multiPlayerText->AddComponent<dae::TextRenderComponent>();


	text = "MultiPlayer F3";
	multiPlayerText->GetComponent<dae::TextComponent>()->SetFont(font);
	multiPlayerText->GetComponent<dae::TextComponent>()->SetColor(color);
	multiPlayerText->GetComponent<dae::TextComponent>()->SetText(text);

	// Transform
	multiPlayerText->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y);

	scene.Add(multiPlayerText);




	const auto player = std::make_shared<dae::GameObject>();
	SetupPlayerControls(inputManager, player, scene);





	// ---- Play manual -----
	const auto playManual = std::make_shared<dae::GameObject>();
	scene.Add(playManual);

	playManual->AddComponent<dae::PlayManualComponent>();

	// Keyboard
	{
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("WASD", "Move");

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("X", "schoot a bullet");
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("Z", "breaks diamond blocks\n");
	}
	// Controller
	{
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("DPAD", "Move");
		
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("X", "schoot a bullet");
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("B", "breaks diamond blocks\n");
	}

}


void load()
{
//	// Multiplayer
//	unsigned controller1 = inputManager.AddController();
//
////	LoadMultiPlayer("level6", { 250, 150 }, controller, controller1);
//	LoadMultiPlayer("level5", { 250, 150 }, controller, controller1);
//	LoadMultiPlayer("level4", { 250,125 }, controller, controller1);
//

	LoadStartScreen("StartScreen");

}

int main(int, char*[]) 
{
	std::string filepath = "../Data/";

	m_Engine.Run(load);
    return 0;
}
