#include "TransformComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

engine::TransformComponent::TransformComponent(engine::GameObject* pOwner, glm::vec3 position, glm::vec2 scale)
	: Component("TransformCP", pOwner)
	, m_WorldPosition { glm::vec3{ 0,0,0 } }
	, m_LocalPosition{ glm::vec3{ 0,0,0 } }
	, m_Scale { scale }						// TODO : Local and World Scale and local and world rotation
	, m_CenterOffset{ glm::vec3{ 0,0,0 } }
	, m_IsPositionDirty { true }
	, m_pCollisionCP { nullptr }
{
	// World position will automatically be updated when required
	SetLocalPosition(position);

}

// Changing the position will always be the localPosition
// Next time worldPosition is asked, it will be updated with the local before returning
void engine::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	m_IsPositionDirty = true;  // Everytime localPosition is changed, the worldPosition also needs to be updated
}

const glm::vec3 engine::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

// Always check if the localPosition has changed before returning
const glm::vec3 engine::TransformComponent::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		// Update the world position
		UpdateWorldPosition();
	}

	return m_WorldPosition;
}

void engine::TransformComponent::UpdateWorldPosition()
{
	if (m_IsPositionDirty)
	{
		auto pOwner = GetOwner();
		if (pOwner != nullptr)
		{
			if (pOwner->getParent() == nullptr)
			{
				// No parent -> WorldPos = LocalPos
				m_WorldPosition = m_LocalPosition + m_CenterOffset;
			}
			else
			{
				// Calculate the worldPosition with the parentWorldPos and the localPos of the child
				m_WorldPosition = pOwner->getParent()->GetWorldPosition() + m_LocalPosition;

			}

			pOwner->SavePreviousWorldPosition(m_WorldPosition);

			// If there is a collisionCP update its boundingBox with the position
			if (m_pCollisionCP)
			{
				m_pCollisionCP->SetBoundingBox(m_WorldPosition);
			}

			// Update children position to move along with the parent
			//pOwner->UpdateChildrenPosition();

		}
	}

	m_IsPositionDirty = false;
}

void engine::TransformComponent::Update([[maybe_unused]] const float deltaTime)
{
	// Update position using physics (Velocity etc)

}


void engine::TransformComponent::SetScale(const glm::vec2& scale)
{
	m_Scale = scale;

}

// WorldPosition needs to be updated
void engine::TransformComponent::SetPositionDirty()
{
	m_IsPositionDirty = true;

}

void engine::TransformComponent::SetCenterOffset(const glm::vec3& centerOffset)
{
	m_CenterOffset = centerOffset;
	m_IsPositionDirty = true;
}

void engine::TransformComponent::AddCollisionCP(engine::CollisionComponent* pComponent)
{
	m_pCollisionCP = pComponent;
}


const glm::vec2 engine::TransformComponent::GetScale() const
{
	return m_Scale;
}

void engine::TransformComponent::ReceiveMessage( const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "CollisionCP")
		{
			m_pCollisionCP = nullptr;
		}
	}
}

engine::TransformComponent::~TransformComponent()
{
	std::cout << "TransformComponent destructor" << std::endl;
}