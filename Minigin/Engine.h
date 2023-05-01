#ifndef GAME_ENGINE_ENGINE
#define GAME_ENGINE_ENGINE

#include <string>
#include <functional>

struct Window;
class IGame;
namespace engine
{
	
	class Engine
	{
	public:
		// Indicate where the data is and the window properties
		explicit Engine(const std::string& dataPath, const Window& window);  
		~Engine();
		void Run();

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;
	};
}

#endif