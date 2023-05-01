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
	engine::Engine engine("../Data/", window);

	std::unique_ptr<Game> game = std::make_unique<Game>(window);
	engine.Run();

	return 0;
}