#ifndef HEALTH_COMPONENT
#define HEALTH_COMPONENT

#include "Component.h"
#include <glm/glm.hpp>
#include "Subject.h"
#include <memory>

namespace engine
{
	class Observer;
}

class HealthComponent final: public engine::Component
{
public:
	HealthComponent(engine::GameObject* pOwner, unsigned int lives);
	~HealthComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void DecrementHealth(unsigned int amount);
	void AddObserver(engine::Observer* pObserver);

	const unsigned int GetLives() const;


private:
	unsigned int m_Lives;									// Amount of lives the GameObject has
	std::unique_ptr<engine::Subject> m_HealthSubject;
};

#endif
