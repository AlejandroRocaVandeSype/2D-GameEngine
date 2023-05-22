#include "AI_BeeCP.h"
#include "GameObject.h"
#include "EnemyCP.h"
#include "MoveComponent.h"
#include "FormationCP.h"
#include <iostream>

AI_BeeCP::AI_BeeCP(engine::GameObject* pOwner)
	:Component("AI_BeeCP", pOwner)
	, m_pEnemyCP { nullptr }
	, m_pMoveCP { nullptr }
	, m_pTransformCP{ nullptr }
	, m_InFormation{ false }
{
	m_pEnemyCP = pOwner->GetComponent<EnemyCP>();
	m_pMoveCP = pOwner->GetComponent<MoveComponent>();
	m_pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
}

AI_BeeCP::~AI_BeeCP()
{

}

void AI_BeeCP::Update(const float deltaTime)
{
	if (m_pMoveCP != nullptr && m_pEnemyCP != nullptr && m_pTransformCP != nullptr)
	{
		// If enemy moved outside the formation, go back to it
		if (m_InFormation == false)
		{
			auto currentPos = m_pTransformCP->GetWorldPosition();

			glm::vec3 targetPos{ GetOwner()->getParent()->GetComponent<engine::TransformComponent>()->GetWorldPosition() + m_pEnemyCP->GetFormationPos() };
			// Normalized Vector from the enemy position to the target to get the direction
			glm::vec3 direction{ glm::normalize(targetPos - currentPos) };

			float distance{ glm::distance(targetPos, currentPos) };
			if (distance > 2.f)
			{
				m_pMoveCP->Move(deltaTime, direction);
			}
			else
			{
				m_InFormation = true;
			}
		}
		

	}
}

void AI_BeeCP::ReceiveMessage( const std::string& , const std::string& )
{

}
