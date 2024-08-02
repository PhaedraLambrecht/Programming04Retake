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
#include "Componennts/HealthComponent.h"
#include "Componennts/SchootComponent.h"
#include "Componennts/RotatorComponent.h"
#include "Commands/SchootCommand.h"
#include "Commands/AddPointsCommand.h"
#include "Commands/DamageCommand.h"
#include "Events/EventManager.h"


//temp, look into changing this
constexpr unsigned int g_WindowWidth{ 640 }, g_WindowHeight{ 480 };
dae::Minigin m_Engine("../Data/", g_WindowWidth, g_WindowHeight);


void loadSound(dae::SoundSystem* sounds, const dae::SoundData& soundData)
{
	sounds->NotifySound(soundData);
}

void LoadLevel(const std::string& levelName, const float , const int , const glm::vec2 offset)
{
	std::string level{"../Data/" + levelName + ".txt"};
	std::string characters{ "BlueTank.png" };
	std::string iceBlockTexture{ "iceBlock.png" };
	std::string eggBlockTexture{ "eggBlock.png" };
	std::string enemyBlockTexture{ "snoBee1.png" };
	std::string diamandBlockTexture{ "diamondBlock.png" };
	std::string playerTexture{ "penguin2Down.png" };
	
	
	auto& scene = dae::SceneManager::GetInstance().CreateScene(levelName);
	scene.SetActive(true);

	// Sound
	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();
	dae::SoundData backgroundMusic = { 1, 0.1f,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true };
	dae::SoundData schootEffect = { 1, 1,  dae::SoundData::SoundType::SoundEffect, "Sound/Shoot.mp3", true };
	loadSound(sounds, backgroundMusic);
	loadSound(sounds, schootEffect);

	auto& inputManager = dae::InputManager::GetInstance();


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
		collision->SetActive(true);
		collision->SetScene(&scene);

		collision->SetCollisionData({ "world", backGroundImage.get() });
		collision->SetBounds(backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x, backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y);
	}



	dae::LoadLevelFromFile levelLoader{};
	levelLoader.LoadLevel(level);
	std::vector<std::vector<int>> leveldata = levelLoader.GetLevel();

	//// ---- Load level ----
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

				iceblock->GetComponent<dae::ImageComponent>()->SetTexture(iceBlockTexture);

				dae::CollisionComponent* iceCollision = iceblock->AddComponent<dae::CollisionComponent>();
				scene.AddCollision(iceCollision);
				iceCollision->SetActive(true);
				iceCollision->SetScene(&scene);
				
				iceCollision->SetCollisionData({ "iceBlock", iceblock.get() });
				iceCollision->SetBounds(iceblock->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x, iceblock->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y);

				iceblock->GetComponent<dae::TransformComponent>()->SetLocalPosition(x * 32 + offset.x - g_WindowWidth/4, i * 32 + offset.y - g_WindowHeight/4);

				scene.Add(iceblock);

			}
			else if (objectType == 2)
			{
				const auto eggblock = std::make_shared<dae::GameObject>();
				eggblock->AddComponent<dae::ImageRenderComponent>();
				eggblock->AddComponent<dae::ImageComponent>();

				eggblock->GetComponent<dae::ImageComponent>()->SetTexture(eggBlockTexture);


				dae::CollisionComponent* eggCollision = eggblock->AddComponent<dae::CollisionComponent>();
				scene.AddCollision(eggCollision);
				eggCollision->SetActive(true);
				eggCollision->SetScene(&scene);

				eggCollision->SetCollisionData({ "iceBlock", eggblock.get() });
				eggCollision->SetBounds(eggblock->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x, eggblock->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y);

				eggblock->GetComponent<dae::TransformComponent>()->SetLocalPosition(x * 32 + offset.x - g_WindowWidth / 4, i * 32 + offset.y - g_WindowHeight / 4);

				scene.Add(eggblock);
			}
			else if (objectType == 3)
			{
				const auto enemyblock = std::make_shared<dae::GameObject>();
				enemyblock->AddComponent<dae::ImageRenderComponent>();
				enemyblock->AddComponent<dae::ImageComponent>();
				enemyblock->GetComponent<dae::ImageComponent>()->SetTexture(enemyBlockTexture);

				enemyblock->GetComponent<dae::TransformComponent>()->SetLocalPosition(x * 32 + offset.x - g_WindowWidth / 4, i * 32 + offset.y - g_WindowHeight / 4);

				scene.Add(enemyblock);
			}
			else if (objectType == 4)
			{
				const auto diomandblock = std::make_shared<dae::GameObject>();
				diomandblock->AddComponent<dae::ImageRenderComponent>();
				diomandblock->AddComponent<dae::ImageComponent>();

				diomandblock->GetComponent<dae::ImageComponent>()->SetTexture(diamandBlockTexture);

				dae::CollisionComponent* diamondCollision = diomandblock->AddComponent<dae::CollisionComponent>();
				scene.AddCollision(diamondCollision);
				diamondCollision->SetActive(true);
				diamondCollision->SetScene(&scene);

				diamondCollision->SetCollisionData({ "iceBlock", diomandblock.get() });
				diamondCollision->SetBounds(diomandblock->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x, diomandblock->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y);

				diomandblock->GetComponent<dae::TransformComponent>()->SetLocalPosition(x * 32 + offset.x - g_WindowWidth / 4, i * 32 + offset.y - g_WindowHeight / 4);

				scene.Add(diomandblock);
			}
			else if (objectType == 5)
			{
				 glm::vec2 location{ x + offset.x, i + offset.y };
				 // Creating the player
				 const auto player = dae::GameLoader().AddPlayer(scene, 0, inputManager, false, playerTexture, location);


				 // Setting up some needed variables
				 dae::MoveCommand* command{ nullptr };
				 constexpr float originalSpeed{ 100.0f };
				 constexpr glm::vec2 playerMoveDirectionx{ 1,0 };
				 constexpr glm::vec2 playerMoveDirectiony{ 0,1 };

				 {
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
				 }
				
				 {
					 unsigned controller = inputManager.AddController();


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
						 dae::ControllerInput{ 0, dae::ControlerButton::B, dae::ButtonState::Up, scene.GetName() });

					 // Lifes taken
					 inputManager.AddControllerCommand<dae::DamageCommand>(
						 std::make_unique<dae::DamageCommand>(player.get(), 1),
						 dae::ControllerInput{ 0, dae::ControlerButton::A, dae::ButtonState::Up, scene.GetName() });

					 // Schooting
					 inputManager.AddControllerCommand<dae::SchootCommand>(
						 std::make_unique<dae::SchootCommand>(player.get(), 0),
						 dae::ControllerInput{ 0, dae::ControlerButton::X, dae::ButtonState::Up, scene.GetName() });
				 }


			}
		}
	}
}


void LoadLevel(const std::string& levelName, const glm::vec2 )
{
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

	// Sound
	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();
	dae::SoundData backgroundMusic = { 1, 0.1f,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true };
	dae::SoundData schootEffect = { 1, 1,  dae::SoundData::SoundType::SoundEffect, "Sound/Shoot.mp3", true };
	loadSound(sounds, backgroundMusic);
	loadSound(sounds, schootEffect);


	// Background
	{
		const auto backGroundImage = std::make_shared<dae::GameObject>();

		// Image
		const auto image = backGroundImage->AddComponent<dae::ImageComponent>();
		backGroundImage->AddComponent<dae::ImageRenderComponent>();
		backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("level.png");
		backGroundImage->GetComponent<dae::TransformComponent>()->SetLocalPosition(g_WindowWidth / 2 - image->GetTextureDimensions().x / 2, g_WindowHeight / 2 - image->GetTextureDimensions().y / 2);

		scene.Add(backGroundImage);

		dae::CollisionComponent* collision = backGroundImage->AddComponent<dae::CollisionComponent>();
		scene.AddCollision(collision);
		collision->SetScene(&scene);

		collision->SetCollisionData({ "world", backGroundImage.get() });
		collision->SetBounds(backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x, backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y);
	}
}



void load()
{
	LoadLevel("Level", 0, 0, { 250,100 });

}

int main(int, char*[]) 
{
	std::string filepath = "../Data/";


	m_Engine.Run(load);
    return 0;
}
