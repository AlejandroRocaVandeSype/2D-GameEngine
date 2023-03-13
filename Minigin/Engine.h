#ifndef GAME_ENGINE_ENGINE
#define GAME_ENGINE_ENGINE

#include <string>
#include <functional>

namespace dae
{
	class Game;
	class Engine
	{
	public:
		explicit Engine(const std::string& dataPath);
		~Engine();
		void Run(const std::function<Game*()>& loadGame);

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;
	};
}

#endif