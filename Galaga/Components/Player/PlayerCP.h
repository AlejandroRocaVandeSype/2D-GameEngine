#ifndef GALAGA_PLAYERCP
#define GALAGA_PLAYERCP

#include "Component.h"
#include "Observer.h"
#include <glm/vec2.hpp>

class PlayerCP final : public engine::Component, public engine::Observer
{
public:
	PlayerCP(engine::GameObject* pOwner, unsigned int health, unsigned int playerIdx, const glm::vec2& windowLimits);
	virtual ~PlayerCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

	unsigned int GetPlayerIndex();


private:
	unsigned int m_PlayerIdx;				// Keep track of which player is this
};


#endif

