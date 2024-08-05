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
#include "Commands/StartButtonCommand.h"


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
#include "HitComponennt.h"
#include "HitCommand.h"



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
	// Setting up some needed variables
	dae::MoveCommand* command{ nullptr };
	constexpr float originalSpeed{ 100.0f };
	constexpr glm::vec2 playerMoveDirectionx{ 1,0 };
	constexpr glm::vec2 playerMoveDirectiony{ 0,1 };

	// controller and keyboard
	unsigned controller = inputManager.AddController();
	{
		// Movement
		command = inputManager.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectiony, -originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_UP, dae::ButtonState::Pressed, scene.GetName() }
		);

		command = inputManager.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectiony, originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_DOWN,dae::ButtonState::Pressed, scene.GetName() }
		);

		command = inputManager.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectionx, -originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_LEFT, dae::ButtonState::Pressed, scene.GetName() }
		);

		command = inputManager.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectionx, originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_RIGHT, dae::ButtonState::Pressed, scene.GetName() }
		);

		// points added
		inputManager.AddControllerCommand<dae::AddPointsCommand>(
			std::make_unique<dae::AddPointsCommand>(player.get()),
			dae::ControllerInput{ 1, dae::ControlerButton::B, dae::ButtonState::Up, scene.GetName() });

		// Lifes taken
		inputManager.AddControllerCommand<dae::DamageCommand>(
			std::make_unique<dae::DamageCommand>(player.get(), 1),
			dae::ControllerInput{ 1, dae::ControlerButton::A, dae::ButtonState::Up, scene.GetName() });

		// Schooting
		inputManager.AddControllerCommand<dae::SchootCommand>(
			std::make_unique<dae::SchootCommand>(player.get(), 1),
			dae::ControllerInput{ 1, dae::ControlerButton::X, dae::ButtonState::Up, scene.GetName() });


		// Movement
		command = inputManager.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectiony, -originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed, scene.GetName() }
		);

		command = inputManager.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectiony, originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed, scene.GetName() }
		);


		command = inputManager.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectionx, -originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed, scene.GetName() }
		);

		command = inputManager.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectionx, originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed, scene.GetName() }
		);

		// points added
		inputManager.AddKeyboardCommand<dae::AddPointsCommand>(
			std::make_unique<dae::AddPointsCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_Q , dae::ButtonState::Up, scene.GetName() });

		// Lifes taken
		inputManager.AddKeyboardCommand<dae::DamageCommand>(
			std::make_unique<dae::DamageCommand>(player.get(), 1),
			dae::KeyboardInput{ SDL_SCANCODE_E, dae::ButtonState::Up, scene.GetName() });

		// Schooting
		inputManager.AddKeyboardCommand<dae::SchootCommand>(
			std::make_unique<dae::SchootCommand>(player.get(), 0),
			dae::KeyboardInput{ SDL_SCANCODE_X, dae::ButtonState::Up, scene.GetName() });

		// Hit
		inputManager.AddKeyboardCommand<dae::HitCommand>(
			std::make_unique<dae::HitCommand>(player.get(), 0),
			dae::KeyboardInput{ SDL_SCANCODE_Z, dae::ButtonState::Up, scene.GetName() });

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

void CreateEnemyObject(dae::Scene& scene, const glm::vec2& position, const std::string& texture, std::shared_ptr<dae::GameObject> player, const std::shared_ptr<dae::GameObject>& backGroundImage, const glm::vec2& offset)
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
	enemy->AddComponent<dae::RecognizerEnemy>()->Initialize( enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().x, enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().y, 10, 10, player);

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


	scene.Add(enemy);
}





void LoadSinglePlayerScene(const std::string& levelName, const glm::vec2 offset)
{
	// Base variables
	std::string levelFile{ "../Data/" + levelName + ".txt" };
	const std::unordered_map<int, std::string> textureMap
	{
		{1, "iceBlock.png" },
		{ 2, "eggBlock.png" },
		{ 3, "snoBee1.png" },
		{ 4, "diamondBlock.png" },
		{ 5, "penguin2Down.png" }
	};

	// Create scene
	auto& scene = dae::SceneManager::GetInstance().CreateScene(levelName);
	scene.SetActive(true);

	// Setup input and sound
	auto& inputManager = dae::InputManager::GetInstance();
	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();

	// Load sounds
	dae::SoundData backgroundMusic = { 1, 0.1f,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true };
	dae::SoundData schootEffect = { 1, 1,  dae::SoundData::SoundType::SoundEffect, "Sound/Shoot.mp3", true };
	loadSound(sounds, backgroundMusic);
	loadSound(sounds, schootEffect);




	// Load Level boarder
	const auto backGroundImage = std::make_shared<dae::GameObject>();

	// Image
	const auto image = backGroundImage->AddComponent<dae::ImageComponent>();
	backGroundImage->AddComponent<dae::ImageRenderComponent>();
	backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("level.png");

	// Transform
	backGroundImage->GetComponent<dae::TransformComponent>()->SetLocalPosition(g_WindowWidth / 2 - image->GetTextureDimensions().x / 2, g_WindowHeight / 2 - image->GetTextureDimensions().y / 2);

	// Collision
	SetupCollision(backGroundImage, "world", scene);

	scene.Add(backGroundImage);


 
 
	// Load level
	dae::LoadLevelFromFile levelLoader{};
	levelLoader.LoadLevel(levelFile);
	std::vector<std::vector<int>> leveldata = levelLoader.GetLevel();


	std::shared_ptr<dae::GameObject> player{};
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
				player = dae::GameLoader().AddPlayer(scene, 0, inputManager, false, textureMap.at(objectType));

				// controller and keyboard
				SetupPlayerControls(inputManager, player, scene);

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
				CreateBlockObject(scene, { x, i }, textureMap.at(objectType), "iceBlock", offset);
			}
			if (objectType == 2)
			{
			//	CreateBlockObject(scene, { x, i }, textureMap.at(objectType), "eggBlock", offset);

				const auto block = std::make_shared<dae::GameObject>();

				// Image
				block->AddComponent<dae::ImageComponent>();
				block->AddComponent<dae::ImageRenderComponent>();
				block->GetComponent<dae::ImageComponent>()->SetTexture(textureMap.at(objectType));

				// Transform
				SetUpTransform(block, { x, i }, offset);

				// Collision
				SetupCollision(block, "eggBlock", scene);

				block->AddComponent<dae::EggBlockComponent>();
				block->GetComponent<dae::EggBlockComponent>()->SetPlayer(player);
				block->GetComponent<dae::EggBlockComponent>()->SetBackground(backGroundImage.get());

				scene.Add(block);
			}
			if (objectType == 3)
			{
				CreateEnemyObject(scene, { x, i }, textureMap.at(objectType), player, backGroundImage, offset);
			}
			if (objectType == 4)
			{
			//	CreateBlockObject(scene, { x, i }, textureMap.at(objectType), "diamondBlock", offset);

				const auto diamondBlock = std::make_shared<dae::GameObject>();

				// Image
				diamondBlock->AddComponent<dae::ImageComponent>();
				diamondBlock->AddComponent<dae::ImageRenderComponent>();
				diamondBlock->GetComponent<dae::ImageComponent>()->SetTexture(textureMap.at(objectType));

				// Transform
				SetUpTransform(diamondBlock, { x, i }, offset);

				diamondBlock->AddComponent<dae::DiamondBlockComponent>();
				diamondBlock->GetComponent<dae::DiamondBlockComponent>()->Initialize(player);


				// Collision
				SetupCollision(diamondBlock, "diamondBlock", scene);
				auto boundHitCallback = std::bind(&dae::DiamondBlockComponent::OnHitCallback, diamondBlock->GetComponent<dae::DiamondBlockComponent>(), std::placeholders::_1, std::placeholders::_2);
				diamondBlock->GetComponent<dae::CollisionComponent>()->SetCallback(boundHitCallback);



				scene.Add(diamondBlock);
			}

		}
	}

}

void LoadStartScreen(const std::string& levelName)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(levelName);
	scene.SetActive(true);


	// Setup input and sound
	//auto& inputManager = dae::InputManager::GetInstance();
	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();

	// Load sounds
	dae::SoundData backgroundMusic = { 1, 0.1f,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true };
	loadSound(sounds, backgroundMusic);



	//
	//const auto& singlePlayerText = std::make_shared<dae::GameObject>();




	//	const auto livesDisplay =
	//livesDisplay->AddComponent<TextComponent>();
	//livesDisplay->AddComponent<TextRenderComponent>();
	//livesDisplay->GetComponent<TextComponent>()->SetFont(font);
	//livesDisplay->GetComponent<TextComponent>()->SetColor(color);
	//livesDisplay->AddComponent<LivesComponent>();


	//livesDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(0.0f, 0.0f);
	//livesDisplay->GetComponent<dae::LivesComponent>()->SetPlayerIndex(playerIndex);
	//livesDisplay->GetComponent<dae::LivesComponent>()->SetLives(player);

	//scene.Add(livesDisplay);

	//const auto block = std::make_shared<dae::GameObject>();

	//// Image
	//block->AddComponent<dae::ImageComponent>();
	//block->AddComponent<dae::ImageRenderComponent>();
	//block->GetComponent<dae::ImageComponent>()->SetTexture(texture);

	//// Transform
	//SetUpTransform(block, position, offset);

	//// Collision
	//SetupCollision(block, collisionTag, scene);

	//scene.Add(block);

}


void load()
{
	LoadSinglePlayerScene("level1", { 250,100 });
	LoadSinglePlayerScene("level2", { 250,125 });
	LoadSinglePlayerScene("level3", { 250,150 });

	dae::SceneManager::GetInstance().SwitchScene("level1");


}

int main(int, char*[]) 
{
	std::string filepath = "../Data/";

	m_Engine.Run(load);
    return 0;
}



//void loadGameLevel()
//{
//	// ---- Play manual -----
//	const auto playManual = std::make_shared<dae::GameObject>();
//	scene.Add(playManual);
//
//	playManual->AddComponent<dae::PlayManualComponent>();
//
//	// Keyboard
//	{
//		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("WASD", "Move");
//
//		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("E", "Lose a life");
//		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("Q", "Gain 100 points");
//
//		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("X", "schoot a bullet - Do not do this with both keyboard and controller\n");
//
//	}
//	// Controller
//	{
//		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("DPAD", "Move");
//
//		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("A_BUTTON", "Lose a life");
//		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("B_BUTTON", "Gain 100 points");
//
//		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("X", "schoot a bullet - Do not do this with both keyboard and controller\n");
//	}
//}
