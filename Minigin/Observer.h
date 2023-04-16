#ifndef GAME_ENGINE_OBSERVER
#define GAME_ENGINE_OBSERVER
#include <vector>

namespace dae
{
	class GameObject;
}
class Event;
class Subject;
class Observer
{

public:
	Observer() = default;
	virtual ~Observer() {};  // Observers will unregister itself from the subjects its observing
	virtual void OnNotify(dae::GameObject* gameObject, const Event& event) = 0;

private:
	std::vector<Subject*> m_Subjects;	// All subjects its observing

};

#endif