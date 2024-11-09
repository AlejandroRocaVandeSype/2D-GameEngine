#include "PlayerInputCP.h"
#include "InputManager.h"
#include "../../Commands/MoveCommand.h"
#include "../../Commands/FireCommand.h"
#include "../../Commands/MenuSelectionCommand.h"
#include "../../Commands/ToggleMuteCommand.h"
#include "GameObject.h"
#include "Controller.h"
#include "PlayerCP.h"

PlayerInputCP::PlayerInputCP(engine::GameObject* pOwner)
	: Component("PlayerInputCP", pOwner)
	, m_ControllerIdx { -1 }
{
	
}

// When two players are playing :
//		PLAYER 1 -> Keyboard + 2º controller
//		PLAYER 2 -> 1º controller
void PlayerInputCP::TwoPlayersGameplayInput()
{
	auto playerIdx = GetOwner()->GetComponent <PlayerCP>()->GetPlayerIndex();
	if (playerIdx == 2)
	{
		// Player 2 -> Only Add controller input. No keyboard
		// Player 2 will have Controller 0
		GameplayControllerInput(playerIdx - 2);
		return;
	}

	// ADD an extra Controller for the Player 1
	auto& input = engine::InputManager::GetInstance();
	m_ControllerIdx = input.GetFreeController();

	// PLAYER 1 KEYBOARD AND CONTROLLER INPUT
	GameplayKeyboardInput();
	GameplayControllerInput(m_ControllerIdx);

}

// INPUT SETUP FOR THE MENU NAVIGATION
void PlayerInputCP::MenuInput(MenuSelectionCP* pMenuSelectionCP)
{
	auto& input = engine::InputManager::GetInstance();
	auto playerCP{ GetOwner()->GetComponent<PlayerCP>() };

	// Only Player 1 can control the menu
	if (playerCP != nullptr && playerCP->GetPlayerIndex() == 2)
		return;

	// Keyboard input for the MENU
	SDL_KeyCode keyEnter{ SDLK_RETURN };
	SDL_KeyCode keyArrowDown{ SDLK_DOWN };
	SDL_KeyCode keyArrowUP{ SDLK_UP };
	SDL_KeyCode keyS{ SDLK_w};
	SDL_KeyCode keyW{ SDLK_s };

	std::unique_ptr<Command> upSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, -1);
	std::unique_ptr<Command> downSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 1);
	std::unique_ptr<Command> upSelectionCommand2 = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, -1);
	std::unique_ptr<Command> downSelectionCommand2 = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 1);
	std::unique_ptr<Command> selectOption = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 0);

	input.BindCommand(std::move(upSelectionCommand), keyArrowUP, engine::InputType::Down);
	input.BindCommand(std::move(downSelectionCommand), keyArrowDown, engine::InputType::Down);
	input.BindCommand(std::move(upSelectionCommand2), keyS, engine::InputType::Down);
	input.BindCommand(std::move(downSelectionCommand2), keyW, engine::InputType::Down);
	input.BindCommand(std::move(selectOption), keyEnter, engine::InputType::Down);

	// Menu controller input for Player 1
	AddMenuControllerInput(pMenuSelectionCP);
}

void PlayerInputCP::AddMenuControllerInput(MenuSelectionCP* pMenuSelectionCP)
{
	auto& input = engine::InputManager::GetInstance();
	if (m_ControllerIdx == -1)
	{
		// No controller assigned yet to this player
		m_ControllerIdx = input.GetFreeController();
		if (m_ControllerIdx == -1)
			return;		// No free controller found
	}

	// Input for the menu
	std::unique_ptr<Command> upSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, -1);
	std::unique_ptr<Command> downSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 1);
	std::unique_ptr<Command> selectOption = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 0);

	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadUp, engine::InputType::Down, std::move(upSelectionCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadDown, engine::InputType::Down, std::move(downSelectionCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(selectOption));
	
}

// Assign the controller buttons for Gameplay to the specified controller 
void PlayerInputCP::GameplayControllerInput(unsigned controllerIdx)
{
	m_ControllerIdx = controllerIdx;

	auto& input = engine::InputManager::GetInstance();
	// Input for the Gameplay
	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ -1, 0, 0 });
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });
	std::unique_ptr<Command> fireCommand = std::make_unique<FireCommand>(GetOwner(), glm::vec3{ 0, -1, 0 });

	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadLeft, engine::InputType::Pressed, std::move(moveLeftCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadRigth, engine::InputType::Pressed, std::move(moveRightCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(fireCommand));
}


void PlayerInputCP::GameplayKeyboardInput()
{
	auto& input = engine::InputManager::GetInstance();

	// Keys A and D will be used to move Player from left to right
	SDL_KeyCode keyA{ SDLK_a };
	SDL_KeyCode keyD{ SDLK_d };
	SDL_KeyCode key_Space{ SDLK_SPACE };
	SDL_KeyCode key_F{ SDLK_f };

	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ -1, 0, 0 });
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });
	std::unique_ptr<Command> fireCommand = std::make_unique<FireCommand>(GetOwner(), glm::vec3{ 0, -1, 0 });
	std::unique_ptr<Command> toggleMuteCommand = std::make_unique<ToggleMuteCommand>();
	

	// Bind all commands with their corresponding keys
	input.BindCommand(std::move(moveLeftCommand), keyA, engine::InputType::Pressed);
	input.BindCommand(std::move(moveRightCommand), keyD, engine::InputType::Pressed);
	input.BindCommand(std::move(fireCommand), key_Space, engine::InputType::Down);
	input.BindCommand(std::move(toggleMuteCommand), key_F, engine::InputType::Down);
}

void PlayerInputCP::Update( const float )
{
}

void PlayerInputCP::ReceiveMessage( const std::string& ,  const std::string& )
{

}


PlayerInputCP::~PlayerInputCP()
{	}