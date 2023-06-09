#include "MissileCP.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "HealthComponent.h"
#include <iostream>

MissileCP::MissileCP(engine::GameObject* pOwner, bool IsPlayerMissile)
	:Component("MissileBehaviourCP", pOwner)
	, m_EnemyPoints { 0 }, m_MissileSubject{ nullptr}
{
	m_pMoveComponent = pOwner->GetComponent<MoveComponent>();

	if (IsPlayerMissile)
	{
		m_CollisionWith = "CollisionWith Enemy";
	}
	else
	{
		m_CollisionWith = "CollisionWith Player";
	}
}

MissileCP::~MissileCP()
{
	std::cout << "MissileCP destructor" << std::endl;
}

void MissileCP::Update( const float )
{
	if (m_pMoveComponent != nullptr)
	{
		if (!m_pMoveComponent->InsideBoundaries())
		{
			// Missile outside boundaries -> Deactivate it
			GetOwner()->SetIsActive(false);
		}
	}
}

void MissileCP::OnNotify(engine::GameObject* gameObject, const engine::Event& event)
{
	if (event.IsSameEvent(m_CollisionWith))
	{
		auto enemyHealthCP = gameObject->GetComponent<HealthComponent>();
		if (enemyHealthCP != nullptr)
		{
			enemyHealthCP->DecrementHealth(1);
		}
		
		auto missileHealthCP = GetOwner()->GetComponent<HealthComponent>();
		if (missileHealthCP != nullptr)
		{
			missileHealthCP->DecrementHealth(1);
		}
	}

	if (event.IsSameEvent("GameObjectDied"))
	{
		// Deactivate the missile
		GetOwner()->SetIsActive(false);
	}
	
}

// Add an observer to "Observe"
void MissileCP::AddObserver(engine::Observer* pObserver)
{
	if (m_MissileSubject == nullptr)
	{
		m_MissileSubject = std::make_unique<engine::Subject>();
	}

	m_MissileSubject->AddObserver(pObserver);

}

void MissileCP::SetEnemyPoints(int enemyValue)
{
	m_EnemyPoints = enemyValue;
	
	// Everytime points are updated we notify 
	if (m_MissileSubject != nullptr)
	{
		engine::Event updatePointsEvent{ "UpdatePoints" };
		m_MissileSubject->NotifyObservers(GetOwner(), updatePointsEvent);
	}

}

int MissileCP::GetEnemyPoints() const
{
	return m_EnemyPoints;
}

void MissileCP::ReceiveMessage( const std::string& , const std::string& )
{

}