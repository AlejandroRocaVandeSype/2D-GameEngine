#include "MissileCP.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "HealthComponent.h"
#include <iostream>

MissileCP::MissileCP(engine::GameObject* pOwner)
	:Component("MissileBehaviourCP", pOwner)
{
	m_pMoveComponent = pOwner->GetComponent<MoveComponent>();
}

MissileCP::~MissileCP()
{
	std::cout << "MissileBehaviourCP destructor" << std::endl;
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
	if (event.IsSameEvent("CollisionWith Enemy"))
	{
		auto enemyHealthCP = gameObject->GetComponent<HealthComponent>();
		if (enemyHealthCP != nullptr)
		{
			enemyHealthCP->DecrementHealth(1);
		}
		
		// Deactivate the missile
		GetOwner()->SetIsActive(false);
	}
	
}

void MissileCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}