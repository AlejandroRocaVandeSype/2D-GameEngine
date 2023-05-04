#include "MissileBehaviourCP.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "HealthComponent.h"
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
			// Bullet outside boundaries -> Destroy it
			GetOwner()->GetComponent<HealthComponent>()->Kill();
		}
	}
}

void MissileBehaviourCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}