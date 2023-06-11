#include "HealthComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "Observer.h"

HealthComponent::HealthComponent(engine::GameObject* pOwner, unsigned int lives)
	: Component("HealthCP", pOwner)
	, m_Lives { lives }
{
	
}

HealthComponent::~HealthComponent()
{
}

// Add an observer to "Observe" the HealthComponent
void HealthComponent::AddObserver(engine::Observer* pObserver)
{
	if (m_HealthSubject == nullptr)
	{
		m_HealthSubject = std::make_unique<engine::Subject>();
	}

	m_HealthSubject->AddObserver(pObserver);

}

void HealthComponent::Update(const float )
{

}

void HealthComponent::DecrementHealth(unsigned int amount)
{
	if (m_Lives > 0 && amount > 0)
	{
		if (amount >= m_Lives)
		{
			m_Lives = 0;

			if (m_HealthSubject != nullptr)
			{
				engine::Event dieEvent{ "GameObjectDied" };
				m_HealthSubject->NotifyObservers(GetOwner(), dieEvent);
			}
		}
		else
		{
			m_Lives -= amount;
		}

		if (m_HealthSubject != nullptr)
		{
			engine::Event HealthDecEvent{ "HealthDecremented" };
			m_HealthSubject->NotifyObservers(GetOwner(), HealthDecEvent);
		}
		
	}	
}

void HealthComponent::ResetHealth(unsigned int newHealth)
{
	m_Lives = newHealth;
}


void HealthComponent::ReceiveMessage(const std::string&, const std::string& )
{

}


const unsigned int HealthComponent::GetLives() const
{
	return m_Lives;
}