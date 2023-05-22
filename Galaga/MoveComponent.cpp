#include "MoveComponent.h"
#include "GameObject.h"

MoveComponent::MoveComponent(engine::GameObject* pOwner, float speed, const Boundaries& boundaries)
	: Component("MoveCP", pOwner)
	, SPEED{ speed }
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

		pos += direction * SPEED * deltaTime;

		// Check if GameObject inside boundaries
		if (pos.x > m_Boundaries.LeftLimit() && pos.x + m_GObjectSize.x
			<= m_Boundaries.RightLimit())
		{
			// Inside x limit
			if (pos.y > m_Boundaries.TopLimit() && pos.y + m_GObjectSize.y
				<= m_Boundaries.BotLimit())
			{
				// Inside boundaries
				m_IsInsideBoundaries = true;

				transformCP->SetLocalPosition(pos);
				return;
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

		if (m_Boundaries.IsMoveRestricted() == false)
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

void MoveComponent::ChangeDirection(const glm::vec3& newDirection)
{
	if (m_AutoMovement)
	{
		// Only change direction if autoMovement 
		m_Direction = glm::normalize(newDirection);
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