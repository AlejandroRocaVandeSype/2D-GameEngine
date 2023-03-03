#include "TransformComponent.h"
#include <iostream>

TransformComponent::TransformComponent(dae::GameObject* pOwner, glm::vec3 position)
	: Component("TransformCP", pOwner),
	m_position{ position }
{

}

TransformComponent::TransformComponent(dae::GameObject* pOwner, float x, float y, float z)
	:TransformComponent(pOwner, glm::vec3{x, y, z})
{}

TransformComponent::~TransformComponent()
{
	std::cout << "TransformComponent destructor" << std::endl;
}
void TransformComponent::Update([[maybe_unused]] const float deltaTime)
{
	// Update position using physics (Velocity etc)


}

void TransformComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

void TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void TransformComponent::SetPosition(glm::vec3 position)
{
	m_position = position;
}