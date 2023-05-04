#include "MoveComponent.h"
#include "GameObject.h"

MoveComponent::MoveComponent(engine::GameObject* pOwner, float speed, const Boundaries& boundaries)
	: Component("MoveCP", pOwner)
	, m_Speed{ speed }
	, m_Boundaries { boundaries }
	, m_GObjectSize{ }
	, m_AutoMovement{ false }
	, m_Direction{}				// This direction wont be use for this GO
	, m_IsInsideBoundaries { true }
{
	 auto renderCP = pOwner->GetComponent<engine::RenderComponent>();

	 if (renderCP != nullptr)
	 {
		 m_GObjectSize = renderCP->GetTextureSize();
	 }
	 
}

MoveComponent::MoveComponent(engine::GameObject* pOwner, float speed, const Boundaries& boundaries, const glm::vec3& direction)
	:MoveComponent(pOwner, speed, boundaries)
{
	m_AutoMovement = true;
	m_Direction = glm::normalize(direction);   // Normalized vector with the same direction but with lenght = 1
}

// Moves in the direction specified respecting the boundaries if movement is restricted
void MoveComponent::Move(float deltaTime, const glm::vec3& direction)
{
	auto transformCP = GetOwner()->GetComponent<engine::TransformComponent>();

	if (transformCP)
	{
		glm::vec3 pos = transformCP->GetLocalPosition();

		pos += direction * m_Speed * deltaTime;

		// Check if GameObject inside boundaries
		if (pos.x > m_Boundaries.leftLimit && pos.x + m_GObjectSize.x
			<= m_Boundaries.rightLimit)
		{
			// Inside x limit
			if (pos.y > m_Boundaries.topLimit && pos.y + m_GObjectSize.y
				<= m_Boundaries.botLimit)
			{
				// Inside boundaries
				m_IsInsideBoundaries = true;
			}
			else
			{
				m_IsInsideBoundaries = false;
			}
		}
		else
		{
			m_IsInsideBoundaries = false;
		}

		if (m_Boundaries.isMoveRestricted == true)
		{
			if (m_IsInsideBoundaries == true)
			{
				transformCP->SetLocalPosition(pos);
			}
		}
		else
		{
			// There is no movement restriction
			transformCP->SetLocalPosition(pos);
		}
	
	}
}

void MoveComponent::Update(const float deltaTime)
{
	if (m_AutoMovement)
	{
		Move(deltaTime, m_Direction);
	}
}

void MoveComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{
	
}

bool MoveComponent::InsideBoundaries() const
{
	return m_IsInsideBoundaries;
}

MoveComponent::~MoveComponent()
{
	
}