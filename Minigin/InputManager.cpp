#include "InputManager.h"
#include "backends/imgui_impl_sdl.h"
#include <iostream>

// If false means we want to close the game
bool dae::InputManager::ProcessInput(float deltaTime)
{
	// Handle keyboard Input
	bool keepGameRunning = ProcessKeyboardInput(deltaTime);
	if (keepGameRunning == false)
	{
		// Close the game
		return keepGameRunning;
	}

	if (int(m_Controllers.size()) < MAX_CONTROLLERS)
	{
		CheckControllerConnected();
	}

	// Process controllers input if there are any controllers
	if (!m_Controllers.empty())
	{
		return ProcessControllersInput(deltaTime);
	}

	return true;
}

bool dae::InputManager::ProcessKeyboardInput(float deltaTime)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) 
		{
			// User wants to close the game
			return false;
		}

		if (e.type == SDL_KEYDOWN)
		{
			// Check if the key pressed is binded to any command..
			auto keyDown{ e.key.keysym.sym };
			KeyTypeKeyPair keyPair{ std::make_pair(keyDown, dae::InputType::KeyPressed) };
			m_KeyboardCommandItr = m_KeyBoardCommands.find(keyPair);
			
			m_LastKeyPressed = keyDown;

		}

		if (e.type == SDL_KEYUP)
		{
			SDL_Keycode keyUp{ e.key.keysym.sym };
			if (m_LastKeyPressed == keyUp)
			{
				m_KeyboardCommandItr = m_KeyBoardCommands.end();
				m_LastKeyPressed = SDLK_UNKNOWN; // Reset last pressed key
			}

		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	if (m_KeyboardCommandItr != m_KeyBoardCommands.end())
	{
		// A key binded to a Command has been downed, released or is being pressed -> Execute the command
		m_KeyboardCommandItr->second->Execute(deltaTime);
	}

	return true;
}

// -----------------------------------------------------------------------------
//			*Checks if a new Controller has been added to the Game*
// This will only be checked the first time the Controller has been added to the 
// game. Once a controller is added, we use the isConnected parameter from the 
// controller object itselt to check if it disconnects or reconnect
// -----------------------------------------------------------------------------
void dae::InputManager::CheckControllerConnected()
{
	// We can still add more controllers to the Game
	unsigned int controllerIdx{ unsigned(m_Controllers.size()) };
	if (Controller::IsNewControllerAdded(controllerIdx))
	{
		// New controller
		m_Controllers.emplace_back(std::make_unique<Controller>(controllerIdx));
	}

}

// Process Input from all conected controllers
bool dae::InputManager::ProcessControllersInput(float deltaTime)
{
	for (const auto& controller : m_Controllers)
	{
		if (controller->IsConnected())
		{
			//std::cout << "Update controller " << controller->GetControllerIdx() << "\n";	
			controller->Update();

			// Loop through all the available commands
			for (const auto& controllerCommand : m_ControllerCommands)
			{
				unsigned controllerIdx{ controllerCommand.first.first };
				if (controller->GetControllerIdx() == controllerIdx)
				{
					// Check if the button of this controller is pressed
					auto button{ controllerCommand.first.second };
					if (controller->IsPressed(button))
					{
						controllerCommand.second->Execute(deltaTime);
						return true;
					}
				}
			}
		
		}
	}

	return true;
}

// Bind Commands to keys.
// If key already exits in the map it will simply swap the command associated with the key
void dae::InputManager::BindCommand( std::unique_ptr<Command> command, SDL_Keycode key, dae::InputType type)
{
	if (type == InputType::Default)
	{
		type = InputType::KeyPressed;
	}

	KeyTypeKeyPair keyPair{ std::make_pair(key, type) };
	m_KeyBoardCommands[keyPair] = std::move(command);
}

// Bind commands to a existing Controller
void  dae::InputManager::BindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button,
	std::unique_ptr<Command> command)
{
	ControllerKey keyPair{ std::make_pair(controllerIdx, button) };

	m_ControllerCommands[keyPair] = std::move(command);
	
}

// Unbind the command associated to the key passed through parameter
// This will delete the element from the command map
void  dae::InputManager::UnbindCommand(SDL_Keycode key, dae::InputType type)
{
	if (type == InputType::Default)
	{
		type = InputType::KeyPressed;
	}

	KeyTypeKeyPair keyPair{ std::make_pair(key, type) };
	auto commandItr = m_KeyBoardCommands.find(keyPair);
	if (commandItr != m_KeyBoardCommands.end())
	{
		m_KeyBoardCommands.erase(commandItr);
	}
	
}

void dae::InputManager::UnbindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button)
{
	ControllerKey keyPair{ std::make_pair(controllerIdx, button) };
	auto commandItr = m_ControllerCommands.find(keyPair);
	if (commandItr != m_ControllerCommands.end())
	{
		m_ControllerCommands.erase(commandItr);
	}
	
}

void dae::InputManager::UnbindAllCommands()
{
	for (auto commandItr{ m_KeyBoardCommands.begin() }; commandItr != m_KeyBoardCommands.end();)
	{
		commandItr = m_KeyBoardCommands.erase(commandItr);
	}

	for (auto commandItr{ m_ControllerCommands.begin() }; commandItr != m_ControllerCommands.end();)
	{
		commandItr = m_ControllerCommands.erase(commandItr);
	}
}