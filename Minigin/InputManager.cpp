#include <SDL.h>
#include "InputManager.h"
#define WIN32_LEAND_AND_MEAN
#include <Windows.h>
#include <XInput.h>
#include "backends/imgui_impl_sdl.h"
#include <iostream>

// If false means we want to close the game
bool dae::InputManager::ProcessInput([[maybe_unused]] float deltaTime)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) {
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

// Bind Commands to keys.
// If key already exits in the map it will simply swap the command associated with the key
void dae::InputManager::BindCommand(const SDL_KeyCode& key, std::unique_ptr<Command> command)
{

	m_KeyBoardCommands[key] = std::move(command);

}