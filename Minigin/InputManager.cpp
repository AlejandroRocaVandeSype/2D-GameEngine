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
		// There is still room to add more controllers to the game
		CheckControllerConnected();
	}
	
	if (!m_Controllers.empty())
	{
		// THere is at least one controller conected -> Process Controller Input
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
			KeyTypeKeyPair keyPair{ std::make_pair(keyDown, dae::InputType::Pressed) };

			if (m_KeyBoardCommands.find(keyPair) != m_KeyBoardCommands.end())
			{
				m_KeyboardCommandItr = m_KeyBoardCommands.find(keyPair);
				m_LastKeyPressed = keyDown;
				m_Pressed = true;
			}
			
		}

		if (e.type == SDL_KEYUP)
		{
			
			SDL_Keycode keyUp{ e.key.keysym.sym };
			if (m_LastKeyPressed == keyUp)
			{
				m_KeyboardCommandItr = m_KeyBoardCommands.end();
				m_LastKeyPressed = SDLK_UNKNOWN; // Reset last pressed key
				m_Pressed = false;
			}
			else
			{
				KeyTypeKeyPair keyPair{ std::make_pair(keyUp, dae::InputType::Up) };
				if (m_KeyBoardCommands.find(keyPair) != m_KeyBoardCommands.end())
				{
					m_KeyboardCommandItr = m_KeyBoardCommands.find(keyPair);
					m_KeyboardCommandItr->second->Execute(deltaTime);
				}
			}
			
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	if (m_KeyboardCommandItr != m_KeyBoardCommands.end() && m_Pressed)
	{
		// A key binded to a Command is being pressed -> Execute the command
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
			controller->Update();

			// Loop through all the available commands
			for (const auto& controllerCommand : m_ControllerCommands)
			{
				// To which controller is the command associate 
				unsigned controllerIdx{ controllerCommand.first.first };
				if (controller->GetControllerIdx() == controllerIdx)
				{
					// Command found for this Controller ->
					// Check if any button was pressed/released etc
					auto button{ controllerCommand.first.second };
					
					if (button.second == InputType::Pressed)
					{
						if (controller->IsPressed(button.first))
						{
							controllerCommand.second->Execute(deltaTime);		
							continue;  // Check next command
						}
					}

					if (button.second == InputType::Up)
					{
						if (controller->IsUp(button.first))
						{
							controllerCommand.second->Execute(deltaTime);	
							continue;
						}
					}

					if (button.second == InputType::Down)
					{
						if (controller->IsDown(button.first))
						{
							controllerCommand.second->Execute(deltaTime);
							continue;
						}
					}
										
				}
			} // End FOR loop commands
		
		} // End IF Controller
	}

	return true;
}

// BIND COMMAND TO A KEY
// If key already exits in the map it will simply swap the command associated with the key
void dae::InputManager::BindCommand( std::unique_ptr<Command> command, SDL_Keycode key, dae::InputType type)
{
	if (type == InputType::Default)
	{
		type = InputType::Pressed;
	}

	KeyTypeKeyPair keyPair{ std::make_pair(key, type) };
	m_KeyBoardCommands[keyPair] = std::move(command);
}

// BIND COMMAND TO AN EXISTING CONTROLLER
void  dae::InputManager::BindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button,
										InputType type, std::unique_ptr<Command> command)
{
	ControllerButton buttonType{ std::make_pair(button, type) };
	ControllerKey keyPair{ std::make_pair(controllerIdx, buttonType) };

	m_ControllerCommands[keyPair] = std::move(command);
	
}

// Unbind the command associated to the key passed through parameter
// This will delete the element from the command map
void  dae::InputManager::UnbindCommand(SDL_Keycode key, dae::InputType type)
{
	if (type == InputType::Default)
	{
		type = InputType::Pressed;
	}

	KeyTypeKeyPair keyPair{ std::make_pair(key, type) };
	auto commandItr = m_KeyBoardCommands.find(keyPair);
	if (commandItr != m_KeyBoardCommands.end())
	{
		m_KeyBoardCommands.erase(commandItr);
	}
	
}

void dae::InputManager::UnbindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button,
										InputType type)
{
	ControllerButton buttonType{ std::make_pair(button, type) };
	ControllerKey keyPair{ std::make_pair(controllerIdx, buttonType) };
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