#include "KillCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Event.h"
#include "LivesUIComponent.h"

KillCommand::KillCommand(dae::GameObject* actor)
	: Command()
	, m_Actor{ actor }
{
	m_ActorDiedEvent = std::make_unique<Subject>();
	m_ActorDiedEvent->AddObserver(m_Actor->GetComponent<LivesUIComponent>());

}

KillCommand::~KillCommand()
{

}

void KillCommand::Execute([[maybe_unused]] float deltaTime)
{
	HealthComponent* healthCP{ m_Actor->GetComponent<HealthComponent>() };
	if (healthCP != nullptr && healthCP->GetLives() > 0)
	{
		healthCP->DecrementHealth(1);
		Event PlayerDiedEvent{ "PlayerDiedEvent" };
		m_ActorDiedEvent->NotifyObservers(m_Actor, PlayerDiedEvent);
	}
	
}



