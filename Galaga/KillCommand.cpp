#include "KillCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"

KillCommand::KillCommand(dae::GameObject* actor)
	: Command()
	, m_Actor{ actor }
{
	
}

KillCommand::~KillCommand()
{

}

void KillCommand::Execute([[maybe_unused]] float deltaTime)
{
	HealthComponent* healthCP{ m_Actor->GetComponent<HealthComponent>() };
	if (healthCP != nullptr)
	{
		healthCP->DecrementHealth(1);
	}
	
}



