#include "AI_BeeCP.h"
#include "GameObject.h"
#include "EnemyCP.h"
#include "MoveComponent.h"
#include "FormationCP.h"
#include "RotatorComponent.h"
#include "MissileManagerCP.h"
#include "Scene.h"
#include <glm/gtc/constants.hpp>
#include <iostream>

AI_BeeCP::AI_BeeCP(engine::GameObject* pOwner)
	:Component("AI_BeeCP", pOwner)
	, m_pEnemyCP { nullptr }
	, m_pMoveCP { nullptr }
	, m_pTransformCP{ nullptr }
	, m_pMissileManagerCP { nullptr }
	, m_DiagonalDiveMaxTime{ 0.f }, m_ElapsedDiagonalDive { 0.f }
	, m_AttackState { AttackState::breakFormation }, m_Direction { 1.f, 1.f, 0.f }
	, ROTATION_TIME{ 3.f }, m_RotationRadius{ 80.f }, m_HasShoot { false }, m_AmountMissiles{ 0 }
	, m_ElapsedShootTime { 0.f }, m_WaitBetweenShoot { 0.5f }

{
	m_pEnemyCP = pOwner->GetComponent<EnemyCP>();
	m_pMoveCP = pOwner->GetComponent<MoveComponent>();
	m_pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
	m_pRotatorCP = pOwner->GetComponent<RotatorComponent>();
	m_pMissileManagerCP = pOwner->GetComponent<MissileManagerCP>();
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
			case EnemyCP::ENEMY_STATE::waiting:
				// While waiting he needs to keep moving with the formation
				m_pTransformCP->SetPositionDirty();
				break;
			case EnemyCP::ENEMY_STATE::moveToFormation:
				UpdateMoveToFormation(deltaTime);
				break;
			case EnemyCP::ENEMY_STATE::attack:
				UpdateAttack(deltaTime);
				break;
		}
	}
}

// Enemy moves back to its position in the formation
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
		m_pMoveCP->ChangeSpeed(glm::vec2{120.f, 120.f});		// Reduce its speed
	}
}

void AI_BeeCP::UpdateAttack(const float deltaTime)
{
	auto currentPos = m_pTransformCP->GetWorldPosition();
	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	switch (m_AttackState)
	{
		case AI_BeeCP::AttackState::breakFormation:
		{		
			// Break formation and determine on which direction dive
			if (currentPos.x > window.width / 2.f)
			{
				// Move to the left
				m_Direction.x = -1;
				m_AtRightSide = false;
			}
			else
			{
				m_Direction.x = 1;
				m_AtRightSide = true;
			}
			m_DiagonalDiveMaxTime = float((std::rand() % 2) + 1);
			m_AttackState = AttackState::diagonalDive;

			// Charge 1 or 2 missiles to shoot (or zero)
			 m_AmountMissiles = std::rand() % 3;
			break;
		}	
		case AI_BeeCP::AttackState::diagonalDive:
			UpdateDiagonalDive(deltaTime, currentPos, window);
			break;

		case AI_BeeCP::AttackState::verticalDive:
		{
			// Move downwards until certain limit
			m_pMoveCP->Move(deltaTime, m_Direction);
			if (currentPos.y > (window.height - 120.f) )
			{

				m_AttackState = AttackState::roundSwoop;

				if (m_pRotatorCP != nullptr)
				{
					bool posRot = false;
					auto rotationCenter = m_pTransformCP->GetLocalPosition();
					float rotationAngle{}, targetRotaion{};
					if (m_AtRightSide)
					{
						rotationCenter.x -= m_RotationRadius;
						rotationAngle = 0.f;
						targetRotaion = glm::pi<float>();
						posRot = true;
					}
					else
					{
						rotationCenter.x += m_RotationRadius;
						rotationAngle = glm::pi<float>();
						targetRotaion = 0;

					}

					m_pRotatorCP->SetDataRotation(m_RotationRadius, ROTATION_TIME, targetRotaion, rotationCenter, posRot, rotationAngle);
				}
				
			}

			break;
		}
		case AI_BeeCP::AttackState::roundSwoop:
		{
			if (m_pRotatorCP != nullptr)
			{
				m_pRotatorCP->Rotate(deltaTime);
				if (m_pRotatorCP->IsRotationFinish())
				{
					m_pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::moveToFormation);
					m_AttackState = AttackState::breakFormation;
					m_HasShoot = false;
				}
			}		
			break;
		}
		
	}
	

	



}

// Dives diagonally downards. While this there is a chance to also shoot up to two missiles
void AI_BeeCP::UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window)
{
	// Shoot missiles if there any to shoot
	if (!m_HasShoot && m_AmountMissiles > 0)
	{
		m_ElapsedShootTime += deltaTime;
		if (m_ElapsedShootTime > m_WaitBetweenShoot)
		{
			if (m_pMissileManagerCP != nullptr)
			{
				m_pMissileManagerCP->Fire(glm::vec3{ 1.f, 1.f, 0.f });
			}
			m_MissilesShoot++;
			if (m_MissilesShoot == m_AmountMissiles)
			{
				// No more missiles to shoot
				m_HasShoot = false;
				m_MissilesShoot = 0;
				m_ElapsedShootTime = 0.f;
			}
		}	
	}

	// Downward Diagonal movement until max Time or before it leaves window boundaries
	if (m_ElapsedDiagonalDive < m_DiagonalDiveMaxTime && (currentPos.x > 0 && currentPos.x < window.width))
	{
		m_ElapsedDiagonalDive += deltaTime;
		m_pMoveCP->Move(deltaTime, m_Direction);
	}
	else
	{
		m_ElapsedDiagonalDive = 0.f;
		m_AttackState = AttackState::verticalDive;
		m_Direction.x = 0.f;		// Now only move downwards

	}

}


void AI_BeeCP::ReceiveMessage( const std::string& , const std::string& )
{

}
