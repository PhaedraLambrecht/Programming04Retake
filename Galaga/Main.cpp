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
#include "Componennts/CollisionComponent.h"

// Commands
#include "Commands/SchootCommand.h"
#include <iostream>

#include "LoadLevelFromFile.h"


//temp, look into changing this
constexpr unsigned int g_WindowWidth{ 640 }, g_WindowHeight{ 480 };
dae::Minigin m_Engine("../Data/", g_WindowWidth, g_WindowHeight);


void loadSound(dae::SoundSystem* sounds)
{
	sounds->NotifySound(dae::SoundData{ 0, 0,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true });
	sounds->NotifySound(dae::SoundData{ 1, 0,  dae::SoundData::SoundType::SoundEffect, "Sound/Shoot.mp3", true });
}


void LoadLevel(const std::string& levelName, const float , const int , const glm::vec2 offset)
{
	std::string level{"../Data/" + levelName + ".txt"};
	std::string characters{ "BlueTank.png" };
	std::string blocks{ "iceBlock.png" };


	auto& scene = dae::SceneManager::GetInstance().CreateScene(levelName);
	scene.SetActive(true);

	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();
	loadSound(sounds);


	// Background
	{
		const auto backGroundImage = std::make_shared<dae::GameObject>();

		// Image
		const auto image = backGroundImage->AddComponent<dae::ImageComponent>();
		backGroundImage->AddComponent<dae::ImageRenderComponent>();
		backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("level.png");
		backGroundImage->GetComponent<dae::TransformComponent>()->SetLocalPosition(g_WindowWidth/2 - image->GetTextureDimensions().x /2, g_WindowHeight / 2 - image->GetTextureDimensions().y / 2);
		
		scene.Add(backGroundImage);
	
		dae::CollisionComponent* collision = backGroundImage->AddComponent<dae::CollisionComponent>();
		scene.AddCollision(collision);
		collision->SetScene(&scene);

		collision->SetCollisionData({ "world", backGroundImage.get() });
		collision->SetBounds(backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x, backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y);
	}


	dae::LoadLevelFromFile levelLoader{};
	levelLoader.LoadLevel(level);
	std::vector<std::vector<int>> leveldata = levelLoader.GetLevel();

	// ---- Load level ----
	for (int i{}; i < leveldata.size(); ++i)
	{
		for (int x{}; x < leveldata[i].size(); ++x)
		{
			int objectType = leveldata[i][x];
			if (objectType == 0)
			{
				continue;
			}

			if (objectType == 1)
			{
				const auto iceblock = std::make_shared<dae::GameObject>();
				iceblock->AddComponent<dae::ImageRenderComponent>();
				iceblock->AddComponent<dae::ImageComponent>();

				iceblock->GetComponent<dae::ImageComponent>()->SetTexture(blocks);

				dae::CollisionComponent* iceCollision = iceblock->AddComponent<dae::CollisionComponent>();
				scene.AddCollision(iceCollision);
				iceCollision->SetScene(&scene);

				iceblock->GetComponent<dae::TransformComponent>()->SetLocalPosition(x * 32 + offset.x - g_WindowWidth/4, i * 32 + offset.y - g_WindowHeight/4);

				scene.Add(iceblock);

			}
		}
	}

}





void LoadLevel(dae::Scene& scene)
{
	const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };

	// Background
	{
		const auto backGroundImage = std::make_shared<dae::GameObject>();
		backGroundImage->AddComponent<dae::ImageComponent>();
		backGroundImage->AddComponent<dae::ImageRenderComponent>();

		backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("level.png");


		scene.Add(backGroundImage);
	}


	//// FPS
	//{
	//	const SDL_Color fpsColor{ 255, 255, 0 };
	//
	//
	//	const auto fpsComponent = std::make_shared<dae::GameObject>();
	//	fpsComponent->AddComponent<dae::TextComponent>();
	//	fpsComponent->AddComponent<dae::TextRenderComponent>();
	//	fpsComponent->AddComponent<dae::FPSComponent>();
	//
	//
	//	fpsComponent->GetComponent<dae::TextComponent>()->SetFont(font);
	//	fpsComponent->GetComponent<dae::TextComponent>()->SetColor(fpsColor);
	//	fpsComponent->GetComponent<dae::TransformComponent>()->SetLocalPosition(0.0f, 10.0f);
	//
	//	scene.Add(fpsComponent);
	//}
}

void LoadPlayerAndEnemies(dae::Scene& scene, dae::InputManager& input, const unsigned int playerIndex, const bool IsControllerUsed)
{
	// Creating the player
	const auto player = dae::GameLoader().AddPlayer(scene, 0, input, IsControllerUsed, "BlueTank.png");


	
	// Setting up some needed variables
	dae::MoveCommand* command{ nullptr };
	constexpr float originalSpeed{ 100.0f };
	constexpr glm::vec2 playerMoveDirectionx{ 1,0 };
	constexpr glm::vec2 playerMoveDirectiony{ 0,1 };

	// Controls
	if (IsControllerUsed)
	{
		unsigned controller = input.AddController();
	
	
		// Movement
		command = input.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectiony, -originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_UP, dae::ButtonState::Pressed, scene.GetName()}
		);
	
		command = input.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectiony, originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_DOWN,dae::ButtonState::Pressed, scene.GetName()}
		);
	
		command = input.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectionx, -originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_LEFT, dae::ButtonState::Pressed, scene.GetName()}
		);
	
		command = input.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectionx, originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_RIGHT, dae::ButtonState::Pressed, scene.GetName()}
		);
	
	
	
	
		// points added
		input.AddControllerCommand<dae::AddPointsCommand>(
			std::make_unique<dae::AddPointsCommand>(player.get()),
			dae::ControllerInput{ playerIndex, dae::ControlerButton::B, dae::ButtonState::Up, scene.GetName()});
	
		// Lifes taken
		input.AddControllerCommand<dae::DamageCommand>(
			std::make_unique<dae::DamageCommand>(player.get(), 1),
			dae::ControllerInput{ playerIndex, dae::ControlerButton::A, dae::ButtonState::Up, scene.GetName()});
	
		// Schooting
		input.AddControllerCommand<dae::SchootCommand>(
			std::make_unique<dae::SchootCommand>(player.get(), playerIndex),
			dae::ControllerInput{ playerIndex, dae::ControlerButton::X, dae::ButtonState::Up, scene.GetName()});
	}
	else
	{
		// Movement
		command = input.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectiony, -originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed, scene.GetName()}
		);
	
		command = input.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectiony, originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed, scene.GetName()}
		);
	
	
		command = input.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectionx, -originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed, scene.GetName()}
		);
	
		command = input.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(player.get(), playerMoveDirectionx, originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed, scene.GetName()}
		);
	 
		// points added
		input.AddKeyboardCommand<dae::AddPointsCommand>(
			std::make_unique<dae::AddPointsCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_Q , dae::ButtonState::Up, scene.GetName()});
	
		// Lifes taken
		input.AddKeyboardCommand<dae::DamageCommand>(
			std::make_unique<dae::DamageCommand>(player.get(), 1),
			dae::KeyboardInput{ SDL_SCANCODE_E, dae::ButtonState::Up, scene.GetName()});
	
		// Schooting
		input.AddKeyboardCommand<dae::SchootCommand>(
			std::make_unique<dae::SchootCommand>(player.get(), playerIndex),
			dae::KeyboardInput{ SDL_SCANCODE_X, dae::ButtonState::Up, scene.GetName()});
	
	}


	// ---- Enemies -----
	for (int i = 0; i < 5; i++)
	{
		const auto enemy = dae::GameLoader().AddEnemy(scene, "GreenTank.png", player);
	}

}



void loadGameLevel()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	scene.SetActive(false);
	auto& inputManager = dae::InputManager::GetInstance();
	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();

	LoadLevel(scene);
	loadSound(sounds);

	LoadPlayerAndEnemies(scene, inputManager, 0, false); // PLayer 0


	// ---- Play manual -----
	const auto playManual = std::make_shared<dae::GameObject>();
	scene.Add(playManual);

	playManual->AddComponent<dae::PlayManualComponent>();

	// Keyboard
	{
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation(dae::PlayerManual{ "WASD", "Move" });

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation(dae::PlayerManual{ "E", "Lose a life" });
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation(dae::PlayerManual{ "Q", "Gain 100 points" });

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation(dae::PlayerManual{ "X", "schoot a bullet - Do not do this with both keyboard and controller\n" });

	}
	// Controller
	{
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation(dae::PlayerManual{ "DPAD", "Move" });

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation(dae::PlayerManual{ "A_BUTTON", "Lose a life" });
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation(dae::PlayerManual{ "B_BUTTON", "Gain 100 points" });

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation(dae::PlayerManual{ "X", "schoot a bullet - Do not do this with both keyboard and controller\n" });
	}
}



void LoadNothing()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Main");
	scene.SetActive(true);


	LoadLevel(scene);
}



void load()
{
//	LoadNothing();

//	loadGameLevel();

	LoadLevel("Level", 0, 0, { 250,100 });
}

int main(int, char*[]) 
{
	std::string filepath = "../Data/";


	m_Engine.Run(load);
    return 0;
}
