#ifndef GAME_ENGINE_SUBJECT
#define GAME_ENGINE_SUBJECT

#include <vector>

// *** Class that contains all the observers observing the corresponding object *** 

namespace engine
{
	class GameObject;
	class Observer;
	class Event;
	class Subject final
	{

	public:
		~Subject();
		void AddObserver(engine::Observer* observer);
		void RemoveObserver(engine::Observer* observer);
		void NotifyObservers(engine::GameObject* gameObject, const engine::Event& event);


	private:
		std::vector<engine::Observer*> m_Observers{};

	};
}
#endif
