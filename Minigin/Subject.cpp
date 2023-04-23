#include "Subject.h"
#include "Observer.h"
#include "Event.h"

Subject::~Subject()
{
	
	for (const auto& observer : m_Observers)
	{
		observer->UnRegisterSubject(this);
	}

}

void Subject::AddObserver(Observer* observer)
{
	m_Observers.push_back(observer);
	observer->RegisterSubject(this);
}

void Subject::RemoveObserver(Observer* observer)
{
	auto observerItr{ std::find(m_Observers.begin(), m_Observers.end(), observer) };
	if (observerItr != m_Observers.end())
	{
		m_Observers.erase(observerItr);
	}
}

void Subject::NotifyObservers(dae::GameObject* gameObject, const Event& event)
{
	for (const auto& observer : m_Observers)
	{
		observer->OnNotify(gameObject, event);
	}
}