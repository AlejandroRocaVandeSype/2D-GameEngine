#include "RotatorComponent.h"
#include "GameObject.h"
#include <iostream>
#include <glm/gtc/constants.hpp>

/* Makes the object move in a circle around its parent if it has one. Otherwise, it does it around its own center */
RotatorComponent::RotatorComponent(engine::GameObject* pOwner)
	:Component("RotatorCP", pOwner)
	, m_Radius { 0.f }
	, m_RotationTime { 0.f }
	, FULL_ROTATION_VALUE { glm::two_pi<float>() }
	, HALF_ROTATION_VALUE{ glm::pi<float>() }
	, m_Angle { 0.f }
	, m_RotationCenter { glm::vec3{ 0, 0, 0} }
	, m_pTransformCP { nullptr }
	, m_IsRotationFinish { false }
{
	
	if (pOwner != nullptr)
	{
		m_pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
	}
	
}

RotatorComponent::~RotatorComponent()
{
	std::cout << "RotatorCP destructor" << std::endl;
}

void RotatorComponent::Update(const float)
{
	
}

void RotatorComponent::SetDataRotation(float radius, float rotationTime, float rotationTarget, const glm::vec3 center, 
	bool isPositiveRot, float startAngle)
{
	m_Radius = radius;
	m_RotationTime = rotationTime;
	m_RotationCenter = center;
	m_IsPositiveRot = isPositiveRot;
	m_RotationTarget = rotationTarget;
	// Reset data needed
	m_Angle = startAngle;
	m_IsRotationFinish = false;
}

void RotatorComponent::Rotate(const float deltaTime)
{
	if (m_IsPositiveRot)
	{
		// INC ANGLE
		m_Angle += FULL_ROTATION_VALUE * deltaTime / m_RotationTime;
		if(m_Angle > m_RotationTarget)
		{
			m_IsRotationFinish = true;
		}
	}
	else
	{
		// DEC ANGLE
		m_Angle -= FULL_ROTATION_VALUE * deltaTime / m_RotationTime;
		if (m_Angle < m_RotationTarget)
		{
			m_IsRotationFinish = true;
		}
	}

	if (!m_IsRotationFinish)
	{
		// Update the rotation position
		auto currentLocalPos = m_pTransformCP->GetLocalPosition();
		currentLocalPos.x = m_RotationCenter.x + m_Radius * cos(m_Angle);
		currentLocalPos.y = m_RotationCenter.y + m_Radius * sin(m_Angle);
		m_pTransformCP->SetLocalPosition(currentLocalPos);

	}
}

const bool RotatorComponent::IsRotationFinish() const
{
	return m_IsRotationFinish;
}

void RotatorComponent::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pTransformCP = nullptr;
		}
	}


}