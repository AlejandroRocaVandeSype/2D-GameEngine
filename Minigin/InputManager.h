#ifndef GAME_ENGINE_INPUTMANAGER
#define GAME_ENGINE_INPUTMANAGER

#include "Singleton.h"
#include <vector>
#include <map>
#include <memory>		// std::unique_ptrs
#include "Command.h"	//  inc Header -> unique_ptr needs to know the size of the class in order to manage memory for it.
#include "Controller.h"
#include "SDL.h"

namespace dae
{
	enum class InputType {
		KeyPressed,
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);

		// Binding/Unbinding commands
		void BindCommand(SDL_Keycode key, dae::InputType type, std::unique_ptr<Command> command);
		void BindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button, std::unique_ptr<Command> command);
		void UnbindCommand(SDL_Keycode key);
		
	private :

		bool ProcessKeyboardInput(float deltaTime);
		bool ProcessControllersInput(float deltaTime);
		void CheckControllerConnected();

		// *** KEYBOARD ***
		using KeyTypeKeyPair = std::pair<SDL_Keycode, InputType>;
		// All registered Commands for the Keyboard	
		using KeyBoardCommandsMap = std::map<KeyTypeKeyPair, std::unique_ptr<Command>>;
		KeyBoardCommandsMap m_KeyBoardCommands;
		SDL_Keycode m_LastKeyPressed{ SDLK_UNKNOWN };
		KeyBoardCommandsMap::iterator m_KeyboardCommandItr{ m_KeyBoardCommands.end() };

		// *** CONTROLLERS ***
		// All available controllers to be used 
		std::vector<std::unique_ptr<Controller>> m_Controllers{};  
		const int MAX_CONTROLLERS{ 2 };		
		// Button binded to a controller
		using ControllerKey = std::pair<unsigned, Controller::XboxControllerButton>;	
		// A command will be binded to the indicated controller with the indicated button
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;	
		ControllerCommandsMap m_ControllerCommands;

	};

}

#endif