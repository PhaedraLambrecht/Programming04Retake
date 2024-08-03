#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "Input/InputManager.h"
#include "Scene/SceneManager.h"
#include "Base/Renderer.h"
#include "Resources/ResourceManager.h"

#include "GameTime.h"
#include <chrono>
#include <thread>

#include "Events/EventManager.h"
#include "Sound/SoundManager.h"
#include "Sound/SDLSoundSystem.h"

#include <iostream>


void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string& dataPath, int windowWidth, int windowHeight)
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_window = SDL_CreateWindow(
		"Programming 4 retake",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL
	);
	if (m_window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_window);

	ResourceManager::GetInstance().Init(dataPath);
	auto& sounds = SoundManager::GetInstance();
	sounds.Init(dataPath);

	sounds.SetSoundSystem(std::make_unique<SDLSoundSystem>());
}

dae::Minigin::~Minigin()
{
	SceneManager::GetInstance().Destroy();
	Renderer::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();

	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	auto& eventHandler = EventManager::GetInstance();


	bool doContinue = true;
	float lag{ 0.0f };

	while (doContinue)
	{
		GameTime::GetInstance().Update();
		lag += GameTime::GetInstance().GetDeltaTime();


		doContinue = input.ProcessInput();
		eventHandler.HandleEvents();

		while (lag >= GameTime::GetInstance().GetFixedTimeStep())
		{
			sceneManager.FixedUpdate(GameTime::GetInstance().GetFixedTimeStep());
			lag -= GameTime::GetInstance().GetFixedTimeStep();
		}

		sceneManager.Update();
		renderer.Render();


		const auto sleepTime = GameTime::GetInstance().GetPreviousTime() + std::chrono::milliseconds(GameTime::GetInstance().GetMSPerFrame()) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);
	}
}

SDL_Window* dae::Minigin::GetWindow()
{
	return m_window;
}
