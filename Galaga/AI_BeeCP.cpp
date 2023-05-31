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
	, m_DiagonalDiveMaxTime{ 0.f }, m_ElapsedDiagonalDive { 0.f }, m_AtRightSide { false }
	, m_AttackState { AttackState::breakFormation }, m_Direction { 1.f, 1.f, 0.f }, m_MissileDir { 0.f, 1.f, 0.f }
	, ROTATION_TIME{ 3.f }, m_RotationRadius{ 80.f }, m_HasShoot { true }, m_AmountMissiles{ 0 }, m_MissilesShoot { 0 }
	, m_ElapsedShootTime { 0.f }, m_WaitBetweenShoot { 0.3f }

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
		if(m_pEnemyCP->GetCurrentState() == EnemyCP::ENEMY_STATE::attack)
		{
			UpdateAttack(deltaTime);
		}
	}

}

void AI_BeeCP::UpdateAttack(const float deltaTime)
{
	auto currentPos = m_pTransformCP->GetWorldPosition();
	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	switch (m_AttackState)
	{
		case AI_BeeCP::AttackState::breakFormation:	
			InitAttackData(currentPos, window);
			break;

		case AI_BeeCP::AttackState::diagonalDive:
			UpdateDiagonalDive(deltaTime, currentPos, window);
			break;

		case AI_BeeCP::AttackState::verticalDive:
			UpdateVerticalDive(deltaTime, currentPos, window);
			break;
		
		case AI_BeeCP::AttackState::roundSwoop:
		{
			// Use the rotator component to make a round backwards
			if (m_pRotatorCP != nullptr)
			{
				m_pRotatorCP->Rotate(deltaTime);
				if (m_pRotatorCP->IsRotationFinish())
				{
					m_pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::moveToFormation);
					m_AttackState = AttackState::breakFormation;
				}
			}		
			break;
		}
		
	}

	// Shoot missiles if there are any to shoot
	if (!m_HasShoot)
	{
		FireMissile(deltaTime);
	}
	
}

// Break formation and determine on which direction dive and fire
void AI_BeeCP::InitAttackData(const glm::vec3& currentPos, const engine::Window& window)
{
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
	// Random between float numbers
	m_DiagonalDiveMaxTime = float(((std::rand()) / float(RAND_MAX / 1)) + 1.f);
	m_AttackState = AttackState::diagonalDive;

	// Charge 1 or 2 missiles to shoot (or zero)
	m_AmountMissiles = std::rand() % 3;
	if (m_AmountMissiles > 0)
	{
		m_HasShoot = false;
		CalculateMissileDirection();
	}
	
}

void AI_BeeCP::CalculateMissileDirection()
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	engine::GameObject* pPlayer = sceneManager.FindGameObjectByTag("Player");

	if (pPlayer != nullptr)
	{
		engine::TransformComponent* pPlayerTransformCP = pPlayer->GetComponent<engine::TransformComponent>();
		if (pPlayerTransformCP != nullptr)
		{
			// Missile lways go downwards. The x direction will be determine depending where the player is
			float playerXPos{ pPlayerTransformCP->GetWorldPosition().x };
			if (playerXPos < m_pTransformCP->GetWorldPosition().x)
			{
				m_MissileDir.x = -1.f;
			}
			else
			{
				m_MissileDir.x = 1.f;
			}

			return;
		}
	}

	// No player or no valid player pos = no fire missiles
	m_HasShoot = true;

}

// Dives diagonally downards. While this there is a chance to also shoot up to two missiles
void AI_BeeCP::UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window)
{
	
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

void AI_BeeCP::FireMissile(const float deltaTime)
{
	m_ElapsedShootTime += deltaTime;
	if (m_ElapsedShootTime > m_WaitBetweenShoot)
	{
		if (m_pMissileManagerCP != nullptr)
		{
			m_pMissileManagerCP->Fire(m_MissileDir);		
		}

		m_MissilesShoot++;
		if (m_MissilesShoot == m_AmountMissiles)
		{
			// No more missiles to shoot
			m_HasShoot = true;
			m_MissilesShoot = 0;
		}

		m_ElapsedShootTime = 0.f;
	}
}

// Dive downwards until certain limit
void AI_BeeCP::UpdateVerticalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window)
{
	
	m_pMoveCP->Move(deltaTime, m_Direction);
	if (currentPos.y > (window.height - 120.f))
	{
		m_AttackState = AttackState::roundSwoop;
		if (m_pRotatorCP != nullptr)
		{
			bool posRot = false;
			auto rotationCenter = m_pTransformCP->GetLocalPosition();
			float rotationAngle{}, targetRotation{};
			if (m_AtRightSide)
			{
				rotationCenter.x -= m_RotationRadius;
				rotationAngle = 0.f;
				targetRotation = glm::pi<float>();
				posRot = true;
			}
			else
			{
				rotationCenter.x += m_RotationRadius;
				rotationAngle = glm::pi<float>();
				targetRotation = 0;

			}

			m_pRotatorCP->SetDataRotation(m_RotationRadius, ROTATION_TIME, targetRotation, 
				rotationCenter, posRot, rotationAngle);
		}
	}
}




void AI_BeeCP::ReceiveMessage( const std::string& , const std::string& )
{

}
