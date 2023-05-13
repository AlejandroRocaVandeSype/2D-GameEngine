#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Engine.h"
#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Servicealocator.h"
#include "SDL_SoundSystem.h"
#include "Logging_Sound_System.h"
#include <chrono>
#include "structs.h"

SDL_Window* g_window{};

using namespace std::chrono;

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

engine::Engine::Engine(const std::string &dataPath, const engine::Window& window)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		window.title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int(window.width),
		int(window.height),
		SDL_WINDOW_OPENGL
	);
	
	
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	
	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	// INIT SOUND SYSTEM
	#if _DEBUG
		// If debug mode use the log version
		Servicealocator::Register_sound_system(new Logging_Sound_System(new SDL_SoundSystem()));
	#else
		Servicealocator::Register_sound_system(new SDL_SoundSystem());
	#endif
}

engine::Engine::~Engine()
{
	Renderer::GetInstance().Destroy();
	Servicealocator::ShutDownSoundSystem();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void engine::Engine::Run()
{
	auto& input = InputManager::GetInstance();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	bool doContinue = true;

	// Limit the FPS and reduce CPU usage
	const int TARGET_FRAME_RATE{ 60 };
	const int TARGET_FRAME_TIME{ 1000 / TARGET_FRAME_RATE };
	
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (doContinue)
	{
		const auto currentTime = high_resolution_clock::now();
		const float deltaTime = duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		
		doContinue = input.ProcessInput(deltaTime);
		
		// Update Game
		sceneManager.Update(deltaTime);
	
		// Render Game
		renderer.Render();

		const auto sleepTime = currentTime + milliseconds(TARGET_FRAME_TIME) - high_resolution_clock::now();
		if (sleepTime.count() > 0)
		{
			// Sleep to avoid going too fast and max our fps to 60
			std::this_thread::sleep_for(sleepTime);
		}
			
	}

}
