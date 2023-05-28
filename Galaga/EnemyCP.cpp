#include "EnemyCP.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "Servicealocator.h"
#include "SoundIDs.h"
#include "MoveComponent.h"
#include <iostream>

EnemyCP::EnemyCP(engine::GameObject* pOwner, const std::string& enemyType, const std::string& spriteFilePath, 
	const glm::vec3 formationPos, unsigned int health)
	: Component("EnemyCP", pOwner)
	, m_FormationPos { formationPos }, m_CurrentState { ENEMY_STATE::moveToFormation }, m_EnemyType { enemyType }
{
	if (pOwner != nullptr)
	{
		auto renderCP = pOwner->AddComponent<engine::RenderComponent>(pOwner, spriteFilePath);
		auto healthCP = pOwner->AddComponent<HealthComponent>(pOwner, health);
		MoveComponent::Boundaries enemyBoundaries{};  // No boundaries restriction
		pOwner->AddComponent<MoveComponent>(pOwner, 200.f, enemyBoundaries);
		auto collisionCP = pOwner->AddComponent<engine::CollisionComponent>(pOwner, renderCP->GetTextureSize());
		collisionCP->AddObserver(this);
		healthCP->AddObserver(this);
	
	}
}

EnemyCP::~EnemyCP()
{

}

void EnemyCP::Update(const float )
{

}


void EnemyCP::ReceiveMessage( const std::string&, const std::string& )
{

}

void EnemyCP::OnNotify(engine::GameObject* gameObject, const engine::Event& event)
{
	// Enemy crash into the player
	if (event.IsSameEvent("CollisionWith Player") || event.IsSameEvent("CollisionWith PlayerMissile"))
	{
		// Enemy loose a life
		auto enemyHealthCP = GetOwner()->GetComponent<HealthComponent>();
		if (enemyHealthCP != nullptr)
		{
			enemyHealthCP->DecrementHealth(1);
		}

		if (event.IsSameEvent("CollisionWith PlayerMissile"))
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