#include "RotatorComponent.h"
#include "GameObject.h"
#include <glm/gtc/constants.hpp>

/* Makes the object move in a circle around its parent if it has one. Otherwise, it does it around its own center */
RotatorComponent::RotatorComponent(dae::GameObject* pOwner, float radius, float rotationTime)
	:Component("RotatorCP", pOwner)
	, RADIUS { radius }
	, ROTATION_TIME { rotationTime }
	, FULL_ROTATION_VALUE { glm::two_pi<float>() }
	, m_Angle { 0.f }
	, m_CirclePos { glm::vec3{ 0, 0, 0} }
	, m_Center {}
{
	if (pOwner != nullptr)
	{
		m_pTransformCP = pOwner->GetComponent<TransformComponent>();

		// If no parent, the gameObject will rotate around its own center
		m_Center = m_pTransformCP->GetWorldPosition();

		if (pOwner->getParent() != nullptr)
		{
			m_pParentTransformCP = pOwner->getParent()->GetComponent<TransformComponent>();
		}
	}
}

RotatorComponent::~RotatorComponent()
{
	std::cout << "RotatorCP destructor" << std::endl;
}

void RotatorComponent::Update(const float deltaTime)
{
	// The less is the rotation time the faster the angle will be incremented 
	m_Angle += FULL_ROTATION_VALUE * deltaTime / ROTATION_TIME;

	if (m_pTransformCP != nullptr)
	{	
		if (m_pParentTransformCP != nullptr)
		{
			// If there is a parent rotate around him
			m_Center = m_pParentTransformCP->GetWorldPosition();
		}

		m_CirclePos.x = m_Center.x + (RADIUS * cos(m_Angle));
		m_CirclePos.y = m_Center.y + (RADIUS * sin(m_Angle));

		if (m_pParentTransformCP != nullptr)
		{
			// If it has a parent calculate the pos relative to him
			// TODO : This should be automatically calculated in the SetLocalPos
			glm::vec3 relativePos{ m_CirclePos.x - m_Center.x , m_CirclePos.y - m_Center.y , 0.f };
			m_pTransformCP->SetLocalPosition(relativePos);
			
		}
		else
		{
			// No parent then its localPos is the worldPos
			m_pTransformCP->SetLocalPosition(m_CirclePos);
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