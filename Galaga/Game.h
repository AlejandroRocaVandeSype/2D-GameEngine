#ifndef GAME_ENGINE_GAME
#define GAME_ENGINE_GAME

#include <memory>
#include "structs.h"
#include <vector>

namespace engine
{
	class Scene;
	class GameObject;
}

class Game final
{
	public:

		Game(const engine::Window& window);
		~Game();
		Game(const Game& other) = delete;
		Game(Game&& other) = delete;
		Game& operator=(const Game& other) = delete;
		Game& operator=(Game&& other) = delete;

	private:

		void Initialize();			// Initialize everything needed for the Game
		void SetupBackground();

		const engine::Window m_Window;
		std::vector <std::shared_ptr<engine::GameObject>> m_vSceneGObjects;		// All gameObjects from the scene
};


#endif

