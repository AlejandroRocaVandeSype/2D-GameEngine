#include "MoveComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "GameObject.h"


MoveComponent::MoveComponent(dae::GameObject* pOwner, float speed)
	: Component("MoveCP", pOwner)
{

	auto& input = dae::InputManager::GetInstance();
	
	SDL_KeyCode keyA{ SDLK_a };
	SDL_KeyCode keyW{ SDLK_w };
	SDL_KeyCode keyS{ SDLK_s };
	SDL_KeyCode keyD{ SDLK_d };

	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ -1, 0, 0 }, speed);
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 1, 0, 0 }, speed);
	std::unique_ptr<Command> moveUpCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 0, -1, 0 }, speed);
	std::unique_ptr<Command> moveDownCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 0, 1, 0 }, speed);

	// Bind all commands with their corresponding keys
	input.BindCommand(keyA, std::move(moveLeftCommand));
	m_Keys.push_back(keyA);
	input.BindCommand(keyD, std::move(moveRightCommand));
	m_Keys.push_back(keyD);
	input.BindCommand(keyW, std::move(moveUpCommand));
	m_Keys.push_back(keyW);
	input.BindCommand(keyS, std::move(moveDownCommand));
	m_Keys.push_back(keyS);
	
}


void MoveComponent::Update([[maybe_unused]] const float deltaTime)
{


}

void MoveComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

MoveComponent::~MoveComponent()
{
	// Make sure we dont try to move 
	auto& input = dae::InputManager::GetInstance();

	for (const auto& key : m_Keys)
	{
		//input.UnbindCommand(key);
	}
}