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
#include "RecognizerEnemy.h"
#include "Enemy/EnemyController.h"
#include "Componennts/HealthComponent.h"
#include "EggBlockComponent.h"
#include "VolumeCommand.h"
#include "DiamondBlockComponent.h"

#include "StartSinglePlayerCommand.h"
#include "StartingMultiPlayerCommand.h"


#include "PlayerNameCommand.h"
#include "NameComponent.h"


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

void SetUpTransform(std::shared_ptr<dae::GameObject> object, const glm::vec2& position, glm::vec2 offset)
{
	object->GetComponent<dae::TransformComponent>()->SetLocalPosition(
													position.x * 25 + offset.x - g_WindowWidth / 8,
													position.y * 25 + offset.y - g_WindowHeight / 8
													);
}

void SetupPlayerControls(dae::InputManager& inputManager, std::shared_ptr<dae::GameObject> player, dae::Scene& scene)
{
	// Game commnads
	{
		// Increase volume
		inputManager.AddKeyboardCommand<dae::IncreaseVolumeCommand>(
			std::make_unique<dae::IncreaseVolumeCommand>(),
			dae::KeyboardInput{ SDL_SCANCODE_F7, dae::ButtonState::Up, scene.GetName() });

		// Decrease volume
		inputManager.AddKeyboardCommand<dae::DecreaseVolumeCommand>(
			std::make_unique<dae::DecreaseVolumeCommand>(),
			dae::KeyboardInput{ SDL_SCANCODE_F6, dae::ButtonState::Up, scene.GetName() });

		// Mute volume
		inputManager.AddKeyboardCommand<dae::MuteVolumeCommand>(
			std::make_unique<dae::MuteVolumeCommand>(),
			dae::KeyboardInput{ SDL_SCANCODE_F5, dae::ButtonState::Up, scene.GetName() });



		// Start single player
		inputManager.AddKeyboardCommand<dae::StartSinglePlayerCommand>(
			std::make_unique<dae::StartSinglePlayerCommand>(),
			dae::KeyboardInput{ SDL_SCANCODE_F2, dae::ButtonState::Up, scene.GetName() });

		// Start multi player
		inputManager.AddKeyboardCommand<dae::StartingMultiPlayerCommand>(
			std::make_unique<dae::StartingMultiPlayerCommand>(),
			dae::KeyboardInput{ SDL_SCANCODE_F3, dae::ButtonState::Up, scene.GetName() });



		// Letters going down the alphabet
		inputManager.AddKeyboardCommand<dae::PlayerNameDownCommand>(
			std::make_unique<dae::PlayerNameDownCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_DOWN, dae::ButtonState::Up, scene.GetName() });

		// Letters going up the alphabet
		inputManager.AddKeyboardCommand<dae::PlayerNameUpCommand>(
			std::make_unique<dae::PlayerNameUpCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_UP, dae::ButtonState::Up, scene.GetName() });


		// Confirming the letter in the name
		inputManager.AddKeyboardCommand<dae::PlayerNameEnterCommand>(
			std::make_unique<dae::PlayerNameEnterCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_RIGHT, dae::ButtonState::Up, scene.GetName() });
	}
}

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
	{
		const auto& singlePlayerText = std::make_shared<dae::GameObject>();

		// Text
		singlePlayerText->AddComponent<dae::TextComponent>();
		singlePlayerText->AddComponent<dae::TextRenderComponent>();


		text = "SinglePlayer F2";
		singlePlayerText->GetComponent<dae::TextComponent>()->SetFont(font);
		singlePlayerText->GetComponent<dae::TextComponent>()->SetColor(color);
		singlePlayerText->GetComponent<dae::TextComponent>()->SetText(text);

		// Transform
		singlePlayerText->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y - 50.f);

		scene.Add(singlePlayerText);
	}




	// Multiplayer
	{
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
	}


	// Player
	{
		const auto player = std::make_shared<dae::GameObject>();
		scene.Add(player);

		player->GetComponent<dae::TransformComponent>()->SetLocalPosition(g_WindowWidth / 2, g_WindowHeight / 2 - 100.f);
		player->AddComponent<dae::TextComponent>();
		player->AddComponent<dae::TextRenderComponent>();

		player->GetComponent<dae::TextComponent>()->SetFont(font);
		player->GetComponent<dae::TextComponent>()->SetColor(color);
		player->AddComponent<dae::NameComponent>();

		SetupPlayerControls(inputManager, player, scene);
	}





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
	LoadStartScreen("StartScreen");
}

int main(int, char*[]) 
{
	std::string filepath = "../Data/";

	m_Engine.Run(load);
    return 0;
}
