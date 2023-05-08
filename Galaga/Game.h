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

		// ALL GAME SCENES
		const std::string FIRST_STAGE;
		const std::string SECOND_STAGE;
		const std::string THIRD_STAGE;

		// GAMEOBJECT TAGS
		const std::string ENEMY_TAG;
		const std::string PLAYER_TAG;
		const std::string LEVEL_TAG;
		const std::string UI_TAG;

		const engine::Window m_Window;

};


#endif

