#include "Subject.h"
#include "Observer.h"
#include "Event.h"

engine::Subject::~Subject()
{
	for (const auto& observer : m_Observers)
	{
		observer->UnRegisterSubject(this);
	}
}

void engine::Subject::AddObserver(Observer* observer)
{
	m_Observers.push_back(observer);
	observer->RegisterSubject(this);
}

void engine::Subject::RemoveObserver(engine::Observer* observer)
{
	auto observerItr{ std::find(m_Observers.begin(), m_Observers.end(), observer) };
	if (observerItr != m_Observers.end())
	{
		m_Observers.erase(observerItr);
	}
}

void engine::Subject::NotifyObservers(engine::GameObject* gameObject, const engine::Event& event)
{
	for (const auto& observer : m_Observers)
	{
		observer->OnNotify(gameObject, event);
	}
}