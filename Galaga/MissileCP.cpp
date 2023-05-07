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

void MissileCP::Update([[maybe_unused]] const float deltaTime)
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

void MissileCP::OnNotify([[maybe_unused]]engine::GameObject* gameObject, const engine::Event& event)
{
	if (event.IsSameEvent("CollisionWith Enemy"))
	{
		// Missile collision with an enemy
		//GetOwner()->SetIsActive(false);
		
	}
	
}

void MissileCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}