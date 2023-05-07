#include "InputManager.h"
#include "backends/imgui_impl_sdl.h"
#include <iostream>

// If false means we want to close the game
bool engine::InputManager::ProcessInput(float deltaTime)
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


// -----------------------------------------------------------------------------
//							*PROCESS KEYBOARD INPUT*
// SDL_PollEven -> To check if a key is beind pressed (down once) or released (up once)
// SDL_GetKeyboardState -> is used to check if a key is being pressed continuously 
// between frames.
// -----------------------------------------------------------------------------
bool engine::InputManager::ProcessKeyboardInput(float deltaTime)
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
			auto keyDown{ e.key.keysym.sym };
			KeyTypeKeyPair keyPair{ std::make_pair(keyDown, engine::InputType::Down) };

			auto commandItr = m_KeyBoardCommands.find(keyPair);
			if (commandItr != m_KeyBoardCommands.end())
			{
				// Founded a key binded to a Command -> Execute it
				commandItr->second->Execute(deltaTime);
			}
		}

		if (e.type == SDL_KEYUP)
		{
			auto keyUp{ e.key.keysym.sym };
			KeyTypeKeyPair keyPair{ std::make_pair(keyUp, engine::InputType::Up) };

			auto commandItr = m_KeyBoardCommands.find(keyPair);
			if (commandItr != m_KeyBoardCommands.end())
			{
				commandItr->second->Execute(deltaTime);
			}
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	// CHECK IF ANY KEY IS BEING HOLD CONTINUOUSLY
	const Uint8* state = SDL_GetKeyboardState(NULL);
	for (const auto& keyBoardCommand : m_KeyBoardCommands)
	{
		auto inputType = keyBoardCommand.first.second;
		if (inputType == engine::InputType::Pressed)
		{
			// keyInput == Pressed -> Check if the key is being pressed
			if (state[SDL_GetScancodeFromKey(keyBoardCommand.first.first)])
			{
				keyBoardCommand.second->Execute(deltaTime);
			}
		}
	}

	return true;
}

// -----------------------------------------------------------------------------
//			*Checks if a new Controller has been added to the Game*
// This will only be checked the first time the Controller has been added to the 
// game. Once a controller is added, we use the isConnected parameter from the 
// controller object itselt to check if it disconnects or reconnect
// -----------------------------------------------------------------------------
void engine::InputManager::CheckControllerConnected()
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
bool engine::InputManager::ProcessControllersInput(float deltaTime)
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
void engine::InputManager::BindCommand( std::unique_ptr<Command> command, SDL_Keycode key, engine::InputType type)
{
	if (type == InputType::Default)
	{
		type = InputType::Down;
	}

	KeyTypeKeyPair keyPair{ std::make_pair(key, type) };
	m_KeyBoardCommands[keyPair] = std::move(command);
}

// BIND COMMAND TO AN EXISTING CONTROLLER
void  engine::InputManager::BindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button,
										InputType type, std::unique_ptr<Command> command)
{
	ControllerButton buttonType{ std::make_pair(button, type) };
	ControllerKey keyPair{ std::make_pair(controllerIdx, buttonType) };

	m_ControllerCommands[keyPair] = std::move(command);
	
}

// Unbind the command associated to the key passed through parameter
// This will delete the element from the command map
void  engine::InputManager::UnbindCommand(SDL_Keycode key, engine::InputType type)
{
	if (type == InputType::Default)
	{
		type = InputType::Down;
	}

	KeyTypeKeyPair keyPair{ std::make_pair(key, type) };
	auto commandItr = m_KeyBoardCommands.find(keyPair);
	if (commandItr != m_KeyBoardCommands.end())
	{
		m_KeyBoardCommands.erase(commandItr);
	}
	
}

void engine::InputManager::UnbindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button,
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

void engine::InputManager::UnbindAllCommands()
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

int engine::InputManager::GetFreeController() const
{
	if (int(m_Controllers.size()) == MAX_CONTROLLERS)
	{
		// No more available controllers to use
		return -1;
	}

	return int(m_Controllers.size());
}

bool engine::InputManager::IsPlayer1Connected() const
{
	// If there are commands bind to the keyboard means player 1 is connected
	return !m_KeyBoardCommands.empty();
}