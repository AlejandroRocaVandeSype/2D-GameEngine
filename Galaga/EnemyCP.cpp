#include "EnemyCP.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "Servicealocator.h"
#include "SoundIDs.h"
#include "MoveComponent.h"
#include "RotatorComponent.h"
#include "MissileManagerCP.h"
#include "PlayerCP.h"
#include <iostream>

EnemyCP::EnemyCP(engine::GameObject* pOwner, const std::string& enemyType, const std::string& spriteFilePath, 
	const glm::vec3 formationPos, unsigned int health)
	: Component("EnemyCP", pOwner)
	, m_FormationPos { formationPos }, m_CurrentState { ENEMY_STATE::moveToFormation }, m_EnemyType { enemyType }
	, m_pTransformCP { nullptr }, m_pMoveCP { nullptr }
{
	if (pOwner != nullptr)
	{
		auto renderCP = pOwner->AddComponent<engine::RenderComponent>(pOwner, spriteFilePath);
		auto healthCP = pOwner->AddComponent<HealthComponent>(pOwner, health);
		MoveComponent::Boundaries enemyBoundaries{};  // No boundaries restriction
		m_pMoveCP = pOwner->AddComponent<MoveComponent>(pOwner, glm::vec2{ 220.f, 220.f }, enemyBoundaries);
		auto collisionCP = pOwner->AddComponent<engine::CollisionComponent>(pOwner, renderCP->GetTextureSize());
		pOwner->AddComponent<RotatorComponent>(pOwner);
		collisionCP->AddObserver(this);
		healthCP->AddObserver(this);

		int maxMissiles{ 2 };
		glm::vec2 missileSpeed{ 80.f, 400.f };
		pOwner->AddComponent<MissileManagerCP>(pOwner, maxMissiles, missileSpeed, "enemy", "Sprites/enemyMissile.png");

		m_pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
	
	}
}

EnemyCP::~EnemyCP()
{

}

void EnemyCP::Update(const float deltaTime)
{
	if (m_pMoveCP != nullptr && m_pTransformCP != nullptr)
	{
		switch (m_CurrentState)
		{
		case EnemyCP::ENEMY_STATE::waiting:
			// While waiting he needs to keep moving with the formation
			m_pTransformCP->SetPositionDirty();
			break;
		case EnemyCP::ENEMY_STATE::moveToFormation:
			UpdateMoveToFormation(deltaTime);
			break;

			// Attack state will be updated by the AI component
		}
	}
}

// Enemy moves back to its position in the formation
void EnemyCP::UpdateMoveToFormation(const float deltaTime)
{
	auto currentPos = m_pTransformCP->GetWorldPosition();

	glm::vec3 targetPos{ GetOwner()->getParent()->GetComponent<engine::TransformComponent>()->GetWorldPosition() 
		+ m_FormationPos };
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
		m_CurrentState = EnemyCP::ENEMY_STATE::waiting;
		m_pMoveCP->ChangeSpeed(glm::vec2{ 80.f, 160.f });		// Reduce its speed
	}
}

void EnemyCP::ReceiveMessage( const std::string& message, const std::string& value)
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
	}
}

void EnemyCP::OnNotify(engine::GameObject* gameObject, const engine::Event& event)
{
	// Enemy crash into the player
	if (event.IsSameEvent("CollisionWith Player") || event.IsSameEvent("CollisionWith playerMissile"))
	{
		// Enemy loose a life
		auto enemyHealthCP = GetOwner()->GetComponent<HealthComponent>();
		if (enemyHealthCP != nullptr)
		{
			enemyHealthCP->DecrementHealth(1);
		}

		if (event.IsSameEvent("CollisionWith playerMissile"))
		{
			gameObject->SetIsActive(false);
		}
		else
		{
			auto playerHealthCP = gameObject->GetComponent<HealthComponent>();
			if (playerHealthCP != nullptr)
			{
				playerHealthCP->DecrementHealth(1);
			}
		}	
	}

	if (event.IsSameEvent("GameObjectDied"))
	{ 
		// Enemies will become inactive when they die (do not destroy the gameObject)
		GetOwner()->SetIsActive(false);
		auto& soundSystem = engine::Servicealocator::Get_Sound_System();
		soundSystem.PlaySound(short(Sounds::enemyDie));
	}
}

void EnemyCP::ChangeCurrentState(EnemyCP::ENEMY_STATE newState)
{
	m_CurrentState = newState;
}

EnemyCP::ENEMY_STATE EnemyCP::GetCurrentState() const
{
	return m_CurrentState;
}

glm::vec3 EnemyCP::GetFormationPos() const
{
	return m_FormationPos;
}

const std::string& EnemyCP::GetType() const
{
	return m_EnemyType;
}