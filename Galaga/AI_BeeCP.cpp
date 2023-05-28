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
		switch (m_pEnemyCP->GetCurrentState())
		{
		case EnemyCP::ENEMY_STATE::moveToFormation:
			UpdateMoveToFormation(deltaTime);
			break;
		case EnemyCP::ENEMY_STATE::attack:
			UpdateAttack(deltaTime);
			break;
		}
	}
}

// Enemy moves back to the formation
void AI_BeeCP::UpdateMoveToFormation(const float deltaTime)
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
		// Wait until recieve orders to attack
		m_pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::waiting);
	}
}

void AI_BeeCP::UpdateAttack(const float deltaTime)
{
	auto currentPos = m_pTransformCP->GetWorldPosition();

	glm::vec3 targetPos{ 300.f, 400.f, 0.f };
	glm::vec3 direction{ glm::normalize(targetPos - currentPos) };

	float distance{ glm::distance(targetPos, currentPos) };
	if (distance > 2.f)
	{
		m_pMoveCP->Move(deltaTime, direction);
	}
	else
	{
		m_pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::moveToFormation);
	}


}


void AI_BeeCP::ReceiveMessage( const std::string& , const std::string& )
{

}
