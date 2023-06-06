#include "PlayerInputCP.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "FireCommand.h"
#include "MenuSelectionCommand.h"
#include "GameObject.h"
#include "Controller.h"

PlayerInputCP::PlayerInputCP(engine::GameObject* pOwner)
	: Component("PlayerInputCP", pOwner)
	, m_PlayerDied{ false }
	, m_ControllerIdx { -1 }
{
	/*
	auto& input = engine::InputManager::GetInstance();

	if (input.IsPlayer1Connected())
	{
		// If player1 is already playing then add controller movement for player 2
		AddControllerMovement();
	}
	else
	{
		
		// Keys A and D will be used to move Player from left to right
		SDL_KeyCode keyA{ SDLK_a };
		SDL_KeyCode keyD{ SDLK_d };
		SDL_KeyCode key_Space{ SDLK_SPACE };

		std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ -1, 0, 0 });
		std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 1, 0, 0 });
		std::unique_ptr<Command> fireCommand = std::make_unique<FireCommand>(pOwner, glm::vec3{ 0, -1, 0 });

		// Bind all commands with their corresponding keys
		input.BindCommand(std::move(moveLeftCommand), keyA, engine::InputType::Pressed);
		input.BindCommand(std::move(moveRightCommand), keyD, engine::InputType::Pressed);
		input.BindCommand(std::move(fireCommand), key_Space, engine::InputType::Down);
	}
	*/
}

void PlayerInputCP::AddGameplayInput()
{
	auto& input = engine::InputManager::GetInstance();
	if (input.IsPlayer1Connected())
	{
		// Player 1 is already connected -> PLayer 2 only can use controller input
		AddControllerInput();
		return;
	}

	// Keys A and D will be used to move Player from left to right
	SDL_KeyCode keyA{ SDLK_a };
	SDL_KeyCode keyD{ SDLK_d };
	SDL_KeyCode key_Space{ SDLK_SPACE };

	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{-1, 0, 0});
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });
	std::unique_ptr<Command> fireCommand = std::make_unique<FireCommand>(GetOwner(), glm::vec3{ 0, -1, 0 });

	// Bind all commands with their corresponding keys
	input.BindCommand(std::move(moveLeftCommand), keyA, engine::InputType::Pressed);
	input.BindCommand(std::move(moveRightCommand), keyD, engine::InputType::Pressed);
	input.BindCommand(std::move(fireCommand), key_Space, engine::InputType::Down);

	AddControllerInput();

}

void PlayerInputCP::AddMenuInput(MenuSelectionCP* pMenuSelectionCP)
{
	auto& input = engine::InputManager::GetInstance();
	
	// Only Player 1 can control the menu
	if (input.IsPlayer1Connected())
		return;

	// Keyboard input for the MENU
	SDL_KeyCode keyEnter{ SDLK_RETURN };
	SDL_KeyCode keyArrowDown{ SDLK_DOWN };
	SDL_KeyCode keyArrowUP{ SDLK_UP };

	std::unique_ptr<Command> upSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, -1);
	std::unique_ptr<Command> downSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 1);
	std::unique_ptr<Command> selectOption = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 0);

	input.BindCommand(std::move(upSelectionCommand), keyArrowUP, engine::InputType::Down);
	input.BindCommand(std::move(downSelectionCommand), keyArrowDown, engine::InputType::Down);
	input.BindCommand(std::move(selectOption), keyEnter, engine::InputType::Down);

	// Menu controller input
	AddControllerInput(pMenuSelectionCP);
}

void PlayerInputCP::AddControllerInput(MenuSelectionCP* pMenuSelectionCP)
{
	auto& input = engine::InputManager::GetInstance();
	if (m_ControllerIdx == -1)
	{
		// No controller assigned yet to this player
		m_ControllerIdx = input.GetFreeController();
		if (m_ControllerIdx == -1)
			return;		// No free controller found
	}

	if (pMenuSelectionCP != nullptr)
	{
		// Input for the menu
		std::unique_ptr<Command> upSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, -1);
		std::unique_ptr<Command> downSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 1);
		std::unique_ptr<Command> selectOption = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 0);

		input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadUp, engine::InputType::Down, std::move(upSelectionCommand));
		input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadDown, engine::InputType::Down, std::move(downSelectionCommand));
		input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(selectOption));
	}
	else
	{
		// Input for the Gameplay
		std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ -1, 0, 0 });
		std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });
		std::unique_ptr<Command> fireCommand = std::make_unique<FireCommand>(GetOwner(), glm::vec3{ 0, -1, 0 });

		input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadLeft, engine::InputType::Pressed, std::move(moveLeftCommand));
		input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadRigth, engine::InputType::Pressed, std::move(moveRightCommand));
		input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(fireCommand));

	}

}

void PlayerInputCP::Update( const float )
{


}

void PlayerInputCP::ReceiveMessage( const std::string& ,  const std::string& )
{

}

void  PlayerInputCP::SetPlayerDied()
{
	m_PlayerDied = true;
}

PlayerInputCP::~PlayerInputCP()
{
	// Make sure all commands are unbind if the player dies during gameplay (not when closing game)
	if (m_PlayerDied)
	{
		auto& input = engine::InputManager::GetInstance();

		input.UnbindAllCommands(m_ControllerIdx);

		m_PlayerDied = false;
	}
	
}