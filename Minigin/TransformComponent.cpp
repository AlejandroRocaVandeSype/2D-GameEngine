#include "TransformComponent.h"
#include <iostream>


TransformComponent::TransformComponent(glm::vec3 position)
	: Component(ComponentType::TransformCP),
	m_position{ position }
{

}

TransformComponent::TransformComponent(float x, float y, float z)
	:TransformComponent(glm::vec3{x, y, z})
{

}

void TransformComponent::Update([[maybe_unused]] const float deltaTime)
{
	// Update position using physics (Velocity etc)
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