#ifndef GAME_ENGINE_INPUTMANAGER
#define GAME_ENGINE_INPUTMANAGER

#include "Singleton.h"
#include <unordered_map>
#include <memory>		// std::unique_ptrs
#include "Command.h"	//  inc Header -> unique_ptr needs to know the size of the class in order to manage memory for it.
#include "SDL.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);

		void BindCommand(const SDL_Keycode key, std::unique_ptr<Command> command);
		void UnbindCommand(const SDL_Keycode key);
		
	private :

		bool ProcessKeyboardInput(const SDL_Event& e, float deltaTime);

		std::unordered_map<SDL_Keycode, std::unique_ptr<Command>> m_KeyBoardCommands;  // All registered Commands for the Keyboard

	};

}

#endif