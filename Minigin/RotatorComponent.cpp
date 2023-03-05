#include "RotatorComponent.h"
#include "GameObject.h"
#include <glm/gtc/constants.hpp>

RotatorComponent::RotatorComponent(dae::GameObject* pOwner)
	:Component("RotatorCP", pOwner)
	, m_Radius { 40.f }
	, m_RotationTime { 2.f }
	, m_Angle { 0.f }
	, m_PosOnCircle { glm::vec3{ 0, 0, 0}}
	, m_Center {}
{
	if (pOwner != nullptr)
	{
		m_pTransformCP = pOwner->GetComponent<TransformComponent>();

		// If no parent gameObject will rotate around its own center
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
	
	m_Angle += glm::two_pi<float>() * deltaTime / m_RotationTime;
	
	//std::cout << m_Angle << std::endl;

	if (m_pTransformCP != nullptr)
	{	

		if (m_pParentTransformCP != nullptr)
		{
			// If there is a parent rotate around him
			m_Center = m_pParentTransformCP->GetWorldPosition();
		}

		
		//std::cout << "Center : " << m_Center.x << ", "<< m_Center.y << std::endl;
		m_PosOnCircle.x = m_Center.x + (m_Radius * cos(m_Angle));
		m_PosOnCircle.y = m_Center.y + (m_Radius * sin(m_Angle));

		if (m_pParentTransformCP != nullptr)
		{
			glm::vec3 relativePos{ m_PosOnCircle.x - m_Center.x , m_PosOnCircle.y - m_Center.y , 0.f };
			m_pTransformCP->SetLocalPosition(relativePos);
			
		}
		else
		{
			m_pTransformCP->SetLocalPosition(m_PosOnCircle);
		}
		//std::cout << "PosCircle : " << m_PosOnCircle.x << ", " << m_PosOnCircle.y << std::endl;

		
	}
	
	


}

void RotatorComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pTransformCP = nullptr;
		}
	}


}