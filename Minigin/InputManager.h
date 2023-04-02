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
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);

		void NewAvailableController(unsigned controllerIdx);

		// Binding/Unbinding commands
		void BindCommand(SDL_Keycode key, std::unique_ptr<Command> command);
		void BindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button, std::unique_ptr<Command> command);
		void UnbindCommand(SDL_Keycode key);
		
	private :

		bool ProcessKeyboardInput(float deltaTime);
		bool ProcessControllersInput(float deltaTime);
		void CheckControllerConnected();

		// *** KEYBOARD ***
		// All registered Commands for the Keyboard	
		std::map<SDL_Keycode, std::unique_ptr<Command>> m_KeyBoardCommands{};  
		
		// *** CONTROLLERS ***
		// All available controllers to be used (doesn't mean that are connected to the PC)
		std::vector<std::unique_ptr<Controller>> m_Controllers{};  
		const int MAX_CONTROLLERS{ 2 };		
		const unsigned MAX_VALID_CONTROLLER_IDX{ 3 };			// More than this value is not a valid controller index
		// Button binded to a controller
		using ControllerKey = std::pair<unsigned, Controller::XboxControllerButton>;	
		// A command will be binded to the indicated controller with the indicated button
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;	
		ControllerCommandsMap m_ControllerCommands;

	};

}

#endif