#include "MoveComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "KillCommand.h"
#include "GameObject.h"
#include "Controller.h"

MoveComponent::MoveComponent(engine::GameObject* pOwner, float speed)
	: Component("MoveCP", pOwner)
{

	auto& input = engine::InputManager::GetInstance();
	
	SDL_KeyCode keyA{ SDLK_a };
	SDL_KeyCode keyD{ SDLK_d };

	SDL_KeyCode keyK{ SDLK_k };

	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ -1, 0, 0 }, speed);
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 1, 0, 0 }, speed);
	std::unique_ptr<Command> killCommand = std::make_unique<KillCommand>(pOwner);
	//std::unique_ptr<Command> moveUpCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 0, -1, 0 }, speed);
	//std::unique_ptr<Command> moveDownCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 0, 1, 0 }, speed);

	// Bind all commands with their corresponding keys
	input.BindCommand(std::move(moveLeftCommand), keyA, engine::InputType::Pressed);
	input.BindCommand(std::move(moveRightCommand), keyD, engine::InputType::Pressed);
	input.BindCommand(std::move(killCommand), keyK, engine::InputType::Down);


	
}


MoveComponent::MoveComponent(engine::GameObject* pOwner, float speed, unsigned controllerIdx)
	: Component("MoveCP", pOwner)
{

	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ -1, 0, 0 }, speed);
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 1, 0, 0 }, speed);
	std::unique_ptr<Command> killCommand = std::make_unique<KillCommand>(pOwner);

	//std::unique_ptr<Command> moveUpCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 0, -1, 0 }, speed);
	//std::unique_ptr<Command> moveDownCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 0, 1, 0 }, speed);

	auto& input = engine::InputManager::GetInstance();

	input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadLeft, engine::InputType::Pressed, std::move(moveLeftCommand));
	input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadRigth, engine::InputType::Pressed, std::move(moveRightCommand));
	input.BindCommand(controllerIdx, Controller::XboxControllerButton::ButtonY, engine::InputType::Up, std::move(killCommand));

	//input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadUp, std::move(moveUpCommand));
	//input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadDown, std::move(moveDownCommand));
}


void MoveComponent::Update([[maybe_unused]] const float deltaTime)
{


}

void MoveComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

MoveComponent::~MoveComponent()
{
	/*
	// Make sure we dont try to move 
	auto& input = engine::InputManager::GetInstance();

	for (const auto& key : m_Keys)
	{
		input.UnbindCommand(key);
	}
	*/
}