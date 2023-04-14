#ifndef GAME_ENGINE_SUBJECT
#define GAME_ENGINE_SUBJECT

#include <vector>

// *** Class that contains all the observers observing the corresponding object *** 

class Observer;
class GameObject;
class Event;
class Subject final
{

public:
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);
	void NotifyObservers(GameObject* gameObject, const Event& event);

private:
	std::vector<Observer*> m_Observers;

};
#endif
