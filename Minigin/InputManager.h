#ifndef GAME_ENGINE_INPUTMANAGER
#define GAME_ENGINE_INPUTMANAGER

#include "Singleton.h"
#include <vector>
#include <map>
#include <memory>		// std::unique_ptrs
#include "Command.h"	//  inc Header -> unique_ptr needs to know the size of the class in order to manage memory for it.
#include "Controller.h"
#include "SDL.h"
#include <iostream>
#include <SDL_mixer.h>

namespace engine
{
	enum class InputType {
		Pressed,		
		Up,
		Down,
		Default			// = Pressed
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:

		bool ProcessInput(float deltaTime);

		// Binding/Unbinding commands
		void BindCommand(std::unique_ptr<Command> command, SDL_Keycode key, engine::InputType type = InputType::Default);
		void BindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button, InputType type, std::unique_ptr<Command> command);
		void UnbindCommand(SDL_Keycode key, InputType type = InputType::Default);
		void UnbindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button, InputType type);
		void UnbindAllCommands();
		void UnbindAllCommands(unsigned int controllerIdx);


		int GetFreeController() const;
		bool IsPlayer1Connected() const;
		
	private :

		bool ProcessKeyboardInput(float deltaTime);
		bool ProcessControllersInput(float deltaTime);
		void CheckControllerConnected();

		// *** KEYBOARD ***
		using KeyTypeKeyPair = std::pair<SDL_Keycode, InputType>;					// SDL_Keycode - InputType
		// All registered Commands for the Keyboard	
		using KeyBoardCommandsMap = std::map<KeyTypeKeyPair, std::unique_ptr<Command>>;
		KeyBoardCommandsMap m_KeyBoardCommands;

		// *** CONTROLLERS ***
		// All available controllers to be used 
		std::vector<std::unique_ptr<Controller>> m_Controllers{};  
		const int MAX_CONTROLLERS{ 2 };		
		// Button binded to a controller
		using ControllerButton = std::pair< Controller::XboxControllerButton, InputType>;	// Contains the button and the type ( pressed, Down etc)
		using ControllerKey = std::pair<unsigned, ControllerButton>;						// ControllerIndex - ControllerButton
		// A command will be binded to the indicated controller with the indicated button
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;	
		ControllerCommandsMap m_ControllerCommands;

		

	};

}

#endif