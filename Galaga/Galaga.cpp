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
	engine::Window window{ "Galaga", 560.f , 640.f };
	engine::Engine engine("../Data/", window);

	std::unique_ptr<Game> game = std::make_unique<Game>(window);
	engine.Run();

	return 0;
}