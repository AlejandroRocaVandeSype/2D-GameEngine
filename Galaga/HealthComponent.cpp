#include "HealthComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "LivesUIComponent.h"
#include "Observer.h"
#include <iostream>

HealthComponent::HealthComponent(engine::GameObject* pOwner, unsigned int lives)
	: Component("HealthCP", pOwner)
	, m_Lives { lives }
{
	
}

HealthComponent::~HealthComponent()
{
	std::cout << "Health Component destructor" << std::endl;
}

// Add an observer to "Observe" the HealthComponent
void HealthComponent::AddObserver(engine::Observer* pObserver)
{
	if (m_ActorDiedEvent == nullptr)
	{
		m_ActorDiedEvent = std::make_unique<engine::Subject>();
	}
	
	m_ActorDiedEvent->AddObserver(pObserver);

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

		if (m_ActorDiedEvent != nullptr)
		{
			engine::Event HealthDecEvent{ "HealthDecremented" };
			m_ActorDiedEvent->NotifyObservers(GetOwner(), HealthDecEvent);
		}
		
	}	
}

void HealthComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}


const unsigned int HealthComponent::GetLives() const
{
	return m_Lives;
}