#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "Game.h"

int main(int, char* [])
{
	Window window{ "Galaga", 640.f , 480.f };
	dae::Engine engine("../Data/", window);

	std::unique_ptr<dae::Game> game = std::make_unique<dae::Game>(window);
	engine.Run();

	return 0;
}