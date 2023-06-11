#ifndef GAME_ENGINE_OBSERVER
#define GAME_ENGINE_OBSERVER

#include <vector>
#include "Subject.h"
#include "Event.h"

namespace engine
{
	class GameObject;
	class Observer
	{

	public:
		Observer() = default;
		virtual ~Observer() { UnRegisterAllSubjects(); };  // Observers will unregister itself from the subjects that are observing
		virtual void OnNotify(engine::GameObject* gameObject, const engine::Event& event) = 0;

		void RegisterSubject(engine::Subject* subject) { m_Subjects.emplace_back(subject); };
		void UnRegisterAllSubjects()
		{
			for (auto subjectItr = m_Subjects.begin(); subjectItr != m_Subjects.end();)
			{
				(*subjectItr)->RemoveObserver(this);
				subjectItr = m_Subjects.erase(subjectItr);
			}

		};
		void UnRegisterSubject(engine::Subject* pSubject)
		{
			auto subjectItr{ std::find(m_Subjects.begin(), m_Subjects.end(), pSubject) };
			if (subjectItr != m_Subjects.end())
			{
				pSubject->RemoveObserver(this);
				m_Subjects.erase(subjectItr);
			}
		};

	private:
		std::vector<engine::Subject*> m_Subjects;	// Keep track of all subjects observing

	};
}
#endif