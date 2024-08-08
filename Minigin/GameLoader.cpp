// General includes
#include "GameLoader.h"
#include "GameObject.h"

// scene includes
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

// Soundm includes
#include "Sound/SoundSystem.h"

// Input includes
#include "Input/InputManager.h"

// Resources includes
#include "Resources/ResourceManager.h"


// Image components includes
#include "Componennts/ImageComponent.h"
#include "Componennts/ImageRenderComponent.h"

// Collision component includes
#include "Componennts/CollisionComponent.h"

// Health & Points component includes
#include "Componennts/HealthComponent.h"
#include "Componennts/AddPointsComponnent.h"

// Schoot & Hit component includes
#include "Componennts/SchootComponent.h"
#include "HitComponennt.h"

// Commands includes
#include "Commands/MoveCommand.h"
#include "Commands/SchootCommand.h"
#include "HitCommand.h"
#include "SkipLevelCommand.h"
#include "StartSinglePlayerCommand.h"
#include "VolumeCommand.h"

// EggWall & DiamondWall component includes
#include "EggBlockComponent.h"
#include "DiamondBlockComponent.h"

// Enemy components includes
#include "Enemy/BaseEnemyComponent.h"
#include "Enemy/RecognizerEnemy.h"
#include "Enemy/EnemyController.h"

// Rotator component includes
#include "Componennts/RotatorComponent.h"

// Text component includes
#include "Componennts/TextComponent.h"
#include "Componennts/TextRenderComponent.h"

// Lives & Score component includes
#include "Componennts/LivesComponent.h"
#include "Componennts/ScoreComponent.h"

// HighScore component includes
#include "HighScoreComponent.h"



namespace dae
{	
	void GameLoader::LoadSound(SoundSystem* soundSystem, const SoundData& soundData)
	{
		soundSystem->NotifySound(soundData);
	}

	std::shared_ptr<GameObject> GameLoader::LoadLevelBorder(Scene& scene)
	{	// Load Level boarder
		const auto backGroundImage = std::make_shared<dae::GameObject>();

		// Image
		const auto image = backGroundImage->AddComponent<dae::ImageComponent>();
		backGroundImage->AddComponent<dae::ImageRenderComponent>();
		backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("level.png");

		// Transform
		backGroundImage->GetComponent<dae::TransformComponent>()->SetLocalPosition(g_WindowWidth / 2 - image->GetTextureDimensions().x / 2, g_WindowHeight / 2 - image->GetTextureDimensions().y / 2);

		// Collision
		SetUpBaseCollision(backGroundImage.get(), "World", scene);


		scene.Add(backGroundImage);
		return backGroundImage;
	}

	std::shared_ptr<GameObject> GameLoader::LoadPlayer(Scene& scene, int playerIndex, unsigned int conntroller, const std::string texture, const glm::vec2 position, const glm::vec2 offset)
	{
		const auto playerObject = std::make_shared<GameObject>();
		scene.Add(playerObject);
	
		playerObject->AddComponent<dae::RotatorComponent>();

		// Set the position
		SetUpTransform(playerObject.get(), position, offset);
	

		// Add the image component
		{
			playerObject->AddComponent<ImageComponent>();
			playerObject->AddComponent<ImageRenderComponent>();
			playerObject->GetComponent<ImageComponent>()->SetTexture(texture);
		}
	
		// Add the healt component
		{
			playerObject->AddComponent<HealthComponent>();
			playerObject->GetComponent<HealthComponent>()->SetPlayerIndex(playerIndex);
			playerObject->GetComponent<HealthComponent>()->SetHealth(4);
		}

		// Add the points component
		{
			playerObject->AddComponent<AddPointsComponnent>();
			playerObject->GetComponent<AddPointsComponnent>()->SetPlayerIndex(playerIndex);
		}
	
		// Add schoot & hit component
		{
			playerObject->AddComponent<SchootComponent>();
			playerObject->GetComponent<SchootComponent>()->SetScene(&scene);

			playerObject->AddComponent<HitComponennt>();
			playerObject->GetComponent<HitComponennt>()->SetScene(&scene);
		}



		// Add collision component
		SetUpBaseCollision(playerObject.get(), "Player", scene);
	
	
		if (playerIndex == 0)
		{
			AddKeyboard(scene, playerObject.get());
		}
		AddControleler(scene, playerObject.get(), conntroller);

		SetupPlayerControls(playerObject.get(), scene);

		// Add UI
		if (playerIndex == 0)
		{
			SetUpUI(scene, playerIndex, playerObject.get(), { 0, 0 });
		}
		else if (playerIndex == 1)
		{
			SetUpUI(scene, playerIndex, playerObject.get(), {0, g_WindowHeight - 50.0f });
		}

	
		return playerObject;
	}

	std::shared_ptr<GameObject> GameLoader::LoadEnemy(dae::Scene& scene, const glm::vec2& position, const std::string& texture, std::vector<dae::GameObject*> players, const std::shared_ptr<dae::GameObject>& backGroundImage, const glm::vec2& offset)
	{
		const auto enemy = std::make_shared<dae::GameObject>();
		scene.Add(enemy);


		// Image
		{
			enemy->AddComponent<dae::ImageComponent>();
			enemy->AddComponent<dae::ImageRenderComponent>();
			enemy->GetComponent<dae::ImageComponent>()->SetTexture(texture);
		}


		// Transform
		SetUpTransform(enemy.get(), position, offset);


		// Health
		{
			enemy->AddComponent<dae::HealthComponent>()->SetHealth(1);
		}


		// Enemy components
		enemy->AddComponent<dae::RecognizerEnemy>()->Initialize(enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().x, enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().y, 10, 10, players);

		const auto baseEnemy = enemy->GetComponent<dae::BaseEnemyComponent>();
		enemy->GetComponent<dae::RecognizerEnemy>()->SetWindowDimensions(backGroundImage->GetComponent<dae::TransformComponent>()->GetWorldPosition().x,
			backGroundImage->GetComponent<dae::TransformComponent>()->GetWorldPosition().y,
			backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x * 2,
			backGroundImage->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y * 2
		);

		enemy->AddComponent<dae::EnemyController>();



		// Collision
		SetUpBaseCollision(enemy.get(), "Enemy", scene);

		auto boundHitCallback = std::bind(&dae::BaseEnemyComponent::OnHitCallback, baseEnemy, std::placeholders::_1, std::placeholders::_2);
		enemy->GetComponent<dae::CollisionComponent>()->SetCallback(boundHitCallback);



		return enemy;
	}

	void GameLoader::LoadWall(Scene& scene, const glm::vec2 position, const std::string& texture, const glm::vec2 offset)
	{
		const auto wall = std::make_shared<dae::GameObject>();

		// Image
		{
			wall->AddComponent<dae::ImageComponent>();
			wall->AddComponent<dae::ImageRenderComponent>();
			wall->GetComponent<dae::ImageComponent>()->SetTexture(texture);
		}


		// Transform
		SetUpTransform(wall.get(), position, offset);

		// Collision
		SetUpBaseCollision(wall.get(), "Wall", scene);

		scene.Add(wall);
	}

	void GameLoader::LoadEggWall(Scene& scene, const glm::vec2 position, const std::string& texture, const glm::vec2 offset, GameObject* background, std::vector<dae::GameObject*> player)
	{
		const auto eggWall = std::make_shared<dae::GameObject>();

		// Image
		eggWall->AddComponent<dae::ImageComponent>();
		eggWall->AddComponent<dae::ImageRenderComponent>();
		eggWall->GetComponent<dae::ImageComponent>()->SetTexture(texture);
		// Transform
		SetUpTransform(eggWall.get(), position, offset);

		eggWall->AddComponent<dae::EggBlockComponent>();
		eggWall->GetComponent<dae::EggBlockComponent>()->SetPlayer(player);
		eggWall->GetComponent<dae::EggBlockComponent>()->SetBackground(background);



		// Collision
		SetUpBaseCollision(eggWall.get(), "EggWall", scene);

		auto boundHitCallback = std::bind(&dae::EggBlockComponent::OnHitCallback, eggWall->GetComponent<dae::EggBlockComponent>(), std::placeholders::_1, std::placeholders::_2);
		eggWall->GetComponent<dae::CollisionComponent>()->SetCallback(boundHitCallback);




		scene.Add(eggWall);
	}

	void GameLoader::LoadDiamondWall(Scene& scene, const glm::vec2 position, const std::string& texture, const glm::vec2 offset, std::vector<dae::GameObject*> player)
	{
		const auto diamondWall = std::make_shared<dae::GameObject>();

		// Image
		diamondWall->AddComponent<dae::ImageComponent>();
		diamondWall->AddComponent<dae::ImageRenderComponent>();
		diamondWall->GetComponent<dae::ImageComponent>()->SetTexture(texture);
	
		
		// Transform
		SetUpTransform(diamondWall.get(), position, offset);


		// Diamond component
		{		
			diamondWall->AddComponent<dae::DiamondBlockComponent>();
		}


		// Collision
		SetUpBaseCollision(diamondWall.get(), "DiamondWall", scene);

	


		scene.Add(diamondWall);
	}

	void GameLoader::LoadHighScoreScene(const std::string& sceneName)
	{
		const SDL_Color color{ 0, 255, 0 };
		const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };


		auto& scene = dae::SceneManager::GetInstance().CreateScene(sceneName);
		scene.SetActive(true);


		const auto& highScoreText = std::make_shared<dae::GameObject>();

		highScoreText->GetComponent<dae::TransformComponent>()->SetLocalPosition(g_WindowWidth / 2 - 100.f, g_WindowHeight / 2);

		// Text
		highScoreText->AddComponent<dae::TextComponent>();
		highScoreText->AddComponent<dae::TextRenderComponent>();

		highScoreText->GetComponent<dae::TextComponent>()->SetFont(font);
		highScoreText->GetComponent<dae::TextComponent>()->SetColor(color);

		highScoreText->AddComponent<dae::HighScoreComponent>();
		highScoreText->GetComponent<dae::HighScoreComponent>()->AddScore(SceneManager::GetInstance().m_Score, SceneManager::GetInstance().GetSceneByName("StartScreen").m_playerName);
		highScoreText->GetComponent<dae::HighScoreComponent>()->LoadHighScores();


		scene.Add(highScoreText);
	}



	void GameLoader::SetUpBaseCollision(GameObject* object, const std::string& tag, Scene& scene)
	{
		dae::CollisionComponent* collision = object->AddComponent<dae::CollisionComponent>();
		scene.AddCollision(collision);
		collision->SetScene(&scene);

		collision->SetCollisionData({ tag, object });
		collision->SetBounds(
			object->GetComponent<dae::ImageComponent>()->GetTextureDimensions().x,
			object->GetComponent<dae::ImageComponent>()->GetTextureDimensions().y
		);
	}

	void GameLoader::SetUpTransform(GameObject* object, const glm::vec2& position, const glm::vec2 offset)
	{
		object->GetComponent<dae::TransformComponent>()->SetLocalPosition(
			position.x * 20 + offset.x - g_WindowWidth / 9,
			position.y * 20 + offset.y - g_WindowHeight / 9
		);	
	}

	void GameLoader::SetUpUI(Scene& scene, int playerIndex, GameObject* player, glm::vec2 position)
	{
		const SDL_Color color{ 0, 255, 0 };
		const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };
		
		
		// Lives display
		{
			const auto livesDisplay = std::make_shared<dae::GameObject>();
			livesDisplay->GetComponent<TransformComponent>()->SetLocalPosition(position.x, position.y);

			// Text
			{
				livesDisplay->AddComponent<TextComponent>();
				livesDisplay->AddComponent<TextRenderComponent>();
				livesDisplay->GetComponent<TextComponent>()->SetFont(font);
				livesDisplay->GetComponent<TextComponent>()->SetColor(color);
			}

			// Lives
			{
				livesDisplay->AddComponent<LivesComponent>();
				livesDisplay->GetComponent<dae::LivesComponent>()->SetPlayerIndex(playerIndex);
				livesDisplay->GetComponent<dae::LivesComponent>()->SetLives(player);
			}
	
			scene.Add(livesDisplay);
		}
		
		
		// Score display
		{
			const auto scoreDisplay = std::make_shared<dae::GameObject>();
			scoreDisplay->GetComponent<TransformComponent>()->SetLocalPosition(position.x, position.y + 25.0f);

			// Text
			{
				scoreDisplay->AddComponent<TextComponent>();
				scoreDisplay->AddComponent<TextRenderComponent>();
				scoreDisplay->GetComponent<TextComponent>()->SetFont(font);
				scoreDisplay->GetComponent<TextComponent>()->SetColor(color);
			}

			// Score
			{
				scoreDisplay->AddComponent<ScoreComponent>();
				scoreDisplay->GetComponent<ScoreComponent>()->SetPlayerIndex(playerIndex);
			}

			
		
			scene.Add(scoreDisplay);
		}
	}


	void GameLoader::SetupPlayerControls(GameObject* player, dae::Scene& scene)
	{
		auto& inputManager = InputManager::GetInstance();

		{
			// Game commnads

			// Skipping level
			inputManager.AddKeyboardCommand<dae::SkipLevelCommand>(
				std::make_unique<dae::SkipLevelCommand>(player),
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

	void GameLoader::AddControleler(Scene& scene, GameObject* player, unsigned controller)
	{
		auto& inputManager = InputManager::GetInstance();

		MoveCommand* command{ nullptr };
		constexpr float originalSpeed{ 100.0f };
		constexpr glm::vec2 playerMoveDirectionx{ 1,0 };
		constexpr glm::vec2 playerMoveDirectiony{ 0,1 };


		// Movement
		{
			command = inputManager.AddControllerCommand<dae::MoveCommand>(
				std::make_unique<dae::MoveCommand>(player, playerMoveDirectiony, -originalSpeed),
				dae::ControllerInput{ controller, dae::ControlerButton::DPAD_UP, dae::ButtonState::Pressed, scene.GetName() }
			);

			command = inputManager.AddControllerCommand<dae::MoveCommand>(
				std::make_unique<dae::MoveCommand>(player, -playerMoveDirectiony, originalSpeed),
				dae::ControllerInput{ controller, dae::ControlerButton::DPAD_DOWN,dae::ButtonState::Pressed, scene.GetName() }
			);

			command = inputManager.AddControllerCommand<dae::MoveCommand>(
				std::make_unique<dae::MoveCommand>(player, -playerMoveDirectionx, -originalSpeed),
				dae::ControllerInput{ controller, dae::ControlerButton::DPAD_LEFT, dae::ButtonState::Pressed, scene.GetName() }
			);

			command = inputManager.AddControllerCommand<dae::MoveCommand>(
				std::make_unique<dae::MoveCommand>(player, playerMoveDirectionx, originalSpeed),
				dae::ControllerInput{ controller, dae::ControlerButton::DPAD_RIGHT, dae::ButtonState::Pressed, scene.GetName() }
			);
		}


		// Schooting
		{
			inputManager.AddControllerCommand<dae::SchootCommand>(
				std::make_unique<dae::SchootCommand>(player, 1),
				dae::ControllerInput{ controller, dae::ControlerButton::X, dae::ButtonState::Up, scene.GetName() });
		}

		// Hit
		{
			inputManager.AddControllerCommand<dae::HitCommand>(
				std::make_unique<dae::HitCommand>(player, 0),
				dae::ControllerInput{ controller, dae::ControlerButton::B, dae::ButtonState::Up, scene.GetName() });
		}
	}

	void GameLoader::AddKeyboard(Scene& scene, GameObject* player)
	{
		auto& inputManager = InputManager::GetInstance();

		MoveCommand* command{ nullptr };
		constexpr float originalSpeed{ 100.0f };
		constexpr glm::vec2 playerMoveDirectionx{ 1,0 };
		constexpr glm::vec2 playerMoveDirectiony{ 0,1 };



		// Movement
		{
			command = inputManager.AddKeyboardCommand<dae::MoveCommand>(
				std::make_unique<dae::MoveCommand>(player, playerMoveDirectiony, -originalSpeed),
				dae::KeyboardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed, scene.GetName() }
			);

			command = inputManager.AddKeyboardCommand<dae::MoveCommand>(
				std::make_unique<dae::MoveCommand>(player, -playerMoveDirectiony, originalSpeed),
				dae::KeyboardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed, scene.GetName() }
			);


			command = inputManager.AddKeyboardCommand<dae::MoveCommand>(
				std::make_unique<dae::MoveCommand>(player, -playerMoveDirectionx, -originalSpeed),
				dae::KeyboardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed, scene.GetName() }
			);

			command = inputManager.AddKeyboardCommand<dae::MoveCommand>(
				std::make_unique<dae::MoveCommand>(player, playerMoveDirectionx, originalSpeed),
				dae::KeyboardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed, scene.GetName() }
			);
		}
	
	
		// Schooting
		{
			inputManager.AddKeyboardCommand<dae::SchootCommand>(
				std::make_unique<dae::SchootCommand>(player, 0),
				dae::KeyboardInput{ SDL_SCANCODE_X, dae::ButtonState::Up, scene.GetName() });
		}

		// Hit
		{
			inputManager.AddKeyboardCommand<dae::HitCommand>(
				std::make_unique<dae::HitCommand>(player, 0),
				dae::KeyboardInput{ SDL_SCANCODE_Z, dae::ButtonState::Up, scene.GetName() });
		}

	}

}