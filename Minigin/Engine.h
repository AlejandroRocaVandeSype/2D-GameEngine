#pragma once
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