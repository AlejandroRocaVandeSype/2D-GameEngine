#include "HealthComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "LivesUIComponent.h"
#include <iostream>

HealthComponent::HealthComponent(dae::GameObject* pOwner, unsigned int lives)
	: Component("HealthCP", pOwner)
	, m_Lives { lives }
{
	m_ActorDiedEvent = std::make_unique<Subject>();

	if (pOwner != nullptr)
	{
		m_ActorDiedEvent->AddObserver(pOwner->GetComponent<LivesUIComponent>());
	}
	
}

HealthComponent::~HealthComponent()
{
	std::cout << "Health Component destructor" << std::endl;
}

void HealthComponent::Update([[maybe_unused]] const float deltaTime)
{

}

void HealthComponent::DecrementHealth(unsigned int amount)
{
	if (m_Lives > 0)
	{
		if (amount > m_Lives)
		{
			m_Lives = 0;
		}
		else
		{
			m_Lives -= amount;
		}

		Event HealthDecEvent{ "HealthDecremented" };
		m_ActorDiedEvent->NotifyObservers(GetOwner(), HealthDecEvent);
	}	
}

void HealthComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}


const unsigned int HealthComponent::GetLives() const
{
	return m_Lives;
}