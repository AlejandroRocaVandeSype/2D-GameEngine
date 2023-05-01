#include "KillCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include <iostream>

KillCommand::KillCommand(dae::GameObject* actor)
	: Command()
	, m_Actor{ actor }
{
	
}

KillCommand::~KillCommand()
{
	std::cout << "Kill Command destructor\n";
}

void KillCommand::Execute([[maybe_unused]] float deltaTime)
{
	HealthComponent* healthCP{ m_Actor->GetComponent<HealthComponent>() };
	if (healthCP != nullptr)
	{
		healthCP->DecrementHealth(1);
	}
	
}



