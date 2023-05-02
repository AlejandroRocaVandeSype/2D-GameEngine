#include "MoveComponent.h"
#include "GameObject.h"

MoveComponent::MoveComponent(engine::GameObject* pOwner, float speed, const Boundaries& boundaries)
	: Component("MoveCP", pOwner)
	, m_Speed{ speed }
	, m_Boundaries { boundaries }
	, m_GObjectSize{ }
{
	 auto renderCP = pOwner->GetComponent<engine::RenderComponent>();

	 if (renderCP != nullptr)
	 {
		 m_GObjectSize = renderCP->GetTextureSize();
	 }
	 
}

// Moves in the direction specified respecting the boundaries
void MoveComponent::Move(float deltaTime, glm::vec3& direction)
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
				transformCP->SetLocalPosition(pos);
			}
		}
	
	}
}

void MoveComponent::Update([[maybe_unused]] const float deltaTime)
{


}

void MoveComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{
	
}

MoveComponent::~MoveComponent()
{
	/*
	// Make sure we dont try to move 
	auto& input = engine::InputManager::GetInstance();

	for (const auto& key : m_Keys)
	{
		input.UnbindCommand(key);
	}
	*/
}