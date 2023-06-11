#include "AI_BeeCP.h"
#include "GameObject.h"
#include "EnemyCP.h"
#include "MoveComponent.h"
#include "FormationCP.h"
#include "RotatorComponent.h"
#include "MissileManagerCP.h"
#include "Scene.h"
#include <glm/gtc/constants.hpp>

AI_BeeCP::AI_BeeCP(engine::GameObject* pOwner)
	:Component("AI_BeeCP", pOwner)
	, m_pEnemyCP { nullptr }
	, m_pMoveCP { nullptr }
	, m_pTransformCP{ nullptr }, m_pRotatorCP { nullptr }
	, m_DiagonalDiveMaxTime{ 0.f }, m_ElapsedDiagonalDive { 0.f }, m_AtRightSide { false }
	, m_AttackState { AttackState::breakFormation }, m_Direction { 1.f, 1.f, 0.f }
	, m_RotationTime{ 1.5f }, m_RotationRadius{ 30.f }

{
	if (pOwner != nullptr)
	{
		m_pEnemyCP = pOwner->GetComponent<EnemyCP>();
		m_pMoveCP = pOwner->GetComponent<MoveComponent>();
		m_pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
		m_pRotatorCP = pOwner->GetComponent<RotatorComponent>();
		
	}
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

		case AI_BeeCP::AttackState::loop:
			UpdateBreakLoop(deltaTime);
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

	// Loop data
	InitLoopData();

	m_AttackState = AttackState::loop;	// Do a loop before diving
}


void AI_BeeCP::InitLoopData()
{
	if (m_pRotatorCP != nullptr)
	{
		m_RotationRadius = 30.f;
		m_RotationTime = 1.5f;
		bool positiveRot = false;
		auto rotationCenter = m_pTransformCP->GetLocalPosition();
		float rotationAngle{}, targetRotation{};

		// Rotate on the opposite direction that the enemy will dive
		if (m_Direction.x > 0)
		{
			rotationCenter.x -= m_RotationRadius;
			rotationAngle = 0.f;
			targetRotation = -glm::pi<float>();
		}
		else
		{
			rotationCenter.x += m_RotationRadius;
			rotationAngle = glm::pi<float>();
			targetRotation = glm::two_pi<float>();		// Half rotation
			positiveRot = true;
		}

		m_pRotatorCP->SetDataRotation(m_RotationRadius, m_RotationTime, targetRotation,
			rotationCenter, positiveRot, rotationAngle);
	}
}


void AI_BeeCP::UpdateBreakLoop(const float deltaTime)
{

	if (m_pRotatorCP != nullptr)
	{
		m_pRotatorCP->Rotate(deltaTime);
		if (m_pRotatorCP->IsRotationFinish())
		{
			m_AttackState = AttackState::diagonalDive;
			if (m_pEnemyCP != nullptr)
			{
				m_pEnemyCP->CalculateMissileDirection();
			}
			m_RotationRadius = 80.f;		// Bigger rotation for the loop backwards state
			m_RotationTime = 3.f;
		}
	}
	else
	{
		// If no rotator just go directly to attack behaviour
		AI_BeeCP::AttackState::diagonalDive;
	}
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

// Dive downwards until certain limit
void AI_BeeCP::UpdateVerticalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window)
{
	
	m_pMoveCP->Move(deltaTime, m_Direction);
	if (currentPos.y > (window.height - 120.f))
	{
		m_AttackState = AttackState::roundSwoop;
		if (m_pRotatorCP != nullptr)
		{
			bool positiveRot = false;
			auto rotationCenter = m_pTransformCP->GetLocalPosition();
			float rotationAngle{}, targetRotation{};
			if (m_AtRightSide)
			{
				rotationCenter.x -= m_RotationRadius;
				rotationAngle = 0.f;
				targetRotation = glm::pi<float>();
				positiveRot = true;
			}
			else
			{
				rotationCenter.x += m_RotationRadius;
				rotationAngle = glm::pi<float>();
				targetRotation = 0;

			}

			m_pRotatorCP->SetDataRotation(m_RotationRadius, m_RotationTime, targetRotation,
				rotationCenter, positiveRot, rotationAngle);
		}
	}
}

void AI_BeeCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pTransformCP = nullptr;
		}
		if (value == "MoveCP")
		{
			m_pMoveCP = nullptr;
		}
		if (value == "RotatorCP")
		{
			m_pRotatorCP = nullptr;
		}

		if (value == "EnemyCP")
		{
			m_pEnemyCP = nullptr;
		}
	}
}

void AI_BeeCP::Reset()
{
	m_AttackState = AttackState::breakFormation;

	m_DiagonalDiveMaxTime = 0.f;
	m_ElapsedDiagonalDive = 0.f;
	m_AtRightSide = false;
	m_Direction = glm::vec3{ 1.f, 1.f, 0.f };
	m_RotationRadius = 40.f;

}

