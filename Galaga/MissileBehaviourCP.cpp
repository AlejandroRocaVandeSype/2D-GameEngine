#include "MissileBehaviourCP.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "HealthComponent.h"
#include "Event.h"
#include <iostream>

MissileBehaviourCP::MissileBehaviourCP(engine::GameObject* pOwner)
	:Component("MissileBehaviourCP", pOwner)
{
	m_pMoveComponent = pOwner->GetComponent<MoveComponent>();
}

MissileBehaviourCP::~MissileBehaviourCP()
{
	std::cout << "MissileBehaviourCP destructor" << std::endl;
}

void MissileBehaviourCP::Update([[maybe_unused]] const float deltaTime)
{
	if (m_pMoveComponent != nullptr)
	{
		if (!m_pMoveComponent->InsideBoundaries())
		{
			// Bullet outside boundaries -> Deactivate it
			GetOwner()->SetIsActive(false);
		}
	}
}

void MissileBehaviourCP::OnNotify([[maybe_unused]] engine::GameObject* gameObject, const engine::Event& event)
{
	if (event.IsSameEvent("CollisionWith Enemy"))
	{
		GetOwner()->SetIsActive(false);
	}
	
}

void MissileBehaviourCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}