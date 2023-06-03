#ifndef GAME_ENGINE_GAME
#define GAME_ENGINE_GAME

#include <string>
class Game final
{
	public:

		Game();
		~Game();
		Game(const Game& other) = delete;
		Game(Game&& other) = delete;
		Game& operator=(const Game& other) = delete;
		Game& operator=(Game&& other) = delete;

	private:

		void Initialize();			// Initialize everything needed for the Game
		void AddAllSounds();

		// ALL GAME SCENES
		const std::string FIRST_STAGE;
		const std::string SECOND_STAGE;
		const std::string THIRD_STAGE;




};


#endif

