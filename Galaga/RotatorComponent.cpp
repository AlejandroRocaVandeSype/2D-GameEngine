#include "RotatorComponent.h"
#include "GameObject.h"
#include <iostream>
#include <glm/gtc/constants.hpp>

/* Makes the object move in a circle around its parent if it has one. Otherwise, it does it around its own center */
RotatorComponent::RotatorComponent(engine::GameObject* pOwner, float radius, float rotationTime)
	:Component("RotatorCP", pOwner)
	, RADIUS { radius }
	, ROTATION_TIME { rotationTime }
	, FULL_ROTATION_VALUE { glm::two_pi<float>() }
	, m_Angle { 0.f }
	, m_CirclePos { glm::vec3{ 0, 0, 0} }
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

void RotatorComponent::Update(const float deltaTime)
{
	if (m_pTransformCP != nullptr)
	{
		engine::GameObject* pOwner{ GetOwner() };
		if (pOwner != nullptr)
		{
			// The less is the rotation time the faster the angle will be incremented 
			m_Angle += FULL_ROTATION_VALUE * deltaTime / ROTATION_TIME;

			m_CirclePos.x = RADIUS * cos(m_Angle);
			m_CirclePos.y = RADIUS * sin(m_Angle);
			

			if (pOwner->getParent())
			{
				m_pTransformCP->SetLocalPosition(m_CirclePos);
			}
			else
			{
				m_pTransformCP->SetCenterOffset(m_CirclePos);
			}
			
		}
		
	}

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