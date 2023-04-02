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

	if (m_Controllers.size() < MAX_CONTROLLERS)
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
			SDL_Keycode key{ e.key.keysym.sym };
			auto commandItr = m_KeyBoardCommands.find(key);

			if (commandItr != m_KeyBoardCommands.end())
			{
				// Founded a key binded to a Command -> Execute it
				commandItr->second->Execute(deltaTime);
			}
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
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
bool dae::InputManager::ProcessControllersInput([[maybe_unused]] float deltaTime)
{
	for (const auto& controller : m_Controllers)
	{
		if (controller->IsConnected())
		{

			std::cout << "Update controller " << controller->GetControllerIdx() << "\n";
			/*
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
			*/
		}
	}

	return true;
}

// Add the controller to the Input Manager available controllers. 
// However, it is not binded to any command yet
void dae::InputManager::NewAvailableController(unsigned controllerIdx)
{

	if (controllerIdx > MAX_VALID_CONTROLLER_IDX)
	{
		std::cerr << "Controller Index " << controllerIdx << " is invalid ! Controller not added. \n";
		return;
	}

	if (int(m_Controllers.size()) < MAX_CONTROLLERS)
	{

		bool IsAdded{ false };
		for (const auto& controllers : m_Controllers)
		{
			if (controllers->GetControllerIdx() == controllerIdx)
			{
				// This controller is already added
				IsAdded = true;
				std::cerr << "Controller Index " << controllerIdx << " is already added ! Controller not added. \n";
				break;
			}
		}

		if (!IsAdded)
		{
			// New controller
			m_Controllers.emplace_back(std::make_unique<Controller>(controllerIdx));
		}
	
	}
	
}

// Bind Commands to keys.
// If key already exits in the map it will simply swap the command associated with the key
void dae::InputManager::BindCommand(SDL_Keycode key, std::unique_ptr<Command> command)
{
	m_KeyBoardCommands[key] = std::move(command);
}

// Bind commands to a existing Controller
void  dae::InputManager::BindCommand(unsigned int controllerIdx, const Controller::XboxControllerButton& button,
	std::unique_ptr<Command> command)
{
	ControllerKey key{ std::make_pair(controllerIdx, button) };


	m_ControllerCommands[key] = std::move(command);
	
}

// Unbind the command associated to the key passed through parameter
// This will delete the element from the command map
void dae::InputManager::UnbindCommand(SDL_Keycode key)
{
	auto commandItr = m_KeyBoardCommands.find(key);
	if (commandItr != m_KeyBoardCommands.end())
	{
		m_KeyBoardCommands.erase(commandItr);
	}
}