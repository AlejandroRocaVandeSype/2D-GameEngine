#ifndef GAME_ENGINE_GAME
#define GAME_ENGINE_GAME

#include <memory>
#include "structs.h"

namespace engine
{
	class Scene;
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

		const engine::Window m_Window;

};


#endif

