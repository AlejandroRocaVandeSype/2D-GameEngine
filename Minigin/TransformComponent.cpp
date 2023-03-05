#include "TransformComponent.h"
#include "GameObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

TransformComponent::TransformComponent(dae::GameObject* pOwner, glm::vec3 position, float rotation, glm::vec3 scale)
	: Component("TransformCP", pOwner)
	, m_WorldPosition { glm::vec3{ 0,0,0 } }
	, m_LocalPosition{ glm::vec3{ 0,0,0 } }
	, m_ScaleVector { scale }
	, m_Rotation { rotation }
	, m_IsPositionDirty { false }
{
	// World position will automatically be updated when required
	SetLocalPosition(position);

	// TODO : Local and World Scale and local and world rotation

	// create a scale matrix using the scale vector and 
	//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_ScaleVector);
}

TransformComponent::~TransformComponent()
{
	std::cout << "TransformComponent destructor" << std::endl;
}
void TransformComponent::Update([[maybe_unused]] const float deltaTime)
{
	// Update position using physics (Velocity etc)


}


void TransformComponent::UpdateWorldPosition()
{
	if (m_IsPositionDirty)
	{
		auto pOwner = GetOwner();
		if (pOwner != nullptr)
		{
			if (pOwner->getParent() == nullptr)
			{
				// No parent -> WorldPos == LocalPos
				m_WorldPosition = m_LocalPosition;
			}
			else
			{
				//std::cout << "Parent : " << pOwner->getParent()->GetWorldPosition().x << ", " << pOwner->getParent()->GetWorldPosition().y << std::endl;
				//std::cout << "LocalPositionChild : " << m_LocalPosition.x << ", " << m_LocalPosition.y << std::endl;
				// 
				// Calculate the worldPosition with the parentWorldPos and the localPos of the child
				m_WorldPosition = pOwner->getParent()->GetWorldPosition() + m_LocalPosition;

				//std::cout << "m_WorldPosition : " << m_WorldPosition.x << ", " << m_WorldPosition.y << std::endl;

			}

			// Update children position to move along with the parent
			pOwner->UpdateChildrenPosition();

			


		}
	}

	m_IsPositionDirty = false;
}

void TransformComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}


// Changing the position will always be the localPosition
// Next time worldPosition is asked, it will be updated with the local before returning
void TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	SetPositionDirty();  // Everytime localPosition is changed, the worldPosition also needs to be updated
}

// WorldPosition needs to be updated
void TransformComponent::SetPositionDirty()
{
	m_IsPositionDirty = true;
	
}

// Always check if the localPosition has changed before returning
const glm::vec3 TransformComponent::GetWorldPosition() 
{
	if (m_IsPositionDirty)
	{
		// Update the world position
		UpdateWorldPosition();
	}
	
	return m_WorldPosition;
}

const glm::vec3 TransformComponent::GetLocalPosition() const 
{
	return m_LocalPosition;
}