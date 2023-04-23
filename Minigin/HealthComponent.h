#ifndef HEALTH_COMPONENT
#define HEALTH_COMPONENT

#include "Component.h"
#include <glm/glm.hpp>
#include "Subject.h"
#include <memory>

class Observer;
class HealthComponent final: public Component
{
public:
	HealthComponent(dae::GameObject* pOwner, unsigned int lives);
	~HealthComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void DecrementHealth(unsigned int amount);
	void AddObserver(Observer* pObserver);

	const unsigned int GetLives() const;


private:
	unsigned int m_Lives;			// Amount of lives the GameObject has
	std::unique_ptr<Subject> m_ActorDiedEvent;
};

#endif
