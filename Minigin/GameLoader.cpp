#include "GameLoader.h"

// Base
#include "GameObject.h"

// Scene
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

// Input
#include "Input/InputManager.h"
#include "Input/Controller.h"

#include "Events/EventManager.h"

// Resources
#include "Resources/ResourceManager.h"

// Image components
#include "Componennts/ImageComponent.h"
#include "Componennts/ImageRenderComponent.h"

// Transform/RotatorComponent component
#include "Componennts/TransformComponent.h"
#include "Componennts/RotatorComponent.h"

// Health component
#include "Componennts/HealthComponent.h"

// Points componnent
#include "Componennts/AddPointsComponnent.h"

// Schoot component
#include "Componennts/SchootComponent.h"

// Collision component
#include "Componennts/CollisionComponent.h"

// Text component
#include "Componennts/TextComponent.h"
#include "Componennts/TextRenderComponent.h"

// Lives component
#include "Componennts/LivesComponent.h"

// Score component
#include "Componennts/ScoreComponent.h"

// Commands
#include "Commands/MoveCommand.h"
#include "Commands/SchootCommand.h"
#include "Commands/AddPointsCommand.h"
#include "Commands/DamageCommand.h"

// Enemy components
#include "Enemy/BaseEnemyComponent.h"
#include "Enemy/RecognizerEnemy.h"
#include "Enemy/EnemyController.h"

#include <random>




namespace dae
{
	std::shared_ptr<GameObject> GameLoader::AddPlayer(Scene& scene, int playerIndex, InputManager& , const bool , const std::string texture)
	{
		const auto playerObject = std::make_shared<GameObject>();
		scene.Add(playerObject);
	


		// Add the transform/rotatorComponent component
		playerObject->AddComponent<RotatorComponent>();

		playerObject->GetComponent<TransformComponent>()->SetLocalPosition(200.0f, 200.0f);


		// Add the image component
		ImageComponent* image = playerObject->AddComponent<ImageComponent>();
		playerObject->AddComponent<ImageRenderComponent>();

		playerObject->GetComponent<ImageComponent>()->SetTexture(texture);


		// Add the healt component
		playerObject->AddComponent<HealthComponent>();

		playerObject->GetComponent<HealthComponent>()->SetPlayerIndex(playerIndex);
		playerObject->GetComponent<HealthComponent>()->SetHealth(4);


		// Add the points component
		playerObject->AddComponent<AddPointsComponnent>();

		playerObject->GetComponent<AddPointsComponnent>()->SetPlayerIndex(playerIndex);


		// Add schoot component
		playerObject->AddComponent<SchootComponent>();

		playerObject->GetComponent<SchootComponent>()->SetScene(&scene);


		// Add collision component
		CollisionComponent* collision = playerObject->AddComponent<CollisionComponent>();
		scene.AddCollision(collision);
		collision->SetScene(&scene);

		collision->SetCollisionData({ "Player", playerObject.get() });
		collision->SetBounds(image->GetTextureDimensions().x, image->GetTextureDimensions().y);


		// Add UI
		AddUI(scene, playerIndex, playerObject.get());


		return playerObject;
	}

	std::shared_ptr<GameObject> GameLoader::AddEnemy(Scene& scene, const std::string texture, std::shared_ptr<GameObject> player)
	{
		const auto enemyObject = std::make_shared<GameObject>();
		scene.Add(enemyObject);



		// Add the transform component
		float x = getRandomFloat(0, g_WindowWidth);
		float y = getRandomFloat(0, g_WindowHeight);
		enemyObject->GetComponent<TransformComponent>()->SetLocalPosition(x, y);


		// Add the image component
		ImageComponent* image = enemyObject->AddComponent<ImageComponent>();
		enemyObject->AddComponent<ImageRenderComponent>();

		enemyObject->GetComponent<ImageComponent>()->SetTexture(texture);


		// Add the healt component
		enemyObject->AddComponent<HealthComponent>();

		enemyObject->GetComponent<HealthComponent>()->SetHealth(1);


		// Add the recognizer component
		enemyObject->AddComponent<RecognizerEnemy>();
		const auto baseEnemy = enemyObject->GetComponent<BaseEnemyComponent>();

		enemyObject->GetComponent<RecognizerEnemy>()->Initialize(enemyObject->GetComponent<TransformComponent>()->GetWorldPosition().x, enemyObject->GetComponent<TransformComponent>()->GetWorldPosition().y, 10, 10, player);
		//enemyObject->GetComponent<dae::RecognizerEnemy>()->SetWindowDimensions(g_WindowWidth, g_WindowHeight);
		

		// Add enemy controller
		enemyObject->AddComponent<EnemyController>();


		// Add collision component
		CollisionComponent* collision = enemyObject->AddComponent<CollisionComponent>();
		scene.AddCollision(collision);
		collision->SetScene(&scene);

		collision->SetCollisionData({ "Enemy", enemyObject.get() });
		collision->SetBounds(image->GetTextureDimensions().x, image->GetTextureDimensions().y);

		auto boundHitCallback = std::bind(&dae::BaseEnemyComponent::OnHitCallback, baseEnemy, std::placeholders::_1, std::placeholders::_2);
		collision->SetCallback(boundHitCallback);

	
		return enemyObject;
	}


	// I have no clue why this does not work =) (it works in main though)
	void GameLoader::AddControleler(Scene& scene, int playerIndex, InputManager& inputManager, GameObject* player)
	{
		unsigned controller = inputManager.AddController();
		
		MoveCommand * command{ nullptr };
		constexpr float originalSpeed{ 100.0f };
		constexpr glm::vec2 playerMoveDirectionx{ 1,0 };
		constexpr glm::vec2 playerMoveDirectiony{ 0,1 };



		// Movement
		// move up
		command = inputManager.AddControllerCommand<MoveCommand>
		(
			std::make_unique<MoveCommand>
			(
				player, 
				playerMoveDirectiony, 
				-originalSpeed
			),
			ControllerInput{ controller, ControlerButton::DPAD_UP, ButtonState::Pressed, scene.GetName()}
		);

		// move down
		command = inputManager.AddControllerCommand<MoveCommand>
			(
				std::make_unique<MoveCommand>
				(
					player,
					playerMoveDirectiony,
					originalSpeed
				),
				ControllerInput{ controller, ControlerButton::DPAD_DOWN, ButtonState::Pressed, scene.GetName() }
		);


		// Move left
		command = inputManager.AddControllerCommand<MoveCommand>
			(
				std::make_unique<MoveCommand>
				(
					player,
					playerMoveDirectionx,
					-originalSpeed
				),
				ControllerInput{ controller, ControlerButton::DPAD_LEFT, ButtonState::Pressed, scene.GetName() }
		);

		// Move right
		command = inputManager.AddControllerCommand<MoveCommand>
			(
				std::make_unique<MoveCommand>
				(
					player,
					playerMoveDirectionx,
					originalSpeed
				),
				ControllerInput{ controller, ControlerButton::DPAD_RIGHT, ButtonState::Pressed, scene.GetName() }
		);



		// Schoot
		inputManager.AddControllerCommand<SchootCommand>
		(
			std::make_unique<dae::SchootCommand>
			(
				player, 
				playerIndex
			),
			dae::ControllerInput{ static_cast<unsigned int>(playerIndex), dae::ControlerButton::X, dae::ButtonState::Up, scene.GetName()}
		);

		// Points
		//inputManager.AddControllerCommand<AddPointsCommand>
		//(
		//	std::make_unique<AddPointsCommand>
		//	(
		//		player,
		//		playerIndex
		//	),
		//	ControllerInput{ static_cast<unsigned int>(playerIndex), ControlerButton::X, ButtonState::Up, scene.GetName() }
		//);

		// Health
		inputManager.AddControllerCommand<DamageCommand>
			(
				std::make_unique<DamageCommand>
				(
					player,
					playerIndex
				),
				ControllerInput{ static_cast<unsigned int>(playerIndex), ControlerButton::X, ButtonState::Up, scene.GetName() }
		);
	}

	void GameLoader::AddKeyboard(Scene& scene, int playerIndex, InputManager& inputManager, GameObject* player)
	{
		MoveCommand* command{ nullptr };
		constexpr float originalSpeed{ 100.0f };
		constexpr glm::vec2 playerMoveDirectionx{ 1,0 };
		constexpr glm::vec2 playerMoveDirectiony{ 0,1 };



		// Movement
		// move up
		command = inputManager.AddKeyboardCommand<MoveCommand>
			(
				std::make_unique<MoveCommand>
				(
					player,
					playerMoveDirectiony,
					-originalSpeed
				),
				KeyboardInput{ SDL_SCANCODE_W, ButtonState::Up, scene.GetName() }
		);

		// move down
		command = inputManager.AddKeyboardCommand<MoveCommand>
			(
				std::make_unique<MoveCommand>
				(
					player,
					playerMoveDirectiony,
					originalSpeed
				),
				KeyboardInput{ SDL_SCANCODE_S, ButtonState::Up, scene.GetName() }
		);


		// Move left
		command = inputManager.AddKeyboardCommand<MoveCommand>
			(
				std::make_unique<MoveCommand>
				(
					player,
					playerMoveDirectionx,
					-originalSpeed
				),
				KeyboardInput{ SDL_SCANCODE_A, ButtonState::Up, scene.GetName() }
		);

		// Move right
		command = inputManager.AddKeyboardCommand<MoveCommand>
			(
				std::make_unique<MoveCommand>
				(
					player,
					playerMoveDirectionx,
					originalSpeed
				),
				KeyboardInput{ SDL_SCANCODE_D, ButtonState::Up, scene.GetName() }
		);



		// Schoot
		inputManager.AddKeyboardCommand<SchootCommand>
			(
				std::make_unique<dae::SchootCommand>
				(
					player,
					playerIndex
				),
				KeyboardInput{ SDL_SCANCODE_X, ButtonState::Up, scene.GetName() }
		);

		// Points
		//inputManager.AddKeyboardCommand<AddPointsCommand>
		//	(
		//		std::make_unique<AddPointsCommand>
		//		(
		//			player,
		//			playerIndex
		//		),
		//		KeyboardInput{ SDL_SCANCODE_Q, ButtonState::Up, scene.GetName() }
		//);

		// Health
		inputManager.AddKeyboardCommand<DamageCommand>
			(
				std::make_unique<DamageCommand>
				(
					player,
					playerIndex
				),
				KeyboardInput{ SDL_SCANCODE_E, ButtonState::Up, scene.GetName() }
		);
	}

	void GameLoader::AddUI(Scene& scene, int playerIndex, GameObject* player)
	{
		const SDL_Color color{ 0, 255, 0 };
		const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };


		// Lives display
		{
			const auto livesDisplay = std::make_shared<dae::GameObject>();
			livesDisplay->AddComponent<TextComponent>();
			livesDisplay->AddComponent<TextRenderComponent>();
			livesDisplay->GetComponent<TextComponent>()->SetFont(font);
			livesDisplay->GetComponent<TextComponent>()->SetColor(color);
			livesDisplay->AddComponent<LivesComponent>();


			livesDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(0.0f, 0.0f);
			livesDisplay->GetComponent<dae::LivesComponent>()->SetPlayerIndex(playerIndex);
			livesDisplay->GetComponent<dae::LivesComponent>()->SetLives(player);

			scene.Add(livesDisplay);
		}


		// Score display
		{
			const auto scoreDisplay = std::make_shared<dae::GameObject>();
			scoreDisplay->AddComponent<TextComponent>();
			scoreDisplay->AddComponent<TextRenderComponent>();
			scoreDisplay->GetComponent<TextComponent>()->SetFont(font);
			scoreDisplay->GetComponent<TextComponent>()->SetColor(color);
			scoreDisplay->AddComponent<ScoreComponent>();


			scoreDisplay->GetComponent<TransformComponent>()->SetLocalPosition(0.0f, 40.0f);
			scoreDisplay->GetComponent<ScoreComponent>()->SetPlayerIndex(playerIndex);

			scene.Add(scoreDisplay);
		}
	}

	float GameLoader::getRandomFloat(float min, float max)
	{
		static std::random_device rd;  // Obtain a random number from hardware
		static std::mt19937 engine(rd()); // Seed the generator

		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(engine);
	}
}