#include "PlayerInputCP.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "FireCommand.h"
#include "GameObject.h"
#include "Controller.h"

PlayerInputCP::PlayerInputCP(engine::GameObject* pOwner)
	: Component("PlayerInputCP", pOwner)
{
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
}

void PlayerInputCP::AddControllerMovement()
{
	auto& input = engine::InputManager::GetInstance();

	int controllerIdx = input.GetFreeController();
	if (controllerIdx != -1)
	{
		std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ -1, 0, 0 });
		std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });
		std::unique_ptr<Command> fireCommand = std::make_unique<FireCommand>(GetOwner(), glm::vec3{ 0, -1, 0 });

		input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadLeft, engine::InputType::Pressed, std::move(moveLeftCommand));
		input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadRigth, engine::InputType::Pressed, std::move(moveRightCommand));
		input.BindCommand(controllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(fireCommand));

	}
	
}

void PlayerInputCP::Update([[maybe_unused]] const float deltaTime)
{


}

void PlayerInputCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

PlayerInputCP::~PlayerInputCP()
{
	
}