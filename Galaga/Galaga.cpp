#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "Game.h"

dae::Game* loadGame(const Window& window)
{
	dae::Game* pGame{ new dae::Game(window) };

	return pGame;
}

int main(int, char* [])
{
	Window window{ "Galaga", 640.f , 480.f };
	dae::Engine engine("../Data/", window);

	// Lambda function that captures the window parameter and calls
	// the loadGame function with it
	engine.Run([&window]() { return loadGame(window); });

	return 0;
}